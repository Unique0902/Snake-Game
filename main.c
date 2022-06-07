#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include <stdlib.h>

# define LIFE 10
# define MAP_LENG 30
# define MAP_HEIGHT 30
# define SNAKE_MOVE_SECOND 300

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
	system("mode con cols=100 lines=40 | title �����");
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
	printf("                              1) ���� ����   \n\n");
	printf("                              2) ���� ���   \n\n");
	printf("                              3) ������   \n\n");
	printf("                            ���� 1, 2, 3 �� �Է�\n");
}

void printMap() {
	int i, j;
	printf("\n");
	printf(" ");
	for (i = 0; i < MAP_LENG * 2; i++)
		printf("=");
	for (i = 0; i < MAP_HEIGHT - 2; i++) {
		printf("\n");
		printf("��");
		for (j = 0; j < MAP_LENG * 2; j++)
			printf(" ");
		printf("��");
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
	gotoxyPrintStr((snake->body)[0].xpos, (snake->body)[0].ypos, "��");
	for (i = 1; i < snake->length; i++) {
		gotoxyPrintStr((snake->body)[i].xpos, (snake->body)[i].ypos, "��");
	}
}
void clearSnake(Snake* snake) {
	int i;
	gotoxyPrintStr((snake->body)[0].xpos, (snake->body)[0].ypos, "  ");
	for (i = 1; i < snake->length; i++) {
		gotoxyPrintStr((snake->body)[i].xpos, (snake->body)[i].ypos, "  ");
	}
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

void moveSnake(Snake* snake) {
	Loc temp = (snake->body)[0];
	Loc temp2;
	Loc memoryTail = (snake->body)[snake->length - 1];
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
void moveSnakeInTime(Snake* snake) {
	int inputKey;
	int breakFlag = 0;
	clock_t start = clock();
	while (1)
	{
		if (_kbhit())
		{
			inputKey = _getch();
			if (inputKey == 224) {
				breakFlag = 0;
				inputKey = _getch();
				switch (inputKey)
				{
				case 72: // �� ����Ű
					if (snake->direction == UP || snake->direction == DOWN) {
						breakFlag = 1;
						break;
					}
					snake->direction = UP;
					moveSnake(snake);
					break;
				case 75: // ���� ����Ű
					if (snake->direction == LEFT || snake->direction == RIGHT) {
						breakFlag = 1;
						break;
					}
					snake->direction = LEFT;
					moveSnake(snake);
					break;
				case 77: // ������ ����Ű
					if (snake->direction == RIGHT || snake->direction == LEFT) {
						breakFlag = 1;
						break;
					}
					snake->direction = RIGHT;
					moveSnake(snake);
					break;
				case 80: // �Ʒ� ����Ű
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
					break;
				}
			}
		}
		if ((clock() - start) > SNAKE_MOVE_SECOND) {
			moveSnake(snake);
			start = clock();
			if (isGameOver(snake)) {
				break;
			}
		}
	}
}

void startGame() {
	Snake snake;
	clearScreen();
	snakeInit(&snake);
	printMap();
	printSnake(&snake);
	moveSnakeInTime(&snake);
	return;
}

void showHowGame() {
	clearScreen();
	printf("\n\n");
	printf("���� �ڽ��� ���� �ε����� �ʰ� ����� ������ ���� ���̸� �÷���������.\n\n");
	printf("����� �����ϰ� �����˴ϴ�.");
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
			printf("���� ����\n");
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


/*
int main() {
	gotoxy(1, 1);
	printf("��");
	_getch();
	return 0;
}
*/


