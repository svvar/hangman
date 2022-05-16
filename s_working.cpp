/*main file*/
#include <iostream>
#include <string> // використовуються функції : string.find(), string.push_back() string::npos, string.size()
#include <cctype> // використовуються функції : toupper()
#include <fstream> // використовуються функції : file.open(), file.close(), getline()
#include <cstdlib> // використовуються функції : srand(), rand(), exit()
#include <chrono>
#define PHASES 9 // фази шибенеці (перша + 8 помилок)
#define ALPHASIZE 33
using namespace std;

class manikin; // основний клас
// функції :
string charValidation(string chIn, bool &ind);
string randomSelector(string fileName); // функція, що повертає випадкове слово зі словника
int strCompare(string word, string current);  // функція, що порівнює ключове слово і поточне
void play(manikin* player1, int& score1, int& counter); // основна функція гри


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
	manikin() { // конструктор : створити гру для 2 гравців
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

	manikin(int mode) { // конструктор : створити гру для 1 гравця
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

	inline void print_phase(int i) { cout << man[i]; } // надрукувати поточну фазу шибениці
	inline string getWord() { return word; }		   // повертає ключове слово
	inline int getPhase() { return phase; }			   // повертає поточну фазу
	inline bool isend() { return end; }				   // повертає статус гри (кінець або ні)

	int step() { // один крок гри, повертає рахунок
		if (phase == PHASES - 1) { // якщо гравець на 8-ій фазі,
			phase++;			   // то перейти до останньої
			end = true; 		   // та закінчити гру
			return 0;              // на рахунок повертається 0
		}
        string ch;
		if (end == false) { // якщо гра продовжується
			cout << "\n" << currentAppearance << endl; // надрукувати поточний ввід користувача
			cout << "\nВгадайте букву: ";
            bool ind;
			do{
				ind = false;
                cin >> ch; // користувач вводить нову літеру
                ch = charValidation(ch, ind); // перевірка вводу (лише літери && лише 1 символ)
            }while(!ind);       

            cout << "\033[2J\033[1;1H"; //команда для очищення екрану

			// якщо літера не знаходиться у використаних та не у ключовому слові, тоді перейти до наступної фази
			if (usedSymbols.find(ch) == string::npos && word.find(ch) == string::npos) { phase++; }

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
			}

			if (usedSymbols.find(ch) == string::npos) { // якщо літера ще не входила до використаних
				usedSymbols.append(ch); // додати її
				usedSymbols.push_back(' '); // додати проміжок
			}
			else { cout << "Цей символ ви вже вгадували!" << endl; } // інакше вивести повідомлення про використання
		}

		if (strCompare(word, currentAppearance)) { // порівнюємо ввід і ключове слово, якщо true
			end = true; // закінчуємо гру, гравець переміг
			cout << "СЛОВО ВІДГАДАНЕ! - " << currentAppearance << endl;
			return 1; // на рахунок повертається 1
		}

		cout << "Використані симовли: " << usedSymbols << endl;

		return 0;
	}
};

