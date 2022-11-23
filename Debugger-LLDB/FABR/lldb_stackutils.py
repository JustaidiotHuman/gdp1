#!/usr/bin/env python

from __future__ import print_function
import lldb
import shlex
import optparse
import binascii

def __lldb_init_module(debugger, internal_dict):
  debugger.HandleCommand(
  'command script add -f lldb_stackutils.stack_all stack_all')

def stack_all(debugger, arguments, result, internal_dict):
  """
  Produce a full dump of all stackframes including animated base pointers
  """

  process = debugger.GetSelectedTarget().GetProcess()
  target = debugger.GetSelectedTarget()
  frame  = process.GetSelectedThread().GetSelectedFrame()

  # parse arguments
  raw_args = shlex.split(arguments)
  values = get_values(generate_option_parser(), raw_args, result)
  (architecture, endianness) = (None, None)
  if result.Succeeded():
    (architecture, endianness) = values
  else:
    return

  variables = frame.get_all_variables()

  # Compute minimal address of all variables
  # This compensates for the red-zone of leaf functions used in x64 ABI
  min_addr = frame.fp
  for v in variables:
      #print("at address %x is variable: %s" % (int(v.GetAddress()), v.name) )
      if int(v.GetAddress()) < min_addr:
            min_addr = int(v.GetAddress())

  # Adjust the start address to a multiple of 8
  start_addr = min_addr - (min_addr % 8)

  # Search for the frame of function main
  cur_frame = frame
  while not (cur_frame.name == 'main'):
      cur_frame = frame.parent

  # Frame pointers are always aligned to multiples of the address architecture
  #end_addr = cur_frame.fp + (cur_frame.fp % 8)
  end_addr = cur_frame.fp

  bytes_to_read = (end_addr - start_addr + 8)

  # read the entire stack blob
  err = lldb.SBError()
  stack_blob = process.ReadMemory(start_addr,bytes_to_read, err)

  # dump stack blob with animated frame pointers
  if err.Success():
      cur_fp = frame.fp
      cur_idx = 0

      #print("")
      #print("Start Address = {}".format(hex(start_addr)))
      #print("  Min Address = {}".format(hex(min_addr)))
      #print("  End Address = {}".format(hex(end_addr)))
      #print("Stack Pointer = {}".format(hex(frame.sp)))
      #print("Frame Pointer = {}".format(hex(frame.fp)))
      #print("Stack blob has length %d" % (len(stack_blob)))
      print("")
      if start_addr < frame.sp:
          print("Attention: x64 ABI red zone of leaf frame #%d:" % cur_idx)
          #print("Start Address = {}".format(hex(start_addr)))
          #print("Stack Pointer = {}".format(hex(frame.sp)))
          #print("Frame Pointer = {}".format(hex(frame.fp)))
          print("")

      for wi in range(0,bytes_to_read,8):
          # Print a single gigantic 8 byte word
          #print("%x: 0x%s" % (start_addr+wi, binascii.hexlify(stack_blob[wi:wi+8]) ))

          # Print single bytes; use python3 print options.
          # See future import at the top of this file.
          print("%x:" % (start_addr+wi), end='')
          for b in stack_blob[wi:wi+8]:
            print(" %s" % binascii.hexlify(b), end='')

          if start_addr+wi == cur_fp:
              print("  <-- fp of frame #%d" % cur_idx, end='')
              cur_fp = process.ReadPointerFromMemory(cur_fp,err)
              cur_idx += 1
              # Assume success
              if start_addr+wi == end_addr:
                  print(": main", end='')

          print("")

  else:
    # report error
    pass

# TODO
# Provide more switches for printing options
#
# - single byte dump vs. dump words of architcture length

#
# --------------------------------------------------
# Parser for argument string
# --------------------------------------------------

def get_values(parser, raw_args, result):
  try:
    (options, args) = parser.parse_args(raw_args)
  except:
    result.SetError(parser.usage)
    return

  if not options.endianness in ["big", "little"]:
    result.SetError("endianness can only be 'little' or 'big'")
    return

  if not options.architecture in ["x64"]:
    result.SetError("architecture can only be 'x64'")
    return

  return (options.architecture, options.endianness)

def generate_option_parser():
  usage = "usage: stack_all <-a|--architecture> <ARCH_NAME> [options]"
  parser = optparse.OptionParser(usage=usage)
  parser.add_option("-a", "--architecture",
           action="store",
           default='x64',
           dest="architecture",
           help="Define the architecture of the processor")
  parser.add_option("-e", "--endianness",
           action="store",
           default='little',
           dest="endianness",
           help="Define pattern endianness")
  return parser

