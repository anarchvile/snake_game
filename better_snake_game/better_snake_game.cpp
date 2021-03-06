#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <glut.h>
#include <ctime>

#define COLUMNS 80
#define ROWS 40
#define UP 1
#define DOWN -1
#define RIGHT 2
#define LEFT -2

using namespace std;

int fps = 10;
int gridX, gridY;
int snakeLength = 5;
int index = 0;
int posX[1600] = { 20, 20, 20, 20, 20 }, posY[1600] = { 20, 19, 18, 17, 16 };
short sDirection = RIGHT;
bool gameOver = false;
bool food = true;
int foodX, foodY;
int score = 0;

void random(int &x, int &y)
{
	int maxX = gridX - 2;
	int maxY = gridY - 2;
	int min = 1;
	srand(time(NULL));
	x = min + rand() % (maxX - min);
	y = min + rand() % (maxY - min);
}

void initGrid(int x, int y)
{
	gridX = x;
	gridY = y;
}

void unit(int x, int y)
{
	if (x == 0 || y == 0 || x == gridX - 1 || y == gridY - 1)
	{
		glLineWidth(3);
		glColor3f(0, 0.75f, 0);
	}
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + 1, y);
	glVertex2f(x + 1, y + 1);
	glVertex2f(x, y + 1);
	glEnd();
}

void drawGrid()
{
	for (int i = 0; i < gridX; ++i)
		for (int j = 0; j < gridY; ++j)
			if (i == 0 || i == gridX - 1 || j == 0 || j == gridY - 1)
				unit(i, j);
}

void drawSnake()
{
	for (int i = snakeLength - 1; i > 0; --i)
	{
		posX[i] = posX[i - 1];
		posY[i] = posY[i - 1];
	}
	if (sDirection == UP) ++posY[0];
	else if (sDirection == DOWN) --posY[0];
	else if (sDirection == RIGHT) ++posX[0];
	else if (sDirection == LEFT) --posX[0];
	for (int i = 0; i < snakeLength; ++i)
	{
		if (i == 0) glColor3f(0, 1, 0);
		else glColor3f(0, 0.5f, 0);
		glRectd(posX[i], posY[i], posX[i] + 1, posY[i] + 1);
	}
	if (posX[0] == 0 || posX[0] == gridX - 1 || posY[0] == 0 || posY[0] == gridY - 1) gameOver = true;
	if (posX[0] == foodX && posY[0] == foodY)
	{
		++score;
		++snakeLength;
		food = true;

		if (score % 5 == 0)
		{
			fps += 5;
		}
	}
	for (int i = 1; i < snakeLength; ++i)
	{
		if (posX[i] == posX[0] && posY[i] == posY[0]) gameOver = true;
	}
}

void drawFood()
{
	if (food) random(foodX, foodY);
	food = false;
	glColor3f(0, 1.0f, 0);
	glRectf(foodX, foodY, foodX + 1, foodY + 1);

	for (int i = 0; i < snakeLength; ++i)
		if (foodX == posX[i] && foodY == posY[i]) 
			random(foodX, foodY);
}

void timerCallback(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / fps, timerCallback, 0);
}

void arrowMovement(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (sDirection != DOWN) sDirection = UP;
		break;
	case GLUT_KEY_DOWN:
		if (sDirection != UP) sDirection = DOWN;
		break;
	case GLUT_KEY_RIGHT:
		if (sDirection != LEFT) sDirection = RIGHT;
		break;
	case GLUT_KEY_LEFT:
		if (sDirection != RIGHT) sDirection = LEFT;
		break;
	}
}

void alternateMovement(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	case 'W':
		if (sDirection != DOWN) sDirection = UP;
		break;
	case 's':
	case 'S':
		if (sDirection != UP) sDirection = DOWN;
		break;
	case 'd':
	case 'D':
		if (sDirection != LEFT) sDirection = RIGHT;
		break;
	case 'a':
	case 'A':
		if (sDirection != RIGHT) sDirection = LEFT;
		break;
	}
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawGrid();
	drawSnake();
	drawFood();
	glutSwapBuffers();
	if (gameOver)
	{
		std::string tempFinalScore = std::to_string(score);
		std::wstring finalScore = std::wstring(tempFinalScore.begin(), tempFinalScore.end());
		MessageBox(NULL, finalScore.c_str(), L"Final Score", MB_OK);
		exit(0);
	} 
}

void reshapeCallback(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glOrtho(0, COLUMNS, 0, ROWS, -1, 1);	
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(COLUMNS * 10, ROWS * 10);
	glutCreateWindow("Snake");
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshapeCallback);
	glutTimerFunc(0, timerCallback, 0);
	glutKeyboardFunc(alternateMovement);
	glutSpecialFunc(arrowMovement);
	initGrid(COLUMNS, ROWS);
	glutMainLoop();
	return 0;
}
