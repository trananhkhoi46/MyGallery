LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)
$(call import-add-path, $(LOCAL_PATH))

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/NDKHelper/NDKCallbackNode.cpp \
../../Classes/NDKHelper/NDKHelper.cpp \
../../Classes/jansson/dump.c \
../../Classes/jansson/error.c \
../../Classes/jansson/hashtable.c \
../../Classes/jansson/load.c \
../../Classes/jansson/memory.c \
../../Classes/jansson/pack_unpack.c \
../../Classes/jansson/strbuffer.c \
../../Classes/jansson/strconv.c \
../../Classes/jansson/utf.c \
../../Classes/jansson/value.c \
../../Classes/StickerScene.cpp \
../../Classes/AlbumScene.cpp \
../../Classes/BScrollView.cpp \
../../Classes/StickerHelper.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/HomeScene.cpp \
../../Classes/SocialPlugin.cpp \
../../Classes/BaseScene.cpp \
../../Classes/SettingScene.cpp \
../../Classes/CppUtils.cpp \
../../Classes/SplashScene.cpp

LOCAL_CPPFLAGS := -DSDKBOX_ENABLED
LOCAL_LDLIBS := -landroid \
-llog
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/NDKHelper
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/jansson
LOCAL_WHOLE_STATIC_LIBRARIES := PluginFacebook \
sdkbox

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module, ./sdkbox)
$(call import-module, ./pluginfacebook)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
