/*
This program is a simple lexical analysis for c language. Mainly use c language and some additional
c++ characteristics.

If you do not use Visual Studio, delete pch.h

Input file is source.txt
Output file is out.txt
*/

#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unordered_set>
#include <fstream>
#include <iostream>
using namespace std;

#define MAX 100 //set the max length of a token

ofstream outFile;		//output result as a file
ifstream inFile;		//input file
char token[MAX];		//store token
char ch;				//latest char in file stream
int pos = 0;			//pointer to the position of char in this string
int numberOfLines = 1;	//count analyzed lines
int numberOfWords = 0;	//count analyzed words
int numberOfChars = -1; //count analyzed chars without EOF

unordered_set<string> keyList{ "auto", "double", "int", "struct", "break", " else", "long", "switch",
							  "case ", "enum", "register", "typedef", "char", "extern", "return", "union",
							  "const", "float", "short", "unsigned", "continue", "for ", "signed", "void",
							  "default", "goto", "sizeof ", "volatile", " do", "if", "while ", "static" }; //an unordered_set storing key words of c language

unordered_set<char> boundList{
	'.', '{', '}', '[', ']', '(', ')', ',', ';', '#', '\\', '\'', '\"', '\?', ':' }; //an unordered_list for all bounds

unordered_set<char> quoteList{
	'\'', '\"' }; //an unordered_list for quotes

//function getChar gets next char from file stream, and if it is EOF, print analyze finished.
char getChar()
{
	ch = char(inFile.peek());
	inFile.get();
	if (ch == EOF)
		cout << "Analyze finished" << endl; //finish the analyze
	numberOfChars++;						//count analyzed char
	return ch;								//return the char for classification
}

//check if variable ch is blank, throw all blank char
void getBlank()
{
	while (ch == ' ' || ch == '\t' || ch == '\b' || ch == '\n')
	{
		if (ch == '\n')
		{
			numberOfLines++;
		}
		getChar();
	}
}

//connect variable to token
void connect(char ch)
{
	token[pos++] = ch;
}

//if variable ch is a letter
int isLetter(char ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return 1;
	else
		return 0;
}

//if variable ch is a digit
int isDigit(char ch)
{
	if (ch >= '0' && ch <= '9')
		return 1;
	else
		return 0;
}

//if variable ch is a bound
int isBound(char ch, unordered_set<char> boundList)
{
	return boundList.count(ch);
}

//if variable ch is a quote
int isQuote(char ch, unordered_set<char> quoteList)
{
	return quoteList.count(ch);
}

//clean the variable ch and back out token position
void retract()
{
	pos--;
	ch = ' ';
}

//if the string in token is a key word in c language
int KeyList(char token[], unordered_set<string> keyList)
{
	return keyList.count(token);
}

