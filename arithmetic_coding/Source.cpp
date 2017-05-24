#include "Arithmetic.h"
#define filename_input "input.txt"
#define filename_output "output.txt"
#define symbol_break '\0'
#define test_count 10000
#define length_of_string 40
#define test_mode false
#define test_output true
using namespace std;
bool arithmetic::debug_mode = false;

void gen(char* p, size_t n)
{
	while (n--)
	{
		char ch;
		while ((ch = (char)(rand() % (127 - '0' + 1) + '0')), !isalnum(ch));
		*p++ = ch;
	}
	*p = 0;
}

void test()
{
	for (int i = 0; i < test_count; i++)
	{
		char s[length_of_string + 1] = { 0 };
		gen(s, length_of_string);
		string text(s);
		text.push_back(symbol_break);

		arithmetic coding;
		coding.input_string(text);
		coding.get_abc();
		coding.get_frequency();
		coding.encode_text();

		string encode = coding.get_encode();

		coding.decode_text();
		string decode = coding.get_decode();

		cout << "Test #" << i << ": ";
		if (text == decode)
		{
			cout << "good";
		}
		else
		{
			cout << "bad" << endl;
			cout << "break at: " << i;
		}
		cout << endl;
		if (test_output)
		{
			cout << "input: " << coding.get_text() << endl;
			cout << "decode: " << decode << endl;
			cout << "encode: " << encode << endl;
		}
		cout << endl;
	}
}

void opener_logo()
{
cout << "                _ _   _                    _   _      " << endl;
cout << "     /\\        (_) | | |                  | | (_)     " << endl;
cout << "    /  \\   _ __ _| |_| |__  _ __ ___   ___| |_ _  ___ " << endl;
cout << "   / /\\ \\ | '__| | __| '_ \\| '_ ` _ \\ / _ \\ __| |/ __|" << endl;
cout << "  / ____ \\| |  | | |_| | | | | | | | |  __/ |_| | (__ " << endl;
cout << " /_/    \\_\\_|  |_|\\__|_| |_|_| |_| |_|\\___|\\__|_|\\___|" << endl;
}

int main()
{
		opener_logo();
		cout << endl;

		arithmetic coding;

		cout << "Input: " << endl;
		cout << "1. From file" << endl;
		cout << "2. From keyboard" << endl;
		cout << "3. Run tests: " << test_count << " times on " << length_of_string << " length strings" << endl;
		cout << "Choose (1 / 2 / 3): ";
		int choose;
		cin >> choose;
		cout << endl;

		if (choose == 3)
		{
			test();
			exit(1);
		}
		else if (choose == 2)
		{
			cout << "Your text to encode: ";
			coding.input_keyboard();
		}
		else if (choose == 1)
		{
			cout << "Name of file to open (0 - default): ";
			string filename_in;
			cin >> filename_in;
			if (filename_in.at(0) == '0')
			{
				filename_in = filename_input;
			}
			coding.get_file(filename_in);
		}
		else
		{
			cout << "Your input text is incorrect" << endl;
			exit(1);
		}

		coding.get_abc();
		coding.get_frequency();
		coding.encode_text();
		coding.show_table();

		string encode = coding.get_encode();
		coding.show_table();

		coding.decode_text();
		string decode = coding.get_decode();

		cout << "encode: " << endl << encode << endl << endl;
		cout << "decode: " << endl << decode << endl;

		if (coding.get_text() == decode)
		{
			cout << "Input text = decode text" << endl;
		}
		else
		{
			cout << "Input text != decode text" << endl;
		}

		coding.output_file(filename_output);
	return 0;
}