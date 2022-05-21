/*main file*/
#include <iostream>
#include <string> // використовуються функції : string.find(), string.push_back() string::npos, string.size()
#include <cctype> // використовуються функції : toupper()
#include <fstream> // використовуються функції : file.open(), file.close(), getline()
#include <cstdlib> // використовуються функції : srand(), rand(), exit()
#include <chrono>

#include "manikinClass.h"
#include "functions.h"
using namespace std;


int main() {
	setlocale(LC_ALL, "Ukrainian");
	string answer; // відповідь користувача на продовження гри
	int score[2] = { 0, 0 }; // рахунок для обох гравців
	int counter[2]; // лічильник кроків для обох гравців
	do {
		answer = ' ';
		string mode;
		do {
			cout << "Виберіть режим гри (1 - з комп'ютером, 2 - для двох): ";
			cin >> mode;
			if (mode != "1" && mode != "2") // перевірка вводу
				cout << "\x1b[31mНеправильний вибір, повторіть!\x1b[97m" << endl;
			fseek(stdin, 0, SEEK_END); // очищення буфера
		} while (mode != "1" && mode != "2"); // зациклити, доки ввід не буде вірним

		if (mode == "1") { // режим 1 : користувач і комп'ютер
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
				cout << "\nЩе одне слово? (т) - ";
				cin >> reply;
                cout << "\033[2J\033[1;1H"; // команда очищення екрану
			} while (reply == "т" || reply == "Т");
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
					if (score[0] == maxScore) { cout << "\x1b[32mГравець 1 переміг!!!!\x1b[97m\n\n"; }
					else if (score[1] == maxScore) { cout << "\x1b[32mГравець 2 переміг!!!!\x1b[97m\n\n"; }
					fseek(stdin, 0, SEEK_END); // очищення буфера
				} while (score[0] != maxScore && score[1] != maxScore);

				cout << "\nЩе одна гра? (т) - ";
				cin >> reply;
                cout << "\033[2J\033[1;1H"; // команда для очищення екрану
				fseek(stdin, 0, SEEK_END); // очищення буфера
				// скидання лічильників рахунку
				score[0] = 0;
				score[1] = 0;
			} while (reply == "т" || reply == "Т");
		}

		cout << "\n\nЗіграти нову гру? (т) - ";
		fseek(stdin, 0, SEEK_END); // очищення буфера
		cin >> answer;
        cout << "\033[2J\033[1;1H"; // команда для очищення екрану
		fseek(stdin, 0, SEEK_END); // очищення буфера

	} while (answer == "т" || answer == "Т");
}