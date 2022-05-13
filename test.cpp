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
// functions :
int charValidation(string chIn);
string randomSelector(string fileName); // function that return random keyword from dictionary file
int strCompare(string word, string current);  // function that compare two strings
void play(manikin* player1, int& score1, int& counter); // game function


// Hangman output (temporary)
class manikin {
private:
	int phase = 0;			  // phase counter
	bool end = false;		  // end of ?
	string usedSymbols;		  // used symbols per 1 match
	string word;			  // keyword
	string currentAppearance; // word entered by user
	string man[PHASES] = { "", "\n  |\n  |\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |\n  |\n  |\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |    |_|\n  |\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |   /|_|\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    /\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    / \\ \n  | GAME OVER\n _|__________" };

public:
	manikin() { // constructor : create game for 2 persons
		cout << "Введіть слово: ";
		cin >> word;
		cout << "\x1b[1A"  // Move cursor up one
			<< "\x1b[2K"; // Delete the entire line
		for (int i = 0; i < (int)word.size()/2; i++)
			currentAppearance.push_back('_');
		currentAppearance += "                         \0";
	}
	manikin(int mode) { // constructor : create game for 1 person
		word = randomSelector("dict.txt");
		cout << word << endl;

		for (int i = 0; i < (int)word.size()/2; i++)
			currentAppearance.push_back('_');
		currentAppearance += "                         \0";
	}
	inline void print_phase(int i) { cout << man[i]; } // print curent phase of hangman
	inline string getWord() { return word; }		   // (where used?)
	inline int getPhase() { return phase; }			   // return curent phase
	inline bool isend() { return end; }				   // return game status (end or not)

	int step() { // do one step in the match, return score
		if (phase == PHASES - 1) { // if user on 8th phase,
			phase++;			   // go to last phase
			end = true; 		   // and end the game
			return 0;
		}
        string ch;
		if (end == false) { // if game continue
			cout << "\n" << currentAppearance << endl; // print current input
			cout << "\nВгадайте букву: ";
			//cout << "\x1b[1A" << "\x1b[2K";  // Move cursor up one  Delete the entire line
            int ind;
			do{
				ind = 0;
                cin >> ch; // get new letter
                ind = charValidation(ch);
            }while(!ind);       //check input (only letters && only 1 char)

            //char ch = chIn[0];
            //ch = tolower(ch);

            //cout << "\033[2J\033[1;1H"; //special screen clean command

			// if letter is not in the used symbols and not in the keyword then go to next phase
			if (usedSymbols.find(ch) == string::npos && word.find(ch) == string::npos) { phase++; }

			// if letter is in the keyword
			if (word.find(ch) != string::npos /*|| word.find(toupper(ch)) != string::npos*/) {
				cout << "Буква знайдена!!" << endl;
    			int nPos = word.find(ch, 0); // first occurrence
    			while(nPos != string::npos){

					currentAppearance.replace(nPos, 2, ch);
					currentAppearance.insert(nPos+2 ,"_");					
					nPos = word.find(ch, nPos + 1);
									
    			}
				

			}
			currentAppearance = currentAppearance.substr(0, word.size()-1);

			if (usedSymbols.find(ch) == string::npos) { // if letter was not in the used symbols
				usedSymbols.append(ch); // then add it
				usedSymbols.push_back(' '); // and add a space
			}
			else { cout << "Цей символ ви вже вгадували!" << endl; } // otherwise print that letter was used
		}

		if (strCompare(word, currentAppearance)) { // comparing input and keyword, if true
			end = true; // then end the game, user win
			cout << "СЛОВО ВІДГАДАНЕ! - " << currentAppearance << endl;
			return 1;
		}

		cout << "Використані симовли: " << usedSymbols << endl;

		return 0;
	}
};

