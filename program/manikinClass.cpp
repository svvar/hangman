#include "manikinClass.h"
#include "functions.h"
#include "constants.h"

#include <iostream>
using namespace std;

	manikin::manikin() { // конструктор : створити гру для 2 гравців
		cout << "Введіть слово: ";
		cin >> word;
		cout << "\x1b[1A"  // перемістити курсор на одну позицію вверх
			<< "\x1b[2K"; // очистити весь рядок
		for (int i = 0; i < (int)word.size()/2; i++)
			currentAppearance += "ˍ";
		
		for(int i = 0; i < (int)word.size(); i++){ // підтримка для дефізу та апострофа
			if(word[i] == '-') {
				currentAppearance += "ˍ";
				word.replace(i, 1, "־");
				currentAppearance.replace(i, 2, "־");
			}
			
			else if(word[i] == '\''){
				currentAppearance += "ˍ";
				word.replace(i, 1, "ʼ");
				currentAppearance.replace(i, 2, "ʼ");
			}			
		}
		currentAppearance += "\0";
	}

	manikin::manikin(int mode) { // конструктор : створити гру для 1 гравця
		word = randomSelector("dict.txt");

		for (int i = 0; i < (int)word.size()/2; i++)
			currentAppearance += "ˍ";

		for(int i = 0; i < (int)word.size(); i++){
			if(word[i] == '-') {
				word.replace(i, 1, "-");
				currentAppearance.replace(i, 2, "-");
			}
			
			else if(word[i] == '\''){
				word.replace(i, 1, "ʼ");
				currentAppearance.replace(i, 2, "ʼ");
			}		
		}
		currentAppearance += "\0";
	}

		int manikin::step() { // один крок гри, повертає рахунок
		if (phase == PHASES - 1) { // якщо гравець на 8-ій фазі,
			phase++;			   // то перейти до останньої
			end = true; 		   // та закінчити гру
			return 0;              // на рахунок повертається 0
		}
        string ch, ch2;
		if (end == false) { // якщо гра продовжується
			cout << "\n" << currentAppearance << endl; // надрукувати поточний ввід користувача
			cout << "\nВгадайте букву: ";
            bool ind;
			do{
				ind = false;
                cin >> ch; // користувач вводить нову літеру
                ch = charValidation(ch, ch2, word, ind); // перевірка вводу (лише літери && лише 1 символ)
            }while(ind == false);

            cout << "\033[2J\033[1;1H"; //команда для очищення екрану

			// якщо літера не знаходиться у використаних та не у ключовому слові, тоді перейти до наступної фази
			if (usedSymbols.find(ch2) == string::npos && word.find(ch) == string::npos) { phase++; }

			// якщо літера присутня у ключовому слові
			if (word.find(ch) != string::npos) {
				cout << "Буква знайдена!!" << endl;
    			int nPos = word.find(ch); // перша позиція
    			while(nPos != (int)string::npos){ // доки маємо збіги
					if(currentAppearance.substr(nPos, 2) != "־" && currentAppearance.substr(nPos, 2) != "ʼ"){					
						currentAppearance.replace(nPos, 2, ch); // змінюємо поточне слово за позицією
					}
					nPos = word.find(ch, nPos + 1); // перехід до наступної позиції								
    			}

				//коли в слові є і великка і маленька літери, перевіряємо ще раз!
				nPos = word.find(ch2); // перша позиція
    			while(nPos != (int)string::npos){ // доки маємо збіги
					if(currentAppearance.substr(nPos, 2) != "־" && currentAppearance.substr(nPos, 2) != "ʼ"){					
						currentAppearance.replace(nPos, 2, ch2); // змінюємо поточне слово за позицією
					}
					nPos = word.find(ch2, nPos + 1); // перехід до наступної позиції								
    			}
			}

			if (usedSymbols.find(ch2) == string::npos) { // якщо літера ще не входила до використаних
				usedSymbols.append(ch2); // додати її
				usedSymbols.push_back(' '); // додати проміжок
			} //\x1b[31m - red  ||  \x1b[97m - white
			else { cout << "\x1b[31mЦей символ ви вже вгадували!\x1b[97m" << endl; } // інакше вивести повідомлення про використання
		}

		if (strCompare(word, currentAppearance)) { // порівнюємо ввід і ключове слово, якщо true
			end = true; // закінчуємо гру, гравець переміг
			cout << "\x1b[32mСЛОВО ВІДГАДАНЕ! - " << currentAppearance << "\x1b[97m\n";
			return 1; // на рахунок повертається 1
		}

		cout << "Використані симовли: " << usedSymbols << endl;

		return 0;
	}