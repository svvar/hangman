/*main file*/
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#define PHASES 9
using namespace std;

// functions
class manikin;
string randomSelector(string fileName);
int strCompare(string word, string current);
void play(manikin *player1, int &score1, int &counter);

// Hangman output (temporary)
class manikin
{
private:
	int phase = 0;			  // phase counter
	bool end = false;		  // end of ?
	string usedSymbols;		  // used symbols per 1 match
	string word;			  // keyword
	string currentAppearance; // word entered by user
	string man[PHASES] = {"", "\n  |\n  |\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |\n  |\n  |\n  |\n _|__________",
						  "\n _______\n  |     |\n  |    (_)\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |    |_|\n  |\n  |\n _|__________",
						  "\n _______\n  |     |\n  |    (_)\n  |   /|_|\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |\n  |\n _|__________",
						  "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    /\n  |\n _|__________",
						  "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    / \\ \n  | GAME OVER\n _|__________"};

public:
	manikin()
	{ // constructor : create game for 2 persons
		cout << "Введіть слово: ";
		cin >> word;
		cout << "\x1b[1A"  // Move cursor up one
			 << "\x1b[2K"; // Delete the entire line
		for (int i = 0; i < (int)word.size(); i++)
		{
			currentAppearance.push_back('_');
		}
		currentAppearance.push_back('\0');
	}
	manikin(int mode)
	{ // constructor : create game for 1 person
		word = randomSelector("dict.txt");

		for (int i = 0; i < (int)word.size(); i++)
		{
			currentAppearance.push_back('_');
		}
		currentAppearance.push_back('\0');
	}
	inline void print_phase(int i) { cout << man[i]; } // print curent phase of hangman
	inline string getWord() { return word; }		   //(where used?)
	inline int getPhase() { return phase; }			   // return curent phase
	inline bool isend() { return end; }				   // return game status (end or not)

	int step()
	{ //(?)
		char ch;
		if (phase == PHASES - 1)
		{				// if user on 8th phase,
			phase++;	// go to last phase
			end = true; // and end the game
			return 0;
		}
		if (end == false)
		{
			cout << "\n"
				 << currentAppearance << endl;
			cout << "\nВгадайте букву: ";
			cin >> ch;

			if (usedSymbols.find(ch) == string::npos && word.find(ch) == string::npos)
			{
				phase++;
			}

			if (word.find(ch) != string::npos)
			{
				cout << "Буква знайдена!!" << endl;
				for (int i = 0; i < (int)word.size(); i++)
					if (word[i] == ch)
						currentAppearance[i] = ch;
			}
			// else{cout << "Цей символ ви вже вгадували!" << endl;}

			if (usedSymbols.find(ch) == string::npos)
			{
				usedSymbols.push_back(ch);
				usedSymbols.push_back(' ');
			}
			else
			{
				cout << "Цей символ ви вже вгадували!" << endl;
			}
		}

		if (strCompare(word, currentAppearance))
		{
			end = true;
			cout << "СЛОВО ВІДГАДАНЕ!" << endl;
			return 1;
		}

		cout << "Використані симовли: " << usedSymbols << endl;

		return 0;
	}
};

