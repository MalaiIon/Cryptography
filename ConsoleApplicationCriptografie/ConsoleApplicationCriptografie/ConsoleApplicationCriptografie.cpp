#include "pch.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <vector>
#include <locale>
#include <conio.h>
using namespace std;

class Playfair 
{	
public:
	string keyword;
	string ukey; //unrepeated keyword
	string plain;
	string cipher;
	string generic = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
	char pftable[5][5]; // 5x5 matrix table
	void setKey(string keyword)
	{
		this->keyword = keyword;
		//Capitalize all characters and replace all j with i in keyword
		for (int i = 0; i < (int)keyword.length(); i++)
		{
			keyword[i] = toupper((char)keyword[i]);
			if (keyword[i] == 'J') { keyword[i] = 'I'; }
		}
		//find unrepeated characters in keyword
		int x = 0, itrepeated = 0;
		cout << endl << "Tracing the processes of playfair Cipher" << endl;
		for (int i = 0; i < (int)keyword.length(); i++)
		{
			for (int j = 0; j < i; j++)
			{
				if (keyword[i] == keyword[j])
				{
					itrepeated++;
					cout << "Discard Repeated Character->";
				}
			}
			if (itrepeated == 0)
			{
				cout << "Store Non-Repeated Character to ukey(" << x << ")-";
				ukey += keyword[i]; //Note to self: don't use ukey[x] index on this part since ukey is unallocated pointer
				x++;
			}
			cout << keyword[i] << "--> obtained from keyword(" << i << ") " << endl;
			itrepeated = 0;
		}
		cout << endl << "Length of keyword(" << keyword << ") = " << keyword.length() << "; removing repetitions(" << ukey << ") = " << ukey.length() << endl;
		//Now its time to put this ukey in matrix form of 5x5
		//Inserting into matrix
		cout << endl << "Inserting unrepeated keyword into playfair table in matrix form 5x5::";
		//First inserting ukey[k]
		int a = -1, b = 0;
		for (int i = 0; i < (int)ukey.length(); i++)
		{
			if (i % 5 == 0) { cout << endl; b = 0; a++; }
			pftable[a][b] = (char)ukey[i];
			cout << pftable[a][b] << "[" << a << "]" << "[" << b << "]" << " "; //JUST TRACING (Uncomment this line to see trace result)
			b++;
		}

		x = 0;
		itrepeated = 0;
		for (int i = 0; i < (int)generic.length(); i++)
		{
			for (int j = 0; j < (int)ukey.length(); j++)
			{
				if (generic[i] == (char)ukey[j])
				{
					itrepeated++;
					break;
				}
			}
			if (!itrepeated)
			{
				if (((int)ukey.length() + x) % 5 == 0) { cout << endl; b = 0; a++; }
				pftable[a][b] = (char)generic[i];
				cout << pftable[a][b] << "[" << a << "]" << "[" << b << "]" << " ";// JUST TRACING (Uncomment this line to see trace result)
				x++;
				b++;
			}
			itrepeated = 0;
		}


		//At this point, keyword is properly formatted into the 5x5 matrix
	}
	void setPlain(string plain)
	{
		this->plain = plain;
	}
	void setCipher(string cipher)
	{
		this->cipher = cipher;
	}
	int * MapThisLetter(char Char2m)
	{
		static int r[2];
		r[0] = 0;
		r[1] = 0;

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {

				if (pftable[i][j] == Char2m) {
					cout << pftable[i][j];
					r[0] = i;
					r[1] = j;
					break;
				}
			}
		}
		return r;
	}
	void encrypt()
	{
		int *position;
		bool flag = 0;
		int indexx1;
		int indexy1;
		int indexx2;
		int indexy2;
		for (int i = 0; i < (int)plain.length(); i++)
		{
			plain[i] = toupper((char)plain[i]);
			if (plain[i] == 'J') { plain[i] = 'I'; }
		}
		cout << endl << endl << "Now mapping each letter to its corresponding cipher character from the playfair table:" << endl;
		for (int i = 0; i < (int)plain.length(); i++)
		{
			position = MapThisLetter(toupper((char)plain[i++]));
			indexx1 = *(position);
			indexy1 = *(position + 1);
			cout << "[" << indexx1 << "," << indexy1 << "] & ";
			if (((char)plain[i - 1] == (char)plain[i]) || (i >= (int)plain.length() && (int)plain.length() % 2 != 0)) {
				position = MapThisLetter('X');
				indexx2 = *(position);
				indexy2 = *(position + 1);
				cout << "[" << indexx2 << ", " << indexy2 << "]";
				flag = 1;//Indicates (a letter has been replaced with X) OR (a position has been padded with X)
			}// breaks at last line if odd length plaintext is entered
			if (!flag) {
				position = MapThisLetter(toupper((char)plain[i]));
				indexx2 = *(position);
				indexy2 = *(position + 1);
				cout << "[" << indexx2 << ", " << indexy2 << "]";
			}
			//at shis point we already have x1,y1 and x2,y2 available for two letters of plaintext at a time. 
			//now we apply certain conditional comparisions
			if (indexx1 == indexx2) {
				//if x1==x2 i.e. when horizontal match then right shift
				indexy1 = (indexy1 + 1) % 4;
				indexy2 = (indexy2 + 1) % 4;
				cout << " maps to: " << pftable[indexx1][indexy1] << "[" << indexx1 << ", " << indexy1 << "] & " << pftable[indexx2][indexy2] << "[" << indexx2 << ", " << indexy2 << "] respectively" << endl;
			}
			else if (indexy1 == indexy2) {
				//if y1==y2 i.e when vertical match then down shift
				indexx2 = (indexx2 + 1);
				if (indexx2 > 4) {
					indexx2 = indexx2 % 4;
				}
				indexx1 = (indexx1 + 1) % 4;
				cout << " maps to: " << pftable[indexx1][indexy1] << "[" << indexx1 << ", " << indexy1 << "] & " << pftable[indexx2][indexy2] << "[" << indexx2 << ", " << indexy2 << "] respectively" << endl;
			}
			else {
				//exchange x1 and x2 in boxes
				indexy1 = indexy1 + indexy2;
				indexy2 = indexy1 - indexy2;
				indexy1 = indexy1 - indexy2;
				cout << " maps to: " << pftable[indexx1][indexy1] << "[" << indexx1 << ", " << indexy1 << "] & " << pftable[indexx2][indexy2] << "[" << indexx2 << ", " << indexy2 << "] respectively" << endl;
			}
			cipher += pftable[indexx1][indexy1];
			cipher += pftable[indexx2][indexy2];
			if (flag) {//this flag is TRUE IF (selected both string are same) or (odd End-of-line has been reached)
				if ((char)plain[i - 1] == (char)plain[i]) { flag = 0; }//reseting the flag to Not-END-of-Line IFF selected both letters are the same  
				if (flag) { break; } //no resetting; break IF odd End-of-line is padded with character X at the last position
			}
		}
	}
	void decrypt()
	{
		int *position;
		bool flag = 0;
		int indexx1;
		int indexy1;
		int indexx2;
		int indexy2;
		cout << endl << endl << "Now mapping each letter to its corresponding cipher character from the playfair table:" << endl;
		for (int i = 0; i < (int)cipher.length(); i++)
		{
			position = MapThisLetter(toupper((char)cipher[i++]));
			indexx1 = *(position);
			indexy1 = *(position + 1);
			cout << "[" << indexx1 << "," << indexy1 << "] & ";
			if (((char)cipher[i - 1] == (char)cipher[i]) || (i >= (int)cipher.length() && (int)cipher.length() % 2 != 0)) {
				position = MapThisLetter('X');
				indexx2 = *(position);
				indexy2 = *(position + 1);
				cout << "[" << indexx2 << ", " << indexy2 << "]";
				flag = 1;//Indicates (a letter has been replaced with X) OR (a position has been padded with X)
			}// breaks at last line if odd length plaintext is entered
			if (!flag) {
				position = MapThisLetter(toupper((char)cipher[i]));
				indexx2 = *(position);
				indexy2 = *(position + 1);
				cout << "[" << indexx2 << ", " << indexy2 << "]";
			}
			//at shis point we already have x1,y1 and x2,y2 available for two letters of plaintext at a time. 
			//now we apply certain conditional comparisions
			if (indexx1 == indexx2) {
				//if x1==x2 i.e. when horizontal match then LEFT shift
				indexy1 = (indexy1 - 1) % 4;
				indexy2 = (indexy2 - 1) % 4;
				cout << " maps to: " << pftable[indexx1][indexy1] << "[" << indexx1 << ", " << indexy1 << "] & " << pftable[indexx2][indexy2] << "[" << indexx2 << ", " << indexy2 << "] respectively" << endl;
			}
			else if (indexy1 == indexy2) {
				//if y1==y2 i.e when vertical match then UP shift
				indexx1 = (indexx1 - 1);
				indexx1 %= 4;
				indexx2 = (indexx2 - 1);
				indexx2 %= 4;
				if (indexx2 > 4) {
					indexx2 = indexx2 % 4;
				}
				cout << " maps to: " << pftable[indexx1][indexy1] << "[" << indexx1 << ", " << indexy1 << "] & " << pftable[indexx2][indexy2] << "[" << indexx2 << ", " << indexy2 << "] respectively" << endl;
			}
			else {
				//exchange x1 and x2 in boxes
				indexy1 = indexy1 + indexy2;
				indexy2 = indexy1 - indexy2;
				indexy1 = indexy1 - indexy2;
				cout << " maps to: " << pftable[indexx1][indexy1] << "[" << indexx1 << ", " << indexy1 << "] & " << pftable[indexx2][indexy2] << "[" << indexx2 << ", " << indexy2 << "] respectively" << endl;
			}
			plain += pftable[indexx1][indexy1];
			plain += pftable[indexx2][indexy2];
			if (flag) {//this flag is TRUE IF (selected both string are same) or (odd End-of-line has been reached)
				if ((char)cipher[i - 1] == (char)cipher[i]) { flag = 0; }//reseting the flag to Not-END-of-Line IFF selected both letters are the same  
				if (flag) { break; } //no resetting; break IF odd End-of-line is padded with character X at the last position
			}
		}
	}
	string getPlainText()
	{
		return plain;
	}
	string getCipherText()
	{
		return cipher;
	}
	Playfair() { std::cout << "MyClass constructed\n"; }
	~Playfair() { std::cout << "MyClass destroyed\n"; }
};



