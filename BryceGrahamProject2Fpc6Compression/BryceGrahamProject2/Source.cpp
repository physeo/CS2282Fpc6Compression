/**********************************************
Bryce Graham
CS 2282
Dr. Beard
Project 2: A C++ program that makes use of bit twiddling,
masks and shifts in order to input and output a character
encryption code called Fpc6
**********************************************/

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
using namespace std;

class Fpc6
{
	unsigned int * fpc6Array;
	unsigned int fpc6ArrayLength;
public:
	Fpc6();
	Fpc6(int _length);

	// Converts a string into the Fpc6 format and then compresses it into a single unsigned int.
	unsigned int packFpc6(char (&charsToPack)[5])
	{
		unsigned int storedChars[5];
		for (unsigned int i = 0; i < 5; i++)
			storedChars[i] = AsciiToFpc6(charsToPack[i]);

		return storedChars[0] << 24 | storedChars[1] << 18 | storedChars[2] << 12 | storedChars[3] << 6 | storedChars[4];
	}

	// Takes a compressed Fpc6 and unpacks it back into an array of encoded Fpc6 characters.
	unsigned int * unpackFpc6(unsigned int fpc6ToUnpack)
	{
		unsigned int * unpackedChars = new unsigned[5];
		
		unpackedChars[0] = fpc6ToUnpack >> 24 & 0X3F;
		unpackedChars[1] = fpc6ToUnpack >> 18 & 0X3F;
		unpackedChars[2] = fpc6ToUnpack >> 12 & 0X3F;
		unpackedChars[3] = fpc6ToUnpack >> 6 & 0X3F;
		unpackedChars[4] = fpc6ToUnpack & 0X3F;

		return unpackedChars;
	}

	// Converts a string into groups of 5 fpc6 characters, compresses them into 32 bit integers and stores them in an array
	unsigned int * storeFpc6Strings(string stringToStore)
	{
		char charsToPack[5];
		unsigned int convertedChars;
		unsigned int count = 0;
		int beg = 0;
		int end = 5;
		string subString = stringToStore.substr(beg, end);
		do
		{
			for (unsigned int i = 0; i < 5; i++)
				charsToPack[i] = ' ';
			subString = stringToStore.substr(beg, end);
			for (unsigned int i = 0; i < subString.length(); i++)
				charsToPack[i] = subString[i];
			convertedChars = packFpc6(charsToPack);
			fpc6Array[count] = convertedChars;
			count++;
			beg += 5;
		} while (count < fpc6ArrayLength);

		return fpc6Array;
	}

	// Uncompress and convert an array of Fpc6 into a string.
	string restoreFpc6Strings()
	{
		string restoredString;

		for (unsigned int i = 0; i < fpc6ArrayLength; i++)
		{
			for (unsigned int j = 0; j < 5; j++)
			{
				restoredString += fpc6ToAscii(unpackFpc6(fpc6Array[i])[j]);
			}
		}

		return restoredString;
	}

	// Converts a char from Ascii format to Fpc6
	int AsciiToFpc6(char charToEncode)
	{
		int encodedChar;
		if (charToEncode >= 'A' && charToEncode <= 'Z')
			encodedChar = charToEncode - 'A';
		else if (charToEncode >= 'a' && charToEncode <= 'z')
			encodedChar = charToEncode - 'a' + 26;
		else if (charToEncode == ' ')
			encodedChar = 52;
		else if (charToEncode == '.')
			encodedChar = 53;
		else if (charToEncode == '\'')
			encodedChar = 54;
		else if (charToEncode == ';')
			encodedChar = 55;
		else if (charToEncode == ':')
			encodedChar = 56;
		else if (charToEncode == '\n')
			encodedChar = 57;
		else if (charToEncode == '!')
			encodedChar = 58;
		else encodedChar = 63;

		return encodedChar;
	}

	// Converts an Fpc6 to an Ascii character
	int fpc6ToAscii(int charToDecode)
	{
		int decodedChar = 0;
		if (charToDecode >= 0 && charToDecode <= 25)
			decodedChar = charToDecode + 'A';
		else if (charToDecode >= 26 && charToDecode <= 51)
			decodedChar = charToDecode + 'a' - 26;
		else if (charToDecode == 52)
			decodedChar = ' ';
		else if (charToDecode == 53)
			decodedChar = '.';
		else if (charToDecode == 54)
			decodedChar = '\'';
		else if (charToDecode == 55)
			decodedChar = ';';
		else if (charToDecode == 56)
			decodedChar = ':';
		else if (charToDecode == 57)
			decodedChar = '\n';
		else if (charToDecode == 58)
			decodedChar = '!';
		else if (charToDecode == 63)
			decodedChar = '@';
		return decodedChar;
	}

