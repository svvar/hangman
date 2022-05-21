#ifndef FUNCTIONS
#define FUNCTIONS

#include "manikinClass.h"

string charValidation(string ch, string &ch2, string word, bool &ind);  // функція перевірки вводу користувача	

int strCompare(string word, string current); // функція, що порівнює ключове слово і поточне

void play(manikin* player1, int& score1, int& counter); // ігрова функція

string randomSelector(string fileName); // функція, що повертає випадкове ключ-слово зі словника
	

#endif