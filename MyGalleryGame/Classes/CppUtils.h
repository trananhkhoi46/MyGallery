#ifndef __CppUtils__
#define __CppUtils__

#include <stdio.h>
#include <vector>
#include <iostream>
#include <iterator>
#include <cstdlib>
#include <string>
#include <sstream>
#include "cocos2d.h"
USING_NS_CC;

class CppUtils {
public:
	CppUtils() = delete; //Compiler will not compile constructor -> for optimizing point

	static std::string doubleToString(double inValue);
	static double stringToDouble(const std::string& inValue);
	static double myAbs(double inValue);
	static bool isFloatFloatEqual(float x1, float x2, float epsilon);
	static std::vector<std::string> splitStringByDelim(const std::string& st,
			char delim);
	static std::vector<std::string> splitStringByEveryCharacter(const std::string &s);
	static int randomBetween(int x1, int x2);
};

#endif
