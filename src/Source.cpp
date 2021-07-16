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

static const std::string resPath = "../Resources/";

/*Functions for Misc Purposes*/
//ShowWholeFile loads the entire content of a file into memory and outputs it to the screen//
void ShowWholeFile(const std::string path)
{
	std::ifstream inFile(path);
	std::string file, part;
	while (getline(inFile, part))
		file += part + '\n';
	std::cout << file << "\n";
	inFile.close();
}
//ShowHighScores is a specialized version of "ShowWholeFile" with an extra header bar//
void ShowHighScores()
{
	std::cout << std::setw(8) << std::left << "Place" << std::setw(15) << std::left << "Name" << std::setw(8) << std::left << "Score" << "Correct Words\n\n";
	ShowWholeFile(resPath+"HighScores.dat");
}
//LoadHighScores loads the top 10 values of the highscore to their respective vectors//
void LoadHighScores(std::vector<int> &score, std::vector<std::string> &name, std::vector<int> &words, const std::string path)
{
	std::ifstream inFile(path);
	for (int index = 0; index < 10; index++)
	{
		inFile >> name[index] >> name[index] >> score[index] >> words[index];
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
	for (int i = 0; i < words.size(); i++)
		for (int j = words.size() - 1; j> i; j--)
			if (words[j] > words[j - 1])
			{
				Swap(words[j], words[j - 1]);
				Swap(name[j], name[j - 1]);
				Swap(score[j], score[j - 1]);
			}
}
//SaveHighScores just saves the highscores into a file//
void SaveHighScores(std::vector<int> &score, std::vector<std::string> &name, std::vector<int> &words, const std::string path)
{
	std::ofstream outFile(path);
	for (int index = 0; index < 10; index++)
	{
		outFile << std::setw(8) << std::left << index + 1;
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
	{
		inFile >> word;
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
	{
		outFile << Options[index].getOptionName() << ' ';
		outFile << Options[index].getOptionValue() << ' ';
		outFile << Options[index].getOptionMaximum() << '\n';
	}
	outFile.close();
}
//ChangeOptionValue increases or decreases the value of an Option object or loops the value if it hit it's boundary//
void ChangeOptionValue(Option &oneOption, const int change)
{
	if (oneOption.getOptionValue() + change == -1)
		oneOption.setOptionValue(oneOption.getOptionMaximum());
	else if (oneOption.getOptionValue() + change == oneOption.getOptionMaximum() + 1)
		oneOption.setOptionValue(0);
	else
		oneOption.setOptionValue(oneOption.getOptionValue() + change);
}
//PrintOption Prints the name and value(if necessary) of an Option object, and prints an arrow if it's currently selected//
void PrintOption(Option &oneOption, const int active, const int index, const int indent)
{
	if (active == index)
		std::cout << std::setw(indent) << std::right << "->";
	else
		std::cout << std::setw(indent) << std::right << "  ";
	std::cout << std::setw(25) << std::left << oneOption.getOptionName();
	if (oneOption.getOptionMaximum() > 0)
		std::cout << std::setw(3) << std::left << oneOption.getOptionValue();
	std::cout << '\n';
}
//OptionsMenu is the connecting function that handles all the options//
void OptionsMenu(Option GameOptions[])
{
	int active = 0;
	//exit upon escape or enter
	for (char buttonPress = ' '; buttonPress != 27 && buttonPress != 13;)
	{
		system("CLS");
		//print art and instructions
		ShowWholeFile(resPath+"BackgroundTop.dat");
		ShowWholeFile(resPath+"OptionsMenuInstructions.dat");
		//print all the options
		for (int i = 0; i < NUMBEROFOPTIONS; i++)
			PrintOption(GameOptions[i], active, i, MENUOFFSET);
		buttonPress = _getch();
		//act according to input
		if (tolower(buttonPress) == 'w' && active > 0)
			active--;
		else if (tolower(buttonPress) == 's' && active < NUMBEROFOPTIONS - 1)
			active++;
		else if (tolower(buttonPress) == 'a')
			ChangeOptionValue(GameOptions[active], -1);
		else if (tolower(buttonPress) == 'd')
			ChangeOptionValue(GameOptions[active], 1);
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
		for (int i = 0; i < word.length(); i++)
			if (isalpha(word.at(i)))
				processedWord += toupper(word.at(i));
		//check the whole list if the word is repeated or not
		for (int j = 0; j < wordList.size(); j++)
			if (wordList[j] == processedWord)
				repeated = true;
		//if it's not repeated and at least 3 letters, pass the correct word
		if (!repeated && processedWord.length()>2)
		{
			wordList.push_back(processedWord);
			//writing out the processed words as a debug
			std::cout << processedWord << '\n';
		}
	}
	inFile.close();
}
// DictionaryBubbleSort uses Bubble Sort to arrange the dictionary in alphabetical order//
void DictionaryBubbleSort(std::vector<std::string> &wordList)
{
	for (int sorted = 0; sorted < wordList.size(); sorted++)
	{
		for (int unsorted = wordList.size() - 1; unsorted > sorted; unsorted--)
			if (wordList[unsorted] < wordList[unsorted - 1])
				Swap(wordList[unsorted], wordList[unsorted - 1]);
		std::cout << wordList[sorted] << '\n';
	}
}
// DirectLoadDictionary loads a single file as the dictionary. Only use it with processed files!!!//
void DirectLoadDictionary(std::vector<std::string> &wordList, const std::string &path)
{
	wordList.clear();
	std::string word;
	std::ifstream inFile(path);
	while (inFile >> word)
		wordList.push_back(word);
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
		system("CLS");
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
		for (int i = 0; i < wordList.size(); i++)
			outFile << wordList[i] << '\n';
		outFile.close();
	}
}


/*Functions for the Scoring Functionality*/
//isLegit is the Binary Search function that checks if the entered word is found in the dictionary//
bool isLegit(const std::string word, const std::vector<std::string> &wordList)
{
	for (int min = 0, max = wordList.size(); min <= max;)
	{
		if (word == wordList[(min + max) / 2])
			return true;
		else if (word < wordList[(min + max) / 2])
			max = (min + max) / 2 - 1;
		else
			min = (min + max) / 2 + 1;
	}
	return false;
}
//score is the function that scores the game after the words have been entered//
int score(const std::vector<std::string> &wordList, std::vector<Answer> &previousAnswers, const int maxLetters)
{
	//initial score is 0
	int score = 0;
	bool unique = true;
	//for all words entered
	for (int i = 0; i < previousAnswers.size(); i++)
	{
		//write out the word
		std::cout << std::setw(maxLetters) << std::left << previousAnswers[i].getAnswerString();
		unique = true;
		//test if the answer is already present or not
		for (int j = 0; j < i && unique; j++)
			if (previousAnswers[j].getAnswerString() == previousAnswers[i].getAnswerString())
				unique = false;
		//if the answer is unique
		if (unique)
			//if it is a correct word
			if (isLegit(previousAnswers[i].getAnswerString(), wordList))
			{
				//write out the status and increase the score by 1
				std::cout << "   Correct\n";
				score++;
			}
			else
				std::cout << "   Incorrect\n";
		else
			std::cout << "   Repeated\n";
	}
	//state total number of correct words
	std::cout << "\n\n" << "You got " << score << " words right!\n";
	//return the correct number of words
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
	{
		previousAnswers[answerCount].setAnswerString(currentAnswer[currentAnswer.size() - 1]->getFigure() + previousAnswers[answerCount].getAnswerString());
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
		if (!Selection[i].getUsed())
			std::cout << " ___  ";
		else
			std::cout << "      ";
	std::cout << "\n";
	for (int i = 0; i < maxLetter; i++)
		if (!Selection[i].getUsed())
			std::cout << "/   \\ ";
		else
			std::cout << "      ";
	std::cout << "\n";
	for (int i = 0; i < maxLetter; i++)
		if (!Selection[i].getUsed())
			std::cout << "| " << Selection[i].getFigure() << " | ";
		else
			std::cout << "      ";
	std::cout << "\n";
	for (int i = 0; i < maxLetter; i++)
		if (!Selection[i].getUsed())
			std::cout << "\\___/ ";
		else
			std::cout << "      ";
	std::cout << "\n\n";
}
//isValidChoice tests if the letter pressed exists in the selection generated or not//
bool isValidChoice(std::vector<Letter> &Selection, const int maxLetter, const char buttonPress, int &index)
{
	index = 0;
	for (; index < maxLetter; index++)
		if (!Selection[index].getUsed() && Selection[index].getFigure() == toupper(buttonPress))
			return true;
	return false;
}
//ReGenerate is the function that regenerates the selection of letters if necessary//
int ReGenerate(std::vector<Letter> &Selection, const int existLetters)
{
	for (int j = 0; j < existLetters; j++)
		if (Selection[existLetters].getFigure() == Selection[j].getFigure())
		{
			Selection[existLetters].setFigure(rand() % 26 + 65);
			j = 0;
		}
	return 0;
}
//Generate generates a selection of letters for the game//
void GenerateLetters(std::vector<Letter> &Selection, const int maxLetters, const bool repetition)
{
	int vowels = 0;
	for (int i = 0; i < maxLetters; i++)
	{
		Selection[i].setFigure(rand() % 26 + 65);
		if (!repetition)
			ReGenerate(Selection, i);
		Selection[i].setUsed(false);
		if (Selection[i].getFigure() == 'A' || Selection[i].getFigure() == 'I' || Selection[i].getFigure() == 'E' || Selection[i].getFigure() == 'O' || Selection[i].getFigure() == 'U')
			vowels++;
		if (i == maxLetters - 1 && vowels == 0)
			Selection[i].setFigure('A');
	}
}
//GetSeed is the function that asks and sets the randomness of the game//
void GetSeed(int &seed, const int indent)
{
	std::string input;
	system("CLS");
	ShowWholeFile(resPath+"BackgroundTop.dat");
	ShowWholeFile(resPath+"SeedInstructions.dat");
	std::cout << std::setw(indent) << ' ' << "Seed: ";
	std::cin >> input;
	if (input != "Random")
		seed = atoi(input.data());
}
//Usable is a function that tests if a sertain letter is usable or not when the computer guesses//
bool Usable(std::vector<int> unusable, const int j)
{
	//test the banned indexes with the index of the selected letter
	for (int i = 0; i < unusable.size(); i++)
	{
		if (unusable[i] == j)
			return false;
	}
	return true;
}


/*Main Functions*/
//PlayGame is the main game function handling the gameplay//
void PlayGame(Option GameOptions[], const std::vector<std::string> &wordList)
{
	//Declarations and preparations
	int seed = time(NULL);
	int index = 0;
	int wordListIndex = 0;
	char buttonPress;
	std::vector<Letter> Selection(GameOptions[0].getOptionValue());
	std::vector<Letter*> currentAnswer;
	std::vector<Answer> previousAnswers(GameOptions[1].getOptionValue());
	GetSeed(seed, MENUOFFSET);
	srand(seed);
	GenerateLetters(Selection, GameOptions[0].getOptionValue(), GameOptions[2].getOptionValue());

	for (int answerCount = 0; answerCount < GameOptions[1].getOptionValue();)
	{
		system("CLS");
		//print art, instructions and data
		ShowWholeFile(resPath+"BackgroundTop.dat");
		ShowWholeFile(resPath+"GameInstructions.dat");
		PrintSelection(Selection, GameOptions[0].getOptionValue());
		for (int i = 0; i < answerCount; i++)
			std::cout << previousAnswers[i].getAnswerString() << '\n';
		for (int i = 0; i < currentAnswer.size(); i++)
			std::cout << currentAnswer[i]->getFigure();

		//get correct inputs from the user and handle them accordingly
		for (bool correct = false; !correct;)
		{
			correct = true;
			buttonPress = _getch();
			if (buttonPress == 8 && currentAnswer.size()>0)
				DeleteLetter(currentAnswer);
			else if (buttonPress == 13)
				PassAnswer(currentAnswer, previousAnswers, answerCount++);
			else if (isValidChoice(Selection, GameOptions[0].getOptionValue(), buttonPress, index))
				PickLetter(currentAnswer, Selection, index);
			else
				correct = false;
		}
	}
	//show results
	system("CLS");
	PrintSelection(Selection, GameOptions[0].getOptionValue());
	int correctWords = score(wordList, previousAnswers, GameOptions[0].getOptionValue());
	std::cout << "Your Seed was: " << seed << "\n\n";
	system("PAUSE");
	system("CLS");
	//handle highscore
	std::string name;
	std::vector<std::string> names(10);
	std::vector<int> score(10);
	std::vector<int> words(10);
	LoadHighScores(score, names, words, resPath+"HighScores.dat");
	std::cout << "Enter Your Name: ";
	std::cin>>name;

	if (correctWords >= 10)
		score.push_back(10);
	else if (correctWords >= 5)
		score.push_back(8);
	else if (correctWords >= 0)
		score.push_back(2);
	else
		score.push_back(0);

	names.push_back(name);
	words.push_back(correctWords);

	SortHighScores(score, names, words);
	SaveHighScores(score, names, words, resPath+"HighScores.dat");
	system("CLS");
	ShowHighScores();
	_getch();
	system("CLS");
}

//MainMenu function connects everything in the program with a menu//
void MainMenu()
{
	int active = 0;
	Option GameOptions[NUMBEROFOPTIONS];
	Option MainMenuOptions[NUMBEROFMENUOPTIONS];
	std::vector<std::string> wordList;
	//pick a pre processed dictionary.. ONLY ONE!
	/*~10,000 words from DataSet*///DirectLoadDictionary(wordList, "GameResources/allData.dat");
	/*~58,000 words from internet*/DirectLoadDictionary(wordList, resPath+"InternetList.dat");
	LoadOptions(GameOptions, resPath+"Settings.dat", NUMBEROFOPTIONS);
	LoadOptions(MainMenuOptions, resPath+"MainMenuOptions.dat", NUMBEROFMENUOPTIONS);
	for (char buttonPress = ' '; !((buttonPress == 13 || buttonPress == ' ') && active == NUMBEROFMENUOPTIONS - 1);)
	{
		system("CLS");
		//print art and instructions
		ShowWholeFile(resPath+"BackgroundTop.dat");
		ShowWholeFile(resPath+"mainMenuInstructions.dat");
		//print menu options
		for (int i = 0; i < NUMBEROFMENUOPTIONS; i++)
			PrintOption(MainMenuOptions[i], active, i, MENUOFFSET);

		buttonPress = _getch();

		//make the appropriate action
		if (tolower(buttonPress) == 'w' && active > 0)
			active--;
		else if (tolower(buttonPress) == 's' && active < NUMBEROFMENUOPTIONS - 1)
			active++;
		else if (buttonPress == 13 || buttonPress == ' ')
		{
			switch (active)
			{
			case 0: PlayGame(GameOptions, wordList); break;
			case 1: OptionsMenu(GameOptions); break;
			case 2: system("CLS"); MakeDictionary(wordList); break;
			case 3: system("CLS"); ShowHighScores(); _getch(); break;
			case 4: system("CLS"); ShowWholeFile(resPath+"Credits.dat"); _getch(); system("CLS"); break;
			}
		}
	}
}
//Main Function, Does Almost Nothing//
int main()
{
	//loading screen
	ShowWholeFile(resPath+"LoadingScreen.dat");
	Sleep(300);
	MainMenu();
	return 1;
}

