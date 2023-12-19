# AOC with CPP and Makefile

This setup is slightly cursed. Each day file is run from main and it is "linked" from Common.hpp by calling a setup function that must be manually furnished. Then the `solve` function of the day's file is accumulated in a Singleton that main has access to.

# Notes

1. Part 1 was uneventful. Part two would have been easy if I had not mistyped the number map as `0123459789`. It took me several hours to notice that I mistyped a 9 between 5 and 7. Other than that my approach of making a left and right search worked wonders.

2. This whole day was easy since it was mostly finding a running max. I did have an issue related to C++ Regex that its matches and submatches create references to the searched string; a result, of that oversight on my part I was reading dangling references because I was clearing the stringstream the searches belonged to.

3. This was decently dificult for me which I am sure is reflected in the unsophisticated code that resulted. My approach is a multi step process where first I collect all the symbols into a stack. Then I run DFS on each symbol where only numbers and symbols themselves are valid. Then with the map of visited nodes I traverse again all the table and follow a push-and-flush approach to get the numbers (since they are all contiguous). Finally, for part two I use a hash map to identify the owner and counts of each of those numbers so I can use only those which have an asterisk as owner and have two siblings.