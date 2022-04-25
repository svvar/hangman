/*main file*/
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#define PHASES 8
using namespace std;

//functions
string randomSelector(string fileName);
int strCompare(string word, string current);

//Hangman output (temporary)
class manikin {
private:
    int phase = 0;
    int currentPlayer = 1;
    string usedSymbols;
    string word;
    string currentAppearance;
    string man[PHASES] = { "\n  |\n  |\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |\n  |\n  |\n  |\n _|__________",
    "\n _______\n  |     |\n  |    (_)\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |    |_|\n  |\n  |\n _|__________",
    "\n _______\n  |     |\n  |    (_)\n  |   /|_|\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |\n  |\n _|__________",
    "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    /\n  |\n _|__________",
    "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    / \\ \n  | GAME OVER\n _|__________" };

public:
    manikin();
    manikin(int mode) {
        if (mode == 1) {
            word = randomSelector("dict.txt");
        }
        else {
            cout << "Відгадує гравець " << ((currentPlayer == 1) ? 2 : 1) << ". Гравець " << currentPlayer << " загадує\n";
            cout << "Введіть слово: ";
            cin >> word;            
            currentPlayer == 1 ? currentPlayer = 2 : currentPlayer = 1;
        }

        for(int i = 0 ; i < word.size()-1; i++)
            currentAppearance.push_back('_');
        currentAppearance.push_back('\0');
    }
    inline void print_phase(int i) { cout << man[i]; }
    string getWord() { return word; }
    int getPhase() { return phase; }
    void updatePhase() { phase++; }
    int step() {
        char ch;
        cout << "\n" << currentAppearance << endl;
        cout << "Вгадайте букву: ";
        cin >> ch;
        
        if(usedSymbols.find(ch) == string::npos && word.find(ch) == string::npos){phase++;}

        if(word.find(ch) != string::npos){
            int indices[25];
            cout << "Вгадав!!" << endl;
            for(int i = 0; i < word.size(); i++)
                if(word[i] == ch)
                    currentAppearance[i] = ch;

        }
        //else{cout << "Цей символ ви вже вгадували!" << endl;}


        if (usedSymbols.find(ch) == string::npos) {usedSymbols.push_back(ch); usedSymbols.push_back(' ');}
        else {cout << "Цей символ ви вже вгадували!" << endl;}

        //cout << word.size() << "      " << currentAppearance.size() << "     " << strCompare(word, currentAppearance) << endl;
        
        //cout << (word == currentAppearance) << endl;
        //TODO (НЕ ПРАЦЮЄ)*****************************
        if(strCompare(word, currentAppearance)){
            cout << "WIN" << endl;
            return 1;
        }
        //**********************************************
        cout << "Використані симовли: " << usedSymbols << endl;
        return 0;
    }

};


int main() {
    setlocale(LC_CTYPE, "ukr");
    char answer;
    int score1, score2;
    do {
        int mode;
        do {
            cout << "Виберіть режим гри: 1 - з комп'ютером, 2 - для двох ";
            cin >> mode;
            if (mode != 1 && mode != 2)
                cout << "Неправильний вибір, повторіть!" << endl;
        } while (mode != 1 && mode != 2);

        manikin * game;
        if (mode == 1) {
            game = new manikin(1);
        }
        else {
            game = new manikin(2);
        }

        cout << game->getWord() << endl;

        int p = game->getPhase();
        while (p < PHASES) {
            game->print_phase(p);
            //if(p < 8)
                game->step();
            p = game->getPhase();
            //game->updatePhase();
        }

       

        //man.print_phase(1);
       // man.print_phase(3);
       // man.print_phase(8);


        //просто тест (вибір випадкового слова і його друк)
        //string word = randomSelector("dict.txt");
        //cout << word;

        cout << "\n\nЗіграти ще раз? (y/т) - так  ";
        fseek(stdin, 0, SEEK_END);
        cin >> answer;

    } while (answer == 'y' || answer == 'Y' || answer == 'т' || answer == 'Т');
}

int strCompare(string word, string current){
    int size = word.size();
    for(int i = 0; i < size-1; i++){
        if(word[i] != current[i])
            return 0;
    }
    return 1;
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
            word += ch;
        }
        if (ch == ' ')
            spaces++;
    }
    file.close();
    return (word);
}