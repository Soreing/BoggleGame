#include"Letter.h"

void Letter::setFigure(const char &symbol)
{	figure = symbol;	}

void Letter::setUsed(const bool &value)
{	used = value;	}

char Letter::getFigure()
{	return figure;	}

bool Letter::getUsed()
{	return used;	}