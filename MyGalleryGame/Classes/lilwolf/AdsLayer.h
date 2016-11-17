#ifndef __ADS_LAYER_H__
#define __ADS_LAYER_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"

using namespace cocos2d::experimental::ui;
using namespace cocos2d::ui;
USING_NS_CC;
using namespace std;

class AdsLayer : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(AdsLayer);

	void addDialog();
	void removeChildWithEffect();
	vector<Node*> vt_ChildsToBeRemoved;
};

#endif
