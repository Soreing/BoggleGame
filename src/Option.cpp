#include"Option.h"
#include<string>

void Option::setOptionName(const std::string &word)
{	optionName = word;	}

void Option::setOptionValue(const int &value)
{	optionValue = value;	}

void Option::setOptionMaximum(const int &value)
{	optionMaximum = value;	}

std::string Option::getOptionName()
{	return optionName;	}

int Option::getOptionValue()
{	return optionValue;	}

int Option::getOptionMaximum()
{	return optionMaximum;	}