/*main file*/
#include <iostream>
#include <string>
#include <fstream>
#define PHASES 8
using namespace std;

//functions
string randomSelector(string fileName);

//Hangman output (temporary)
class manikin {
private:
    int phase = 1;
    int currentPlayer = 1;
    string word;
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
            cout << "Відгадує гравець " << ((currentPlayer == 1) ? 2 : 1) << ". Гравець " << currentPlayer << " відгадує\n";
            cout << "Введіть слово: ";
            cin >> word;
            currentPlayer == 1 ? currentPlayer = 2 : currentPlayer = 1;
        }
    }
    inline void print_phase(int i) { cout << man[i - 1]; }
    string getWord() { return word; }
    int getPhase() { return phase; }
    void updatePhase() { phase++; }

};


int main() {
    char answer;
    int score1, score2;
    do {
        int mode;
        do {
            cout << "Виберіть режим гри: 1 - з комп'ютером, 2 - для двох";
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

    
        while (game->getPhase() < 9) {
            game->print_phase(game->getPhase());
            game->updatePhase();
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