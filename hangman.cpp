/*main file*/
#include <iostream>
#include <string>
#include <fstream>
#define PHASES 8
using namespace std;

//Hangman output (temporary)
class manikin {
    private :
        string man[PHASES] = {"\n  |\n  |\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |\n  |\n  |\n  |\n _|__________",
        "\n _______\n  |     |\n  |    (_)\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |    |_|\n  |\n  |\n _|__________", 
        "\n _______\n  |     |\n  |    (_)\n  |   /|_|\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |\n  |\n _|__________",
        "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    /\n  |\n _|__________",
        "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    / \\ \n  | GAME OVER\n _|__________"};

    public :
        inline void print_phase(int i) { cout << man[i-1]; }
};

//functions
string randomSelector(string fileName);


int main(){

    manikin man;
    man.print_phase(1);
    man.print_phase(3);
    man.print_phase(8);


    //просто тест (вибір випадкового слова і його друк)
    string word = randomSelector("dict.txt");
    cout << word;
}




string randomSelector(string fileName) {
    //слова в словнику повинні бути записані через пробіл, після останнього слова теж пробіл
    //можна доробити, щоб приймалося розділення слів будь-якими isspace символами
    
    fstream file;
    string dict, word;
    srand(time(0));
    file.open(fileName);

    getline(file, dict);
    if(isspace(dict[0])){
        cout << "В словнику відсутні слова! Помилка!";
        exit(1);
    }
    
    int wordsCount=0;
    for(char buf : dict){
        if(buf == ' ')
            wordsCount++;
    }

    int randomNumber = rand() % wordsCount; 

    int spaces = 0;
    for(char ch : dict){
        if(spaces == randomNumber){
            word += ch;
        }
        if(ch == ' ')
            spaces++;   
    }
    file.close();
    return (word);
}