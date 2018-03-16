This interactive AI uses a depth-first search to explore every possibility and react to the user in an intelligent fashion.

At each recursive iteration, it stores the number of wins, losses, and draws among its children as well as their coordinates in the 3x3 grid and the recursive depth it took to reach each one. This allows it to choose the fastest win and the slowest loss (even though it never loses), so that it appears a bit more human.

For example, given the following scenario with the AI to play:

                   1   2   3
                 -------------
               1 | O | X |   |
                 -------------
               2 | O | X |   |
                 -------------
               3 |   |   |   |
                 -------------

We would like it to choose (2, 3), even though (1, 3) is a forced win as well.
