#include <iostream>
#include <conio.h>
#include <direct.h>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <string>
#include <algorithm>



#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70 

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

string name;
int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char car[4][4] = { ' ','+','+',' ',
					'+','+','+','+',
					' ','+','+',' ',
					'+','+','+','+' };



int carPos = WIN_WIDTH / 2;
int score = 0;
class Dimensions {

public:
	void gotoxy(int x, int y) {
		CursorPosition.X = x;
		CursorPosition.Y = y;
		SetConsoleCursorPosition(console, CursorPosition);
	}

	void setcursor(bool visible, DWORD size) {
		if (size == 0)
			size = 20;

		CONSOLE_CURSOR_INFO lpCursor;
		lpCursor.bVisible = visible;
		lpCursor.dwSize = size;
		SetConsoleCursorInfo(console, &lpCursor);
	}

	void drawBorder() {
		for (int i = 0; i < SCREEN_HEIGHT; i++) {
			for (int j = 0; j < 17; j++) {
				gotoxy(0 + j, i); cout << "|";
				gotoxy(WIN_WIDTH - j, i); cout << "|";
			}
		}
		for (int i = 0; i < SCREEN_HEIGHT; i++) {
			gotoxy(SCREEN_WIDTH, i); cout << "|";
		}
	}

};

class Car : public Dimensions {

public:
	void drawCar() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				gotoxy(j + carPos, i + 22); cout << car[i][j];
			}
		}
	}

	void eraseCar() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				gotoxy(j + carPos, i + 22); cout << " ";
			}
		}
	}
};

class Enemy : public Dimensions {

public:
	void genEnemy(int ind) {
		enemyX[ind] = 17 + rand() % (33);
	}

	void drawEnemy(int ind) {
		if (enemyFlag[ind] == true) {
			gotoxy(enemyX[ind], enemyY[ind]);   cout << "**";
			gotoxy(enemyX[ind], enemyY[ind] + 1); cout << " ** ";
			gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "**";
			gotoxy(enemyX[ind], enemyY[ind] + 3); cout << " ** ";
		}
	}

	void eraseEnemy(int ind) {
		if (enemyFlag[ind] == true) {
			gotoxy(enemyX[ind], enemyY[ind]); cout << "    ";
			gotoxy(enemyX[ind], enemyY[ind] + 1); cout << "    ";
			gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "    ";
			gotoxy(enemyX[ind], enemyY[ind] + 3); cout << "    ";
		}
	}

	void resetEnemy(int ind) {
		eraseEnemy(ind);
		enemyY[ind] = 1;
		genEnemy(ind);
	}



};

class PlayGame : public Dimensions {

public:
	int collision() {
		if (enemyY[0] + 4 >= 23) {
			if (enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9) {
				return 1;
			}
		}
		return 0;
	}

	void gameover() {
		system("cls");
		cout << endl;
		cout << "\t\t--------------------------" << endl;
		cout << "\t\t-------- Game Over -------" << endl;
		cout << "\t\t--------------------------" << endl << endl;
		saveScores();
		cout << "\t\tPress any key to go back to menu.";
		_getch();
	}

	fstream scoreFile;
	string line;

	void sortScores() {
		scoreFile.open("HighScores.txt", ios::in);
		string lines[5];

		//reading scores from file
		while (getline(scoreFile, line)) {
			for (int i = 0; i < 5; i++)
			{
				lines[i] = line;
			}
		}

		//sorting scores
		sort(lines, lines + 5, greater<>());

		//printing out scores
		for (int i = 0; i < 5; i++)
		{
			cout << lines[i] << endl;
		}
		_getch();
	}

	//writes high score then prints high score then 
	void saveScores() {
		system("cls");
		//write to file
		cout << "-----------------SCORES------------------" << endl;
		scoreFile.open("HighScores.txt", ios::app);

		if (scoreFile.is_open()) {
			scoreFile << name << ": " << score << endl;
		}

		string line;
		while (getline(scoreFile, line)) {
			cout << line << endl;
		}
		_getch();
	}

