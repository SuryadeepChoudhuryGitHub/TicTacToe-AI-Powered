# Tic-Tac-Toe with AI — C++ Console Game
 
A classic Tic-Tac-Toe game that runs in your terminal, built entirely in C++. What makes it more interesting than the usual "print a grid and take turns" implementation is the AI — it has three difficulty levels, and the hardest one is genuinely unbeatable. Not "pretty hard to beat." Not "usually wins." Actually unbeatable. It uses the Minimax algorithm under the hood, which means it doesn't guess, it doesn't get lucky, and it doesn't make mistakes. It plays a mathematically perfect game every single time.
 
I built this as my BYOP capstone for the AI & ML course at VIT Bhopal. The idea came from wanting to actually implement an adversarial search algorithm from scratch rather than just study it in theory. Tic-Tac-Toe seemed like the perfect sandbox — small enough to be manageable, but complex enough to make the AI genuinely interesting to build.
 
---
 
## Table of Contents
 
- [What it does](#what-it-does)
- [Getting started](#getting-started)
- [How to play](#how-to-play)
- [How the AI works](#how-the-ai-works)
- [Why three difficulty levels?](#why-three-difficulty-levels)
- [Code layout](#code-layout)
- [Function reference](#function-reference)
- [Things I learned building this](#things-i-learned-building-this)
- [Honest limitations](#honest-limitations)
- [License](#license)
 
---
 
## What it does
 
At its core this is just Tic-Tac-Toe — two players, a 3x3 grid, first to three in a row wins. But there's a bit more going on:
 
- **Two-player mode** — two humans share the keyboard and alternate turns, classic style
- **Single-player vs AI** — play against one of three AI agents, each with a completely different strategy under the hood
- **Three difficulty levels** that are genuinely different from each other, not just the same logic running faster or slower:
  - **Easy** — picks random moves, no strategy whatsoever. Great if you want a warmup or you're playing with someone younger.
  - **Medium** — smart enough to win when it can and block you when it must, but has a real exploitable weakness if you know what you're doing
  - **Hard** — runs the full Minimax algorithm and has zero losing states. The best outcome you can force against it is a draw.
- You get to choose whether X or O goes first before every match
- The AI takes a short pause before announcing its move — it's fake, the computation finishes instantly, but it makes the game feel a lot more natural than watching a move appear the millisecond it's your opponent's turn
- You can rematch as many times as you want without closing and reopening the program
 
---
 
## Getting started
 
There are no dependencies. You don't need to install anything beyond a C++ compiler. Clone or download the repo, compile the single source file, and run it.
 
**Linux / macOS:**
```bash
g++ -std=c++11 -o tictactoe Main.cpp
./tictactoe
```
 
**Windows (MinGW / MSYS2):**
```bash
g++ -std=c++11 -o tictactoe.exe Main.cpp
tictactoe.exe
```
 
**Windows (MSVC — Visual Studio Developer Command Prompt):**
```bash
cl /EHsc /std:c++11 Main.cpp /Fe:tictactoe.exe
tictactoe.exe
```
 
> **Heads up:** If your compiler throws an error about `<thread>` or `<chrono>` not being found, the fix is almost always just making sure `-std=c++11` (or later) is in your compile command. The project uses those headers for the AI thinking delay, and they require C++11 or newer.
 
---
 
## How to play
 
When you run the game, the first thing you'll see is the board printed with numbered positions:
 
```
1|2|3
_____
4|5|6
_____
7|8|9
```
 
Those numbers are your move inputs. Position 1 is top-left, position 9 is bottom-right. When it's your turn, you just type the number of the cell you want and press Enter. The board updates immediately and switches to the next player's turn.
 
Before the first move, the game will ask you a few quick questions:
 
**1. Play vs AI?**
Type `y` to play against the AI, or `n` for two-player mode.
 
**2. Pick a difficulty** (only appears in AI mode):
```
1. Easy
2. Medium
3. Hard
```
Type 1, 2, or 3.
 
**3. Who goes first?**
```
Who starts first(x/o)?
```
Type `x` or `o`. Whoever you pick goes first and the AI gets the other symbol.
 
From there you just play. If you enter a position that's already taken or out of range, the game will tell you and ask again — it won't crash or silently accept a bad move.
 
**Winning and drawing:** Three in a row — horizontally, vertically, or diagonally — wins the game. If all nine cells are filled and nobody has three in a row, it's a draw. Either way, after the match ends you'll be asked if you want to play again. Type `y` to reset the board and go again, or `n` to exit.
 
---
 
## How the AI works
 
This is the interesting part.
 
The three difficulty levels aren't just "easy, medium, hard" in the sense of the AI making fewer or more mistakes. They each use a fundamentally different decision-making strategy, and understanding how they differ is actually the whole point of the project.
 
### Easy — the random agent
 
The easy agent collects all the cells on the board that aren't taken yet, picks one at random using `rand()`, and plays there. That's literally it. No lookahead, no evaluation, no rules. It has no idea whether a move is good or bad. You'll beat it most of the time just by playing normally, but it'll occasionally stumble into a win or a block purely by chance.
 
### Medium — the heuristic agent
 
The medium agent has two rules it checks in order before every move:
 
**Rule 1 — Try to win:** Go through every empty cell and temporarily place its own marker there. If that move would create three in a row, take it immediately.
 
**Rule 2 — Try to block:** If no winning move exists, go through every empty cell and temporarily place *your* marker there. If you'd win from that position, block it.
 
**Rule 3 — Fallback:** If neither of the above applies, just take the first available cell scanning left-to-right, top-to-bottom.
 
This is a solid improvement over random play — it'll never miss a free win, and it'll never let you win in one obvious move. But it has a real exploitable weakness: if you set up a **fork** — two separate ways to win at the same time — it can only block one of them on the next move, and you take the other. The medium agent has no concept of forks and no way to see two moves ahead.
 
### Hard — the Minimax agent
 
The hard agent uses the **Minimax algorithm**, which is a recursive adversarial search. Instead of following rules or picking randomly, it actually simulates the entire rest of the game from the current board position — every possible move you could make, every response it could make, every counter-response, all the way down to the final outcome — and then picks the move that guarantees the best possible result for itself, assuming you also play perfectly.
 
The way it works:
 
- It tries placing its marker in every empty cell
- For each placement, it recursively asks: "if the human plays optimally from here, what's the best I can guarantee?"
- The human player is modeled as someone who always tries to *minimize* the AI's score
- Terminal states get scored: **+10** if the AI wins, **-10** if the human wins, **0** for a draw
- The AI bubbles the best score up through the recursion tree and plays the move that leads to it
 
Here's a rough diagram of what that looks like in code:
 
```
get_best_move() — considers every empty cell
│
├── Places AI marker in cell A → calls minimax(human's turn)
│     ├── evaluate() says AI already won → return +10
│     ├── No moves left → return 0 (draw)
│     └── For each empty cell remaining:
│           Places human marker → calls minimax(AI's turn)
│                 └── ... recurses until board is full or someone wins
│           Restores the cell after each simulation
│     Returns the worst outcome the human can force
│
├── Places AI marker in cell B → same process...
├── Places AI marker in cell C → same process...
│
└── Returns whichever cell had the highest guaranteed score
```
 
Because Tic-Tac-Toe has at most 9! = 362,880 possible game states (and many branches end early when someone wins), this search finishes in a few milliseconds even without any optimization. The 700ms "thinking" delay you see in the game is entirely cosmetic — the AI already knows its move before the delay even starts.
 
The result is an agent that is **provably optimal**. Against perfect play from you, every game ends in a draw. Against anything less than perfect play, the AI will win.
 
---
 
## Why three difficulty levels?
 
This was actually one of the more interesting design decisions in the project.
 
You might wonder: why not just use Minimax for all three levels? The answer is that difficulty in a game isn't about how smart the AI is internally — it's about giving the player a challenge that matches their skill. An unbeatable AI at every level would make the game unplayable for someone who's new to Tic-Tac-Toe strategy.
 
The three agents are designed to expose different levels of strategic thinking:
- **Easy** is for someone who just wants to click around and have fun
- **Medium** rewards players who understand the basics — don't let your opponent win in one move, set up your own wins — but punishes anyone who knows how to fork
- **Hard** is for anyone who wants to genuinely test whether they know the optimal strategy (spoiler: it always ends in a draw if you do)
 
This stratification also made the project more interesting to build. It's easy to write a random agent or a perfect agent. Writing a *deliberately imperfect but still smart* agent — the Medium mode — required more thought about what "medium difficulty" actually means in terms of game-playing behavior.
 
---
 
## Code layout
 
The entire project is one file, `Main.cpp`. There are no headers to manage, no build system to configure, no folders to navigate. Here's how it's organized internally:
 
```
Main.cpp
│
├── Global state
│     board[][]     — the 3x3 grid, chars '1'–'9' for empty cells, 'X'/'O' when taken
│     original[][]  — a clean copy used to reset the board between matches
│     valid         — flag used by update_board() and the input loop
│
├── main()
│     Game loop: mode selection, difficulty selection, turn switching,
│     win/draw detection, replay prompt
│
├── Board functions
│     print_board()          draws the current grid to the console
│     update_board()         places a move, validates it, sets the valid flag
│     check_winner()         checks all rows, columns, and both diagonals
│     clear_board()          resets every cell back to its original digit
│
└── AI functions
      get_random_move()      Easy — random cell from available positions
      get_ai_move()          Medium — win/block heuristic
      evaluate()             scores the board for Minimax (+10, -10, or 0)
      minimax()              recursive game-tree search
      get_best_move()        Hard — calls minimax for every candidate move, returns the best
```
 
One design choice worth noting: empty cells on the board store their own position digit (`'1'` through `'9'`). This means the board is self-describing — you don't need a separate data structure to track which positions are available or to display position hints to the player. The AI functions also use this directly: a cell is "available" if and only if its value is not `'X'` or `'O'`.
 
---
 
## Function reference
 
| Function | What it does |
|---|---|
| `main()` | The full game loop — handles mode/difficulty prompts, turn alternation, win and draw detection, and the rematch prompt |
| `print_board()` | Prints the current 3x3 grid to stdout |
| `update_board(pos, player)` | Validates and applies a move; sets `valid = true` on success, prints an error on failure |
| `check_winner()` | Returns `true` if any row, column, or diagonal has three matching characters |
| `clear_board()` | Copies `original` back into `board`, resetting all cells to their digit values |
| `get_random_move()` | Easy AI — collects all non-X/O cells, returns one at random |
| `get_ai_move(ai, human)` | Medium AI — simulates win/block moves, falls back to first available |
| `evaluate(ai, human)` | Scores the board state: +10 for AI win, -10 for human win, 0 for anything else |
| `minimax(isMax, ai, human)` | Recursive Minimax search — returns the optimal value of the current position |
| `get_best_move(ai, human)` | Hard AI — tries every empty cell via minimax, returns the position with the highest score |
 
---
 
## Things I learned building this
 
**Minimax is much easier to understand once you implement it.** I'd read about it in course material and it made sense on paper, but actually writing it — figuring out when to stop recursing, how to propagate scores up the tree, how to restore the board after each simulated move — made the algorithm click in a way that reading about it never did.
 
**Board representation matters more than you'd think.** Using the position digit as the cell's value (so `'5'` means "cell 5 is empty") turned out to be a really clean design. It made the rendering logic, the validation logic, and the AI logic all share the same representation without any translation layer.
 
**Difficulty is a design problem, not just a technical one.** The hardest part of the medium agent wasn't implementing it — it was deciding *what* it should and shouldn't know. Getting that balance right so it feels like a fair challenge took more thought than I expected.
 
**Debugging recursive state mutation is annoying.** Early on, the Minimax agent had a bug where it wasn't correctly restoring the board after simulating moves. The result was that it would think cells were occupied when they weren't, leading to weird behavior that only showed up several moves into a game. The fix — always store a backup char before modifying a cell and restore it unconditionally after — is simple, but finding the bug took a while.
 
---
 
## Honest limitations
 
**No alpha-beta pruning.** Minimax explores the entire game tree, including branches that are clearly worse than what's already been found. Alpha-beta pruning would skip those branches and could reduce the search space by roughly half. For a 3x3 board it genuinely doesn't matter — the search is fast enough either way — but it's the obvious next step if you wanted to apply this approach to a larger game.
 
**Medium mode can't see forks.** This is intentional. If the medium agent could detect and respond to forks it would play nearly as well as the hard agent, which defeats the point of having a medium difficulty. The fork blind spot is the feature, not the bug.
 
**Non-numeric input at the position prompt isn't fully handled.** If you type a letter instead of a number when the game asks for your move, the behavior depends on the platform and compiler. The game handles invalid positions (already taken, out of range) correctly, but it doesn't explicitly guard against cin receiving something that isn't a digit character.
 
**Single file, no tests.** The project is self-contained in one `.cpp` file, which is fine for this scale but wouldn't be the right approach for anything larger. There's also no automated test suite — correctness was verified by playing through many games and checking edge cases manually.
 
---
 
## License
 
Free to use for learning, coursework, and personal projects.
