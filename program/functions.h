#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "manikinClass.h"

std::string charValidation(std::string ch, std::string &ch2, std::string word, bool &ind);  // функція перевірки вводу користувача	

int strCompare(std::string word, std::string current); // функція, що порівнює ключове слово і поточне

void play(manikin* player1, int& score1, int& counter); // ігрова функція

std::string randomSelector(std::string fileName); // функція, що повертає випадкове ключ-слово зі словника
	

#endif