int main()
{
	system("chcp 1251");
	setlocale(LC_ALL, "UTF8");
	string answer;
	int score[2] = {0, 0};
	int counter[2];
	clock_t t;
	int time;
	do
	{
		answer = ' ';
		int mode;
		do
		{
			cout << "Виберіть режим гри (1 - з комп'ютером, 2 - для двох): ";
			cin >> mode;
			if (mode != 1 && mode != 2)
				cout << "Неправильний вибір, повторіть!" << endl;
		} while (mode != 1 && mode != 2);

		if (mode == 1)
		{
			string reply;
			do
			{
				counter[0] = 0;
				reply = ' ';
				manikin *player1 = new manikin(1);
				t = clock();
				play(player1, score[0], counter[0]);
				time = (clock() - t) / CLOCKS_PER_SEC;
				cout << "\nБуло зроблено " << counter[0] - 1 << " ходів.";
				cout << "\nБуло витрачено " << time << " сек.";
				cout << "\nВаш рахунок: " << score[0] << endl
					 << endl;
				cout << "\nЩе одне слово? (y/т) - ";
				cin >> reply;
			} while (reply == "y" || reply == "Y" || reply == "т" || reply == "Т");
			score[0] = 0;
		}
		else
		{
			int scoreBefore, maxScore;
			string reply;
			do
			{
				int currentPlayer = 1;
				reply = ' ';
				cout << "До якого рахунку гратимете? ";
				cin >> maxScore;
				do
				{
					counter[0] = 0;
					counter[1] = 0;
					cout << "Відгадує гравець " << currentPlayer << ". Гравець " << ((currentPlayer == 1) ? 2 : 1) << " загадує\n";
					scoreBefore = score[currentPlayer - 1];
					manikin *player = new manikin();
					t = clock();
					play(player, score[currentPlayer - 1], counter[currentPlayer - 1]);
					time = (clock() - t) / CLOCKS_PER_SEC;
					cout << "\nБуло зроблено " << counter[currentPlayer - 1] - 1 << " ходів.";
					cout << "\nБуло витрачено " << time << " сек.";
					cout << "\nГравець " << currentPlayer << " ваш рахунок: " << score[currentPlayer - 1] << endl
						 << endl;

					if (score[currentPlayer - 1] == scoreBefore)
						currentPlayer == 1 ? currentPlayer = 2 : currentPlayer = 1;

					if (score[0] == maxScore)
					{
						cout << "Гравець 1 переміг!!!!\n\n";
					}
					if (score[1] == maxScore)
					{
						cout << "Гравець 2 переміг!!!!\n\n";
					}
					fseek(stdin, 0, SEEK_END);
				} while (score[0] != maxScore && score[1] != maxScore);

				cout << "\nЩе одна гра? (y/т) - ";
				cin >> reply;
				fseek(stdin, 0, SEEK_END);

				score[0] = 0;
				score[1] = 1;
			} while (reply == "y" || reply == "Y" || reply == "т" || reply == "Т");
		}

		cout << "\n\nЗіграти нову гру? (y/т) - ";
		fseek(stdin, 0, SEEK_END);
		cin >> answer;
		fseek(stdin, 0, SEEK_END);

	} while (answer == "y" || answer == "Y" || answer == "т" || answer == "Т");
}

int strCompare(string word, string current)
{
	int size = word.size();
	for (int i = 0; i < size; i++)
	{
		if (word[i] != current[i])
			return 0;
	}
	return 1;
}

void play(manikin *player1, int &score1, int &counter)
{
	int p = player1->getPhase();

	while (p < PHASES)
	{
		if (player1->isend() == false)
		{
			player1->print_phase(p);
			score1 += player1->step();
			p = player1->getPhase();
			counter++;
		}
		else
			break;
	}
}

string randomSelector(string fileName)
{
	//слова в словнику повинні бути записані через пробіл, після останнього слова теж пробіл
	//можна доробити, щоб приймалося розділення слів будь-якими isspace символами

	fstream file;
	string dict, word;
	srand(time(0));
	file.open(fileName);

	getline(file, dict);
	if (isspace(dict[0]))
	{
		cout << "В словнику відсутні слова! Помилка!";
		exit(1);
	}

	int wordsCount = 0;
	for (char buf : dict)
	{
		if (buf == ' ')
			wordsCount++;
	}

	int randomNumber = rand() % wordsCount;

	int spaces = 0;
	for (char ch : dict)
	{
		if (spaces == randomNumber)
		{
			if (ch == ' ')
				break;
			word += ch;
		}
		if (ch == ' ')
			spaces++;
	}
	file.close();
	return (word);
}