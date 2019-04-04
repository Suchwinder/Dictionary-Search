/* 
Author: Suchwinder Singh

Take an input from the user that is a valid word and return the pronunciation of the word,
words that are identical to the user input according to pronunciation,
words that have a phoneme added to it or removed or replaced (only one phoneme)
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
using namespace std;

//used ifstream globally since dictionary used everywhere, same for array in respect to replace phoneme use
ifstream dictionary ("cmudict.0.7a");
string myData[20], dicData[20], dicData1[20], dicData2[20];

void splitOnSpace(string s, string& before, string& after) { //seperate a string according to spaces
	// reset strings
	before = ""; 
	after = "";
	// accumulate before space
	int i = 0;
	while (i < s.size() && not isspace(s[i])){ 
		before += s[i]; i++; 
	}
	// skip the space
	i++; 
	// accumulate after space
	while (i < s.size()) { 
		after += s[i]; i++; 
	}
}

void pronunciation(string word, string before, string after, string& currentAfter, string& currentBefore){
	string line="";
	while (getline(dictionary, line)){
		splitOnSpace(line, before, after);
		//see if word is in dictionary
		if (word == before){
			cout << "Pronunciaton:\t"<< after << endl;
			currentAfter = after;
			currentBefore = before;
			break;
		}
	}
	dictionary.close();
	dictionary.open("cmudict.0.7a");
}

void identical(string word, string before, string after, string currentAfter, string currentBefore){
	//adds all the words that are identical in pronunciation to a variable
	string line = "", totalIdentical = "";
	while (getline(dictionary, line)){
		splitOnSpace(line, before, after);
		if ((currentAfter == after) && (currentBefore != before)){
			totalIdentical += (before + " ");
		}
	}
	cout << "Identical:\t" << totalIdentical << endl;
	dictionary.close();
	dictionary.open("cmudict.0.7a");
}

void seperate(string currentAfter){
	int arrayCount = 0;
	string addOn = "";
	currentAfter+= " ";
	//Add each block of values to the array until a space is met, then it moves to the next index
	for (int i=1; i <= currentAfter.size(); i++){
		if(!isspace(currentAfter[i])){
			addOn += currentAfter[i];
		}
		else if (isspace(currentAfter[i])){
			myData[arrayCount] = addOn;
			addOn = "";
			arrayCount++;
		}
	}
}


void dicSeperate(string after, string before){ //array for add phoneme
	int arrayCount = 0;
	string addOn = "";
	after+= " ";
	for (int i=1; i <= after.size(); i++){
		if(!isspace(after[i])){
			addOn += after[i];
		}
		else if (isspace(after[i])){
			dicData[arrayCount] = addOn;
			addOn = "";
			arrayCount++;
		}
	}
}

void dicSeperate1(string after, string before, int& totalValue){ //array for remove phoneme
	int arrayCount = 0;
	string addOn = "";
	after+=" ";
	for (int i=1; i <= after.size(); i++){
		if(!isspace(after[i])){
			addOn += after[i];
		}
		else if (isspace(after[i])){
			dicData1[arrayCount] = addOn;
			addOn = "";
			arrayCount++;
			totalValue++;  //Keep count the size of the indexes actually filled, and use in remove function
		}
	}
}

void dicSeperate2(string after, string before, int& totalValue){ //array for replace phoneme
	int arrayCount = 0;
	string addOn = "";
	after+= " ";
	for (int i=1; i <= after.size(); i++){
		if(!isspace(after[i])){
			addOn += after[i];
		}
		else if (isspace(after[i])){
			dicData2[arrayCount] = addOn;
			addOn = "";
			arrayCount++;
			totalValue++; //Keep count the size of the indexes actually filled, and use in replace function
		}
	}
}

void added(string currentBefore, string currentAfter, string before, string after, string word){
	string addedPhonemes = "", line = "";
	while (getline(dictionary, line)){
		int countArray = 0, dicSpaces = 0, mySpaces = 0, differences = 0, invalid = 0;
		splitOnSpace(line, before, after);
		for (int i=0; i < before.size(); i++){
			if (int(before[i]) == 40){
				invalid++;
			}
		}
		if (invalid == 0){
			for (int i = 0; i < after.size(); i++){
				if (isspace(after[i])){
					dicSpaces++;
				}
			}
			for (int i = 0; i < currentAfter.size(); i++){
				if (isspace(currentAfter[i])){
					mySpaces++;
				}
			}
			//if condition true, iterate index by index in the array and compare them
			if (dicSpaces == (mySpaces+1)){
				//create an array for the dictionary pronunciation for easy comparison with user pronunciation
				dicSeperate (after, before);
				for (int i = 0; i < 20; i++){
					if ((dicData[i] == "") && (myData[countArray] == "")){
						continue;
					}
					else if ((dicData[i] != myData[countArray]) && (word != before)){
						differences++;
					}
					else {
						countArray++;
					}
				}
				if (differences == 1){
					addedPhonemes += (before + " ");
				}
			}
		}
	}
	cout << "Add phoneme:\t" << addedPhonemes << endl;
	dictionary.close();
	dictionary.open("cmudict.0.7a");
}

void removed(string currentBefore, string currentAfter, string before, string after, string word){
	string removedPhonemes = "", line = "";
	while (getline(dictionary, line)){
		int countArray = 0, dicSpaces = 0, mySpaces = 0, differences = 0, invalid = 0, totalValue = 0;
		splitOnSpace(line, before, after);
		for (int i=0; i < before.size(); i++){
			if (int(before[i]) == 40){
				invalid++;
			}
		}
		if (invalid == 0){
			for (int i = 0; i < after.size(); i++){
				if (isspace(after[i])){
					dicSpaces++;
				}
			}
			for (int i = 0; i < currentAfter.size(); i++){
				if (isspace(currentAfter[i])){
					mySpaces++;
				}
			}
			//if condition true, iterate index by index in the array and compare them
			if (dicSpaces == (mySpaces-1)){
				//create an array for the dictionary pronunciation for easy comparison with user pronunciation
				dicSeperate1(after, before, totalValue);
				for (int i = 0; countArray < totalValue; i++){
					if ((dicData1[countArray] != myData[i]) && (word != before)){
						differences++;
					}
					else {
						countArray++;
					}
				}
				if (differences <= 1){
					removedPhonemes += (before + " ");
				}
			}
		}
	}
	cout << "Remove phoneme:\t" << removedPhonemes << endl;
	dictionary.close();
	dictionary.open("cmudict.0.7a");
}

void replaced(string currentBefore, string currentAfter, string before, string after, string word){
	string replacedPhonemes = "", line = "";
	while (getline(dictionary, line)){
		int countArray = 0, dicSpaces = 0, mySpaces = 0, strike = 0, invalid = 0, totalValue = 0;
		splitOnSpace(line, before, after);
		for (int i=0; i < before.size(); i++){
			if (int(before[i]) == 40){
				invalid++;
			}
		}
		if (invalid == 0){
			for (int i = 0; i < after.size(); i++){
				if (isspace(after[i])){
					dicSpaces++;
				}
			}
			for (int i = 0; i < currentAfter.size(); i++){
				if (isspace(currentAfter[i])){
					mySpaces++;
				}
			}
			//if condition true, iterate index by index in the array and compare them
			if (dicSpaces == mySpaces){
				//create an array for the dictionary pronunciation for easy comparison with user pronunciation
				dicSeperate2 (after, before, totalValue);
				for (int i = 0; i < totalValue; i++){
					if ((myData[i] != dicData2[countArray]) && (word != before)){
						strike++;
						countArray++;
					}
					else {
						countArray++;
					}
				}
				if (strike == 1){
					replacedPhonemes += (before + " ");
				}
			}
		}
	}
	cout << "Replace phoneme:\t" << replacedPhonemes << endl;
	dictionary.close();
	dictionary.open("cmudict.0.7a");
}

int main() {
	string newWord, word, before, after, phoneme, currentAfter = "", currentBefore = "", line;
	int wordIn = 0;
	cout << "Enter word: " << endl;
	cin >> word;
	//This checks if the file opens or not, if not it uses the cerr function 
	//to output that the file failed, and to return 0, ending the program.
	if (dictionary.fail()){
		cerr << "Input file failed to open \n";
		return(0);
	}
	//This for loop takes the input given and character by character evaluates it
	for (int i = 0; i < word.length(); i++) {
		int letterNum, newLetter;
		letterNum = int(word[i]);
		//letters must be capital 65-90, or lowercase (97-122), or apostrophe (39)
		if ((letterNum <= 90 && letterNum>= 65) || (letterNum <=122 && letterNum >=97) || (letterNum == 39)){
			//if letter is lowercase then this changes it to capital
			if (letterNum <=122 && letterNum >=97) {
				int newLetter = letterNum - 32;
				newWord += char(newLetter);
			}
			else{
				newWord += char(letterNum);
			}
			continue;
		}
		else{
			cout << "Incorrect Input" << endl;
			return(0);
		}
	}
	//setting the newWord to word, for easier use in my code
	if (newWord != ""){
		word = newWord;
	}
	//while loop reads dictionary to see if user input is in the dictionary
	while (getline(dictionary, line)){
		splitOnSpace(line, before, after);
		if (word == before){
			wordIn = 1;
			break;
		}
		else{
			wordIn = 0;
		}
	}
	//tests conditions to declare if it is in the dictionary or not
	if (wordIn == 0){
		cout << "Not found" << endl;
		return(0);
	}
	//closes and open dictionary fo reuse
	else {
		dictionary.close();
		dictionary.open("cmudict.0.7a");
	}

	pronunciation(word, before, after, currentAfter, currentBefore); //Prints the pronunciation of it;
	identical(word, before, after, currentAfter, currentBefore); //Prints identical words, if they exist;
	seperate(currentAfter); // creates an array for the user input
	added(currentBefore, currentAfter, before, after, word); //Prints word with one added phonemes
	removed(currentBefore, currentAfter, before, after, word); //Prints word with one removed phonemes
	replaced(currentBefore, currentAfter, before, after, word); //Prints word with one replaced phoneme

	return 0;
}