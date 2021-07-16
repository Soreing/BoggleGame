//*The Option class holds a string, serving as the title of the optionName   *//
//*and two integers, the optionValue and the OptionMaximumValue, the top     *//
//*value that optionValue can be assigned to. This class is the base of all  *//
//*navigateable menus and the options used for dinamically changing the game *//

#ifndef OPTION_H
#define OPTION_H
#include<string>
class Option
{
private:
	std::string optionName;
	int optionValue;
	int optionMaximum;
public:
	void setOptionName(const std::string&);
	void setOptionValue(const int&);
	void setOptionMaximum(const int&);
	std::string getOptionName();
	int getOptionValue();
	int getOptionMaximum();
};
#endif