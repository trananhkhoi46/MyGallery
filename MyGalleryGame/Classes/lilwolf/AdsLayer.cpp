
#include "AdsLayer.h"
#include "Resources.h"


bool AdsLayer::init()
{
	if (!Layer::init()) {
		return false;
	}
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	return true;
}
void AdsLayer::addDialog()
{
	auto winSize = Director::getInstance()->getVisibleSize();
	vt_ChildsToBeRemoved.clear();

	auto tutorialBackground = Sprite::create(s_adslayer_background);
	tutorialBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(tutorialBackground);
	vt_ChildsToBeRemoved.push_back(tutorialBackground);

	WebView* _webView = WebView::create();
	_webView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_webView->setPosition(Vec2(tutorialBackground->getContentSize().width/2,tutorialBackground->getContentSize().height/2));
	_webView->setContentSize(tutorialBackground->getContentSize()*0.8);
	_webView->loadURL("http://www.zing.vn");
	_webView->setTag(1996);
	//_webView->setScalesPageToFit(false);
	tutorialBackground->addChild(_webView);
	vt_ChildsToBeRemoved.push_back(_webView);

	auto btn_closeads = Button::create(s_adslayer_closebtn);
	btn_closeads->setPosition(Vec2(_webView->getContentSize().width,_webView->getContentSize().height));
	btn_closeads->setTouchEnabled(true);
	btn_closeads->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	btn_closeads->setPressedActionEnabled(true);
	btn_closeads->addTouchEventListener(
				[this](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
				{
					if(type == cocos2d::ui::Widget::TouchEventType::ENDED)
					{
						removeChildWithEffect();
					}
				});
	_webView->addChild(btn_closeads);
	vt_ChildsToBeRemoved.push_back(btn_closeads);

	//Effect
	auto fadeIn = FadeIn::create(0.7f);
	tutorialBackground->runAction(fadeIn);
}

void AdsLayer::removeChildWithEffect()
{
	CCLog("size child tutorial: %d",vt_ChildsToBeRemoved.size());
	if(vt_ChildsToBeRemoved.size() == 0)
			return;

	for (int i = 0; i < vt_ChildsToBeRemoved.size(); i++) {
		auto sp = vt_ChildsToBeRemoved[i];
		auto fadeOut = FadeOut::create(0.7f);
		if(sp->getTag() == 1996)
			fadeOut = FadeOut::create(0);
		auto removeFunc = CallFunc::create([=]{
			sp->removeFromParent();
		});
		sp->runAction(Sequence::create(fadeOut,removeFunc,nullptr));
	}
	vt_ChildsToBeRemoved.clear();

}
