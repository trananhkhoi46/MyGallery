#include "BLabel.h"

BLabel* BLabel::createWithTTF(TTFConfig config,TextHAlignment textAlignment)
{
	auto label = Label::createWithTTF(config, "", textAlignment);
	return static_cast<BLabel*>(label);
}

void BLabel::showText(string text,float durationPerSplitedString,string splitBy)
{
	Vector<FiniteTimeAction*> vt_labelAction;
	this->setString("");

	if(splitBy != "")
	{
		vector<string> vt_stringToShow = CppUtils::splitString(text,splitBy);
		for(int i=0 ; i< vt_stringToShow.size() ;i++)
		{
			auto func = CallFunc::create([=]() {
				this->setString(this->getString() + vt_stringToShow[i]);
			});
			auto labelAction = Sequence::create(func,DelayTime::create(durationPerSplitedString),nullptr);
			vt_labelAction.pushBack(labelAction);
		}
	}
	else
		for(int i=0 ; i< text.length() ;i++)
		{
			auto func = CallFunc::create([=]() {
				this->setString(this->getString() + text.at(i));
			});
			auto labelAction = Sequence::create(func,DelayTime::create(durationPerSplitedString),nullptr);
			vt_labelAction.pushBack(labelAction);
		}
	this->runAction(Sequence::create(vt_labelAction));
}
