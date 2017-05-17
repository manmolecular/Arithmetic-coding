#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <bitset>
#define filename_input "input.txt"
#define filename_output "output.txt"
#define debug_mode true
#define _low_edge 0
#define _high_edge 65535
using namespace std;

const int first_qtr = _high_edge/4 + 1;
const int half = 2 * first_qtr;
const int third_qtr = 3 * first_qtr;

/* Помещение текста из файла в string text */
string get_file(string filename)
{
	string text;
	fstream input;
	input.open(filename, ios::in);
	while (!input.eof())
	{
		string temp;
		getline(input, temp);
		if (!input.eof())
		{
			temp += '\n';
		}
		text += temp;
	}
	input.close();
	return text;
}

/* Создание алфавита */
string get_abc(string text)
{
	/* Добавить символ - в начало*/
	string abc;
	bool exit_flag = 0;
	abc.push_back('-');
	abc.push_back(text[0]);
	for (int i = 1; i < text.size(); i++)
	{
		exit_flag = 0;
		for (int j = 0; j < abc.size(); j++)
		{
			if (abc[j] == text[i])						//Если в нашем алфавите уже есть такая буква
			{
				exit_flag = 1;							//Выходим, если нашли такую букву
				break;
			}
		}
		if (exit_flag == 1)								//Если мы находили букву ранее
		{
			continue;									//Переходим на следующую итерацию для след. буквы
		}
		else											//Если буква найдена не была
		{
			abc.push_back(text[i]);
			continue;
		}
	}
	sort(abc.begin(), abc.end());
	return abc;
}

/* Получение массива частоты использования символов */
void get_frequency(string text, int *frequencies, string abc)
{
	for (int i = 0; i < abc.size(); i++)
	{
		frequencies[i] = 0;
	}
	for (int j = 0; j < abc.size(); j++)
	{
		for (int i = 0; i < text.size(); i++)
		{
			if (abc[j] == text[i])
			{
				frequencies[j]++;
			}
		}
		if (j > 0)
		{
			frequencies[j] += frequencies[j - 1];
		}
	}
}

void get_next_symbol(string text, string abc, int i, int *current)
{
	/* Возвращаем номер символа в алфавите */
	/* Индекс символа из текста в алфавите */
	bool exit = false;
	for (i; i < text.size() && !exit; i++)
	{
		char temp_text = text[i];
		for (int j = 0; j < abc.size(); j++)
		{
			char temp_abc = abc[j];
			if (temp_text == temp_abc)
			{
				*current = j;
				exit = true;
				break;
			}
		}
	}
}

/* Запись битов */
string write_bits(bool bit, int bits_to_foll)
{
	string temp;
	temp += to_string(bit);
	while (bits_to_foll > 0)
	{
		temp += to_string(!bit);
		bits_to_foll -= 1;
	}
	return temp;
}

void output_file(string filename, string text)
{
	fstream file;
	file.open(filename, ios::out);
	for (int i = 0; i < text.size(); i++)
	{
		file << text[i];
	}
	file.close();
}

string encode_text(int *freq, string text, string abc)
{
	int _low[500];
	int _high[500];
	_low[0] = _low_edge;
	_high[0] = _high_edge;
	int current = 1;	/* Какой элемент берём */
	int i = 0;			/* Где находимся */
	int range = 0;
	/* Del - последняя накопленная частота */
	int del = freq[abc.size() - 1];
	int bits_to_foll = 0;
	string code = "";

	while (i < text.size())
	{
		get_next_symbol(text, abc, i, &current);
		i += 1;
		range = _high[i - 1] - _low[i - 1] + 1;
		_low[i] = _low[i - 1] + (range*freq[current - 1])/del;
		_high[i] = _low[i - 1] + (range*freq[current])/del - 1;
		cout << "[" << _low[i] << "; " << _high[i] << ")" << endl;

		for (;;)
		{
			if (_high[i] < half)
			{
				code += write_bits(0, bits_to_foll);
				bits_to_foll = 0;
			}
			else if (_low[i] >= half)
			{
				code += write_bits(1, bits_to_foll);
				bits_to_foll = 0;
				_low[i] -= half;
				_high[i] -= half;
			}
			else if (_low[i] >= first_qtr && _high[i] < third_qtr)
			{
				bits_to_foll += 1;
				_low[i] -= first_qtr;
				_high[i] -= first_qtr;
			}
			else break;
			_low[i] = 2 * _low[i];
			_high[i] = 2 * _high[i] + 1;
		}
	}
	cout << endl << code << endl;
	return code;
}

int to_int(string text)
{
	int n = 0;
	for (int i = 0; i < sizeof(short)*8; i++) {
		n <<= 1;
		n |= text[i] - '0';
	}
	return n;
}

int decode(int *freq, string text, string abc)
{
	int _low[500];
	int _high[500];
	_low[0] = _low_edge;
	_high[0] = _high_edge;
	int range = 0;
	int cum = 0;
	int del = freq[abc.size() - 1];
	int current = 1;
	unsigned short int value = to_int(text);			//Забираем 2 байта
	int bits_to_go = 0;
	int buffer = 0;
	for (int i = 0; i < text.size(); i++)
	{
		range = (_high[i - 1] - _low[i - 1]) + 1;
		cum = (((value - _low[i - 1]) + 1) * del - 1) / range;
		for (int i = 1; freq[i] > cum;)
		{
			get_next_symbol(text, abc, i, &current);
			i += 1;
			_low[i] = _low[i - 1] + (range * freq[i - 1]) / del - 1;
			_high[i] = _low[i - 1] + (range * freq[i]) / del;
		}
		for (;;)
		{
			if (_high[i] >= half)
			{
				if (_low[i] >= half)
				{
					value -= half; _low[i] -= half; _high[i] -= half;
				}
				else if(_low[i] >= first_qtr && _high[i] < third_qtr)
				{
					value -= first_qtr; _low[i] -= first_qtr; _high[i] -= first_qtr;
				}
				else break;
			}
			_low[i] = 2 * _low[i];
			_high[i] = 2 * _high[i] + 1;

			/* Сдвиг влево и добавление бита */
			fstream input;
			input.open(filename_input, ios::in);
			if (bits_to_go == 0)
			{
				input.read((char *)&buffer, sizeof(unsigned short int));
				bits_to_go = 16;
			}
			bool new_bit = buffer & 1;
			buffer >>= 1;
			bits_to_go--;
			value = (value << 1) + new_bit;
		}
	}
	return value;
}

int main()
{
	string text = get_file(filename_input);
	string abc = get_abc(text);
	int *freq = new int[abc.size()];
	get_frequency(text, freq, abc);

	if (debug_mode)
	{
		cout << text << endl;
		cout << abc << endl;
		for (int i = 0; i < abc.size(); i++)
		{
			cout << abc[i] << " - " << freq[i] << endl;
		}
		cout << endl;
	}

	string code = encode_text(freq, text, abc);
	output_file(filename_output, code);

	int temp = decode(freq, code, abc);
	cout << temp << endl;
	return 0;
}