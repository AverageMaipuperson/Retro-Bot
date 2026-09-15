#pragma once

#include <cocos2dcpp.h>

class PlatformToolbox {
public:
    static void activateGameCenter();
    static bool doesFileExist(std::string filename);
    static void downloadAndSavePromoImage(std::string p0, std::string p1);
    static std::string getUniqueUserID();
    static std::string getUserID();
    static bool isHD();
    static bool isLocalPlayerAuthenticated();
    static bool isNetworkAvailable();
    static bool isSignedInGooglePlay();
    static const std::string loadAndDecryptFileToString(char const* p0, char const* p1);
    static void logEvent(char const* p0);
    static void onGameLaunch();
    static void onNativePause();
    static void onNativeResume();
    static void onToggleKeyboard();
    static void openAppPage();
    static void reportAchievementWithID(char const* p0, int p1);
    static void reportLoadingFinished();
    static void saveAndEncryptStringToFile(std::string p0, char const* p1, char const p2);
    static void sendMail(char const* p0, char const* p1, char const* p2);
    static void setBlockBackButton(bool shouldBlock);
    static void setKeyboardState(bool active);
    static bool shouldResumeSound();
    static void showAchievements();
    static void signInGooglePlay();
    static void signOutGooglePlay();
    static cocos2d::CCSprite* spriteFromSavedFile(std::string spriteName);
    static void startFlurry(char const* p0);
    static void tryShowRateDialog(std::string p0);
};