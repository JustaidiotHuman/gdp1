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
-- From lsb to msb
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