	void updateScore() {
		gotoxy(WIN_WIDTH + 7, 5); cout << "Score: " << score << endl;
	}

	void instructions() {

		system("cls");
		cout << "Instructions";
		cout << "\n----------------";
		cout << "\n Avoid Cars by moving left or right. ";
		cout << "\n\n Press 'a' to move left";
		cout << "\n Press 'd' to move right";
		cout << "\n Press 'escape' to exit";
		cout << "\n\nPress any key to go back to menu";
		_getch();
	}

	void play() {

		cout << "Enter your name:";
		cin >> name;

		carPos = -1 + WIN_WIDTH / 2;
		score = 0;
		enemyFlag[0] = 1;
		enemyFlag[1] = 0;
		enemyY[0] = enemyY[1] = 1;

		system("cls");
		Enemy sed;
		Car sec;

		drawBorder();
		updateScore();
		sed.genEnemy(0);
		sed.genEnemy(1);


		gotoxy(WIN_WIDTH + 7, 2); cout << "Car Game" << endl;
		gotoxy(WIN_WIDTH + 7, 2); cout << "Player: " << name;
		gotoxy(WIN_WIDTH + 6, 4); cout << "----------";
		gotoxy(WIN_WIDTH + 6, 6); cout << "----------";
		gotoxy(WIN_WIDTH + 7, 12); cout << "Control ";
		gotoxy(WIN_WIDTH + 7, 13); cout << "-------- ";
		gotoxy(WIN_WIDTH + 2, 14); cout << " A Key - Left";
		gotoxy(WIN_WIDTH + 2, 15); cout << " D Key - Right";


		gotoxy(18, 5); cout << "Press any key to start";
		_getch();
		gotoxy(18, 5); cout << "                      ";

		while (1) {
			if (_kbhit()) {
				char ch = _getch();
				if (ch == 'a' || ch == 'A') {
					if (carPos > 18)
						carPos -= 4;
				}
				if (ch == 'd' || ch == 'D') {
					if (carPos < 50)
						carPos += 4;
				}
				if (ch == 27) {
					break;
				}
			}

			sec.drawCar();
			sed.drawEnemy(0);
			sed.drawEnemy(1);
			if (collision() == 1) {
				gameover();
				return;
			}
			Sleep(50);
			sec.eraseCar();
			sed.eraseEnemy(0);
			sed.eraseEnemy(1);

			if (enemyY[0] == 10)
				if (enemyFlag[1] == 0)
					enemyFlag[1] = 1;

			if (enemyFlag[0] == 1)
				enemyY[0] += 1;

			if (enemyFlag[1] == 1)
				enemyY[1] += 1;

			if (enemyY[0] > SCREEN_HEIGHT - 4) {
				sed.resetEnemy(0);
				score++;
				updateScore();
			}
			if (enemyY[1] > SCREEN_HEIGHT - 4) {
				sed.resetEnemy(1);
				score++;
				updateScore();
			}
		}
	}


};



int main()
{
	PlayGame dimen;
	dimen.setcursor(0, 0);
	srand((unsigned)time(NULL));

	do {
		system("cls");
		dimen.gotoxy(10, 5); cout << " -------------------------- ";
		dimen.gotoxy(10, 6); cout << " |        Car Game        | ";
		dimen.gotoxy(10, 7); cout << " --------------------------";
		dimen.gotoxy(10, 8); cout << "1. Start Game";
		dimen.gotoxy(10, 9); cout << "2. Instructions";
		dimen.gotoxy(10, 10); cout << "3. See High Scores";
		dimen.gotoxy(10, 11); cout << "4. Quit";
		dimen.gotoxy(10, 13); cout << "Select option: ";
		char op = _getche();

		if (op == '1') dimen.play();
		else if (op == '2') dimen.instructions();
		else if (op == '3') dimen.sortScores();
		else if (op == '4') exit(0);

		/*
		switch (op)
		{

		case '1': dimen.play();
			break;
		case '2' : dimen.instructions();
			break;
		case '3' : dimen.saveScores();
			break;
		case '4': exit(0);
			break;
		default:
			break;
		}
		*/


	} while (1);

	return 0;
}
