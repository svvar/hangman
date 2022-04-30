/*main file*/
#include <iostream>
#include <string> // functions used : string.find(), string.push_back() string::npos, string.size()
#include <cctype> // function used : toupper()
#include <fstream> // functions used : file.open(), file.close(), getline()
//#include <ctime> // functions used : clock(), clock_t, CLOCKS_PER_SEC
#include <cstdlib> // functions used : srand(), rand(), exit()
#include <chrono>
#define PHASES 9 // phases of hangman (first phase + 8 mistakes)
using namespace std;

class manikin; // the main class
class WordGuess;
// functions :
string randomSelector(string fileName); // function that return random keyword from dictionary file
int strCompare(string word, string current);  // function that compare two strings
void play(WordGuess* player1, int& score1, int& counter); // game function

// Hangman output (temporary)
class manikin {
private:
	string man[PHASES] = { "", "\n  |\n  |\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |\n  |\n  |\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |    |_|\n  |\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |   /|_|\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    /\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    / \\ \n  | GAME OVER\n _|__________" };

public:
	inline void print_phase(int i) { cout << man[i]; } // print curent phase of hangman
};

class WordGuess {
	string word;
	string usedSymbols;
	bool end = false;
	int phase = 0;
	string currentAppearance;
public:
	WordGuess() { // constructor : create game for 2 persons
		cout << "������ �����: ";
		cin >> word;
		cout << "\x1b[1A"  // Move cursor up one
			<< "\x1b[2K"; // Delete the entire line
		for (int i = 0; i < (int)word.size(); i++)
		{
			currentAppearance.push_back('_');
		}
		currentAppearance.push_back('\0');
	}
	WordGuess(int mode) { // constructor : create game for 1 person
		word = randomSelector("dict.txt");

		for (int i = 0; i < (int)word.size(); i++) {
			currentAppearance.push_back('_');
		}
		currentAppearance.push_back('\0');
	}
	inline string getWord() { return word; }		   // (where used?)
	inline int getPhase() { return phase; }			   // return curent phase
	inline bool isend() { return end; }				   // return game status (end or not)
	int step() { // do one step in the match, return score
		if (phase == PHASES - 1) { // if user on 8th phase,
			phase++;			   // go to last phase
			end = true; 		   // and end the game
			return 0;
		}
		string chIn;
		if (end == false) { // if game continue
			cout << "\n"
				<< currentAppearance << endl; // print current input
			cout << "\n�������� �����: ";
			do {
				cin >> chIn; // get new letter
				if (chIn.size() > 1) {
					cout << "\x1b[1A" << "\x1b[2K";  // Move cursor up one  Delete the entire line
					cout << "������ ���� 1 �����! �� ���: ";
				}
				else if (!isalpha(chIn[0])) {
					cout << "\x1b[1A" << "\x1b[2K";  // Move cursor up one  Delete the entire line
					cout << "������� ���� ���� �������� �����! ������ �� ���: ";
				}
			} while (!isalpha(chIn[0]) || chIn.size() > 1);       //check input (only letters && only 1 char)

			char ch = chIn[0];
			ch = tolower(ch);

			cout << "\033[2J\033[1;1H"; //special screen clean command

// if letter is not in the used symbols and not in the keyword then go to next phase
			if (usedSymbols.find(ch) == string::npos && word.find(ch) == string::npos) { phase++; }

			// if letter is in the keyword
			if (word.find(ch) != string::npos || word.find(toupper(ch)) != string::npos) {
				cout << "����� ��������!!" << endl;
				for (int i = 0; i < (int)word.size(); i++)
					if (word[i] == ch) { // then add it on its position to input
						currentAppearance[i] = ch;
					}
					else if (word[i] == toupper(ch)) { // uppercase char
						currentAppearance[i] = toupper(ch);
					}
			}

			if (usedSymbols.find(ch) == string::npos) { // if letter was not in the used symbols
				usedSymbols.push_back(ch); // then add it
				usedSymbols.push_back(' '); // and add a space
			}
			else { cout << "��� ������ �� ��� ���������!" << endl; } // otherwise print that letter was used
		}

		if (strCompare(word, currentAppearance)) { // comparing input and keyword, if true
			end = true; // then end the game, user win
			cout << "����� ²�������! - " << currentAppearance << endl;
			return 1;
		}

		cout << "����������� �������: " << usedSymbols << endl;

		return 0;
	}
};

