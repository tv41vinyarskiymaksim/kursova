#include <iostream>
using namespace std;
#define SIZE 10

class MasyuSolver {
private:
    const char board[SIZE][SIZE] = {                        //масив початкового поля
        {'@','O','O','@','O','O','@','O','O','@'},
        {'O','@','O','O','@','@','O','O','@','O'},
        {'O','O','@','O','O','@','O','@','O','O'},
        {'O','@','O','@','@','O','@','@','O','@'},
        {'@','O','@','O','@','@','O','O','@','O'},
        {'O','@','@','O','O','@','O','O','O','@'},
        {'O','@','O','@','O','@','O','O','@','O'},
        {'@','O','O','@','O','O','O','@','O','@'},
        {'O','@','@','O','@','O','@','O','@','O'},
        {'@','O','@','@','O','@','@','O','O','@'}
    };

    char solution[2 * SIZE - 1][2 * SIZE - 1];              //масив для збереження поля із шляхом
    int move_X[4] = {0, 1, 0, -1};                          //можливі ходи (вправо, вниз, вліво, вгору)
    int move_Y[4] = {1, 0, -1, 0};

public:
    MasyuSolver() {                                         //конструктор, ініціалізує всі клітинки розв'язку як "пусті"
        for (int i = 0; i < 2 * SIZE - 1; i++)
            for (int j = 0; j < 2 * SIZE - 1; j++)
                solution[i][j] = '*';
    }

    void printBoard() {                                     //функція для виводу початкового поля
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++)
                cout << board[i][j] << "   ";
            cout << endl << endl;
        }
    }

    void printSolution() {                                  //функція для виводу поля із шляхом
        for (int i = 0; i < 2 * SIZE - 1; i++) {
            for (int j = 0; j < 2 * SIZE - 1; j++) {
                if (i % 2 == 0 && j % 2 == 0)
                    cout << board[i / 2][j / 2];
                else if (solution[i][j] == '-')
                    cout << "---";
                else if (solution[i][j] == '|')
                    cout << '|';
                else if (j % 2 == 0)
                    cout << " ";
                else
                    cout << "   ";
            }
            cout << endl;
        }
    }

    bool solve() {                                          //функція для розв'язання задачі
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] != '@') continue;
                int x = i * 2, y = j * 2;
                int dir = 0;
                int next_X = x + move_X[dir];
                int next_Y = y + move_Y[dir];
                if (isValid(next_X, next_Y)) {
                    solution[x][y] = board[i][j];
                    solution[next_X][next_Y] = '-';
                    if (step(next_X, next_Y, x, y, x, y, dir, 0, 1))
                        return true;
                    solution[next_X][next_Y] = '*';
                    solution[x][y] = '*';
                }
            }
        }
        return false;
    }

private:
    bool isValid(int x, int y) const {                      //перевірка чи наступна клітинка не вийшла за межі поля та не була використана
        return (x >= 0 && x < 2 * SIZE - 1 && y >= 0 && y < 2 * SIZE - 1 && solution[x][y] == '*');
    }

    bool step(int x, int y, int start_X, int start_Y, int prev_X, int prev_Y, int dir, int count_W, int move) { //рекурсивна функція для пошуку наступного кроку
        for (int turn = -1; turn <= 1; turn++) {            //цикл для спроби піти: проти годинникової стрілки (-1), прямо (0) та за годиниковою стрілкою (1)
            if ((x % 2 != 0 || y % 2 != 0) && turn != 0) continue;
            int newDir = (dir + turn + 4) % 4;
            int next_X = x + move_X[newDir];
            int next_Y = y + move_Y[newDir];

            if (!isValid(next_X, next_Y) && !(next_X == start_X && next_Y == start_Y)) continue;

            bool rule = true;

            if (x % 2 == 0 && y % 2 == 0) {                 //перевірка додаткових умов
                char ch = board[x / 2][y / 2];
                if (ch == '@') {
                    if (turn == 0)
                        rule = false;
                    else
                        count_W = 0;
                } else {
                    if (turn != 0)
                        rule = false;
                    else {
                        count_W++;
                        if (count_W == 3)
                            rule = false;
                    }
                }
                solution[x][y] = ch;
            } else {
                char prev_CH = board[prev_X / 2][prev_Y / 2];
                char next_CH = board[next_X / 2][next_Y / 2];
                if (prev_CH == '@' && next_CH == '@') rule = false;
                solution[x][y] = (dir % 2 == 0) ? '-' : '|';
            }

            if (!rule) continue;

            if (next_X == start_X && next_Y == start_Y)
                return newDir == 3;

            if (step(next_X, next_Y, start_X, start_Y, x, y, newDir, count_W, move + 1))
                return true;

            solution[next_X][next_Y] = '*';                 //відкат назад
        }
        return false;
    }
};

int main() {
    MasyuSolver masyu;
    cout << "Дано:" << endl;
    masyu.printBoard();
    if (masyu.solve()) {
        cout << "Результат:" << endl;
        masyu.printSolution();
    } else {
        cout << "Рішення не існує!" << endl;
    }
    return 0;
}
