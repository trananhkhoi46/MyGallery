#include <iostream>
#include <memory>
#include <algorithm>
#include <iterator>
#include <sstream>
#include "CppUtils.h"
#include <sstream>
#include <string>

using namespace std;

std::string CppUtils::doubleToString(double inValue) {
	ostringstream ostr;
	ostr << inValue;
	return ostr.str();
}

double CppUtils::myAbs(double inValue) {
	if (inValue < 0)
		return -inValue;

	return inValue;
}

bool CppUtils::isFloatFloatEqual(float x1, float x2, float epsilon) {
	return myAbs(x1 - x2) <= epsilon;
}

double CppUtils::stringToDouble(const string& inValue) {
	double temp;
	istringstream istr(inValue);
	istr >> temp;
	if (istr.fail() || !istr.eof()) {
		return 0;
	}
	return temp;
}

std::vector<std::string> CppUtils::splitStringByEveryCharacter(const std::string &st){
	int i = st.length() / 2;
	while (st.substr(i, 1) != " " && i < st.length())
		i++;
	if (i + 1 < st.length()) {
		std::string result1 = st.substr(0, i);
		std::string result2 = st.substr(i + 1);
		std::vector<std::string> result = {result1, result2};

		return result;
	}
	std::vector<std::string> result = {st, ""};
	return result;
}
std::vector<std::string> CppUtils::splitStringByDelim(const std::string &s,
		char delim) {
	std::vector < std::string > elems;
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	elems.erase(std::remove(elems.begin(), elems.end(), ""), elems.end()); //Remove temp
	return elems;
}

int CppUtils::randomBetween(int x1, int x2) {
	return CCRANDOM_0_1()*(x2-x1 +1) + x1;
}
