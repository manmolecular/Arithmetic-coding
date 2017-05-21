#include "Arithmetic.h"
#define filename_input "input.txt"
#define filename_output "output.txt"
#define symbol_break '\0'
#define test_count 1000
#define length_of_string 100
using namespace std;

/* Generate random char[n] */
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

int main()
{
	for (int i = 0; i < test_count; i++)
	{
		char s[length_of_string + 1] = { 0 };
		gen(s, length_of_string);
		string text(s);
		text.push_back(symbol_break);

		//string text = "Hello, world! This method is better than Huffman. ";
	
		arithmetic coding;
		coding.input(text);
		coding.get_abc();
		coding.get_frequency();
		coding.encode_text();

		string encode = coding.get_encode();
		//coding.show_table();

		coding.decode_text();
		string decode = coding.get_decode();

		//cout << "encode: " << encode << endl;
		//cout << "decode: " << decode << endl;

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
	}
	return 0;
}