//*The Answer class just stores a string*//
//*Originally it was bigger, but they   *//
//*proved unnecessary. This class now   *//
//*feels kind of useless.               *//

#ifndef ANSWER_H
#define ANSWER_H
#include<string>

class Answer
{
private:
	std::string answerString;
public:
	void setAnswerString(const std::string&);
	std::string getAnswerString();
};
#endif