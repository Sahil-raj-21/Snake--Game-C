#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_OBS 20

// ---------- GLOBAL VARIABLES ----------
int gameOver, paused = 0;
int x, y, foodX, foodY;
int score, level, speed;
int tailX[100], tailY[100], nTail;
int obsX[MAX_OBS], obsY[MAX_OBS], obsCount;
char player[30];
int highScore;

// ---------- DIRECTION ----------
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

// ---------- FUNCTION DECLARATIONS ----------
void Setup();
void Draw();
void Input();
void Logic();
void LevelCheck();
void Obstacles();
void Login();
void LoadHighScore();
void SaveHighScore();
void Color(int);

// ---------- MAIN ----------
int main() {
    char ch;
    Login();
    LoadHighScore();

    do {
        Setup();
        while (!gameOver) {
            Draw();
            Input();
            if (!paused) {
                Logic();
                LevelCheck();
            }
            Sleep(speed);
        }

        SaveHighScore();
        system("cls");
        printf("\nGAME OVER\n");
        printf("Player : %s\n", player);
        printf("Score  : %d\n", score);
        printf("High   : %d\n", highScore);
        printf("\nPress Y to play again: ");
        ch = getch();

    } while (ch == 'y' || ch == 'Y');

    return 0;
}

// ---------- LOGIN ----------
void Login() {
    system("cls");
    printf("Enter Player Name: ");
    gets(player);
}

// ---------- SETUP ----------
void Setup() {
    gameOver = 0;
    paused = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    score = 0;
    level = 1;
    speed = 140;
    nTail = 0;
    foodX = rand() % (WIDTH - 2) + 1;
    foodY = rand() % (HEIGHT - 2) + 1;
    Obstacles();
}

// ---------- DRAW ----------
void Draw() {
    system("cls");

    Color(7);
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) printf("#");

            if (i == y && j == x) {
                Color(10); printf("O");
            }
            else if (i == foodY && j == foodX) {
                Color(12); printf("F");
            }
            else {
                int printed = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        Color(2); printf("o");
                        printed = 1; break;
                    }
                }
                for (int o = 0; o < obsCount; o++) {
                    if (obsX[o] == j && obsY[o] == i) {
                        Color(4); printf("X");
                        printed = 1; break;
                    }
                }
                if (!printed) printf(" ");
            }

            if (j == WIDTH - 1) printf("#");
        }
        printf("\n");
    }

    Color(7);
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    printf("Player: %s | Score: %d | Level: %d | High: %d\n",
           player, score, level, highScore);

    if (paused) printf("== PAUSED ==\n");
}

// ---------- INPUT ----------
void Input() {
    if (_kbhit()) {
        char ch = _getch();
        switch (ch) {
            case 'a': if (dir != RIGHT) dir = LEFT; break;
            case 'd': if (dir != LEFT) dir = RIGHT; break;
            case 'w': if (dir != DOWN) dir = UP; break;
            case 's': if (dir != UP) dir = DOWN; break;
            case 'p': paused = !paused; break;
            case 'x': gameOver = 1; break;
        }
    }
}

// ---------- LOGIC ----------
void Logic() {
    int prevX = tailX[0], prevY = tailY[0];
    tailX[0] = x; tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        int px = tailX[i], py = tailY[i];
        tailX[i] = prevX; tailY[i] = prevY;
        prevX = px; prevY = py;
    }

    switch (dir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
    }

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        gameOver = 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;

    for (int i = 0; i < obsCount; i++)
        if (obsX[i] == x && obsY[i] == y)
            gameOver = 1;

    if (x == foodX && y == foodY) {
        score += 10;
        foodX = rand() % (WIDTH - 2) + 1;
        foodY = rand() % (HEIGHT - 2) + 1;
        nTail++;
    }
}

// ---------- LEVELS ----------
void LevelCheck() {
    if (score >= 150) { level = 4; speed = 60; }
    else if (score >= 100) { level = 3; speed = 80; }
    else if (score >= 50) { level = 2; speed = 100; }
    Obstacles();
}

// ---------- OBSTACLES ----------
void Obstacles() {
    obsCount = level * 4;
    for (int i = 0; i < obsCount; i++) {
        obsX[i] = rand() % (WIDTH - 2) + 1;
        obsY[i] = rand() % (HEIGHT - 2) + 1;
    }
}

// ---------- HIGH SCORE ----------
void LoadHighScore() {
    FILE *fp = fopen("highscore.txt", "r");
    if (fp) {
        fscanf(fp, "%d", &highScore);
        fclose(fp);
    } else highScore = 0;
}

void SaveHighScore() {
    if (score > highScore) {
        FILE *fp = fopen("highscore.txt", "w");
        fprintf(fp, "%d", score);
        fclose(fp);
    }
}

// ---------- COLOR ----------
void Color(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
