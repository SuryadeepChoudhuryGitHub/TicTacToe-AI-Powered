# Console Tic-Tac-Toe with Multi-Level AI (Minimax)
 
> **BYOP Capstone — Artificial Intelligence and Machine Learning Course**  
> Implemented in C++11 | No external dependencies
 
---
 
## Table of Contents
 
- [Overview](#overview)
- [Features](#features)
- [AI Agent Design](#ai-agent-design)
- [How Minimax Works](#how-minimax-works)
- [Requirements](#requirements)
- [Building the Project](#building-the-project)
- [Running the Game](#running-the-game)
- [Gameplay Guide](#gameplay-guide)
- [Project Structure](#project-structure)
- [Function Reference](#function-reference)
- [Known Limitations](#known-limitations)
- [License](#license)
 
---
 
## Overview
 
This project is a fully playable console Tic-Tac-Toe game built in C++, designed as a hands-on exploration of adversarial AI and game theory. The core contribution is a three-tier AI system — ranging from a random agent to an **unbeatable Minimax engine** — that demonstrates how artificial intelligence can achieve provably optimal decision-making in a two-player zero-sum game.
 
The problem being solved: *how should a computer agent choose moves in an adversarial environment where the opponent also plays rationally?* This is the central question of adversarial search in AI, and Tic-Tac-Toe is the ideal minimal environment to study it.
 
---
 
## Features
 
| Feature | Description |
|---|---|
| Player vs Player | Two humans share the keyboard and alternate turns |
| Player vs AI | Single-player mode against one of three AI agents |
| Easy difficulty | Random agent — picks any available cell at random |
| Medium difficulty | Heuristic agent — wins if possible, blocks if necessary |
| Hard difficulty | Minimax agent — provably unbeatable, always forces draw or win |
| First-turn choice | Player selects whether X or O goes first each match |
| Input validation | Rejects already-occupied or out-of-range positions |
| AI thinking delay | 700ms pause before AI announces its move |
| Match replay | Replay without restarting the program; difficulty carries over |
 
---
 
## AI Agent Design
 
The three difficulty levels each use a distinct decision strategy, chosen to provide meaningful challenge progression:
 
### Easy — Random Agent
Collects all unoccupied cells and selects one at random using `rand()`. Has no strategic awareness. Appropriate for beginners or young players. Will occasionally win by luck.
 
### Medium — Heuristic Agent
Applies a two-step priority rule on every turn:
1. **Win check:** Simulate placing its own marker in every available cell. If any placement causes a win, take that move immediately.
2. **Block check:** Simulate placing the human's marker in every available cell. If any placement would cause the human to win, block it.
3. **Fallback:** If neither condition applies, take the first available cell (top-left to bottom-right scan order).
 
This agent defeats random play consistently but is **beatable by a fork strategy** — establishing two simultaneous winning threats that a single block cannot stop.
 
### Hard — Minimax Agent
Implements the **Minimax algorithm**, a recursive adversarial search that explores the complete game tree from the current board state:
 
- The AI (maximizer) always picks the move that yields the **highest guaranteed score**.
- The human (minimizer) is assumed to always pick the move that yields the **lowest guaranteed score**.
- Terminal states score: **+10** (AI wins), **-10** (human wins), **0** (draw).
- The AI can never lose. With optimal human play, every game ends in a draw.
 
---
 
## How Minimax Works
 
```
get_best_move() called on current board
│
├── For each empty cell:
│     Place AI marker → call minimax(isMax=false)
│     │
│     ├── evaluate() → +10 (AI win), return immediately
│     ├── No moves left → return 0 (draw)
│     └── For each empty cell:
│           Place human marker → call minimax(isMax=true)
│           ... (recurse until terminal)
│           Restore cell
│     Restore cell
│
└── Return the cell with highest minimax score
```
 
Because the full game tree has at most 9! = 362,880 leaf nodes and many branches terminate early on a detected win, the search completes in under 5ms on any modern hardware.
 
---
 
## Requirements
 
- **C++11** compatible compiler (`g++`, `clang++`, or MSVC)
- Standard library only — no external packages needed
- Terminal / command prompt
 
---
 
## Building the Project
 
### Linux / macOS
```bash
g++ -std=c++11 -o tictactoe Main.cpp
```
 
### Windows (MinGW / MSYS2)
```bash
g++ -std=c++11 -o tictactoe.exe Main.cpp
```
 
### Windows (MSVC — Developer Command Prompt)
```bash
cl /EHsc /std:c++11 Main.cpp /Fe:tictactoe.exe
```
 
> **Tip:** If your compiler reports `thread` not found, ensure you are using `-std=c++11` or later. The project uses `<chrono>` and `<thread>` for the AI delay.
 
---
 
## Running the Game
 
```bash
./tictactoe        # Linux / macOS
tictactoe.exe      # Windows
```
 
---
 
## Gameplay Guide
 
### Step 1 — The board is displayed
```
1|2|3
_____
4|5|6
_____
7|8|9
```
Numbers 1–9 show the position of each cell. Enter the number of the cell you want to claim on your turn.
 
### Step 2 — Choose game mode
```
Play vs AI? (y/n):
```
Enter `y` for single-player (vs AI) or `n` for two-player mode.
 
### Step 3 — Select difficulty (AI mode only)
```
Select difficulty:
1. Easy
2. Medium
3. Hard
```
 
### Step 4 — Choose who goes first
```
Who starts first(x/o)?
```
The player who starts is assigned that character. The AI is assigned the other.
 
### Step 5 — Take turns
On your turn, type the number of the cell you want (1–9) and press Enter. The AI will automatically take its turn after the delay.
 
### Step 6 — Win, draw, or replay
- Three in a row (horizontal, vertical, or diagonal) wins.
- If all 9 cells fill with no winner, the game is a draw.
- After each match you are asked `Play another match (y/n)?`. Entering `y` resets the board with the same settings.
 
---
 
## Project Structure
 
```
.
├── Main.cpp          # Complete source — game loop, board logic, and AI agents
└── README.md         # This file
```
 
The logical structure inside `Main.cpp`:
 
```
Main.cpp
├── Global state         (board, original, valid flag)
├── main()               (game loop, mode/difficulty selection, turn management)
├── Board functions      (print_board, update_board, check_winner, clear_board)
└── AI functions
    ├── get_random_move()   Easy agent
    ├── get_ai_move()       Medium agent
    ├── evaluate()          Minimax scoring function
    ├── minimax()           Recursive Minimax search
    └── get_best_move()     Hard agent — calls minimax for each candidate move
```
 
---
 
## Function Reference
 
| Function | Purpose |
|---|---|
| `main()` | Top-level game loop: mode selection, turn alternation, win/draw detection, replay |
| `print_board()` | Renders the 3x3 board to stdout with current cell values |
| `update_board(pos, player)` | Places player's marker at position; validates the move |
| `check_winner()` | Scans rows, columns, and diagonals for three-in-a-row |
| `clear_board()` | Restores all cells to their original digit values (1–9) |
| `get_random_move()` | Easy AI: returns a random available cell character |
| `get_ai_move(ai, human)` | Medium AI: win/block heuristic returning best positional char |
| `evaluate(ai, human)` | Scores the current board: +10 AI win, -10 human win, 0 otherwise |
| `minimax(isMax, ai, human)` | Recursive game-tree search; returns the minimax value of the position |
| `get_best_move(ai, human)` | Hard AI: tries all moves via minimax, returns the highest-scoring one |
 
---
 
## Known Limitations
 
- **No alpha-beta pruning:** The Minimax search is exhaustive. Fine for Tic-Tac-Toe but would not scale to larger games without pruning.
- **No fork detection in Medium mode:** The heuristic agent cannot detect or prevent fork setups — a deliberate design choice to keep Medium beatable.
- **Non-numeric input not fully guarded:** Entering a letter instead of a digit on the position prompt may cause unexpected behavior depending on platform.
 
---
 
## License
 
This project is provided for educational and personal use.