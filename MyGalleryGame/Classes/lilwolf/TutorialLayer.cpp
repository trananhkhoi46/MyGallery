
#include "TutorialLayer.h"
#include "Resources.h"


bool TutorialLayer::init()
{
	if (!Layer::init()) {
		return false;
	}
	return true;
}
void TutorialLayer::addDialog(const std::string& tutorialString, float xPos, float yPos,float fontSize,Color3B color)
{
	vt_ChildsToBeRemoved.clear();

	auto tutorialBackground = Sprite::create(s_tutorial);
	tutorialBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	tutorialBackground->setPosition(xPos, yPos);
	this->addChild(tutorialBackground);
	tutorialBackground->setOpacity(0);
	vt_ChildsToBeRemoved.push_back(tutorialBackground);

	//add clue to background
	auto tutorialTextNode = LabelTTF::create(tutorialString, s_font_1, fontSize*s_font_ratio);
	tutorialTextNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	tutorialTextNode->setPosition(tutorialBackground->getContentSize().width/2,tutorialBackground->getContentSize().height/2);
	tutorialTextNode->setColor(color);
	tutorialBackground->addChild(tutorialTextNode);
	vt_ChildsToBeRemoved.push_back(tutorialTextNode);


	//Effect
	auto fadeIn = FadeIn::create(0.7f);
	auto delay = DelayTime::create(0.7f);
	auto moveUp = MoveBy::create(0.7f, Point(0.0f, tutorialBackground->getContentSize().height / 10.0f));
	auto moveDown = MoveBy::create(0.7f, Point(0.0f, -tutorialBackground->getContentSize().height / 10.0f));
	auto seq = Sequence::create(moveUp, moveDown, nullptr);
	auto repeat = RepeatForever::create(seq);
	tutorialBackground->runAction(Sequence::create(fadeIn,delay,repeat,nullptr));

}

void TutorialLayer::removeChildWithEffect()
{
	CCLog("size child tutorial: %d",vt_ChildsToBeRemoved.size());
	if(vt_ChildsToBeRemoved.size() == 0)
			return;

	for (int i = 0; i < vt_ChildsToBeRemoved.size(); i++) {
		auto sp = vt_ChildsToBeRemoved[i];
		auto fadeOut = FadeOut::create(0.7f);
		auto removeFunc = CallFunc::create([=]{
			sp->removeFromParent();
		});
		sp->runAction(Sequence::create(fadeOut,removeFunc,nullptr));
	}
	vt_ChildsToBeRemoved.clear();

}
