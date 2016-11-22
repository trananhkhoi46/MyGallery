#ifndef __B_LABEL_H__
#define __B_LABEL_H__

#include "cocos2d.h"
#include "CppUtils.h"

USING_NS_CC;
using namespace std;


class BLabel : public cocos2d::Label
{
public:
	CREATE_FUNC(BLabel);
	static BLabel* createWithTTF(TTFConfig config,TextHAlignment textAlignment);
	void showText(string text,float durationPerSplitedString,string splitBy);

private:

};

#endif
