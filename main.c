#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define SIZE 8
#ifdef _WIN32
#define OS "nt"
#elif __unix__
#define OS "posix"
#endif 

int gameOver = 0;
int win = 0;
int moveNumber = 0;
int lastInvalidCommand = 0;
char lastCommand[101];
int showRules = 0;

void printGamePlayRules() {
    printf("MOVEMENT:\n\n");
    printf("    For going UP you need to press w|W\n");
    printf("    For going DOWN you need to press s|S\n");
    printf("    For going LEFT you need to press a|A\n");
    printf("    For going RIGHT you need to press d|D\n");
    printf("    After you choose the action you need to press enter\n");
    printf("    For exiting the game write \"exit\"\n");
    printf("    For viewing rules write \"rules\"\n");
    printf("\n");
    printf("Your moves number is %d\n", moveNumber);

    if (lastInvalidCommand == 1) {
        printf("\nYour \"%s\" command is invalid\n",lastCommand);
        lastInvalidCommand = 0;
    }    
    
    if (showRules == 1) {
        printf("\nObjective of this game is to make number 9\n");
        printf("Every time when you make a move a random 1 block will be generated\n");
        printf("To make a bigger number you need to link to same numbers and you will obtain value + 1\n");
        showRules = 0;
    }

    printf("\n");
}

void printEmptyLine() {
    for (int i = 0; i < SIZE; i ++) {
        printf(" ---");
    }
    printf("\n");
}

void printMap(int gameMap[SIZE][SIZE]) {
    if (OS == "nt"){
        system("cls");
    }
    if (OS == "posix") {
        system("clear");
    }

    printGamePlayRules();

    for (int i = 0; i < SIZE; i ++) {
        printEmptyLine();

        for (int j = 0; j < SIZE; j ++) {
            if (j == 0) {
                printf("|");
            }

            if (gameMap[i][j] != 0) {
                printf(" %d ", gameMap[i][j]);
            }
            else {
                printf("   ");
            }
            printf("|");
        }
        printf("\n");
    }

    printEmptyLine();
}

int existsZerosInMatrix(int gameMap[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i ++) {
        for (int j = 0; j < SIZE; j ++) {
            if (gameMap[i][j] == 0) {
                return 1;
            } 
        }
    }
    return 0;
}

int canMakeMove(int gameMap[SIZE][SIZE]) {
    for (int i = 0; i < SIZE - 1; i ++) {
        for (int j = 0; j < SIZE - 1; j ++) {
            if (gameMap[i][j] == gameMap[i][j + 1] || gameMap[i][j] == gameMap[i+1][j]) {
                return 1;
            }
        }
    }
    return 0;
}

void initGameMatrix(int gameMap[SIZE][SIZE]) {
    srand(time(NULL));

    for (int i = 0; i < SIZE; i ++) {
        for (int j = 0; j < SIZE; j ++) {
            gameMap[i][j] = 0;
        }
    }
}

int verifyWining(int gameMap[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i ++) {
        for (int j = 0; j < SIZE; j ++) {
            if (gameMap[i][j] == 9) {
                return 1;
            } 
        }
    }
    return 0;
}

void setSpawnPosition(int gameMap[SIZE][SIZE]) {
    if (verifyWining(gameMap)) {
        win = 1;
        gameOver = 0;
        return;
    }

    if (canMakeMove(gameMap) == 0 || existsZerosInMatrix(gameMap) == 0) {
        gameOver = 1;
        return;
    }

    int i = rand() % SIZE;
    int j = rand() % SIZE;

    while (gameMap[i][j] != 0) {
        i = rand() % SIZE;
        j = rand() % SIZE;
    }

    gameMap[i][j] = 1;
}

void makeDownMove(int gameMap[SIZE][SIZE]) {
    moveNumber ++;

    for (int j = 0; j < SIZE; j ++) {
        int nrDifZero = 0, idx = 0;
        int arrAux[SIZE];
        
        // count number of numbers diff by zero

        for (int i = SIZE-1; i >= 0; i --) {
            if (gameMap[i][j] != 0) {
                arrAux[nrDifZero ++] = gameMap[i][j];
            }
        }

        // put all numbers together

        for (int i = SIZE - 1; i >= 0; i --) {
            if (idx < nrDifZero) {
                gameMap[i][j] = arrAux[idx++];
            } else {
                gameMap[i][j] = 0;
            }
        }

        // make increasing

        int lst = 1;
        for (int i = SIZE - 1;i >= 1; i--) {
            if (gameMap[i][j] == gameMap[i-1][j] && gameMap[i][j] != 0) {
                gameMap[i][j] ++;

                for (int aux = i-1; aux >= lst; aux --) {
                    gameMap[aux][j] = gameMap[aux-1][j];
                } 
                gameMap[lst-1][j] = 0;
                lst ++;
            }
        }
    }
}