void CaesarEncrypt() {
	char messageEncrypt[100], ch;
	int i, key;
	cout << "Enter a message to encrypt: ";
	cin >> messageEncrypt;
	cout << "Enter key: ";
	cin >> key;
	for (i = 0; messageEncrypt[i] != '\0'; ++i) {
		ch = messageEncrypt[i];
		if (ch >= 'a' && ch <= 'z') {
			ch = ch + key;
			if (ch > 'z') {
				ch = ch - 'z' + 'a' - 1;
			}
			messageEncrypt[i] = ch;
		}
		else if (ch >= 'A' && ch <= 'Z') {
			ch = ch + key;
			if (ch > 'Z') {
				ch = ch - 'Z' + 'A' - 1;
			}
			messageEncrypt[i] = ch;
		}
	}
	cout << "Encrypted message: " << messageEncrypt << endl << endl;
}
void CaesarDecrypt() {
	char messageDecrypt[100], ch_1;
	int j, key_1;
	cout << "Enter a message to decrypt: ";
	cin >> messageDecrypt;
	cout << "Enter key: ";
	cin >> key_1;
	for (j = 0; messageDecrypt[j] != '\0'; ++j) {
		ch_1 = messageDecrypt[j];
		if (ch_1 >= 'a' && ch_1 <= 'z') {
			ch_1 = ch_1 - key_1;
			if (ch_1 < 'a') {
				ch_1 = ch_1 + 'z' - 'a' + 1;
			}
			messageDecrypt[j] = ch_1;
		}
		else if (ch_1 >= 'A' && ch_1 <= 'Z') {
			ch_1 = ch_1 - key_1;
			if (ch_1 > 'a') {
				ch_1 = ch_1 + 'Z' - 'A' + 1;
			}
			messageDecrypt[j] = ch_1;
		}
	}
	cout << "Decrypted message: " << messageDecrypt << endl << endl;
}
vector<int> multipliers(int num, int div = 2) // num - kolicestvo bukv
{
	vector<int> ans;
	while (num > 1)
	{
		while (!(num % div))
		{
			ans.push_back(div); //push_back() - stavit element v conet vectora
			num /= div;
		}
		div++;
	}
	return ans; // ans = kolicestvo strok/stolbtov
}