	// Converts a string into compressed fpc6, writes it to a binary file, reads it back in and uncompresses it
	string writeFpc6ToBin(string stringToBin, string fileName)
	{
		storeFpc6Strings(stringToBin);

		unsigned int *inArray = new unsigned int[fpc6ArrayLength];
		fstream binaryIo;
		binaryIo.open(fileName, ios::out | ios::in | ios::binary | ios::trunc);
		binaryIo.seekp(0);
		binaryIo.write((char*)fpc6Array, fpc6ArrayLength * sizeof(fpc6Array[0]));
		binaryIo.seekp(0);

		binaryIo.seekg(0);
		binaryIo.read((char*)inArray, fpc6ArrayLength * sizeof(fpc6Array[0]));
		binaryIo.close();

		return  restoreFpc6Strings();
	}

	~Fpc6()
	{
		delete[] fpc6Array;
	}
};

// Default constructor for Fpc6 creates an array of length 5 and initializes it to 0
Fpc6::Fpc6()
{
	fpc6ArrayLength = (unsigned int) ceil(5 / (double)5); // Divides the string by 5 and rounds it up so the array can fit the encoded 5 character fpc6 ints
	fpc6Array = new unsigned int[5];
	for (int i = 0; i < 5; i++)
		fpc6Array[i] = 0;
}
// Overloaded constructor for Fpc6 creates an array of length _length and initializes it to 0
Fpc6::Fpc6(int _stringLength)
{
	if (_stringLength > 0)
		fpc6ArrayLength = (unsigned int)ceil(_stringLength / (double)5); // Divides the string by 5 and rounds it up so the array can fit the encoded 5 character fpc6 ints
	fpc6Array = new unsigned int[fpc6ArrayLength];
	for (unsigned int i = 0; i < fpc6ArrayLength; i++)
		fpc6Array[i] = 0;
}

string readTextFromFile(string fileName);
void writeTextToFile(string fileName, string stringToFile);
void textFromFileToBin(string fileName);

// Used for testing the Fpc6 class and methods
int main()
{
	// Tests the conversion to and from Fpc6 and Ascii
	Fpc6 code(5);
	char charsToPack[5] = { 'a', 'b', 'c', 'd', 'e' };
	cout << "fpc6ToAscii(AsciiToFpc6('a')) = " << (char) code.fpc6ToAscii(code.AsciiToFpc6('a')) << endl;
	cout << "abcde packed: " << code.packFpc6(charsToPack) << endl;
	unsigned int * a = code.unpackFpc6(code.packFpc6(charsToPack));
	cout << "unpacked abcde: ";
	for (unsigned int i = 0; i < 5; i++)
		cout << *(a + i) << " ";

	cout << endl << endl;

	// Tests the functions that pack an entire string and then unpack it
	string myString = "I am a monkey who lives in a tree.";
	Fpc6 codeString(myString.length());
	codeString.storeFpc6Strings(myString);
	cout << codeString.restoreFpc6Strings() << endl;

	cout << endl << endl;

	textFromFileToBin("myFile.txt");
	
	string myFileString = readTextFromFile("prideFull.txt");
	Fpc6 binFile(myFileString.length());
	cout << binFile.writeFpc6ToBin(myFileString, "prideFull.bin") << endl;
	
	return 0;
}

// Accepts a file name containing ascii text, reads it in, converts it to an fpc6 int array, writes it out
// as a binary file, reads it back in, converts it back to an ascii string and writes it out as a text file
void textFromFileToBin(string fileName)
{
	string myFileString = readTextFromFile(fileName);
	Fpc6 binFile(myFileString.length());
	writeTextToFile(fileName, binFile.writeFpc6ToBin(myFileString, fileName));
}

// Reads text in from a text file
string readTextFromFile(string fileName)
{
	string readText;
	string fileIn;
	ifstream myfile(fileName);
	if (myfile.is_open())
	{
		while (getline(myfile, fileIn))
		{
			readText += fileIn;
			if (!myfile.eof())
				readText += "\r\n";
		}
		myfile.close();
	}
	else return "-1";

	return readText;
}
// Writes a string to a text file.
void writeTextToFile(string fileName, string stringToFile)
{
	ofstream myfile;
	myfile.open(fileName);
	myfile << stringToFile;
	myfile.close();
}



