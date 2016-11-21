#include "BScrollView.h"

BScrollView* BScrollView::createVertical(int numberOfRows, float itemMargin,Size scrollFrameSize)
{
	BScrollView* scrollview = static_cast<BScrollView*>(ScrollView::create());
	auto containerSize = Size(scrollFrameSize.width,
			(numberOfRows * (itemMargin - 1)));
	if (containerSize.height < scrollFrameSize.height)
		containerSize = scrollFrameSize;
	scrollview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	scrollview->setInnerContainerSize(containerSize);
	scrollview->setContentSize(scrollFrameSize);
	scrollview->setInertiaScrollEnabled(true); //Scroll duoc
	scrollview->setDirection(ScrollView::Direction::VERTICAL); //Scroll theo chieu doc
	scrollview->setBounceEnabled(true); //Phan hoi nhe khi scroll het

	scrollview->topPosition = containerSize.height - itemMargin / 2;
	scrollview->leftPosition = itemMargin / 2;
	return scrollview;
}
BScrollView* BScrollView::createHorizontal(int numberOfColumns, float itemMargin,Size scrollFrameSize)
{
	BScrollView* scrollview = static_cast<BScrollView*>(ScrollView::create());
	auto containerSize = Size((numberOfColumns * (itemMargin - 1)),scrollFrameSize.height);
	if (containerSize.width < scrollFrameSize.width)
		containerSize = scrollFrameSize;
	scrollview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	scrollview->setInnerContainerSize(containerSize);
	scrollview->setContentSize(scrollFrameSize);
	scrollview->setInertiaScrollEnabled(true); //Scroll duoc
	scrollview->setDirection(ScrollView::Direction::HORIZONTAL); //Scroll theo chieu ngang
	scrollview->setBounceEnabled(true); //Phan hoi nhe khi scroll het

	scrollview->topPosition = containerSize.height - itemMargin / 2;
	scrollview->leftPosition = itemMargin / 2;
	return scrollview;
}
