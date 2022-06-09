#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include <stdlib.h>

# define SNAKE_SPEED1 100
# define SNAKE_SPEED2 300
# define SNAKE_SPEED3 700
# define SNAKE_SPEED4 1500
# define SNAKE_SPEED5 3000

# define LIFE 10
# define MAP_LENG 30
# define MAP_HEIGHT 30

#define TRUE 1
#define FALSE 0

#define RIGHT 1
#define LEFT 2
#define UP 3
#define DOWN 4

typedef struct _loc {
	int xpos;
	int ypos;
}Loc;

typedef struct _snake {
	int length;
	Loc body[200];
	int direction;
}Snake;

typedef struct _apple {
	Loc loc;
}Apple;


void ClearLindeFromReadBuffer();
void decorateConsole();
int makeRandomNum();
void gotoxy(int x, int y);
void removeConsoleCursor();

void removeConsoleCursor() {
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void ClearLindeFromReadBuffer(void) {
	while (getchar() != '\n');
}
void clearScreen() {
	system("cls");
}
void decorateConsole() {
	system("mode con cols=100 lines=40 | title 뱀게임");
}

int makeRandomNum() {
	srand(time(0));
	return rand() % 100 + 1;
}

void gotoxy(int x, int y) {
	COORD pos = { x * 2,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void drawTitle() {
	printf("\n\n\n");
	printf("             #      #    #  #                  #  #        ##       #    \n");
	printf("             #      #    #  #     #########    #  #      ##  ##     #     \n");
	printf("             ########    ####             #    #  #    ##      ##   #    \n");
	printf("             #      #    #  #             # ####  #      ##  ##     #    \n");
	printf("             ########    #  #             #    #  #        ##       #         \n");
	printf("               ############               #    #  #        #########             \n");
	printf("               #          #                    #  #        #       #   \n");
	printf("               ############                    #  #        #########           \n");
	printf(" \n\n\n");
}

void printMenu() {
	printf("\n\n\n");
	printf("                              1) 게임 시작   \n\n");
	printf("                              2) 게임 방법   \n\n");
	printf("                              3) 나가기   \n\n");
	printf("                            숫자 1, 2, 3 중 입력\n");
}

void printMap() {
	int i, j;
	printf("\n");
	printf(" ");
	for (i = 0; i < MAP_LENG * 2; i++)
		printf("=");
	for (i = 0; i < MAP_HEIGHT - 2; i++) {
		printf("\n");
		printf("ㅐ");
		for (j = 0; j < MAP_LENG * 2; j++)
			printf(" ");
		printf("ㅐ");
	}
	printf("\n");
	printf(" ");
	for (i = 0; i < MAP_LENG * 2; i++)
		printf("=");
	printf("\n");
}

Loc ReturnPresentCurLoc() {
	Loc present;
	CONSOLE_SCREEN_BUFFER_INFO presentCur;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	present.xpos = presentCur.dwCursorPosition.X;
	present.ypos = presentCur.dwCursorPosition.Y;
}

void gotoxyPrintStr(int x, int y, char* str) {
	gotoxy(x, y);
	printf("%s", str);
}
void gotoxyPrintChar(int x, int y, char ch) {
	gotoxy(x, y);
	printf("%c", ch);
}


void snakeInit(Snake* snake) {
	snake->length = 2;
	snake->direction = RIGHT;
	(snake->body)[0].xpos = MAP_LENG / 2;
	(snake->body)[0].ypos = MAP_HEIGHT / 2;
	(snake->body)[1].xpos = MAP_LENG / 2 - 1;
	(snake->body)[1].ypos = MAP_HEIGHT / 2;
}

void printSnake(Snake* snake) {
	int i;
	gotoxyPrintStr((snake->body)[0].xpos, (snake->body)[0].ypos, "ㅁ");
	for (i = 1; i < snake->length; i++) {
		gotoxyPrintStr((snake->body)[i].xpos, (snake->body)[i].ypos, "ㅇ");
	}
}
void clearSnake(Snake* snake) {
	int i;
	gotoxyPrintStr((snake->body)[0].xpos, (snake->body)[0].ypos, "  ");
	for (i = 1; i < snake->length; i++) {
		gotoxyPrintStr((snake->body)[i].xpos, (snake->body)[i].ypos, "  ");
	}
}

void printScore(Snake* snake) {
	gotoxy(MAP_LENG + 5, 1);
	printf("뱀의 길이:%d", snake->length);
}

void makeRandomApple(Apple* apple, Snake* snake) {
	int i, breakFlag = 1;
	srand(time(0));
	while (1) {
		breakFlag = 1;
		apple->loc.xpos = rand() % (MAP_LENG - 1) + 1;
		apple->loc.ypos = rand() % (MAP_HEIGHT - 1) + 2;
		for (i = 0; i < snake->length; i++) {
			if (apple->loc.xpos == (snake->body)[i].xpos && apple->loc.ypos == (snake->body)[i].ypos)
				breakFlag = 0;
		}
		if (breakFlag)
			break;
	}
	gotoxyPrintStr(apple->loc.xpos, apple->loc.ypos, "ㅎ");
}

int isGameOver(Snake* snake) {
	int i;
	if ((snake->body)[0].xpos < 1 || (snake->body)[0].xpos > MAP_LENG) {
		return TRUE;
	}
	if ((snake->body)[0].ypos < 1 || (snake->body)[0].ypos >= MAP_HEIGHT) {
		return TRUE;
	}
	for (i = 1; i < snake->length; i++) {
		if ((snake->body)[0].xpos == (snake->body)[i].xpos && (snake->body)[0].ypos == (snake->body)[i].ypos) {
			return TRUE;
		}
	}
	return FALSE;
}

int isEatApple(Snake* snake, Apple* apple) {
	if ((snake->body)[0].xpos == apple->loc.xpos && (snake->body)[0].ypos == apple->loc.ypos)
		return TRUE;
	return FALSE;
}

void moveSnake(Snake* snake) {
	Loc temp = (snake->body)[0];
	Loc temp2;
	int i;
	clearSnake(snake);
	switch (snake->direction)
	{
	case UP:
		(snake->body)[0].ypos--;
		for (i = 1; i < snake->length; i++) {
			temp2 = (snake->body)[i];
			(snake->body)[i] = temp;
			temp = temp2;
		}
		break;
	case LEFT:
		(snake->body)[0].xpos--;
		for (i = 1; i < snake->length; i++) {
			temp2 = (snake->body)[i];
			(snake->body)[i] = temp;
			temp = temp2;
		}
		break;
	case RIGHT:
		(snake->body)[0].xpos++;
		for (i = 1; i < snake->length; i++) {
			temp2 = (snake->body)[i];
			(snake->body)[i] = temp;
			temp = temp2;
		}
		break;
	case DOWN:
		(snake->body)[0].ypos++;
		for (i = 1; i < snake->length; i++) {
			temp2 = (snake->body)[i];
			(snake->body)[i] = temp;
			temp = temp2;
		}
		break;
	}
	printSnake(snake);
}
void moveSnakeInTime(Snake* snake, Apple* apple) {
	int memoryCleanFlag = 0;
	int inputKey;
	int breakFlag = 0;
	Loc memoryTail;
	clock_t start = clock();
	while (1)
	{
		if (_kbhit())
		{
			inputKey = _getch();
			if (inputKey == 224) {
				breakFlag = 0;
				inputKey = _getch();

				// 먹자마자 길이 늘어나게 하기
				if (memoryCleanFlag) {
					gotoxyPrintStr(memoryTail.xpos, memoryTail.ypos, "  ");
					memoryCleanFlag = 0;
				}
				memoryTail = snake->body[snake->length - 1];

				switch (inputKey)
				{
				case 72: // 위 방향키
					if (snake->direction == UP || snake->direction == DOWN) {
						breakFlag = 1;
						break;
					}
					snake->direction = UP;
					moveSnake(snake);
					break;
				case 75: // 왼쪽 방향키
					if (snake->direction == LEFT || snake->direction == RIGHT) {
						breakFlag = 1;
						break;
					}
					snake->direction = LEFT;
					moveSnake(snake);
					break;
				case 77: // 오른쪽 방향키
					if (snake->direction == RIGHT || snake->direction == LEFT) {
						breakFlag = 1;
						break;
					}
					snake->direction = RIGHT;
					moveSnake(snake);
					break;
				case 80: // 아래 방향키
					if (snake->direction == DOWN || snake->direction == UP) {
						breakFlag = 1;
						break;
					}
					snake->direction = DOWN;
					moveSnake(snake);
					break;
				}				
				if (breakFlag == 0) {
					start = clock();
				}
				if (isGameOver(snake)) {
					gotoxyPrintStr(MAP_LENG / 2, MAP_HEIGHT / 2, "GAME OVER (아무키나 입력)");
					_getch();
					break;
				}
				if (isEatApple(snake, apple)) {
					// 먹자마자 길이 늘어나게 하기
					gotoxyPrintStr(memoryTail.xpos, memoryTail.ypos, "ㅇ");
					memoryCleanFlag = 1;

					makeRandomApple(apple, snake);
					snake->length++;
					printScore(snake);
				}
			}
		}
		if ((clock() - start) > SNAKE_SPEED2) {
			// 먹자마자 길이 늘어나게 하기
			if (memoryCleanFlag) {
				gotoxyPrintStr(memoryTail.xpos, memoryTail.ypos, "  ");
				memoryCleanFlag = 0;
			}
			memoryTail = snake->body[snake->length - 1];

			moveSnake(snake);
			start = clock();
			if (isGameOver(snake)) {
				gotoxyPrintStr(MAP_LENG / 2, MAP_HEIGHT / 2, "GAME OVER (아무키나 입력)");
				_getch();
				break;
			}
			if (isEatApple(snake, apple)) {
				// 먹자마자 길이 늘어나게 하기
				gotoxyPrintStr(memoryTail.xpos, memoryTail.ypos, "ㅇ");
				memoryCleanFlag = 1;

				makeRandomApple(apple, snake);
				snake->length++;
				printScore(snake);
			}
		}
	}
}

void startGame() {
	Snake snake;
	Apple apple;
	clearScreen();
	snakeInit(&snake);
	printMap();
	makeRandomApple(&apple, &snake);
	printSnake(&snake);
	printScore(&snake);
	moveSnakeInTime(&snake, &apple);
	return;
}

void showHowGame() {
	clearScreen();
	printf("\n\n");
	printf("벽과 자신의 몸에 부딪히지 않고 사과를 먹으며 몸의 길이를 늘려나가세요.\n\n");
	printf("사과는 랜덤하게 스폰됩니다.");
	return;
}


void pickMenu() {
	char input;
	while (1) {
		input = _getch();
		switch (input)
		{
		case '1':
			startGame();
			return;
		case '2':
			showHowGame();
			printMenu();
			pickMenu();
			return;
		case '3':
			printf("게임 종료\n");
			return;
		}
	}
}

int main(void) {
	int num, input, life = LIFE;
	removeConsoleCursor();
	decorateConsole();
	drawTitle();
	printMenu();
	pickMenu();
}