int main() {
	setlocale(LC_ALL, "Ukrainian");
	string answer; // відповідь користувача на продовження гри
	int score[2] = { 0, 0 }; // рахунок для обох гравців
	int counter[2]; // лічильник кроків для обох гравців
	do {
		answer = ' ';
		int mode;
		do {
			cout << "Виберіть режим гри (1 - з комп'ютером, 2 - для двох): ";
			cin >> mode;
			if (mode != 1 && mode != 2) // перевірка вводу
				cout << "Неправильний вибір, повторіть!" << endl;
		} while (mode != 1 && mode != 2); // зациклити, доки ввід не буде вірним

		if (mode == 1) { // режим 1 : користувач і комп'ютер
			string reply; // відповідь користувача на продовження відгадування
			do {
				counter[0] = 0; // 0 кроків на початку (скидання лічильника)
				reply = ' ';
				manikin* player1 = new manikin(1); // створення ключового слова і виводу на 1 гравця
				auto start1 = std::chrono::steady_clock::now(); // поточний час
				play(player1, score[0], counter[0]); // ігрова функція
				auto end1 = std::chrono::steady_clock::now();
				auto time1 = std::chrono::duration_cast<std::chrono::seconds>(end1 - start1); // рахує тривалість гри у секундах
				cout << "\nКількість ходів: " << counter[0] - 1;
				cout << "\nВитрачений час (в секундах): " << time1.count();
				cout << "\nВаш рахунок: " << score[0] << endl
					<< endl;
				cout << "\nЩе одне слово? (y/т) - ";
				cin >> reply;
                cout << "\033[2J\033[1;1H"; // команда очищення екрану
			} while (reply == "y" || reply == "Y" || reply == "т" || reply == "Т");
			score[0] = 0; // скидання лічильника рахунку
		}
		else { // режим 2 : 2 гравці-користувачі
			int scoreBefore, maxScore;
			string reply; // відповідь користувача на продовження відгадування
			do {
				int currentPlayer = 1;
				reply = ' ';
				cout << "До якого рахунку гратимете? ";
				cin >> maxScore;
				do {
					counter[0] = 0; // скидання лічильника кроків
					counter[1] = 0; // для обох гравців
					cout << "Відгадує гравець " << currentPlayer << ". Гравець " << ((currentPlayer == 1) ? 2 : 1) << " загадує\n";
					scoreBefore = score[currentPlayer - 1];
					manikin* player = new manikin(); // створити манекен для двох гравців, отримати ключ-слово від користувача
					auto start2 = std::chrono::steady_clock::now(); // поточний час
					play(player, score[currentPlayer - 1], counter[currentPlayer - 1]); // ігрова функція
					auto end2 = std::chrono::steady_clock::now();
					auto time2 = std::chrono::duration_cast<std::chrono::seconds>(end2 - start2); // рахує тривалість гри у секундах
					cout << "\nКількість ходів: " << counter[currentPlayer - 1] - 1;
					cout << "\nВитрачений час (в секундах): " << time2.count();
					cout << "\nГравець " << currentPlayer << " ваш рахунок: " << score[currentPlayer - 1] << endl
						<< endl;

					if (score[currentPlayer - 1] == scoreBefore) { // якщо поточний гравець програв,
						currentPlayer == 1 ? currentPlayer = 2 : currentPlayer = 1;
					} // тоді черга відгатувати переходить до наступного
// умова перемоги - гравець набирає максимальну кількість балів
					if (score[0] == maxScore) { cout << "Гравець 1 переміг!!!!\n\n"; }
					else if (score[1] == maxScore) { cout << "Гравець 2 переміг!!!!\n\n"; }
					fseek(stdin, 0, SEEK_END); // очищення буфера
				} while (score[0] != maxScore && score[1] != maxScore);

				cout << "\nЩе одна гра? (y/т) - ";
				cin >> reply;
                cout << "\033[2J\033[1;1H"; // команда для очищення екрану
				fseek(stdin, 0, SEEK_END); // очищення буфера
				// скидання лічильників рахунку
				score[0] = 0;
				score[1] = 0;
			} while (reply == "y" || reply == "Y" || reply == "т" || reply == "Т");
		}

		cout << "\n\nЗіграти нову гру? (y/т) - ";
		fseek(stdin, 0, SEEK_END); // очищення буфера
		cin >> answer;
        cout << "\033[2J\033[1;1H"; // команда для очищення екрану
		fseek(stdin, 0, SEEK_END); // очищення буфера

	} while (answer == "y" || answer == "Y" || answer == "т" || answer == "Т");
}

string charValidation(string chIn, bool &ind){ // функція перевірки вводу користувача
	for(int i = 0; i < (int)chIn.size(); i++){
		if(isalnum(chIn[i])){
			cout << "Помилка, введіть ще раз: ";
			return 0;
		}
	}

	if(chIn.size() != 2){
		cout << "Введіть лише 1 букву! Ще раз: ";
		return 0;
	}

	string lowAlphabet[] = {"а","б","в","г","ґ","д","е","є","ж","з","и","і","ї","й","к","л","м","н","о","п","р","с","т","у","ф","х","ц","ч","ш","щ","ь","ю","я"};
	string upperAlphabet[] = {"А","Б","В","Г","Ґ","Д","Е","Є","З","Ж","И","І","Ї","Й","К","Л","М","Н","О","П","Р","С","Т","У","Ф","Х","Ц","Ч","Ш","Щ","Ь","Ю","Я"};	
	for(int i = 0; i < ALPHASIZE; i++){
		if(chIn == upperAlphabet[i]){
			chIn = lowAlphabet[i];
		}
		
		if(chIn == lowAlphabet[i]){
			ind = true;
			return chIn;
		}

	}

	cout << "Помилка, введіть ще раз: ";	
	return 0;
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
		cout << "В словнику відсутні слова! Помилка!";
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