#ifndef _SOCIALPLUGIN_H
#define _SOCIALPLUGIN_H
#include "cocos2d.h"
#include <stdlib.h>


 class SocialPlugin {
  public :
	  SocialPlugin();
      ~SocialPlugin();

     // share facebook
     static void shareFacebook();

     // share twitter
     static void shareScreenshotTwitter_CopyThisCodeToYourClass();

     // show toast
     static void showToast(std::string messageToShow);
};

#endif
