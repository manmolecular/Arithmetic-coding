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

int to_int(string text, int _pos)
{
	int n = 0;
	for (int i = _pos; i < sizeof(short)*8 + _pos; i++) 
	{
		n <<= 1;
		n |= text[i] - '0';
	}
	return n;
}

string to_bits_16(int _value)
{
	bitset<16> a(_value); //convent number into bit array
	string mystring = a.to_string<char, char_traits<char>, allocator<char> >();
	return mystring;
}

int add_bit(int buffer, int value)
{
	/* Создаем битсет объекты */
	bitset<16> a(value);
	bitset<16> b(buffer);

	/* Проверяем первый бит в буффере (если 1)*/
	bool check = b.test(0);
	if (check)
	{
		a.set(0);
	}
	else
	{
		a.reset(0);
	}
	value = (unsigned short int)(a.to_ulong());
	return value;
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

	unsigned short int value = to_int(text, 0);		// Забираем 16 бит в value

	int bits_to_go = 0;		// Сколько бит осталось в буфере
	int buffer = 0;			// Сам буфер битов
	int count_taken = 1;	// Сколько раз забрали в буфер (опр. позицию считывания)

	for (int i = 1; i < text.size(); i++)
	{
		range = (_high[i - 1] - _low[i - 1]) + 1;
		cum = (((value - _low[i - 1]) + 1) * del - 1) / range;
		
		int symbol;
		for (symbol = 1; freq[i] > cum; symbol++);

		_low[i] = _low[i - 1] + (range * freq[i - 1]) / del - 1;
		_high[i] = _low[i - 1] + (range * freq[i]) / del;

		if (debug_mode)
		{
			cout << "Symbol is: " << abc.at(symbol) << endl;
			cout << "Value is: " << value << endl;
		}

		for (;;)
		{
			if (_high[i] >= half)
			{
				if (_low[i] >= half)
				{
					value -= half;
					_low[i] -= half;
					_high[i] -= half;
				}
				else if(_low[i] >= first_qtr && _high[i] < third_qtr)
				{
					value -= first_qtr;
					_low[i] -= first_qtr;
					_high[i] -= first_qtr;
				}
				else break;
			}
			_low[i] = 2 * _low[i];
			_high[i] = 2 * _high[i] + 1;

			/* Заполнение буфера */
			if (bits_to_go == 0)
			{
				buffer = to_int(text, 16 * count_taken);
				bits_to_go = 16;
			}
			value = add_bit(value*2, buffer);
			buffer = buffer >> 1;
			bits_to_go--;
		}
	}
	return value;
}

void test_function(string text)
{
	cout << endl << "TEST FUNCTION: " << endl;

	unsigned short int value = to_int(text, 0);			//Забираем 2 байта
	unsigned short int buffer = to_int(text, 16);
	int bits_to_go = 0;


	string _value = to_bits_16(value);
	string _buffer = to_bits_16(buffer);
	cout << "Value (0-15): " << _value << endl;
	cout << "Buffer (16-31): " << _buffer << endl << endl;

	cout << "ADD BIT WITH SLIDE: " << endl;
	cout << "value: " << _value << endl;
	value *= 2;
	_value = to_bits_16(value);
	cout << "value * 2: " << _value << endl;

	/* Создаем битсет объекты */
	bitset<16> a(value);
	bitset<16> b(buffer);

	/* Проверяем первый бит в буффере (если 1)*/
	bool check = b.test(0);
	if (check)
	{
		a.set(0);
	}
	else
	{
		a.reset(0);
	}
	value = (unsigned short int)(a.to_ulong());
	_value = to_bits_16(value);
	cout << "value *2 + new_bit: " << _value << endl;

	//cout << "SLIDE: " << endl;
	//cout << "Buffer (16-31): " << _buffer << endl;
	//buffer *= 2;
	//_buffer = to_bits_16(buffer);
	//cout << "Buffer (16-31) * 2: " << _buffer << endl << endl;
	//bitset<16> c(_buffer);
	//c.reset(15);
	//buffer = (unsigned short int)(c.to_ulong());
	//_buffer = to_bits_16(buffer);
	//cout << "Buffer (16-31) LAST: " << _buffer << endl << endl;
}

int main()
{
	//string text = get_file(filename_input);
	string text = "Hello, world!";
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

	//test_function(code);

	int temp = decode(freq, code, abc);
	cout << temp << endl;
	return 0;
}