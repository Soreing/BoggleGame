//*The Letter Class is built up a char figure*//
//*and a bool used, which signifies if it is *//
//*available for picking or not. This class  *//
//*Is the core of the selection of letters   *//
//*during the game                           *//

#ifndef LETTER_H
#define LETTER_H

class Letter
{
private:
	char figure;
	bool used;
public:
	void setFigure(const char&);
	void setUsed(const bool &);
	char getFigure();
	bool getUsed();
};
#endif