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
      cout << "³����� ������� " << ((currentPlayer == 1) ? 2 : 1) << ". ������� " << currentPlayer << " ������\n";
      cout << "������ �����: ";
      cin >> word;
      currentPlayer == 1 ? currentPlayer = 2 : currentPlayer = 1;
    }

    for (int i = 0; i < word.size(); i++)
      currentAppearance.push_back('_');
    currentAppearance.push_back('\0');
  }
  inline void print_phase(int i) { cout << man[i]; }
  string getWord() { return word; }
  int getPhase() { return phase; }
  void updatePhase() { phase++; }
  int step(bool& end) {
    if (end == false) {
      char ch;
      cout << "\n" << currentAppearance << endl;
      cout << "�������� �����: ";
      cin >> ch;

      if (usedSymbols.find(ch) == string::npos && word.find(ch) == string::npos) { phase++; }

      if (word.find(ch) != string::npos) {
        int indices[25];
        cout << "������!!" << endl;
        for (int i = 0; i < word.size(); i++)
          if (word[i] == ch)
            currentAppearance[i] = ch;

      }
      //else{cout << "��� ������ �� ��� ���������!" << endl;}


      if (usedSymbols.find(ch) == string::npos) { usedSymbols.push_back(ch); usedSymbols.push_back(' '); }
      else { cout << "��� ������ �� ��� ���������!" << endl; }
    }
    //cout << word.size() << "      " << currentAppearance.size() << "     " << strCompare(word, currentAppearance) << endl;

    //cout << (word == currentAppearance) << endl;
    //TODO (�� ����ު)*****************************
    if (strCompare(word, currentAppearance)) {
      end = true;
      cout << "WIN" << endl;
      return 1;
    }
    //**********************************************
    cout << "���������� �������: " << usedSymbols << endl;
    return 0;
  }

};


int main() {
  setlocale(LC_CTYPE, "ukr");
  bool end = false; // �� ��������� ��� ���������� �������, ������� ��� ��� �������
  char answer;
  int score1, score2;
  do {
    int mode;
    do {
      cout << "������� ����� ���: 1 - � ����'������, 2 - ��� ���� ";
      cin >> mode;
      if (mode != 1 && mode != 2)
        cout << "������������ ����, ��������!" << endl;
    } while (mode != 1 && mode != 2);

    manikin* game;
    if (mode == 1) {
      game = new manikin(1);
    }
    else {
      game = new manikin(2);
    }

    cout << game->getWord() << endl;

    int p = game->getPhase();
    while (p < PHASES) {
      if (end == false) {
        game->print_phase(p);
        //if(p < 8)
        game->step(end);
        p = game->getPhase();
        //game->updatePhase();
      }
      else
        break;
    }



    //man.print_phase(1);
   // man.print_phase(3);
   // man.print_phase(8);


    //������ ���� (���� ����������� ����� � ���� ����)
    //string word = randomSelector("dict.txt");
    //cout << word;

    cout << "\n\nǳ����� �� ���? (y/�) - ���  ";
    fseek(stdin, 0, SEEK_END);
    cin >> answer;

  } while (answer == 'y' || answer == 'Y' || answer == '�' || answer == '�');
}

int strCompare(string word, string current) {
  int size = word.size();
  for (int i = 0; i < size; i++) {
    if (word[i] != current[i])
      return 0;
  }
  return 1;
}

string randomSelector(string fileName) {
  //����� � �������� ������ ���� ������� ����� �����, ���� ���������� ����� ��� �����
  //����� ��������, ��� ���������� ��������� ��� ����-����� isspace ���������

  fstream file;
  string dict, word;
  srand(time(0));
  file.open(fileName);

  getline(file, dict);
  if (isspace(dict[0])) {
    cout << "� �������� ������ �����! �������!";
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