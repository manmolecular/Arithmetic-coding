﻿#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <cmath>
#define _low_edge 0
#define _high_edge 65535
#define EOF_symbol '-'
#define symbol_break '\0'
using namespace std;

class arithmetic
{
	private:
		/* Определение половины, четверти и 3/4 */
		const int first_qtr = _high_edge / 4 + 1;
		const int half = 2 * first_qtr;
		const int third_qtr = 3 * first_qtr;
		/* Основные объекты для работы с текстом */
		string text;
		string abc;
		string encode;
		string decode;
		/* Массив частот */
		int *freq;
	protected:
		/* Режим вывода дебаг-информации */
		static bool debug_mode;
	public:
		/* Показать таблицу частот и символов */
		void show_table();
		/* Вернуть закодированную строку */
		string get_encode();
		/* Вернуть раскодированную строку */
		string get_decode();
		/* Вернуть исходный текст */
		string get_text();
		/* Поместить текст из файла в string text*/
		void get_file(string filename);
		/* Поместить текст из входной input строки в string text */
		void input_string(string input);
		/* Ввести входной текст с клавиатуры */
		void input_keyboard();
		/* Создать алфавит */
		void get_abc();
		/* Создать массив частот */
		void get_frequency();
		/* Получение индекса следующего символа */
		void get_next_symbol(int i, int *current);
		/* Записать биты */
		string write_bits(bool bit, int bits_to_foll);
		/* Вывести полученный текст в файл */
		void output_file(string filename);
		/* Метод кодирования текста */
		void encode_text();
		/* Перевод 16 бит строки в int */
		int to_int(int _pos);
		/* Перевод int в биты */
		string to_bits_16(int _value);
		/* Метод добавления бита к int числу */
		int add_bit(int value, int count_taken, bool &flag);
		/* Метод раскодирования текста */
		void decode_text();
};