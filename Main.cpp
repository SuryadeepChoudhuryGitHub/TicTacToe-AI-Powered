#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>


using namespace std;

void print_board();
void update_board(char pos, char player);
bool check_winner();
void clear_board();
char get_random_move();
char get_ai_move(char ai, char human);
int evaluate(char ai, char human);
int minimax(bool isMax, char ai, char human);
char get_best_move(char ai, char human);

vector<vector<char>> board = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
vector<vector<char>> original = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
bool valid = false;

int main(void) {
    char position, player, first, next_mathch;
    int moves = 0;
    bool play = true;

    char aiPlayer = NULL, humanPlayer = NULL;
    int difficulty = 1;
    

    // main loop
    while (play) {
        srand(time(0));
        bool vsAI;
        char choice;
        print_board();
        cout << "Play vs AI? (y/n): ";
        cin >> choice;
        vsAI = (tolower(choice) == 'y');

        if (vsAI) {
            cout << "Select difficulty:\n";
            cout << "1. Easy\n2. Medium\n3. Hard\n";
            cin >> difficulty;
        }


        do {
            cout << "Who starts first(x/o)? ";
            cin >> first;
        } while ((first != 'x' && first != 'X') && (first != 'o' && first != 'O'));

        player = toupper(first);

        char aiPlayer = (player == 'X') ? 'O' : 'X';
        char humanPlayer = player;

        while (true) {
            if (vsAI && player == aiPlayer) {
                if (difficulty == 1) {
                    position = get_random_move();
                }
                else if (difficulty == 2) {
                    position = get_ai_move(aiPlayer, humanPlayer);
                }
                else {
                    position = get_best_move(aiPlayer, humanPlayer);
                }

                cout << "AI is thinking...\n";

                // delay (you can tweak time)
                this_thread::sleep_for(chrono::milliseconds(700));

                cout << "AI chooses: " << position << endl;
                update_board(position, player);
                valid = true;
            }
            else {
                while (valid == false) {
                    printf("%c turn - Enter position:", player);
                    cin >> position;
                    update_board(position, player);
                }
            }
            valid = false;
            moves++;

            bool win = check_winner();

            if (win) {
                cout << player << " Wins!\n";
                break;
            }
            else if (moves == 9) {
                cout << "Draw!\n";
                break;
            }

            player = (player == 'X') ? 'O' : 'X';
        }

        // ask if the user wants to continue
        do {
            cout << "Play another match (y/n): ";
            cin >> next_mathch;
            if (tolower(next_mathch) == 'y') {
                clear_board();
                moves = 0;
            }
            else if (tolower(next_mathch) == 'n') {
                play = false;
                break;
            }
        } while (tolower(next_mathch) != 'y' && tolower(next_mathch) != 'n');
    }
    return 0;
}

void print_board() {
    printf("%c|%c|%c\n_____\n%c|%c|%c\n_____\n%c|%c|%c\n", board[0][0], board[0][1], board[0][2], board[1][0], board[1][1], board[1][2], board[2][0], board[2][1], board[2][2]);
    return;
}

void update_board(char pos, char player) {
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if (pos == board[i][j] && (board[i][j] != 'X' && board[i][j] != 'O')) {
                board[i][j] = player;
                valid = true;
                break;
            }
        }
    }
    if (valid == false) {
        cout << "Invalid or already chosen position!" << endl;
    }
    else {
        print_board();
    }
    return;
}

bool check_winner() {
    // Check horizontaly
    for (size_t i = 0, n = board.size(); i < n; i++) {
        int count = 0;
        for (size_t j = 0, m = board[i].size() - 1; j < m; j++) {
            if (board[i][j] == board[i][j + 1]) {
                count++;
            }
        }
        if (count == 2) {
            return true;
        }
    }

    // Check verticaly
    for (size_t i = 0, n = board.size(); i < n; i++) {
        int count = 0;
        for (size_t j = 0, m = board[i].size() - 1; j < m; j++) {
            if (board[j][i] == board[j + 1][i]) {
                count++;
            }
        }
        if (count == 2) {
            return true;
        }
    }

    // Check diagonal
    if (board[0][0] == board[1][1] && board[0][0] == board[2][2]) {
        return true;
    }
    else if (board[0][2] == board[1][1] && board[0][2] == board[2][0]) {
        return true;
    }
    return false;
}

void clear_board(void) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = original[i][j];
        }
    }
}

//======================================= AI AGENT ====================================

// Easy Mode
char get_random_move() {
    vector<char> available;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                available.push_back(board[i][j]);
            }
        }
    }

    if (!available.empty()) {
        int idx = rand() % available.size();
        return available[idx];
    }

    return '1';
}

// Medium Mode
char get_ai_move(char ai, char human) {
    // 1. Try to win
    for (int i = 1; i <= 9; i++) {
        char pos = i + '0';
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                if (board[r][c] == pos) {
                    board[r][c] = ai;
                    if (check_winner()) {
                        board[r][c] = pos;
                        return pos;
                    }
                    board[r][c] = pos;
                }
            }
        }
    }

    // 2. Try to block human
    for (int i = 1; i <= 9; i++) {
        char pos = i + '0';
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                if (board[r][c] == pos) {
                    board[r][c] = human;
                    if (check_winner()) {
                        board[r][c] = pos;
                        return pos;
                    }
                    board[r][c] = pos;
                }
            }
        }
    }

    // 3. Otherwise pick first available
    for (int i = 1; i <= 9; i++) {
        char pos = i + '0';
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                if (board[r][c] == pos) {
                    return pos;
                }
            }
        }
    }

    return '1'; // fallback
}

// Hard Mode 
int evaluate(char ai, char human) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == ai) return 10;
            if (board[i][0] == human) return -10;
        }

        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == ai) return 10;
            if (board[0][i] == human) return -10;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == ai) return 10;
        if (board[0][0] == human) return -10;
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == ai) return 10;
        if (board[0][2] == human) return -10;
    }

    return 0;
}

int minimax(bool isMax, char ai, char human) {
    int score = evaluate(ai, human);

    if (score == 10 || score == -10)
        return score;

    bool movesLeft = false;
    for (auto& row : board)
        for (auto& cell : row)
            if (cell != 'X' && cell != 'O')
                movesLeft = true;

    if (!movesLeft)
        return 0;

    if (isMax) {
        int best = -1000;

        for (auto& row : board) {
            for (auto& cell : row) {
                if (cell != 'X' && cell != 'O') {
                    char backup = cell;
                    cell = ai;

                    best = max(best, minimax(false, ai, human));

                    cell = backup;
                }
            }
        }
        return best;
    }
    else {
        int best = 1000;

        for (auto& row : board) {
            for (auto& cell : row) {
                if (cell != 'X' && cell != 'O') {
                    char backup = cell;
                    cell = human;

                    best = min(best, minimax(true, ai, human));

                    cell = backup;
                }
            }
        }
        return best;
    }
}

char get_best_move(char ai, char human) {
    int bestVal = -1000;
    char bestMove = '1';

    for (auto& row : board) {
        for (auto& cell : row) {
            if (cell != 'X' && cell != 'O') {
                char backup = cell;
                cell = ai;

                int moveVal = minimax(false, ai, human);

                cell = backup;

                if (moveVal > bestVal) {
                    bestMove = backup;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}