vector<int> delete_in_vector(vector<int> vec, int index)
{
	vector<int> ans;
	for (size_t i = 0; i < vec.size(); i++)
		if (i != index) ans.push_back(vec.at(i));
	return ans;
}

vector<int> get_two(vector<int> nums)
{
	vector<int> ans;

	while (nums.size() > 2)
	{
		int min1 = 0, min2 = 0;
		int mini1 = 0, mini2 = 0;
		for (size_t i = 0; i < nums.size(); i++) { // size_t - typedef unsigned int
			if (nums.at(i) > min1) {
				min1 = min2 = nums.at(i);
				mini1 = mini2 = i;
			}
		}
		for (size_t i = 0; i < nums.size(); i++) {
			if (nums.at(i) <= min1 and i != mini2) {
				min1 = nums.at(i);
				mini1 = i;
			}
		}
		for (size_t i = 0; i < nums.size(); i++) {
			if (nums.at(i) <= min2 and i != mini1) {
				min2 = nums.at(i);
				mini2 = i;
			}
		}
		nums = delete_in_vector(nums, mini1);
		nums = delete_in_vector(nums, (!mini2 ? mini2 : mini2 - 1));
		nums.push_back(min1*min2);
	}

	return nums;
}

vector<vector<char>> transpos(vector<vector<char>> text)
{
	vector<vector<char>> encode(text[0].size());
	int i1 = 0;
	for (size_t i = 0; i < text[0].size(); i++) {
		encode[i].resize(text.size());
		int j1 = 0;
		for (size_t j = 0; j < text.size(); j++) {
			encode[i1][j1] = text.at(j).at(i);
			j1++;
		}
		i1++;
	}
	return encode;
}
void ScytaleEncode(string text) {
	while (multipliers(text.size()).size() < 2) text += ' ';
	vector<int> two = get_two(multipliers(text.size()));
	if (two.at(0) > two.at(1)) 
		two.at(0) ^= two.at(1) ^= two.at(0) ^= two.at(1);
	vector<vector<char>> encode(two.at(0));
	for (int i = 0; i < two.at(0); i++) {
		encode[i].resize(two.at(1));
		for (int j = 0; j < two.at(1); j++)
			encode[i][j] = text[j + two.at(1) * i];
	}
	cout << endl << "Message encode: ";
	encode = transpos(encode);// encrypt message, в первом случае 3 array по 5 элементов а в cout 5 array по 3 элемента 
	for (vector<char> vec : encode)
		for (char c : vec)
			printf("%c", c);
	cout << endl;
}
void ScytaleDecode(string text) {
	while (multipliers(text.size()).size() < 2) text += ' ';
	vector<int> two = get_two(multipliers(text.size()));
	if (two.at(0) > two.at(1)) 
		two.at(0) ^= two.at(1) ^= two.at(0) ^= two.at(1);
	vector<vector<char>> encode(two.at(0));
	for (int i = 0; i < two.at(0); i++) {
		encode[i].resize(two.at(1));
		for (int j = 0; j < two.at(1); j++)
			encode[i][j] = text[j + two.at(1) * i];
	}
	//	printf("encode: ");
	encode = transpos(encode);// encrypt message, в первом случае 3 array по 5 элементов а в cout 5 array по 3 элемента 
	cout << endl << "Message decode: ";
	vector<vector<char>> decode = transpos(encode); //last valory of encode 5
	for (vector<char> vec : decode)
		for (char c : vec)
			printf("%c", c);
	cout << endl;
}

