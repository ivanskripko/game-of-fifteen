#include <iostream>
#include <iomanip>
#include <ctime>
#include <stdlib.h>

using namespace std;

/*
Написать программу для игры в пятнашки (http://ru.wikipedia.org/wiki/Пятнашки).
Программа должна генерировать начальное расположение фишек, затем
пользователь должен управлять «пустышкой», чтобы привести фишки к решению
головоломки. После каждого хода пользователя программа должна выводить новое
расположение фишек. Для хранения расположения фишек нужно использовать
двумерный массив. Печать поля, а также другие вспомогательные действия
нужно вынести в отдельные функции. 
*/

void printField(int** field, int size);
bool isWin(int** field, int size, int genBound);
bool isElementAlreadyInArray(int** field, int size, int elem);
void generateField(int** field, int size, int genBound);
void move(int** field, int size, int key);
void swap (int* a, int* b);
void findEmptyCell(int** field, int size, int* index_i, int* index_j);
void makeSolvable(int** field, int size);

enum Keys {UP = 2, LEFT = 4, RIGHT = 6, DOWN = 8};

int main() 
{  
	srand(time(0));	
	
	int size = 0;
	do {
		cout << "Please input the size of the field (from 2 to 9): ";
		cin >> size;
		cout << endl;
	} while (size < 2 || size > 9);

	int genBound = size * size;

	cout << "\nUse 2, 4, 6, 8 to move cells. Press 0 to exit.\n" << endl;


	int** gameField = new int*[size];
	for (int i = 0; i < size; i++) {
		gameField[i] = new int[size];
	}

	generateField(gameField, size, genBound);	
	printField(gameField, size);

	int key = -1;
	
	while ((key != 0) && (!isWin(gameField, size, genBound))) { 
		cout << "Your turn: ";
		cin >> key;
		move(gameField, size, key);
		printField(gameField, size);
	}
	
	if (isWin(gameField, size, genBound)) {
		cout << "You won! " << endl;		
	}
	else {
		cout << "You lost! " << endl;		
	}

	for (int i = 0; i < size; i++) {
		delete[] gameField[i];
	}
	delete[] gameField;

	return 0;
}

void printField(int** field, int size) 
{	
	const int width = 2;
	cout << right;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (field[i][j] == 0) {
				cout << setw(width + 1) << ' ';
				continue;
			}
			cout << setw(width) << field[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;
}

bool isElementAlreadyInArray(int** field, int size, int elem) 
{
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (elem == field[i][j]) {
				return true;
			}			
		}
	}
	return false;
}

void generateField(int** field, int size, int genBound) 
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			field[i][j] = -1;
		}
	}

	int num = 0;

	// лучше сделать перемешиваниями, т.к. эта функция работает очень долго
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			do {
				num = (rand() % genBound);			
			} while (isElementAlreadyInArray(field, size, num));		
		field[i][j] = num;
		}		
	}

	makeSolvable(field, size);
}

bool isWin(int** field, int size, int genBound) 
{
	bool isWin = false;
	int element = 0;

	if (field[size-1][size-1] == 0) { // нет смысла проверять, если последний элемент не пустышка
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (field[i][j] > element) {
					element++;
				}
			}
		}		
	}		

	if (element == genBound - 1) {
		isWin = true;
	}

	return isWin;
}

void move(int** field, int size, int key) 
{
	int i = 0;
	int j = 0;

	switch(key) {
		case UP:		
			findEmptyCell(field, size, &i, &j);
			if (i > 0) {	
				swap(field[i][j], field[i-1][j]);		
			}
			break;

		case RIGHT:
			findEmptyCell(field, size, &i, &j);
			if (j > 0) {		
				swap(field[i][j], field[i][j-1]);
			}
			break;

		case LEFT:
			findEmptyCell(field, size, &i, &j);
			if (j < size-1) {	
				swap(field[i][j], field[i][j+1]);	
			}
			break;

		case DOWN:
			findEmptyCell(field, size, &i, &j);
			if (i < size-1) {	
				swap(field[i][j], field[i+1][j]);		
			}
			break;

		default:
			break;
	}
}

void swap (int* a, int* b) 
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void findEmptyCell(int** field, int size, int* index_i, int* index_j) 
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (field[i][j] == 0) {
				*index_i = i;
				*index_j = j;
				return;				
			}
		}		
	}
}

//http://coding4.me/2013/10/28/fifteen/
void makeSolvable(int** field, int size) 
{
	int oneDimArraySize = size * size;
	int *oneDimArray = new int[oneDimArraySize];
	int index = 0; 

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			oneDimArray[index++] = field[i][j];
		}
	}

	int numberOfPairs = 0; //число пар, где текущий элемент больше следующего по ходу движения по массиву (кроме 0)

	for (int i = 0; i < oneDimArraySize; i++) {
		if (oneDimArray[i] == 0) {
			continue;			
		} 
		for (int j = i; j < oneDimArraySize; j++) {
			if(oneDimArray[i] > oneDimArray[j] && oneDimArray[j] != 0) {
				numberOfPairs++; 
			} 
		}
	}

	delete[] oneDimArray;

	int emptyCell_i = 0;
	int emptyCell_j = 0;
	findEmptyCell(field, size, &emptyCell_i, &emptyCell_j);

	//Если размер поля нечетный и сумма пар четная, то разрешима
	//Если размер поля четный и сумма пар + номер ряда с пустышкой ( считая от 1 ) четная, то разрешима
	if (size % 2 != 0 && numberOfPairs % 2 == 0) {		
		return;
	} else if (size % 2 == 0 && ((numberOfPairs + emptyCell_i + 1) % 2) == 0) {	
		return;
	} else {		
		if (emptyCell_i == 0) { // если пустышка в первом ряду, то меняем последние 2 фишки
			swap(field[size-1][size-1], field[size-1][size-2]);
		} else {
			swap(field[0][0], field[0][1]);	//иначе меняем первые две, чтобы поменять четность
		}
	}

}