#ifndef __B_SCROLLVIEW_H__
#define __B_SCROLLVIEW_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;

class BScrollView : public ScrollView
{
public:
	static BScrollView* createVertical(int numberOfRows, float itemMargin,Size scrollFrameSize);
	static BScrollView* createHorizontal(int numberOfColumns, float itemMargin,Size scrollFrameSize);
	float topPosition;
	float leftPosition;
};

#endif
