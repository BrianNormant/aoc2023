module Main

import System.File


hash : (List Char) -> Int
hash s = f s 0 where
  h : Char -> Int -> Int
  h c n = mod ( 17 * (n + ord c)) 256
  f : (List Char) -> Int -> Int
  f [] n = n
  f (c::xs) n = f xs (h c n)


main : IO ()
main = let
          f = "Hello\n" {- !(readFile "example.txt") -}
          -- f = replace (\x => x == "\n") "" f
       in
          printLn f
