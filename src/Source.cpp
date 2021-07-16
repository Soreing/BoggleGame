#include"Answer.h"
#include"Option.h"
#include"Letter.h"

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<fstream>
#include<iomanip>
#include<vector>
#include<conio.h>
#include<windows.h>

#define NUMBEROFOPTIONS 3
#define NUMBEROFMENUOPTIONS 6
#define MAINMENUOFFSET 10
#define MENUOFFSET 20
#define PAGE_LENGTH 27

#define BCKSP 8
#define ENTER 13
#define ESC   27
#define UP    72
#define LEFT  75
#define RIGHT 77
#define DOWN  80

static const std::string resPath = "../Resources/";

// Moves teh cursor to the start of the terminal at 0,0
void cursorHome()
{
	HANDLE hOut;
	COORD Position;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

// Empties the screen from text
void emptyScreen()
{
	cursorHome();
	
	for(int i=0;i<PAGE_LENGTH; i++)
	{	std::cout << "                                                                               \n";
	}
	
	cursorHome();
}

/*Functions for Misc Purposes*/
//ShowWholeFile loads the entire content of a file into memory and outputs it to the screen//
void ShowWholeFile(const std::string path)
{
	std::ifstream inFile(path);
	std::string file, part;

	while (getline(inFile, part))
	{	file += part + '\n';
	}
	std::cout << file << "\n";
	
	inFile.close();
}

// Display credits information
void Credits()
{
	ShowWholeFile(resPath+"Credits.dat"); 
	
	// Wait for input and return	
	_getch(); 
	emptyScreen();
	return;
}

//ShowHighScores is a specialized version of "ShowWholeFile" with an extra header bar//
void ShowHighScores()
{
	std::cout << std::setw(8) << std::left << "Place" << std::setw(15) << std::left << "Name" << std::setw(8) << std::left << "Score" << "Correct Words\n\n";
	ShowWholeFile(resPath+"HighScores.dat");
	_getch();
}

//LoadHighScores loads the top 10 values of the highscore to their respective vectors//
void LoadHighScores(std::vector<int> &score, std::vector<std::string> &name, std::vector<int> &words, const std::string path)
{
	std::ifstream inFile(path);

	for (int index = 0; index < 10; index++)
	{	inFile >> name[index] >> name[index] >> score[index] >> words[index];
	}
	
	inFile.close();
}

//Swap functions simply swap two values. it's overloaded to work with int and string//
void Swap(int &value1, int &value2)
{
	int temp = value1;
	value1 = value2;
	value2 = temp;
}

void Swap(std::string &value1, std::string &value2)
{
	std::string temp = value1;
	value1 = value2;
	value2 = temp;
}

//SortHighScores sorts the three elements of the highscores into decreasing order using bubble sort//
void SortHighScores(std::vector<int> &score, std::vector<std::string> &name, std::vector<int> &words)
{
	for (int i = 0; i < (int)score.size(); i++)
	{	for (int j = score.size() - 1; j> i; j--)
		{	if (score[j] > score[j - 1])
			{	Swap(words[j], words[j - 1]);
				Swap(name[j], name[j - 1]);
				Swap(score[j], score[j - 1]);
			}
		}
	}
}
//SaveHighScores just saves the highscores into a file//
void SaveHighScores(std::vector<int> &score, std::vector<std::string> &name, std::vector<int> &words, const std::string path)
{
	std::ofstream outFile(path);
	
	for (int index = 0; index < 10; index++)
	{	outFile << std::setw(8) << std::left << index + 1;
		outFile << std::setw(15) << std::left << name[index];
		outFile << std::setw(8) << std::left << score[index];
		outFile << std::left << words[index] << '\n';
	}
	
	outFile.close();
}


/* Functions for the Options Functionality*/
//LoadOptions initializes an array of Option objects from a file. Used for GameOptions of the MainMenu Options//
void LoadOptions(Option Options[], const std::string path, const int number)
{
	std::ifstream inFile(path);
	std::string word;
	int value;

	for (int index = 0; index < number; index++)
	{	inFile >> word;
		Options[index].setOptionName(word);
		inFile >> value;
		Options[index].setOptionValue(value);
		inFile >> value;
		Options[index].setOptionMaximum(value);
	}

	inFile.close();
}
//SaveOptions saves the array Option Objects' values into their respective file// 
void SaveOptions(Option Options[], const std::string path, const int number)
{
	std::ofstream outFile(path);
	
	for (int index = 0; index < number; index++)
	{	outFile << Options[index].getOptionName() << ' ';
		outFile << Options[index].getOptionValue() << ' ';
		outFile << Options[index].getOptionMaximum() << '\n';
	}
	
	outFile.close();
}
//ChangeOptionValue increases or decreases the value of an Option object or loops the value if it hit it's boundary//
void ChangeOptionValue(Option &option, const int change)
{
	int current = option.getOptionValue();
	int maximum = option.getOptionMaximum() + 1;

	option.setOptionValue((current + change + maximum) % maximum);
}

//PrintOption Prints the name and value(if necessary) of an Option object, and prints an arrow if it's currently selected//
void PrintOption(Option &oneOption, const int active, const int index, const int indent)
{
	std::cout << std::setw(indent) << std::right << (active == index ? "->" : "  ") ;
	std::cout << std::setw(25) << std::left << oneOption.getOptionName();
	
	// Only print the option value if it's a numeric option
	if (oneOption.getOptionMaximum() > 0)
	{	std::cout << std::setw(3) << std::left << oneOption.getOptionValue();
	}
	
	std::cout << '\n';
}

//OptionsMenu is the connecting function that handles all the options//
void OptionsMenu(Option GameOptions[])
{
	int active = 0;
	char btnPress;

	for (;;)
	{
		emptyScreen();

		//print art and instructions
		ShowWholeFile(resPath+"BackgroundTop.dat");
		ShowWholeFile(resPath+"OptionsMenuInstructions.dat");

		//print all the options
		for (int i = 0; i < NUMBEROFOPTIONS; i++)
		{	PrintOption(GameOptions[i], active, i, MENUOFFSET);
		}

		// Take a character Input
		btnPress = _getch();
		if (btnPress == 0 || btnPress == 0xE0)
			btnPress = _getch();

		//act according to input
		if (btnPress == UP && active > 0)
		{	active--;
		}
		else if (btnPress == DOWN && active < NUMBEROFOPTIONS - 1)
		{	active++;
		}
		else if (btnPress == LEFT)
		{	ChangeOptionValue(GameOptions[active], -1);
		}
		else if (btnPress == RIGHT)
		{	ChangeOptionValue(GameOptions[active], 1);
		}
		else if( btnPress == ENTER || btnPress == ESC)
		{	return;
		}
	}

	//save changes
	SaveOptions(GameOptions, resPath+"Settings.dat", NUMBEROFOPTIONS);
}


/*Functions for the Dictionary*/
//ProcessFile loads in a file's contents, strips punctuation, capitalizes letters and adds all words to the dictionary// 
void ProcessFile(std::vector<std::string> &wordList, const std::string path)
{
	bool repeated;
	std::ifstream inFile(path);
	std::string word, processedWord;

	for (; inFile >> word;)
	{
		//reset word properties
		repeated = false;
		processedWord.clear();

		//passes letters to the other string capitalized
		for (int i = 0; i < (int)word.length(); i++)
		{	if (isalpha(word.at(i)))
			{	processedWord += toupper(word.at(i));
			}
		}

		//check the whole list if the word is repeated or not
		for (int j = 0; j < (int)wordList.size(); j++)
		{	if (wordList[j] == processedWord)
			{	repeated = true;
			}
		}

		//if it's not repeated and at least 3 letters, pass the correct word
		if (!repeated && processedWord.length()>2)
		{	wordList.push_back(processedWord);
			std::cout << processedWord << '\n';
		}
	}

	inFile.close();
}

// DictionaryBubbleSort uses Bubble Sort to arrange the dictionary in alphabetical order//
void DictionaryBubbleSort(std::vector<std::string> &wordList)
{
	for (int sorted = 0; sorted < (int)wordList.size(); sorted++)
	{
		for (int unsorted = wordList.size() - 1; unsorted > sorted; unsorted--)
		{	if (wordList[unsorted] < wordList[unsorted - 1])
			{	Swap(wordList[unsorted], wordList[unsorted - 1]);
			}
		}
		std::cout << wordList[sorted] << '\n';
	}
}

// DirectLoadDictionary loads a single file as the dictionary. Only use it with processed files!!!//
void DirectLoadDictionary(std::vector<std::string> &wordList, const std::string &path)
{
	std::string word;
	std::ifstream inFile(path);

	wordList.clear();
	while (inFile >> word)
	{	wordList.push_back(word);
	}

	inFile.close();
}

//MakeDictionary processes, sorts and saves a dictionary file in the resource folder//
void MakeDictionary(std::vector<std::string> &wordList)
{
	char option = ' ';
	for (; toupper(option) != 'Y' && toupper(option) != 'N';)
	{
		std::cout << "!! This could take very long.. this option processes thousands of words, and sorts them. !!\n";
		std::cout << "!! Are you sure you want to continue? (Y/N)";
		option = _getch();
		emptyScreen();
	}

	if (toupper(option) == 'Y')
	{
		wordList.clear();

		//Place Text Files Here To make a Dictionary File
		ProcessFile(wordList, resPath+"ThreeLetter Word01.txt");
		ProcessFile(wordList, resPath+"FourLetter Words02.txt");
		ProcessFile(wordList, resPath+"FiveLetter Words03.txt");
		ProcessFile(wordList, resPath+"FinalData.txt");

		DictionaryBubbleSort(wordList);

		std::ofstream outFile(resPath+"DataSet.dat");

		for (int i = 0; i < (int)wordList.size(); i++)
		{	outFile << wordList[i] << '\n';
		}

		outFile.close();
	}
}

/*Functions for the Scoring Functionality*/
//isLegit is the Binary Search function that checks if the entered word is found in the dictionary//
bool isLegit(const std::string word, const std::vector<std::string> &wordList)
{
	for (int min = 0, max = (int)wordList.size(); min <= max;)
	{
		if (word == wordList[(min + max) / 2])
		{	return true;
		}
		else if (word < wordList[(min + max) / 2])
		{	max = (min + max) / 2 - 1;
		}
		else
		{	min = (min + max) / 2 + 1;
		}
	}
	return false;
}

// Score set of different letters based on their frequency in the english alphabet
//                         A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z
const char scoreSet[26] = {1,3,3,2,1,3,3,2,1,5,4,3,3,2,1,3,5,2,2,1,3,4,3,5,3,5};

//score is the function that scores the game after the words have been entered//
int score(const std::vector<std::string> &wordList, std::vector<Answer> &answers, const int maxLetters, int &score, int &wordCnt)
{
	score   = 0;
	wordCnt = 0;
	bool unique  = true;
	bool correct = true;
	std::string currentWord;

	// For all words entered by the user
	for (int i = 0; i < (int)answers.size(); i++)
	{
		currentWord = answers[i].getAnswerString();
		std::cout << std::setw(maxLetters) << std::left << currentWord;

		// Test if the answer is correct
		correct = isLegit(currentWord, wordList);

		// Test if the answer is already present or not
		unique = true;
		for (int j = 0; j < i && unique; j++)
		{	if (answers[j].getAnswerString() == currentWord)
			{	unique = false;
			}
		}

		// Print out the state of the word
		// Increase the score by some for each letter if it's correct and unique
		if(!correct)
		{	std::cout << "   Incorrect\n";
		}
		else if(!unique)
		{	std::cout << "   Repeated\n";
		}
		else
		{	std::cout << "   Correct\n";
			
			wordCnt++;	
			for(int j = 0; j < (int)currentWord.size(); j++)
			{	score+= scoreSet[currentWord[j]-'A'];
			}
		}
	}

	return score;
}

/*Functions for the Game Functionality*/
//DeleteLetter returns the last letter to the selection//
void DeleteLetter(std::vector<Letter*> &currentAnswer)
{
	currentAnswer[currentAnswer.size() - 1]->setUsed(false);
	currentAnswer.pop_back();
}

//PassAnswer submits the written word//
void PassAnswer(std::vector<Letter*> &currentAnswer, std::vector<Answer> &previousAnswers, const  int answerCount)
{
	while (currentAnswer.size() > 0)
	{	previousAnswers[answerCount].setAnswerString(currentAnswer[currentAnswer.size() - 1]->getFigure() + previousAnswers[answerCount].getAnswerString());
		DeleteLetter(currentAnswer);
	}
}

//PickLetter takes a letter from the selection and adds it to the word being written//
void PickLetter(std::vector<Letter*> &currentAnswer, std::vector<Letter> &Selection, const int &index)
{
	currentAnswer.push_back(&Selection[index]);
	Selection[index].setUsed(true);
}

//PrintSelection prints the generated letters for the game//
void PrintSelection(std::vector<Letter> &Selection, const int maxLetter)
{
	for (int i = 0; i < maxLetter; i++)
	{	std::cout<<  (Selection[i].getUsed() ? "      " : " ___  ");
	}

	std::cout << "\n";
	for (int i = 0; i < maxLetter; i++)
	{	std::cout<<  (Selection[i].getUsed() ? "      " : "/   \\ ");
	}

	std::cout << "\n";
	for (int i = 0; i < maxLetter; i++)
	{	std::cout<<  (Selection[i].getUsed() ? "      " : (std::string("| ") + Selection[i].getFigure() + " | "));
	}

	std::cout << "\n";
	for (int i = 0; i < maxLetter; i++)
	{	std::cout<<  (Selection[i].getUsed() ? "      " : "\\___/ ");
	}
	
	std::cout << "\n\n";
}

//isValidChoice tests if the letter pressed exists in the selection generated or not//
bool isValidChoice(std::vector<Letter> &Selection, const int maxLetter, const char buttonPress, int &index)
{
	index = 0;

	for (; index < maxLetter; index++)
	{	if (!Selection[index].getUsed() && Selection[index].getFigure() == toupper(buttonPress))
		{	return true;
		}
	}

	return false;
}

//ReGenerate is the function that regenerates the selection of letters if necessary//
void ReGenerate(std::vector<Letter> &Selection, const int existLetters)
{
	for (int j = 0; j < existLetters; j++)
	{	if (Selection[existLetters].getFigure() == Selection[j].getFigure())
		{	Selection[existLetters].setFigure(rand() % 26 + 65);
			j = 0;
		}
	}

	return;
}

//Generate generates a selection of letters for the game//
void GenerateLetters(std::vector<Letter> &Selection, const int maxLetters, const bool repetition)
{
	const static std::string vowelList = "AIEOU";
	int vowelCount = 0;

	for (int i = 0; i < maxLetters; i++)
	{
		// Generate a new letter
		Selection[i].setFigure(rand() % 26 + 'A');
		Selection[i].setUsed(false);

		// If letter repetition is not allowed, re-generate the letter if it already exists
		if (!repetition)
		{	ReGenerate(Selection, i);
		}

		// Keep track of the number of vowels
		if( vowelList.find(Selection[i].getFigure()) != std::string::npos)
		{	vowelCount++;
		}

		// If no vowels have been generated, add a random vowel as the last letter
		if (i == maxLetters - 1 && vowelCount == 0)
		{	Selection[i].setFigure(vowelList[rand()% vowelList.size()]);
		}
	}
}

//GetSeed is the function that asks and sets the randomness of the game//
void GetSeed(int &seed, const int indent)
{
	std::string input;

	emptyScreen();
	ShowWholeFile(resPath+"BackgroundTop.dat");
	ShowWholeFile(resPath+"SeedInstructions.dat");

	std::cout << std::setw(indent) << ' ' << "Seed: ";
	std::cin >> input;

	// If the user entered a custom seed, convert it to numbers
	// If the seed is not numeric, use a random seed
	if (input != "Random")
	{	if(atoi(input.data()) != 0)
		{	seed = atoi(input.data());
		}
	}
}

/*Main Functions*/
//PlayGame is the main game function handling the gameplay//
void PlayGame(Option GameOptions[], const std::vector<std::string> &wordList)
{
	// Declarations and preparations
	int index = 0;
	int wordListIndex = 0;
	int seed = (int)time(NULL);
	
	int playerScore;
	int correctWords;
	char buttonPress;

	std::vector<Letter> Selection(GameOptions[0].getOptionValue());
	std::vector<Letter*> currentAnswer;
	std::vector<Answer> previousAnswers(GameOptions[1].getOptionValue());

	// Seeding the random number generator
	GetSeed(seed, MENUOFFSET);
	srand(seed);

	GenerateLetters(Selection, GameOptions[0].getOptionValue(), GameOptions[2].getOptionValue() ? true : false);

	for (int answerCount = 0; answerCount < GameOptions[1].getOptionValue();)
	{
		emptyScreen();

		// Print art, instructions and data
		ShowWholeFile(resPath+"BackgroundTop.dat");
		ShowWholeFile(resPath+"GameInstructions.dat");

		PrintSelection(Selection, GameOptions[0].getOptionValue());

		// Print previously committed words
		for (int i = 0; i < answerCount; i++)
		{	std::cout << previousAnswers[i].getAnswerString() << '\n';
		}
		
		// Print current word being edited
		for (int i = 0; i < (int)currentAnswer.size(); i++)
		{	std::cout << currentAnswer[i]->getFigure();
		}

		// Get correct inputs from the user and handle them accordingly
		for (bool correct = false; !correct;)
		{
			correct = true;
			buttonPress = _getch();
			
			if (buttonPress == BCKSP && currentAnswer.size()>0)
			{	DeleteLetter(currentAnswer);
			}
			else if (buttonPress == ENTER)
			{	PassAnswer(currentAnswer, previousAnswers, answerCount++);
			}
			else if (isValidChoice(Selection, GameOptions[0].getOptionValue(), buttonPress, index))
			{	PickLetter(currentAnswer, Selection, index);
			}
			else
			{	correct = false;
			}
		}
	}

	// Show results
	emptyScreen();
	PrintSelection(Selection, GameOptions[0].getOptionValue());

	score(wordList, previousAnswers, GameOptions[0].getOptionValue(), playerScore, correctWords);

	std::cout << "\n\n" << "You got a score of " << playerScore << " with " << correctWords <<" correct words!\n";
	std::cout << "Your Seed was: " << seed << "\n\n";

	std::cout<< "Press any key to continue...\n";
	_getch();
	emptyScreen();

	// Handle highscore
	std::string name;
	std::vector<std::string> names(10);
	std::vector<int> score(10);
	std::vector<int> words(10);

	LoadHighScores(score, names, words, resPath+"HighScores.dat");

	std::cout << "Enter Your Name: ";
	std::cin >> name;

	score.push_back(playerScore);
	names.push_back(name);
	words.push_back(correctWords);

	SortHighScores(score, names, words);
	SaveHighScores(score, names, words, resPath+"HighScores.dat");

	emptyScreen();
	ShowHighScores();

	_getch();
	emptyScreen();
}

//MainMenu function connects everything in the program with a menu//
void MainMenu()
{
	int  active = 0;
	char btnPress;
	Option GameOptions[NUMBEROFOPTIONS];
	Option MainMenuOptions[NUMBEROFMENUOPTIONS];
	std::vector<std::string> wordList;

	// Pick a pre processed dictionary.. ONLY ONE!
	/*~10,000 words from DataSet */ //DirectLoadDictionary(wordList, "GameResources/allData.dat");
	/*~58,000 words from internet*/ DirectLoadDictionary(wordList, resPath+"InternetList.dat");

	LoadOptions(GameOptions, resPath+"Settings.dat", NUMBEROFOPTIONS);
	LoadOptions(MainMenuOptions, resPath+"MainMenuOptions.dat", NUMBEROFMENUOPTIONS);

	for (;;)
	{
		emptyScreen();

		// Print art and instructions
		ShowWholeFile(resPath+"BackgroundTop.dat");
		ShowWholeFile(resPath+"mainMenuInstructions.dat");

		// Print menu options
		for (int i = 0; i < NUMBEROFMENUOPTIONS; i++)
		{	PrintOption(MainMenuOptions[i], active, i, MENUOFFSET);
		}

		// Take a character Input
		btnPress = _getch();
		if (btnPress == 0 || btnPress == 0xE0)
		{	btnPress = _getch();
		}

		// Make the appropriate action
		if (btnPress == UP && active > 0)
		{	active--;
		}
		else if (btnPress == DOWN && active < NUMBEROFMENUOPTIONS - 1)
		{	active++;
		}
		else if (btnPress == ENTER)
		{	emptyScreen();
			switch (active)
			{	case 0: PlayGame(GameOptions, wordList); break;
				case 1: OptionsMenu(GameOptions); break;
				case 2: MakeDictionary(wordList); break;
				case 3: ShowHighScores(); break;
				case 4: Credits(); break;
				case 5: return;
			}
		}
	}
}

//Main Function, Does Almost Nothing//
int main()
{
	// Loading screen
	ShowWholeFile(resPath+"LoadingScreen.dat");
	Sleep(300);

	MainMenu();
	return 0;
}

