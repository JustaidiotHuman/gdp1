{-# LANGUAGE EmptyDataDecls, RankNTypes, ScopedTypeVariables #-}

module SignedIntegerAsBitList  -- TODO: export without constructor SBL
where

import Data.List
import Data.Foldable

-- We hide the constructor and implement our own instance for show.
data SBList = SBL [Char] deriving (Eq, Ord)

instance Show SBList
   where
   --show (SBL ls) = "fromBitList " ++ show (length ls) ++ " " ++ show ls
   show (SBL ls)
     | len <= 8          = "fromBitList " ++ show (length ls) ++ " " ++ show ls
     | len `mod` 8 /= 0  = "fromBitList " ++ show (length ls)
                           ++ " \"" ++  (take (len `mod` 8) ls)
                           ++ show_aux (drop (len `mod` 8) ls)
                           ++ "\""
     | otherwise         = "fromBitList " ++ show (length ls)
                           ++ " \"" ++  (take 8 ls)
                           ++ show_aux (drop 8 ls)
                           ++ "\""
     where
       len = length ls
       show_aux [] = []
       show_aux xs = "." ++ (take 8 xs) ++ show_aux (drop 8 xs)

-- Representation of bit list ('O'|'L'|'.')* in type SBList

zeropad       :: Int -> [Char]  -> SBList
zeropad numbits ls
  | len <  numbits = SBL $ (replicate (numbits - len) 'O') ++ ls
  | len == numbits = SBL ls
  | otherwise      = error "Function zeropad: argument has too much bits"
  where len = length ls

fromBitList      :: Int -> [Char]  -> SBList
fromBitList numbits cs = zeropad numbits (fromBitList_aux cs)
  where
    fromBitList_aux [] = []
    fromBitList_aux ('O':cs) = 'O' : fromBitList_aux cs
    fromBitList_aux ('L':cs) = 'L' : fromBitList_aux cs
    fromBitList_aux ('.':cs) = fromBitList_aux cs
    fromBitList_aux (c:cs  ) = error $ "Function fromBitList:"
                                    ++ " invalid character '"
                                    ++ [c] ++ "'"


-- Representation of Integer values n in type SBList
mkFromInteger :: Int -> Integer -> SBList

mkFromInteger numbits ival
  | ival < min_val  = error $ "Function mkFromInteger: value "
                              ++ show ival
                              ++ " is too small for signed integer with "
                              ++ show numbits ++ " bits"

  | ival > max_val  = error $ "Function mkFromInteger: value "
                              ++ show ival
                              ++ " is too large for signed integer with "
                              ++ show numbits ++ " bits"
  | otherwise       = zeropad numbits 
                              (encodeToBitList
                                 (representAsTwo'sComplement ival) [])
  where
    modulus = 2^numbits
    max_val = (modulus `div` 2) -  1
    min_val = (modulus `div` 2) * (-1)

    representAsTwo'sComplement i
      | i == 0    = 0
      | i <  0    = modulus + i
      | otherwise = i

    encodeToBitList val acc
      | val == 0   = 'O' : acc
      | val == 1   = 'L' : acc
      | otherwise  = let bit  = val `mod` 2
                         rest = val `div` 2
                     in  if bit == 0
                         then encodeToBitList rest ('O' : acc)
                         else encodeToBitList rest ('L' : acc)

-- Conversion of values of type SBList back to Integer values
asInteger :: SBList -> Integer

asInteger (SBL ls)
  | ival > max_val  = ival - modulus
  | otherwise       = ival
  where
    modulus = 2^(length ls)
    max_val = (modulus `div` 2) -  1
    min_val = (modulus `div` 2) * (-1)

    ival = foldl' (\a c -> if c == 'L' then 2*a + 1 else 2*a) 0 ls

-- Show limits for Integer values encoded as signed integer with numbits
getLimitsSignedInteger:: Int -> String
getLimitsSignedInteger numbits =
  "modulus: " ++ show modulus
     ++ "; min_val: " ++ show min_val
     ++ "; max_val: " ++ show max_val
  where
    modulus = 2^numbits
    max_val = (modulus `div` 2) -  1
    min_val = (modulus `div` 2) * (-1)

-- ----------------------------------------------------------------------
-- Implementation of binary arithmetics on type SBList
-- ----------------------------------------------------------------------

--
-- One's complement (inversion of bits)
--
one's:: SBList -> SBList

one's (SBL bs) = SBL (map (\b -> if b == 'L' then 'O' else 'L') bs)

--
-- Two's complement: fast implementation
-- From rightmost bit (LSB) to the leftmost bit (MSB)
-- * copy all O bits upto the first L bit
-- * copy the first L bit
-- * invert all bits left from the first L bit

two's:: SBList -> SBList

two's (SBL bs) = SBL (two's_aux (reverse bs) False [])
  where
  two's_aux [] inv acc = acc
  two's_aux ('O':bs) False acc = two's_aux bs False ('O':acc)
  two's_aux ('L':bs) False acc = two's_aux bs True  ('L':acc)
  two's_aux bs       True  acc =
     reverse (map (\b -> if b == 'L' then 'O' else 'L') bs) ++ acc

-- Logical gates

not_gate  :: Char -> Char
not_gate  'O' = 'L'
not_gate  'L' = 'O'
not_gate   _  = error "not_gate: invalid input"

and_gate  :: Char -> Char -> Char
and_gate  'O' 'O' = 'O'
and_gate  'O' 'L' = 'O'
and_gate  'L' 'O' = 'O'
and_gate  'L' 'L' = 'L'
and_gate   _   _  = error "and_gate: invalid input"

or_gate   :: Char -> Char -> Char
or_gate  'O' 'O' = 'O'
or_gate  'O' 'L' = 'L'
or_gate  'L' 'O' = 'L'
or_gate  'L' 'L' = 'L'
or_gate   _   _  = error "and_gate: invalid input"

xor_gate  :: Char -> Char -> Char
xor_gate  'O' 'O' = 'O'
xor_gate  'O' 'L' = 'L'
xor_gate  'L' 'O' = 'L'
xor_gate  'L' 'L' = 'O'
xor_gate   _   _  = error "and_gate: invalid input"

-- Half adder for a single bit implemented by an and gate and an xor gate
-- See e.g. https://de.wikipedia.org/wiki/Halbaddierer
--
-- Inputs:
--     a: the bit of first  addend
--     b: the bit of second addend
--
-- Outputs:
--  cout: the carry out bit
--     s: the sum of bits a and b

half_adder :: Char -> Char -> (Char, Char)

half_adder a b = 
  let cout = and_gate a b
      s    = xor_gate a b
  in (cout, s)

--
-- Full adder for a single bit implemented by two half adders and an or-gate
-- See e.g. https://de.wikipedia.org/wiki/Volladdierer
--
-- Inputs:
--     a: the bit of first  addend
--     b: the bit of second addend
--   cin: the carry in
--
-- Outputs:
--  cout: the carry out bit
--     s: the sum of bits a and b

full_adder :: Char -> Char -> Char -> (Char, Char)

full_adder a b cin = 
  let (ha1_cout, ha1_s) = half_adder a b
      (ha2_cout, ha2_s) = half_adder ha1_s cin
      cout              = or_gate ha1_cout ha2_cout
      s                 = ha2_s
  in (cout, s)

--
-- Carry Ripple Adder for n bits without inverter
-- This version cannot be used directly for subtraction.
-- We need to compute the two's complement of the second argument before.
--

-- The raw adder without overflow detection

carry_ripple_adder_raw :: SBList -> SBList -> (Char, SBList)
carry_ripple_adder_raw (SBL as) (SBL bs) =
  let (cout, ss) = carry_ripple_adder_raw_aux full_adder
                     'O' [] (reverse as) (reverse bs)
  in (cout, SBL ss)
  where
    carry_ripple_adder_raw_aux _ cin acc []     []     = (cin, acc)
    carry_ripple_adder_raw_aux f cin acc (a:as) (b:bs) =
      let (co, so) = f a b cin
      in carry_ripple_adder_raw_aux f co (so:acc) as bs