void makeUpMove(int gameMap[SIZE][SIZE]) {
    moveNumber ++;

    for (int j = 0; j < SIZE; j ++) {
        int nrDifZero = 0, idx = 0;
        int arrAux[SIZE];
        
        // count number of numbers diff by zero

        for (int i = 0; i < SIZE; i ++) {
            if (gameMap[i][j] != 0) {
                arrAux[nrDifZero ++] = gameMap[i][j];
            }
        }

        // put all numbers together

        for (int i = 0; i < SIZE; i ++) {
            if (idx < nrDifZero) {
                gameMap[i][j] = arrAux[idx++];
            } else {
                gameMap[i][j] = 0;
            }
        }

        // make increasing

        int lst = SIZE - 2;
        for (int i = 0; i < SIZE - 1; i ++) {
            if (gameMap[i][j] == gameMap[i+1][j] && gameMap[i][j] != 0) {
                gameMap[i][j] ++;

                for (int aux = i+1; aux <= lst; aux ++) {
                    gameMap[aux][j] = gameMap[aux+1][j];
                } 
                gameMap[lst+1][j] = 0;
                lst --;
            }
        }
    }
}

void makeLeftMove(int gameMap[SIZE][SIZE]) {
    moveNumber ++;

    for (int i = 0; i < SIZE; i ++) {
        int nrDifZero = 0, idx = 0;
        int arrAux[SIZE];
        
        // count number of numbers diff by zero

        for (int j = 0; j < SIZE; j ++) {
            if (gameMap[i][j] != 0) {
                arrAux[nrDifZero ++] = gameMap[i][j];
            }
        }

        // put all numbers together

        for (int j = 0; j < SIZE; j ++)  {
            if (idx < nrDifZero) {
                gameMap[i][j] = arrAux[idx++];
            } else {
                gameMap[i][j] = 0;
            }
        }

        // make increasing

        int lst = SIZE - 2;
        for (int j = 0;j < SIZE -1; j++) {
            if (gameMap[i][j] == gameMap[i][j+1] && gameMap[i][j] != 0) {
                gameMap[i][j] ++;

                for (int aux = j+1; aux <= lst; aux ++) {
                    gameMap[i][aux] = gameMap[i][aux+1];
                } 
                gameMap[i][lst + 1] = 0;
                lst --;
            }
        }
    }
}

void makeRightMove(int gameMap[SIZE][SIZE]) {
    moveNumber ++;

    for (int i = 0; i < SIZE; i ++) {
        int nrDifZero = 0, idx = 0;
        int arrAux[SIZE];
        
        // count number of numbers diff by zero

        for (int j = SIZE-1; j >= 0; j --) {
            if (gameMap[i][j] != 0) {
                arrAux[nrDifZero ++] = gameMap[i][j];
            }
        }

        // put all numbers together

        for (int j = SIZE - 1; j >= 0; j --) {
            if (idx < nrDifZero) {
                gameMap[i][j] = arrAux[idx++];
            } else {
                gameMap[i][j] = 0;
            }
        }

        // make increasing

        int lst = 1;
        for (int j = SIZE - 1;j >= 1; j--) {
            if (gameMap[i][j] == gameMap[i][j-1] && gameMap[i][j] != 0) {
                gameMap[i][j] ++;

                for (int aux = j-1; aux >= lst; aux --) {
                    gameMap[i][aux] = gameMap[i][aux-1];
                } 
                gameMap[i][lst - 1] = 0;
                lst ++;
            }
        }
    }
}

void gameLoop() {
    int gameMap[SIZE][SIZE];
    initGameMatrix(gameMap);
    setSpawnPosition(gameMap);
    
    while (gameOver == 0)
    {
        printMap(gameMap);

        if (!gameOver) {
            printf("\nNew command: ");

            char command[101];
            scanf("%s", command);
            strcpy(lastCommand, command);

            if (strcmp(command, "exit") == 0) {
                printf("\n");
                return;
            }

            if (strcmp(command, "rules") == 0) {
                showRules = 1;
                continue;
            }

            if (strlen(command) != 1 || !strchr("awsdAWSD",command[0])) {
                lastInvalidCommand = 1;
                continue;
            }

            switch (command[0])
            {
                case 's':
                case 'S':
                    makeDownMove(gameMap);
                    break;
                case 'w':
                case 'W':
                    makeUpMove(gameMap);
                    break;
                case 'a':
                case 'A':
                    makeLeftMove(gameMap);
                    break;
                case 'd':
                case 'D':
                    makeRightMove(gameMap);
                    break;
            }

            setSpawnPosition(gameMap);
            printMap(gameMap);
        }
    }

    if (win == 1) {
        printf("\nYou win!!!!\n");
    } else {
        printf("\nYou lose!!!!\n");
    }
}

int main() {
    gameLoop();
    return 0;
}