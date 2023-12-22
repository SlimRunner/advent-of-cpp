# Advent of Code 2023
## CPP and Makefile

This setup is slightly cursed. Each day file is run from main and it is "linked" from Common.hpp by calling a setup function that must be manually furnished. Then the `solve` function of the day's file is accumulated in a Singleton that main has access to.

## Daily Log

| # | Title | Stars | Difficulty | Quick notes |
| - | - | - | - | ----- |
| 1 | [Trebuchet?!](#day-1) | ⭐⭐ | easy | pay attention to hardcoded maps and lists |
| 2 | [Cube Conundrum](#day-2) | ⭐⭐ | easy | be more careful of dangling references |
| 3 | [Gear Ratios](#day-3) | ⭐⭐ | medium | unfamiliar terrain, used DFS |
| 4 | [Scratchcards](#day-4) | ⭐⭐ | easy | please pay attention to where you advance your pointers samir |
| 5 | [If You Give A Seed A Fertilizer](#day-5) | ⭐ | hard |  |
| 6 | [Wait For It](#day-6) |  |  |  |

<!-- | \# | Title | Stars | Difficulty | Quick notes | -->


## Extended Notes
### Day 1
#### Part 1
* It went smoothly.
* I used a normal LTR search iteratively to find the first and last item.

#### Part 2
* I got stuck in part 2 because I mistyped a map of numbers like this: `0123459789`. It took me several hours to notice that I mistyped a 9 between 5 and 7.
* I realized that it was easier to use LTR search for the first number and RTL search to find the last item. I ran two sets of those for normal and spelled numbers separately.

---
### Day 2
#### Part 1
* It did not go smoothly because of skill issues with the STL. The challenge itself was easy.
  * I misinterpreted what std::regex_search did in the background, so I deleted the string used for the search before reading the results which resulted in reading dangling references.
* I used a linear search to find cube counts which surpassed the prescribed amounts and accumulated their day values on the go.

#### Part 2
* It went smoothly.
* Edited code from part 1 to also find the running max of each day by color and operate them as prescribed in the challenge.

---
### Day 3
#### Part 1
* This was fairly difficult. I was unfamiliar with the shape of the problem which was reflected in the unsophisticated code that resulted.
* My approach was a multi step process. First, I hardcoded all the symbols in a list and traversed the grid to store their location in a stack. Then, I used that stack to run DFS on the location of each symbol where symbols added all their neighbors and numbers only their left and right. Finally, with the map of visited nodes I traversed the grid once more to get the numbers with a push-and-flush approach.

#### Part 2
* It was easier since my code from part one provided a backbone to solve this one much faster.
* I decided to extend my code from part one to identify the "owner" of each part number found above during the DFS. This way I was able to insert these numbers into a hash map and easily add only the ones whose owner was an asterisk and had two siblings.

---
### Day 4
#### Part 1
* It went smoothly.
* I turned the winning numbers into a set and simply queried them against the lotto numbers. If they existed, count them.
#### Part 2
* It went smoothly.
* I did a small refactor to include a per card counter which was used to accumulate the number of winning numbers times the appropriate count of cards.

---
### Day 5
#### Part 1
  * It went smoothly.
  * Simply brute forced the almanac to get the smallest seed value.
#### Part 2 (missing)
  * Oh boy.
  * I should have realized where this was going when my brute force approach scalated to O(n<sup>3</sup>).

---
### Day 6
#### Part 1
#### Part 2

<!-- ---
### Day \#
#### Part 1
#### Part 2 -->
