import os
import sys
import re

# this script assumes days are called day##.cpp or day##.cc
ROOT = '.'
ROOT_SRC = './src/'
ROOT_INCL = './include/'

def main(args):
  DISP_SIZE = 14
  ID_COL_SIZE = 16
  RX_YEAR = r'(?<=y)\d+'

  yearOptions = dict()
  yearPrompt = ''

  folderList = [f for f in os.listdir(ROOT_SRC) if os.path.isdir(f'{ROOT_SRC}{f}')]

  for folder in folderList:
    yearMatch = re.search(RX_YEAR, folder)
    thisPath = f'{ROOT_SRC}{folder}/'
    dispPath = thisPath if len(thisPath) < DISP_SIZE else f'...{thisPath[-5:]}'
    if yearMatch:
      yearID = yearMatch.group(0)
      yearOptions[yearID] = thisPath
      yearPrompt += f'  {dispPath: <{DISP_SIZE}}{f"[{yearID}]": >{ID_COL_SIZE}}\n'
  
  yearPrompt += f'  {"exit": <{DISP_SIZE}}{"[q]": >{ID_COL_SIZE}}\n'

  print('List of available years')
  print(yearPrompt)
  yearInput = input('choose year to add a challenge: ')

  while (yearInput != 'q' and yearInput not in yearOptions):
    print('input is not valid\n')
    print('List of available years')
    print(yearPrompt)
    yearInput = input('choose year to add a challenge: ')

  if (yearInput != 'q'):
    RX_DAY = r'(?<=day)\d{2}(?=.cpp|.cc)'

    yearPath = yearOptions[yearInput]
    daysMissing = [True for x in range(25)]

    dayList = [f for f in os.listdir(yearPath) if not os.path.isdir(f'{yearPath}{f}')]

    for day in dayList:
      dayMatch = re.search(RX_DAY, day)
      if dayMatch:
        dayID = int(dayMatch.group(0))
        if (dayID < 1 or dayID > 25):
          continue
        daysMissing[dayID - 1] = False
    
    dayPrompt = ''

    dayListings = set()
    for index, missing in enumerate(daysMissing):
      if missing:
        dayPrompt += f'  {f"day {index + 1}":<{DISP_SIZE}}{f"[{index + 1}]":>{ID_COL_SIZE}}\n'
      else:
        dayListings.add(index + 1)
    dayPrompt += f'  {"exit":<{DISP_SIZE}}{"[q]":>{ID_COL_SIZE}}\n'

    totalMissing = len([d for d in daysMissing if d])
    if totalMissing == 0:
      print('This year is complete. Congrats!')
    else:
      while True:
        print(f'Missing days of year {yearInput}')
        print(dayPrompt)
        dayInput = input('Enter the day to auto generate boilerplate: ')
        try:
          if dayInput == 'q':
            break
          dayInput = int(dayInput)
          if dayInput < 1 or dayInput > 25:
            raise ValueError
        except ValueError:
          print('Your input is not a valid number.')
        except:
          print('An exception occurred.')
        finally:
          dayListings.add(dayInput)
          prepareDay(dayInput, yearInput, dayListings, yearPath)
          break

def prepareDay(day, year, listings, path):
  dayPath = f'{path}day{day:02}.cpp'
  dayContent = getDailyCode(day, year)
  with open(dayPath, 'x') as f:
    f.write(dayContent)

  hppDailyPath = f'{ROOT_INCL}/DailySetup.hpp'
  hppLines = [f'void y{year}d{d:02}(Solutions & sol);' for d in listings]
  hppDailyContent = getHeaderSetup(hppLines)
  with open(hppDailyPath, 'w') as f:
    f.write(hppDailyContent)

  cppDailyPath = f'{ROOT_SRC}utils/DailySetup.cpp'
  cppLines = [f'y{year}d{d:02}(sol);' for d in listings]
  cppDailyContent = getSetup(cppLines)
  with open(cppDailyPath, 'w') as f:
    f.write(cppDailyContent)

def getDailyCode(day, year):
  return f'''\
#include "EntryHeaders.hpp"

namespace {{

void solve(std::string path) {{
  FileParser fp(path);
  auto lines = fp.getLines();

  (void)lines; // unused variable

  std::cout << "P1: " << std::endl;
}}

}} // anon namespace

void y{year}d{day:02}(Solutions & sol) {{
  sol.addEntry(solve, Year::y{year}, Day::d{day:02});
}}
'''

def getHeaderSetup(prototypes):
  prototypes = '\n  '.join(prototypes)
  return f'''\
// automatically generated
// DO NOT EDIT

#pragma once

#include "Solutions.hpp"

{prototypes}

void collectSetups(Solutions & sol);
'''

def getSetup(functionCalls):
  functionCalls = '\n  '.join(functionCalls)
  return f'''\
// automatically generated
// DO NOT EDIT

#include "DailySetup.hpp"

void collectSetups(Solutions & sol) {{
  {functionCalls}
}}
'''

if __name__ == '__main__':
  args = {}
  lkey = None
  for arg in sys.argv:
    if arg[0] == '-':
      lkey = arg[1:]
      args[lkey] = []
    elif len(args) and lkey is not None:
      args[lkey].append(arg)
  main(args)
