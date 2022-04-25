/*main file*/
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#define PHASES 8
using namespace std;

//functions
class manikin;
string randomSelector(string fileName);
int strCompare(string word, string current);
void gameMode(int mode, manikin *player1, manikin *player2);
void play1(manikin* player1, int& score1);
void play2(manikin* player1, manikin* player2, int& score1, int& score2);

//Hangman output (temporary)
class manikin {
private:
    int phase = 0;
    //int score = 0;
    bool end = false;
    string usedSymbols;
    string word;
    string currentAppearance;
    string man[PHASES] = { "\n  |\n  |\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |\n  |\n  |\n  |\n _|__________",
    "\n _______\n  |     |\n  |    (_)\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |    |_|\n  |\n  |\n _|__________",
    "\n _______\n  |     |\n  |    (_)\n  |   /|_|\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |\n  |\n _|__________",
    "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    /\n  |\n _|__________",
    "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    / \\ \n  | GAME OVER\n _|__________" };

public:
    manikin() {
        cout << "Введіть слово: ";
        cin >> word;
		cout
            << "\x1b[1A" // Move cursor up one
            << "\x1b[2K"; // Delete the entire line
        for (int i = 0; i < word.size(); i++)
            currentAppearance.push_back('_');
        currentAppearance.push_back('\0');
    }
    manikin(int mode) {
        word = randomSelector("dict.txt");        

        for (int i = 0; i < word.size(); i++)
            currentAppearance.push_back('_');
        currentAppearance.push_back('\0');
    }
    inline void print_phase(int i) { cout << man[i]; }
    string getWord() { return word; }
    int getPhase() { return phase; }
    //int getScore() { return score; }
    bool isend() { return end; }
    void setWord(string word) {
        cout << "Введіть слово: ";
        cin >> word;
        for (int i = 0; i < word.size(); i++)
            currentAppearance.push_back('_');
        currentAppearance.push_back('\0');
    }
    int step() {
        char ch;
        if (phase == 7) {
            phase++;
            end = true;
            return 0;
        }
        if (end == false) {
            cout << "\n" << currentAppearance << endl;
            cout << "Вгадайте букву: ";
            cin >> ch;

            if (usedSymbols.find(ch) == string::npos && word.find(ch) == string::npos) { 
                phase++; }

            if (word.find(ch) != string::npos) {
                int indices[25];
                cout << "Буква знайдена!!" << endl;
                for (int i = 0; i < word.size(); i++)
                    if (word[i] == ch)
                        currentAppearance[i] = ch;

            }
            //else{cout << "Цей символ ви вже вгадували!" << endl;}


            if (usedSymbols.find(ch) == string::npos) { usedSymbols.push_back(ch); usedSymbols.push_back(' '); }
            else { cout << "Цей символ ви вже вгадували!" << endl; }
        }

        if (strCompare(word, currentAppearance)) {
            end = true;
            //score++;
            cout << "СЛОВО ВІДГАДАНЕ!" << endl;
            return 1;
        }

        cout << "Використані симовли: " << usedSymbols << endl;
        
        return 0;
    }

};


int main() {
    setlocale(LC_CTYPE, "ukr");
    //bool end = false;
    char answer;
    int score[2] = {0, 0};
    do {
        //bool end = false;
        int mode;
        do {
            cout << "Виберіть режим гри: 1 - з комп'ютером, 2 - для двох ";
            cin >> mode;
            if (mode != 1 && mode != 2)
                cout << "Неправильний вибір, повторіть!" << endl;
        } while (mode != 1 && mode != 2);


        if (mode == 1) {
            char reply;
            do {
                reply = ' ';
                manikin *player1 = new manikin(1);
                play1(player1, score[0]);
                cout << "\nВаш рахунок: " << score[0] << endl << endl;
                cout << "\nЩе одне слово? (y/т) - так ";
                cin >> reply;
            } while (reply == 'y' || reply == 'Y' || reply == 'т' || reply == 'Т');
            score[0] = 0;
        }
        else {
            int scoreBefore, maxScore;
            char reply;
            do {
                int currentPlayer = 1;
                reply = ' ';
                cout << "До якого рахунку гратимете? ";
                cin >> maxScore;
                do {                    
                    cout << "Відгадує гравець " << currentPlayer << ". Гравець " << ((currentPlayer == 1) ? 2 : 1) << " загадує\n";
                    scoreBefore = score[currentPlayer - 1];
                    manikin* player = new manikin();
                    play1(player, score[currentPlayer - 1]);
                    cout << "\nГравець "<<currentPlayer<<" ваш рахунок: " << score[currentPlayer-1] << endl << endl;


                    if (score[currentPlayer - 1] == scoreBefore)
                        currentPlayer == 1 ? currentPlayer = 2 : currentPlayer = 1;
                    if (score[0] == maxScore) {
                        cout << "Гравець 1 переміг!!!!\n\n";
                    }
                    if (score[1] == maxScore) {
                        cout << "Гравець 2 переміг!!!!\n\n";
                    }
                    fseek(stdin, 0, SEEK_END);
                } while (score[0] != maxScore && score[1] != maxScore);

                cout << "\nЩе одна гра? (y/т) - так ";
                cin >> reply;
                fseek(stdin, 0, SEEK_END);

                score[0] = 0; score[1] = 1;
            } while (reply == 'y' || reply == 'Y' || reply == 'т' || reply == 'Т');
        }


        cout << "\n\nЗіграти нову гру? (y/т) - так  ";
        fseek(stdin, 0, SEEK_END);
        cin >> answer;
        fseek(stdin, 0, SEEK_END);

    } while (answer == 'y' || answer == 'Y' || answer == 'т' || answer == 'Т');
}

int strCompare(string word, string current) {
    int size = word.size();
    for (int i = 0; i < size; i++) {
        if (word[i] != current[i])
            return 0;
    }
    return 1;
}

void play1(manikin* player1, int& score1) {    
    int p = player1->getPhase();
        
    while (p < PHASES) {
        if (player1->isend() == false) {
            player1->print_phase(p);
            score1 += player1->step();
            p = player1->getPhase();
        }
        else
            break;
    }   
    
}

string randomSelector(string fileName) {
    //слова в словнику повинні бути записані через пробіл, після останнього слова теж пробіл
    //можна доробити, щоб приймалося розділення слів будь-якими isspace символами

    fstream file;
    string dict, word;
    srand(time(0));
    file.open(fileName);

    getline(file, dict);
    if (isspace(dict[0])) {
        cout << "В словнику відсутні слова! Помилка!";
        exit(1);
    }

    int wordsCount = 0;
    for (char buf : dict) {
        if (buf == ' ')
            wordsCount++;
    }

    int randomNumber = rand() % wordsCount;

    int spaces = 0;
    for (char ch : dict) {
        if (spaces == randomNumber) {
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