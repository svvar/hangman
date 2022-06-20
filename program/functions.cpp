#include "functions.h"

#include <iostream>
#include <string> // використовуються функції : string.find(), string.push_back() string::npos, string.size()
#include <cctype> // використовуються функції : toupper()
#include <fstream> // використовуються функції : file.open(), file.close(), getline()
#include <cstdlib> // використовуються функції : srand(), rand(), exit()


using namespace std;

string wordCheck(string word){
	bool flag;				//умова продовження циклу
	do{
		flag = false;			
		cin >> word;		//зчитуємо слово
		int wordsize = word.size();		//розмір слова в байтах

		bool error = false;				//індикатор наявності помилок
		for(int i = 0; i < wordsize; i++){
			if(isalnum(word[i])){			//коли є англійська буква чи цифра
				error = true;
				flag = true;
			}
		}
		
		int punctcount = 0;					//кількість символів(апострофів та дефісів)
		for(int i = 0; i < wordsize; i++){
			if(ispunct(word[i])){			//коли даний символ є знаком пунктуації
				if(word[i]== '\'' || word[i] == '-'){		//коли це дефіс або апостроф
					punctcount++;							
				}
				else{										//інші символи не підходять
					error = true;					
					flag = true;
				}
			}
		}
		if(error){
			cout << "\x1b[31mПомилка, знайдено недопустимі символи, введіть ще раз: \x1b[97m";
		}
		if(punctcount == wordsize){
			cout << "\x1b[31mПомилка, немає букв, введіть ще раз: \x1b[97m";
			flag = true;
		}
	}while(flag);

	return word;
}

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

	string lowAlphabet[] = {"а","б","в","г","ґ","д","е","є","ж","з","и","і","ї","й","к","л","м","н","о","п","р","с","т","у","ф","х","ц","ч","ш","щ","ь","ю","я"};
	string upperAlphabet[] = {"А","Б","В","Г","Ґ","Д","Е","Є","З","Ж","И","І","Ї","Й","К","Л","М","Н","О","П","Р","С","Т","У","Ф","Х","Ц","Ч","Ш","Щ","Ь","Ю","Я"};	
	
	ind = true;
	
	bool success = false; //робоча змінна щоб не шукати зайвий раз
	int position = -1; //позиція введеної букви в алфавіті
	//визначаємо положення букви в алфавіті
	for(string s : lowAlphabet){
		position++;
		if(ch == s){
			success = true;
			break;
		}
	}
	if(!success){
		position = -1;
		for(string s : upperAlphabet){
			position++;
			if(ch == s)
				break;
		}
	}

	ch2 = lowAlphabet[position];
	ch = upperAlphabet[position];	
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
		else {break;} // інакше - поточна фаза остання, перервати гру
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