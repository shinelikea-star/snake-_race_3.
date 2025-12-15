#include <stdio.h>
#include <conio.h>
#include <windows.h> // For Sleep() and system("cls")
#include <stdlib.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20

int gameOver;
int x, y;
int foodX, foodY;
int score;

int tailX[100], tailY[100];
int nTail;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

void Setup() {
    gameOver = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    srand(time(0));
    // Food must spawn inside the walls (1 to WIDTH-2 and 1 to HEIGHT-2)
    foodX = rand() % (WIDTH - 2) + 1;
    foodY = rand() % (HEIGHT - 2) + 1;
    score = 0;
    nTail = 0;
}

void Draw() {
    // system("cls") is slow, but effective for console games.
    system("cls"); 

    // Top wall
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    // Side walls and game area
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {

            if (j == 0) // Left Wall
                printf("#");

            if (i == y && j == x) // Snake Head
                printf("O");
            else if (i == foodY && j == foodX) // Food
                printf("F");
            else { // Empty space or Tail
                int printTail = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o");
                        printTail = 1;
                        break;
                    }
                }
                if (!printTail)
                    printf(" ");
            }

            if (j == WIDTH - 1) // Right Wall
                printf("#");
        }
        printf("\n");
    }

    // Bottom wall
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    printf("Score: %d\n", score);
    printf("Controls: W(up) A(left) S(down) D(right) | X to quit\n");
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                // Prevent immediate reverse movement (LEFT from RIGHT)
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd':
                // Prevent immediate reverse movement (RIGHT from LEFT)
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'w':
                // Prevent immediate reverse movement (UP from DOWN)
                if (dir != DOWN) dir = UP;
                break;
            case 's':
                // Prevent immediate reverse movement (DOWN from UP)
                if (dir != UP) dir = DOWN;
                break;
            case 'x':
                gameOver = 1;
                break;
            default:
                break;
        }
    }
}

void Logic() {
    // 1. Move the tail segments
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    
    // The previous head position becomes the first tail segment
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // 2. Move the snake head
    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    // 3. Wall Collision: Game Over if snake hits the boundary walls.
    // The playable area is 1 to WIDTH-2 and 1 to HEIGHT-2
    // The walls are at x=0, x=WIDTH-1, y=0, y=HEIGHT-1
    if (x <= 0 || x >= WIDTH - 1 || y <= 0 || y >= HEIGHT - 1) {
        gameOver = 1;
    }

    // 4. Self Collision
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = 1;
        }
    }

    // 5. Food Collision
    if (x == foodX && y == foodY) {
        score += 10;
        // Spawn food inside the walls (1 to WIDTH-2 and 1 to HEIGHT-2)
        // Ensure food does not spawn on the snake's tail (optional but good)
        do {
            foodX = rand() % (WIDTH - 2) + 1;
            foodY = rand() % (HEIGHT - 2) + 1;
        } while (x == foodX && y == foodY); // Ensure food doesn't spawn on the head for next frame

        // Increase tail a
        nTail++;
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); // Increased speed to 100ms for better responsiveness
    }

    system("cls");
    printf("--- GAME OVER ---\n");
    printf("Final Score: %d\n", score);
    printf("-----------------\n");
    printf("Press any key to exit...");
    _getch();

    return 0;
}