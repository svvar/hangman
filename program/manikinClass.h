#ifndef MANIKINCLASS_H
#define MANIKINCLASS_H
#include "constants.h"

using namespace std;
// Вивід чоловічка на шибенеці
class manikin {
private:
	int phase = 0;			  // лічильник фаз
	bool end = false;		  // кінець гри
	string usedSymbols;		  // використані літери
	string word;			  // ключове слово
	string currentAppearance; // поточний ввід користувача
	string man[PHASES] = { "", "\n  |\n  |\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |\n  |\n  |\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |    |_|\n  |\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |   /|_|\n  |\n  |\n _|__________", "\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    /\n  |\n _|__________",
							"\n _______\n  |     |\n  |    (_)\n  |   /|_|\\ \n  |    / \\ \n  | GAME OVER\n _|__________" };

public:
	manikin(); // конструктор : створити гру для 2 гравців
	manikin(int mode); // конструктор : створити гру для 1 гравця

	inline void print_phase(int i); // надрукувати поточну фазу шибениці
	inline std::string getWord();		   // повертає ключове слово
	inline int getPhase();		   // повертає поточну фазу
	inline bool isend();			   // повертає статус гри (кінець або ні)

	int step(); // один крок гри, повертає рахунок
		
};

#endif