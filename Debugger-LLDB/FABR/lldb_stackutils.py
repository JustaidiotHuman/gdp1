from __future__ import print_function
import lldb
import binascii

def __lldb_init_module(debugger, internal_dict):
  debugger.HandleCommand(
  'command script add -f lldb_stackutils.stack_all stack_all')

def stack_all(debugger, arguments, result, internal_dict):
  """
  Produce a full dump of all stackframes including animated base pointers
  """

  target  = debugger.GetSelectedTarget()
  process = target.GetProcess()
  frame   = process.GetSelectedThread().GetSelectedFrame()

  # Get size of a pointer for the processors architecture
  arch_size = process.GetAddressByteSize()

  # Get all variables of the current frame (arguments + locals)
  variables = frame.get_all_variables()

  # Compute minimal address of all variables
  # This compensates for the red-zone of leaf functions used in x64 ABI
  min_addr = min(frame.fp, frame.sp)
  for v in variables:
      #print("at address %x is variable: %s" % (int(v.GetLoadAddress()), v.name) )
      if int(v.GetLoadAddress()) < min_addr:
            min_addr = int(v.GetLoadAddress())

  # Adjust the start address to a multiple of arch_size
  start_addr = min_addr - (min_addr % arch_size)

  # Set flag for broken chain of frames to False
  # For arm achitectures the chain of frames is broken
  # near the assembly command 'ret'.
  # In such a case lldb.frame == lldb.frame.parent.
  # Examples for arm64:
  #    22   0x100003e9c <+88>:   add   sp,  sp
  #   *23   0x100003ea0 <+92>:   ret
  chain_broken = False

  # Search for the frame of function main
  cur_frame = frame
  #while not (cur_frame.name == 'main') and not (cur_frame == cur_frame.parent):
  while not (cur_frame.name == 'main'):
      if (cur_frame == cur_frame.parent):
        print("")
        print("Attention: chain of frame pointers is broken near assembly")
        print("           command 'ret' on arm architectures.")
        print("           Therefore, we dump the current and the parent frame, only.")
        print("")
        chain_broken = True
        break
      else:
      	cur_frame = cur_frame.parent

  # Frame pointers are always aligned to multiples of arch_size
  end_addr = cur_frame.fp

  # Compute number of bytes for the entire stack dump
  bytes_to_read = (end_addr - start_addr + arch_size)

  # Read the entire stack blob
  err = lldb.SBError()
  stack_blob = process.ReadMemory(start_addr,bytes_to_read, err)

  # Dump stack blob with animated frame pointers
  if err.Success():
      cur_fp = frame.fp
      cur_idx = 0

      print("")
      if start_addr < frame.sp:
          print("Attention: x64 ABI red zone of leaf frame #%d:" % cur_idx)
          print("")

      for wi in range(0,bytes_to_read,arch_size):
          # Print single bytes; use python3 print options.
          # See future import at the top of this file.
          print("%x:" % (start_addr+wi), end='')
          for b in stack_blob[wi:wi+arch_size]:
            if isinstance(b, int):
            	print(" %02x" % b, end='')
            else:
                print(" %s" % binascii.hexlify(b), end='')

          if start_addr+wi == cur_fp:
              print("  <-- fp of frame #%d" % cur_idx, end='')
              cur_fp = process.ReadPointerFromMemory(cur_fp,err)
              cur_idx += 1
              # Assume success
              if start_addr+wi == end_addr:
                  if not chain_broken:
                      print(": main", end='')

          print("")

  else:
    # report error
    print("Unable to read memory from stack")