int main() {
	setlocale(LC_ALL, "Ukrainian");
	string answer; // user answer for continue the game
	int score[2] = { 0, 0 }; // score points for both playes
	int counter[2]; // steps counter for both players per 1 match
	//auto start, end, time;
	do {
		answer = ' ';
		int mode;
		do {
			cout << "Виберіть режим гри (1 - з комп'ютером, 2 - для двох): ";
			cin >> mode;
			if (mode != 1 && mode != 2) // check if the input correct
				cout << "Неправильний вибір, повторіть!" << endl;
		} while (mode != 1 && mode != 2); // loop until correct input

		if (mode == 1) { // mode : user and computer
			string reply; // user reply for continue guess
			do {
				counter[0] = 0; // 0 steps in the beginning
				reply = ' ';
				manikin* player1 = new manikin(1); // create a keyword and output for 1 user
				auto start1 = std::chrono::steady_clock::now(); // get the current time
				play(player1, score[0], counter[0]); // game function
				auto end1 = std::chrono::steady_clock::now();
				auto time1 = std::chrono::duration_cast<std::chrono::seconds>(end1 - start1); // count game duration in seconds
				cout << "\nКількість ходів: " << counter[0] - 1;
				cout << "\nВитрачений час (в секундах): " << time1.count();
				cout << "\nВаш рахунок: " << score[0] << endl
					<< endl;
				cout << "\nЩе одне слово? (y/т) - ";
				cin >> reply;
                cout << "\033[2J\033[1;1H"; //special screen clean command
			} while (reply == "y" || reply == "Y" || reply == "т" || reply == "Т");
			score[0] = 0; // reset the score
		}
		else { // mode : 2 players - users
			int scoreBefore, maxScore;
			string reply; // user reply for continue guess
			do {
				int currentPlayer = 1;
				reply = ' ';
				cout << "До якого рахунку гратимете? ";
				cin >> maxScore;
				do {
					counter[0] = 0; // step counters
					counter[1] = 0; // reset
					cout << "Відгадує гравець " << currentPlayer << ". Гравець " << ((currentPlayer == 1) ? 2 : 1) << " загадує\n";
					scoreBefore = score[currentPlayer - 1];
					manikin* player = new manikin(); // create a manikin for two players, get keyword from user
					auto start2 = std::chrono::steady_clock::now(); // get the current time
					play(player, score[currentPlayer - 1], counter[currentPlayer - 1]); // game function
					auto end2 = std::chrono::steady_clock::now();
					auto time2 = std::chrono::duration_cast<std::chrono::seconds>(end2 - start2); // count game duration in seconds
					cout << "\nКількість ходів: " << counter[currentPlayer - 1] - 1;
					cout << "\nВитрачений час (в секундах): " << time2.count();
					cout << "\nГравець " << currentPlayer << " ваш рахунок: " << score[currentPlayer - 1] << endl
						<< endl;

					if (score[currentPlayer - 1] == scoreBefore) { // if current player loses,
						currentPlayer == 1 ? currentPlayer = 2 : currentPlayer = 1;
					} // then it's next player's turn
// winning condition - player get max score
					if (score[0] == maxScore) { cout << "Гравець 1 переміг!!!!\n\n"; }
					else if (score[1] == maxScore) { cout << "Гравець 2 переміг!!!!\n\n"; }
					fseek(stdin, 0, SEEK_END); // clean the buffer
				} while (score[0] != maxScore && score[1] != maxScore);

				cout << "\nЩе одна гра? (y/т) - ";
				cin >> reply;
                cout << "\033[2J\033[1;1H"; //special screen clean command
				fseek(stdin, 0, SEEK_END); // clean the buffer
				// reset user's score
				score[0] = 0;
				score[1] = 0;
			} while (reply == "y" || reply == "Y" || reply == "т" || reply == "Т");
		}

		cout << "\n\nЗіграти нову гру? (y/т) - ";
		fseek(stdin, 0, SEEK_END); // clean the buffer
		cin >> answer;
        cout << "\033[2J\033[1;1H"; //special screen clean command
		fseek(stdin, 0, SEEK_END); // clean the buffer

	} while (answer == "y" || answer == "Y" || answer == "т" || answer == "Т");
}

int charValidation(string chIn){
	for(int i = 0; i < (int)chIn.size(); i++){
		if(isalnum(chIn[i])){
			cout << "Помилка, введіть ще раз: ";
			return 0;
		}
	}

	if(chIn.size() != 2){
		cout << "Введіть лише 1 букву! Ще раз: ";
		return 0;
	}

	string alphabet[] = {"А","Б","В","Г","Ґ","Д","Е","Є","З","Ж","И","І","Ї","Й","К","Л","М","Н","О","П","Р","С","Т","У","Ф","Х","Ц","Ч","Ш","Щ","Ь","Ю","Я",
						"а","б","в","г","ґ","д","е","є","ж","з","и","і","ї","й","к","л","м","н","о","п","р","с","т","у","ф","х","ц","ч","ш","щ","ь","ю","я"};
	for(int i = 0; i < 66; i++){
		if(chIn == alphabet[i]){
			return 1;
		}

	}

	cout << "Помилка, введіть ща раз: ";	
	return 0;
}

int strCompare(string word, string current) { // function that compare two strings
	wstring wWord = wstring(word.begin(), word.end());
	wstring wCurrent = wstring(current.begin(), current.end());
	const wchar_t * wWordArr = wWord.c_str();
	const wchar_t * wCurrentArr = wCurrent.c_str();
	int size = wWord.size() -1; // get the keyword's size

	for (int i = 0; i < size; i+=2) { // for each char until the last one compare keyword and user's input
		
		if (wWordArr[i] != wCurrentArr[i]) { // if they aren't the same return 0
			return 0;
		}
	}
	return 1; // otherwise keyword and user's input are the same so return 1
}

void play(manikin* player1, int& score1, int& counter) { // game function
	int p = player1->getPhase(); // get current hangman's phase

	while (p < PHASES) { // while current phase < max amount of phases
		if (player1->isend() == false) { // if game continue
			player1->print_phase(p); // print current phase
			score1 += player1->step(); // add a score point from one step
			p = player1->getPhase(); // change the phase
			counter++; // increment step counter
        }
		else { break; } // current phase is the last, break the game
	}
}

string randomSelector(string fileName) { // function that return random keyword from dictionary file
	fstream file;
	string word;
	srand(time(0)); // random seed by time
	file.open(fileName); // open the file

    string dict((std::istreambuf_iterator<char>(file)),
                (std::istreambuf_iterator<char>()));    // get file data and put into string variable dict

	if (isspace(dict[0])) { // if first char is space symbol then there're no word in the dictionary
		cout << "В словнику відсутні слова! Помилка!";
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