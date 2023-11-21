{-# LANGUAGE EmptyDataDecls, RankNTypes, ScopedTypeVariables #-}

module CarryRippleDemo
where

import SignedIntegerAsBitList(SBList, show,
       fromBitList, mkFromInteger,
       asInteger, asUnsignedInteger, 
       getLimitsSignedInteger,
       one's, two's, add, sub, add_demo, sub_demo);


{-- Examples used in file Overflow/overflow_64_8byte.c

-- Example showing 64bit subtraction (via two's complement)

(mkFromInteger 64 (-9223372036854775808)) `sub_demo` (mkFromInteger 64 (-9223372036854775808))

--output: now overflow -> result is correct

Subtraction explained:
  Integer numbers without parenthesis show the signed integer encoding.
  Integer numbers within  parenthesis show the original integer value.

    LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO   the first  operand  : 9223372036854775808  (-9223372036854775808)
    LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO   the second operand  : 9223372036854775808  (-9223372036854775808)
    LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO   its two's complement: 9223372036854775808  (-9223372036854775808)

    LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO   9223372036854775808  (-9223372036854775808)
    LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO   9223372036854775808  (-9223372036854775808)
(+) ----------------------------------------------------------------
    OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO                     0  (                   0)

   Flags:
     Carry out (ignore)

-- Example showing 64bit addtion with arithmetic overflow

(mkFromInteger 64 (-9223372036854775808)) `add_demo` (mkFromInteger 64 (-9223372036854775808))

-- output: overflow flag is set --> result is incorrect!

Addition explained:
  Integer numbers without parenthesis show the signed integer encoding.
  Integer numbers within  parenthesis show the original integer value.

    LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO   9223372036854775808  (-9223372036854775808)
    LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO   9223372036854775808  (-9223372036854775808)
(+) ----------------------------------------------------------------
    OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO                     0  (                   0)

   Flags:
     Carry out (ignore)
     Arithmetic overflow!

-- reason for the overflow: the carry out of the most significant bit overwrites the sign bit.
-- fix: provide one bit more (here 65 bits)

(mkFromInteger 65 (-9223372036854775808)) `add_demo` (mkFromInteger 65 (-9223372036854775808))

-- output: no overflow anymore --> result is correct!

Addition explained:
  Integer numbers without parenthesis show the signed integer encoding.
  Integer numbers within  parenthesis show the original integer value.

    LLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO   27670116110564327424  ( -9223372036854775808)
    LLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO   27670116110564327424  ( -9223372036854775808)
(+) -----------------------------------------------------------------
    LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO   18446744073709551616  (-18446744073709551616)

   Flags:
     Carry out (ignore)

--}

{-- more examples

-- Same as above bit with only 8 bit

(mkFromInteger 8 (-128)) `sub_demo` (mkFromInteger 8 (-128))

(mkFromInteger 8 (-128)) `add_demo` (mkFromInteger 8 (-128))

(mkFromInteger 9 (-128)) `add_demo` (mkFromInteger 9 (-128))


--}
