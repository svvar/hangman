#include "functions.h"

#include <iostream>
#include <string> // використовуються функції : string.find(), string.push_back() string::npos, string.size()
#include <cctype> // використовуються функції : toupper()
#include <fstream> // використовуються функції : file.open(), file.close(), getline()
#include <cstdlib> // використовуються функції : srand(), rand(), exit()


using namespace std;

string charValidation(string ch, string &ch2, string word, bool &ind){ // функція перевірки вводу користувача
	for(int i = 0; i < (int)ch.size(); i++){
		if(isalnum(ch[i])){
			cout << "\x1b[31mПомилка, введіть ще раз: \x1b[97m";
			return "";
		}
	}

	if(ch.size() != 2){
		cout << "\x1b[31mВведіть лише 1 букву! Ще раз: \x1b[97m";
		return "";
	}

	int wordSize = word.size()/2;
	string * wordChars = new string[wordSize];
	for(int i = 0, j = 0; i < wordSize; i++, j+=2)
		wordChars[i] = word.substr(j, 2);

	string lowAlphabet[] = {"а","б","в","г","ґ","д","е","є","ж","з","и","і","ї","й","к","л","м","н","о","п","р","с","т","у","ф","х","ц","ч","ш","щ","ь","ю","я"};
	string upperAlphabet[] = {"А","Б","В","Г","Ґ","Д","Е","Є","З","Ж","И","І","Ї","Й","К","Л","М","Н","О","П","Р","С","Т","У","Ф","Х","Ц","Ч","Ш","Щ","Ь","Ю","Я"};	
	
	ind = true;
	for(int i = 0; i < ALPHASIZE; i++){
		for(int j = 0; j < wordSize; j++){
			if(ch == upperAlphabet[i]){			//коли користувач ввів велику букву
				string cur = wordChars[j];
				if(cur == upperAlphabet[i]){	//коли в слові ця буква є великою
					ch2 = lowAlphabet[i];
					return ch;
				}
				else if(cur == lowAlphabet[i]){		//коли в слові ця буква є маленькою
					ch2 = lowAlphabet[i];
					ch = lowAlphabet[i];
					return ch;
				}
				else{
					ch = lowAlphabet[i];
					ch2 = ch;
					return ch;
				}
			}

			if(ch == lowAlphabet[i]){		//коли користувач ввід маленьку букву
				string cur = wordChars[j];
				if(cur == lowAlphabet[i]){		//коли в слові ця буква є маленькою
					ch2 = lowAlphabet[i];
					return ch;
				}
				else if(cur == upperAlphabet[i]){	//коли в слові ця буква є великою
					ch2 = ch;
					ch = upperAlphabet[i];
					return ch;
				}
				else{
					ch = lowAlphabet[i];
					ch2 = ch;
					return ch;
				}
			}
		}
	}

	cout << "\x1b[31mПомилка, введіть ще раз: \x1b[97m";	
	return ch;
}

int strCompare(string word, string current) { // функція, що порівнює ключове слово і поточне
	wstring wWord = wstring(word.begin(), word.end());
	wstring wCurrent = wstring(current.begin(), current.end());
	const wchar_t * wWordArr = wWord.c_str();
	const wchar_t * wCurrentArr = wCurrent.c_str();
	int size = wWord.size() -1; // довжина ключового слова

	for (int i = 0; i < size; i+=2) { // для кожного символа до останнього порівняти ключ-слово та ввід користувача
		
		if (wWordArr[i] != wCurrentArr[i]) { // якщо маємо відмінність - повернути 0
			return 0;
		}
	}
	return 1; // інакше ключ-слово та ввід користувача однакові, тому повернути 1
}

void play(manikin* player1, int& score1, int& counter) { // ігрова функція
	int p = player1->getPhase(); // поточна фаза шибениці

	while (p < PHASES) { // доки поточна фаза < макс. к-сті фаз,
		if (player1->isend() == false) { // якщо гра продовжується,
			player1->print_phase(p); // надрукувати поточну фазу
			score1 += player1->step(); // додати бали до поточного рахунку
			p = player1->getPhase(); // змінити фазу
			counter++; // інкрементувати лічильник кроків
        }
		else { break; } // інакше - поточна фаза остання, перервати гру
	}
}

string randomSelector(string fileName) { // функція, що повертає випадкове ключ-слово зі словника
	fstream file;
	string word;
	srand(time(0)); // випадкове початкове число (сід) за часом
	file.open(fileName); // відкрити файл

    string dict((std::istreambuf_iterator<char>(file)),
                (std::istreambuf_iterator<char>()));    // отримати дані з файла у змінну словника

	if (isspace(dict[0])) { // якщо перший символ це "space symbol" тоді словник порожній
		cout << "\x1b[31mВ словнику відсутні слова! Помилка!\x1b[97m";
		exit(1);
	}

	// цикл, що рахує кількість слів у словнику
	int wordsCount = 0;
	for (char buf : dict) { // foreach
		if (buf == '\n') { wordsCount++; }
	}

	int randomNumber = rand() % wordsCount; // випадкове число, максимальне значення якого - загальна кількість слів

	int spaces = 0;
	for (char ch : dict) { // foreach цикл для змінної string
		if (spaces == randomNumber) { // рахуємо слова за проміжками, беремо слово за випадковим індексом
			if (ch == '\n') { break; } // якщо символ - перехід на новий рядок, тоді вихід из цикла
			word += ch; // інакше - додати символ до ключ-слова
		}
		if (ch == '\n') { spaces++; } // інкрементуємо лічильник проміжків
	}
	file.close(); // закрити файл
	return (word); // повернути ключ-слово
}