int main()
{
	inFile.open("source.txt", ios::in);
	outFile.open("out.txt", ios::out);

	getChar();
	getBlank(); //find the first not null char
	while (ch != EOF)
	{
		if (isDigit(ch))
		{ //if variable ch is digit
			pos = 0;
			while (isDigit(ch))
			{
				connect(ch);
				getChar();
			}
			if (isLetter(ch))
			{ //if this token is a string which start with a digit, error
				token[0] = '\0';
				cout << "Line" << numberOfLines << "illegal variable:start with digit" << endl;
				outFile << "Line" << numberOfLines << "illegal variable:start with digit" << endl;
				while (isLetter(ch))
					getChar();
			}
			token[pos++] = '\0';
			if (token[0] != '\0')
				cout << "num"
				<< "\t" << "\t"
				<< "\t" << token << endl;
			outFile << "num"
				<< "\t" << "\t"
				<< "\t" << token << endl;
			numberOfWords++;
			getBlank();
		}

		else if (isLetter(ch))
		{ //identify identifier and keyword
			pos = 0;
			while (isLetter(ch) || isDigit(ch) || ch == '_')
			{
				connect(ch);
				getChar();
			}
			token[pos++] = '\0';
			if (KeyList(token, keyList) == 1)
			{
				cout << "keyword"
					<< "\t" << "\t"
					<< "\t" << token << endl;
				outFile << "keyword"
					<< "\t" << "\t"
					<< "\t" << token << endl;
			}

			else
			{
				cout << "identifier"
					<< "\t"
					<< "\t" << token << endl;
				outFile << "identifier"
					<< "\t"
					<< "\t" << token << endl;
			}
			numberOfWords++;
			getBlank();
		}

		//identify operaters
		else if (ch == '+')
		{ //+,++,+=
			pos = 0;
			connect(ch);
			getChar();
			if (ch == '+' || ch == '=')
			{
				connect(ch);
				token[pos++] = '\0';
				cout << "operator"
					<< "\t"
					<< "\t" << token << endl;
				outFile << "operator"
					<< "\t"
					<< "\t" << token << endl;
				getChar();
				getBlank();
			}
			else
			{
				token[pos++] = '\0';
				cout << "operator"
					<< "\t"
					<< "\t" << token << endl;
				outFile << "operator"
					<< "\t"
					<< "\t" << token << endl;
				getBlank();
			}
		}

		//-,--,-=
		else if (ch == '-')
		{
			pos = 0;
			connect(ch);
			getChar();
			if (ch == '-' || ch == '=')
			{
				connect(ch);
				token[pos++] = '\0';
				cout << "operator"
					<< "\t"
					<< "\t" << token << endl;
				outFile << "operator"
					<< "\t"
					<< "\t" << token << endl;
				getChar();
				getBlank();
			}
			else
			{
				token[pos++] = '\0';
				cout << "operator"
					<< "\t"
					<< "\t" << token << endl;
				outFile << "operator"
					<< "\t"
					<< "\t" << token << endl;
				getBlank();
			}
		}

		//*,*=,%,%=!,!=,>,>=,<,<=,=,==
		else if (ch == '*' || ch == '%' || ch == '!' || ch == '>' || ch == '<' || ch == '=')
		{
			pos = 0;
			connect(ch);
			getChar();
			if (ch == '=')
			{
				connect(ch);
				token[pos++] = '\0';
				cout << "operator"
					<< "\t"
					<< "\t" << token << endl;
				outFile << "operator"
					<< "\t"
					<< "\t" << token << endl;
				getChar();
				getBlank();
			}
			else
			{
				token[pos++] = '\0';
				cout << "operator"
					<< "\t"
					<< "\t" << token << endl;
				outFile << "operator"
					<< "\t"
					<< "\t" << token << endl;
				getBlank();
			}
		}

		// &&
		else if (ch == '&')
		{
			pos = 0;
			connect(ch);
			getChar();
			if (ch == '&')
			{
				connect(ch);
				token[pos++] = '\0';
				cout << "operator"
					<< "\t"
					<< "\t" << token << endl;
				outFile << "operator"
					<< "\t"
					<< "\t" << token << endl;
				getChar();
				getBlank();
			}
			else
			{
				cout << "Line" << numberOfLines << "has illegal word" << endl;
				getChar();
				getBlank();
			}
		}

		// ||
		else if (ch == '|')
		{
			pos = 0;
			connect(ch);
			getChar();
			if (ch == '|')
			{
				connect(ch);
				token[pos++] = '\0';
				cout << "operator"
					<< "\t"
					<< "\t" << token << endl;
				outFile << "operator"
					<< "\t"
					<< "\t" << token << endl;
				getChar();
				getBlank();
			}
			else
			{
				cout << "Line" << numberOfLines << "has illegal word" << endl;
				getChar();
				getBlank();
			}
		}

		else if (ch == '/')
		{ // /,/=,//
			pos = 0;
			connect(ch);
			getChar();
			if (ch == '=')
			{
				connect(ch);
				token[pos++] = '\0';
				cout << "operator"
					<< "\t"
					<< "\t" << token << endl;
				outFile << "operator"
					<< "\t"
					<< "\t" << token << endl;
				getChar();
				getBlank();
			}
			else if (ch == '/')
			{ //delete all comment
				retract();
				while (ch != '\n' && ch != EOF)
				{
					getChar();
				}
				getBlank();
			}
			else if (ch == '*')
			{ // /* */
				retract();
				while (ch != EOF)
				{
					getChar();
					if (ch == '*')
					{
						getChar();
						if (ch == '/' || ch == EOF)
						{
							break;
						}
					}
					else if (ch == EOF)
					{
						break;
					}
				}
				getBlank();
			}
			else
			{
				token[pos++] = '\0';
				cout << "operator"
					<< "\t"
					<< "\t" << token << endl;
				outFile << "operator"
					<< "\t"
					<< "\t" << token << endl;
				getBlank();
			}
		}
		//identify separator
		else if (isBound(ch, boundList) && (!isQuote(ch, quoteList)))
		{
			pos = 0;
			connect(ch);
			token[pos++] = '\0';
			cout << "separator"
				<< "\t"
				<< "\t" << token << endl;
			outFile << "separator"
				<< "\t"
				<< "\t" << token << endl;
			getChar();
			getBlank();
		}
		//identify character string
		else if (isQuote(ch, quoteList))
		{
			pos = 0;
			connect(ch);
			token[pos++] = '\0';
			cout << "separator"
				<< "\t"
				<< "\t" << token << endl;
			outFile << "separator"
				<< "\t"
				<< "\t" << token << endl;
			getChar();
			getBlank();
			pos = 0;
			while (!isQuote(ch, quoteList))
			{
				connect(ch);
				getChar();
			}
			token[pos++] = '\0';
			cout << "character string"
				<< "\t" << token << endl;
			outFile << "character string"
				<< "\t" << token << endl;
			getBlank();
			pos = 0;
			connect(ch);
			token[pos++] = '\0';
			cout << "separator"
				<< "\t"
				<< "\t" << token << endl;
			outFile << "separator"
				<< "\t"
				<< "\t" << token << endl;
			getChar();
			getBlank();
		}

		else
		{
			cout << "Line" << numberOfLines << "has unreadable content" << endl;
			getChar();
			getBlank();
		}
	}
	//print the result
	cout << "total" << '\t' << numberOfLines << '\t' << "lines" << endl;
	cout << "total" << '\t' << numberOfWords << '\t' << "words" << endl;
	cout << "total" << '\t' << numberOfChars << '\t' << "chars" << endl;

	outFile << "total" << '\t' << numberOfLines << '\t' << "lines" << endl;
	outFile << "total" << '\t' << numberOfWords << '\t' << "words" << endl;
	outFile << "total" << '\t' << numberOfChars << '\t' << "chars" << endl;
	inFile.close();
	outFile.close();
	system("pause");
	return 0;

}