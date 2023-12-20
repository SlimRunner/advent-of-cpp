# outline
'''
TODO:
  * add a new file by providing a year and day
    * read all the y20xx files within src and list them as options
    * similarly read all files within that folder to list missing days
  * add the file and the boiler plate (listing 1)
    * substitute the # with the appropriate number
  * rewrite DailySetup.hpp and DailySetup.cpp using templates
    * to make this easier do not edit the fails but do a complete
      rewrite in accordance with listings 2 and 3. However, do make
      clear that those files are not to be edited manually!
  * DO NOT EDIT MAIN AS IT IS UNDER CONSTRUCTION STILL
'''

# listing 1
'''
#include "EntryHeaders.hpp"

namespace {

void solve(std::string path) {
  FileParser fp(path);
  auto lines = fp.getLines();

  (void)path; // unused variable
  (void)lines; // unused variable

  std::cout << "P1: " << std::endl;
}

} // anon namespace

void y####d##(Solutions & sol) {
  sol.addEntry(solve, Year::y####, Day::d##);

'''

# listing 2
'''
// automatically generated
// DO NOT EDIT

#pragma once

#include "Solutions.hpp"

void y####d##(Solutions & sol);
...

void collectSetups(Solutions & sol);

'''

# listing 3
'''
// automatically generated
// DO NOT EDIT

#include "DailySetup.hpp"

void collectSetups(Solutions & sol) {
  y####d##(sol);
  ...
}

'''
