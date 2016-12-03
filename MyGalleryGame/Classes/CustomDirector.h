#ifndef _CUSTOM_DIRECTOR_H_
#define _CUSTOM_DIRECTOR_H_
#include "cocos2d.h"
USING_NS_CC;
class CustomDirector: public Director {
public:
	void popSceneWithTransitionFade(float t)
	{
	    CCASSERT(_runningScene != nullptr, "running scene should not null");

	#if CC_ENABLE_GC_FOR_NATIVE_OBJECTS
	    auto sEngine = ScriptEngineManager::getInstance()->getScriptEngine();
	    if (sEngine)
	    {
	        sEngine->releaseScriptObject(this, _scenesStack.back());
	    }
	#endif // CC_ENABLE_GC_FOR_NATIVE_OBJECTS
	    _scenesStack.popBack();
	    ssize_t c = _scenesStack.size();

	    if (c == 0)
	    {
	        end();
	    }
	    else
	    {
	        _sendCleanupToScene = true;
	        _nextScene = _scenesStack.at(c - 1);
	        Scene* trans = TransitionFade::create(t, _nextScene);
	        _scenesStack.replace(c - 1, trans);
	        _nextScene = trans;
	    }
	}
};
#endif