int main()
{
	int count;
	//Scytale
	string text, text1;
	//Playfair
//	Playfair * playfair = new Playfair;
	Playfair playfair;
//	auto_ptr<Playfair> playfair(new Playfair);
//	playfair.~auto_ptr;
	string plain, cipher, keyword, ukey;
	//MENU
	PrincipalMenu:
	system("CLS");
	cout << "-----------------------------------" << endl;
	cout << "MENU" << endl;
	cout << "-----------------------------------" << endl;
	cout << "[1]. Scytale Cipher" << endl;
	cout << "[2]. Caesar Cipher" << endl;
	cout << "[3]. Playfair Cipher" << endl;
	cout << "[4]. Exit" << endl;
	cout << "-----------------------------------" << endl;
	cout << "Enter number from menu:" << endl;
	cin >> count;
		switch (count)
		{
		case 1:
			system("CLS");
			int count_2;
		MenuScytale:
			cout << "-----------------------------------" << endl;
			cout << "Menu Scytale cipher:" << endl;
			cout << "-----------------------------------" << endl;
			cout << "[1]. Encrypt message" << endl;
			cout << "[2]. Decrypt message" << endl;
			cout << "[3]. Back to principal menu" << endl;
			cout << "[4]. Exit" << endl;
			cin >> count_2;
			switch (count_2)
			{
			case 1:
				cin.ignore();
				cout << "Enter message from encrypt: ";
				getline(cin, text);
				ScytaleEncode(text);
				goto MenuScytale;
				break;
			case 2:
				ScytaleDecode(text);
				goto MenuScytale;
				break;
			case 3:
				goto PrincipalMenu;
				break;
			case 4:
				exit(0);
				break;
			default:
				break;
			}
		case 2:
			system("CLS");
			int count_1;
			MenuCaesar:
			cout << "-----------------------------------" << endl;
			cout << "Menu Caesar cipher:" << endl;
			cout << "-----------------------------------" << endl;
			cout << "[1]. Encrypt message" << endl;
			cout << "[2]. Decrypt message" << endl;
			cout << "[3]. Back to principal menu" << endl;
			cout << "[4]. Exit" << endl;
			cin >> count_1;
			switch (count_1)
			{
			case 1:
				CaesarEncrypt();
				goto MenuCaesar;
				break;
			case 2:
				CaesarDecrypt();
				goto MenuCaesar;
				break;
			case 3:
				goto PrincipalMenu;
				break;
			case 4:
				exit(0);
				break;
			default:
				break;
			}
			break;
		case 3:
			system("CLS");
		MenuPlayfair:
			cout << "-----------------------------------" << endl;
			cout << "Menu Playfair cipher:" << endl;
			cout << "-----------------------------------" << endl;
			cout << "[1]. Encrypt message" << endl;
			cout << "[2]. Decrypt message" << endl;
			cout << "[3]. Back to principal menu" << endl;
			cout << "[4]. Exit" << endl;
			cin >> count_1;
			switch (count_1)
			{
			case 1:
				cout << "Enter key of message to encrypt: ";
				cin >> keyword;
				cout << "Enter text to encrypt: ";
				cin >> plain;
				playfair.setKey(keyword);
				playfair.setPlain(plain);
				playfair.encrypt();
				cout << endl << "Finally, we have the Playfair cipher text is [ " << playfair.getCipherText() << " ]" << endl;
				playfair.~Playfair();
				goto MenuPlayfair;
				break;
			case 2:
				cout << "Enter key of message to decrypt: ";
				cin >> keyword;
				cout << "Enter text to decrypt: ";
				cin >> cipher;
				playfair.setKey(keyword);
				playfair.setCipher(cipher);
				playfair.decrypt();
				cout << endl << "Finally, we have the Plain text is [ " <<  playfair.getPlainText() << " ]" << endl;
				playfair.~Playfair();
				goto MenuPlayfair;
				break;
			case 3:
				goto PrincipalMenu;
				playfair.~Playfair();
				break;
			case 4:
				exit(0);
				break;
			default:
				break;
			}
			break;
		case 4:
			exit(0);
			break;
		default:
			cout << "Wrong input" << endl;
			break;
		}
	system("pause");
	return 0;
}