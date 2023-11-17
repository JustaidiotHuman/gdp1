{-# LANGUAGE EmptyDataDecls, RankNTypes, ScopedTypeVariables #-}

module SignedByte
where

data SByte = SB Int
             deriving (Eq, Ord)

fromInt :: Int -> SByte
fromInt n = if (-128) <= n && n <= 127
            then SB (encode n)
            else error ("Int out of range for SByte: " ++ show n)
              where
                encode m
                  | m == 0    = 0
                  | m <  0    = 256 + m
                  | otherwise = m

instance Show SByte where
   show (SB n) = "fromInt (" 
                  ++ show (if n > 127 then n - 256 else n) 
                  ++ ")"

oneC :: SByte -> SByte
oneC (SB n) = SB (255 - n)

twoC :: SByte -> SByte
twoC (SB n) = if n == 0
              then (SB 0)
              else (SB (256 - n))

-- Reference implementation of addition

scadd :: Int -> Int -> Int
a `scadd` b
   | a + b < (-128) = error "underflow"
   | a + b > ( 127) = error "overflow"
   | otherwise      = a + b


-- Reference implementation of subtraction

{--

Check:
ghci> (fromInt (-127)) `sub` (fromInt (-128))
*** Exception: SByte underflow

ghci> (fromInt (-127)) `add` (fromInt (-128))
*** Exception: SByte underflow

--}
scsub :: Int -> Int -> Int
a `scsub` b
   | a - b < (-128) = error "underflow"
   | a - b > ( 127) = error "overflow"
   | otherwise      = a - b

-- Implementation of addition in type SByte

add :: SByte -> SByte -> SByte
(SB n) `add` (SB m) 
   | n >  127 && m >  127 = let sum = (n + m) `mod` 256
                            in if sum < 128
                            then error "SByte underflow"
                            else (SB sum)
   | n >  127 && m <= 127 = let sum = (n + m) `mod` 256 in (SB sum)
   | n <= 127 && m >  127 = let sum = (n + m) `mod` 256 in (SB sum)
   | n <= 127 && m <= 127 = let sum = n + m
                            in if sum > 127
                               then error "SByte overflow"
                               else (SB sum)

        
-- Implementation of subtraction in type SByte using two's complement

sub :: SByte -> SByte -> SByte
--a `sub` b = a `add` (twoC b)
a `sub` b = if a == b
            then (SB 0)
            else a `add` (twoC b)