int main() {
	setlocale(LC_CTYPE, "ukr"); // (� � ������� ������, ���� �� ���� ���8) 
	//setlocale(LC_ALL, "UTF8");
	string answer; // user answer for continue the game
	int score[2] = { 0, 0 }; // score points for both playes
	int counter[2]; // steps counter for both players per 1 match
	//auto start, end, time;
	do {
		answer = ' ';
		int mode;
		do {
			cout << "������� ����� ��� (1 - � ����'������, 2 - ��� ����): ";
			cin >> mode;
			if (mode != 1 && mode != 2) // check if the input correct
				cout << "������������ ����, ��������!" << endl;
		} while (mode != 1 && mode != 2); // loop until correct input

		if (mode == 1) { // mode : user and computer
			string reply; // user reply for continue guess
			do {
				counter[0] = 0; // 0 steps in the beginning
				reply = ' ';
				WordGuess* player1 = new WordGuess(1); // create a keyword and output for 1 user
				auto start1 = std::chrono::steady_clock::now(); // get the current time
				play(player1, score[0], counter[0]); // game function
				auto end1 = std::chrono::steady_clock::now();
				auto time1 = std::chrono::duration_cast<std::chrono::seconds>(end1 - start1); // count game duration in seconds
				cout << "\nʳ������ ����: " << counter[0] - 1;
				cout << "\n���������� ��� (� ��������): " << time1.count();
				cout << "\n��� �������: " << score[0] << endl
					<< endl;
				cout << "\n�� ���� �����? (y/�) - ";
				cin >> reply;
				cout << "\033[2J\033[1;1H"; //special screen clean command
			} while (reply == "y" || reply == "Y" || reply == "�" || reply == "�");
			score[0] = 0; // reset the score
		}
		else { // mode : 2 players - users
			int scoreBefore, maxScore;
			string reply; // user reply for continue guess
			do {
				int currentPlayer = 1;
				reply = ' ';
				cout << "�� ����� ������� ���������? ";
				cin >> maxScore;
				do {
					counter[0] = 0; // step counters
					counter[1] = 0; // reset
					cout << "³����� ������� " << currentPlayer << ". ������� " << ((currentPlayer == 1) ? 2 : 1) << " ������\n";
					scoreBefore = score[currentPlayer - 1];
					WordGuess* player = new WordGuess(); // create a manikin for two players, get keyword from user
					auto start2 = std::chrono::steady_clock::now(); // get the current time
					play(player, score[currentPlayer - 1], counter[currentPlayer - 1]); // game function
					auto end2 = std::chrono::steady_clock::now();
					auto time2 = std::chrono::duration_cast<std::chrono::seconds>(end2 - start2); // count game duration in seconds
					cout << "\nʳ������ ����: " << counter[currentPlayer - 1] - 1;
					cout << "\n���������� ��� (� ��������): " << time2.count();
					cout << "\n������� " << currentPlayer << " ��� �������: " << score[currentPlayer - 1] << endl
						<< endl;

					if (score[currentPlayer - 1] == scoreBefore) { // if current player loses,
						currentPlayer == 1 ? currentPlayer = 2 : currentPlayer = 1;
					} // then it's next player's turn
// winning condition - player get max score
					if (score[0] == maxScore) { cout << "������� 1 ������!!!!\n\n"; }
					else if (score[1] == maxScore) { cout << "������� 2 ������!!!!\n\n"; }
					fseek(stdin, 0, SEEK_END); // clean the buffer
				} while (score[0] != maxScore && score[1] != maxScore);

				cout << "\n�� ���� ���? (y/�) - ";
				cin >> reply;
				cout << "\033[2J\033[1;1H"; //special screen clean command
				fseek(stdin, 0, SEEK_END); // clean the buffer
				// reset user's score
				score[0] = 0;
				score[1] = 0;
			} while (reply == "y" || reply == "Y" || reply == "�" || reply == "�");
		}

		cout << "\n\nǳ����� ���� ���? (y/�) - ";
		fseek(stdin, 0, SEEK_END); // clean the buffer
		cin >> answer;
		cout << "\033[2J\033[1;1H"; //special screen clean command
		fseek(stdin, 0, SEEK_END); // clean the buffer

	} while (answer == "y" || answer == "Y" || answer == "�" || answer == "�");
}

int strCompare(string word, string current) { // function that compare two strings
	int size = word.size(); // get the keyword's size
	for (int i = 0; i < size; i++) { // for each char until the last one compare keyword and user's input
		if (word[i] != current[i]) { // if they aren't the same return 0
			return 0;
		}
	}
	return 1; // otherwise keyword and user's input are the same so return 1
}

void play(WordGuess* player1, int& score1, int& counter) { // game function
	int p = player1->getPhase(); // get current hangman's phase

	while (p < PHASES) { // while current phase < max amount of phases
		if (player1->isend() == false) { // if game continue
			manikin* phases = new manikin();
			phases->print_phase(p);
			score1 += player1->step(); // add a score point from one step
			p = player1->getPhase(); // change the phase
			counter++; // increment step counter
		}
		else { break; } // current phase is the last, break the game
	}
}

string randomSelector(string fileName) { // function that return random keyword from dictionary file
	//����� � �������� ������� ���� �������� ����� �����, ���� ���������� ����� ��� �����
	//����� ��������, ��� ���������� ��������� ��� ����-����� isspace ���������

	fstream file;
	string word;
	srand(time(0)); // random seed by time
	file.open(fileName); // open the file

	string dict((std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>()));    // get file data and put into string variable dict

	if (isspace(dict[0])) { // if first char is space symbol then there're no word in the dictionary
		cout << "� �������� ������� �����! �������!";
		exit(1);
	}

	// loop that count words in the dictionary
	int wordsCount = 0;
	for (char buf : dict) { // foreach
		if (buf == '\n') { wordsCount++; }
	}

	int randomNumber = rand() % wordsCount; // get random number which is max equals words count

	int spaces = 0;
	for (char ch : dict) { // foreach
		if (spaces == randomNumber) { // got word with random number index
			if (ch == '\n') { break; } // if char is space then break
			word += ch; // otherwise add char to final word
		}
		if (ch == '\n') { spaces++; } // spaces counter
	}
	file.close(); // close the file
	return (word); // return keyword
}