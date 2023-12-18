# AOC with CPP and Makefile

This setup is slightly cursed. Each day file is run from main and it is "linked" from Common.hpp by calling a setup function that must be manually furnished. Then the `solve` function of the day's file is accumulated in a Singleton that main has access to.

# Notes

1. Part 1 was uneventful. Part two would have been easy if I had not mistyped the number map as `0123459789`. It took me several hours to notice that I mistyped a 9 between 5 and 7. Other than that my approach of making a left and right search worked wonders.