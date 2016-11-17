#ifndef __TUTORIAL_LAYER_H__
#define __TUTORIAL_LAYER_H__

#include "cocos2d.h"
#include <string>
#include <vector>

USING_NS_CC;
using namespace std;

class TutorialLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(TutorialLayer);

	void addDialog(const string& tutorialString, float xPos, float yPos,float fontSize,Color3B color);
	void removeChildWithEffect();
	vector<Node*> vt_ChildsToBeRemoved;
};

#endif
