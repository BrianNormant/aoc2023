from std/strutils import isDigit, contains
import std/strformat

const FILE = "./puzzle.txt"

proc part1 =
  let f = open(FILE)
  defer: close(f)
  
  var 
    line:string
    sum = 0

  while readLine(f, line):
    var 
      fdigit = -1
      ldigit = -1
    for i in 0..<line.len:
      if (fdigit == -1 and isDigit(line[i])): fdigit = int(line[i]) - 0x30
      if (ldigit == -1 and isDigit(line[^(i+1)])): ldigit = int(line[^(i+1)]) - 0x30
    sum += fdigit * 10 + ldigit
  echo fmt"Calibration = {sum}"

proc part2 =
  let f = open(FILE)
  defer: close(f)

  var
    line:string
    sum = 0

  while readLine(f, line):
    var
      fdigit = -1
      ldigit = -1
      fstr:string
      lstr:string

    for i in 0..<line.len:
      fstr = line[0..i]
      lstr = line[^(i+1)..^1]
      if fdigit == -1:
        if fstr.contains("one"): fdigit = 1
        elif fstr.contains("two"): fdigit = 2
        elif fstr.contains("three"): fdigit = 3
        elif fstr.contains("four"): fdigit = 4
        elif fstr.contains("five"): fdigit = 5
        elif fstr.contains("six"): fdigit = 6
        elif fstr.contains("seven"): fdigit = 7
        elif fstr.contains("eight"): fdigit = 8
        elif fstr.contains("nine"): fdigit = 9
        elif (fdigit == -1 and isDigit(line[i])):
          fdigit = int(line[i]) - 0x30
      if ldigit == -1:
        if lstr.contains("one"): ldigit = 1
        elif lstr.contains("two"): ldigit = 2
        elif lstr.contains("three"): ldigit = 3
        elif lstr.contains("four"): ldigit = 4
        elif lstr.contains("five"): ldigit = 5
        elif lstr.contains("six"): ldigit = 6
        elif lstr.contains("seven"): ldigit = 7
        elif lstr.contains("eight"): ldigit = 8
        elif lstr.contains("nine"): ldigit = 9
        elif (ldigit == -1 and isDigit(line[^(i+1)])):
          ldigit = int(line[^(i+1)]) - 0x30
    sum += fdigit * 10 + ldigit
  echo fmt"Calibration = {sum}"



when isMainModule:
  part1()
  part2()
