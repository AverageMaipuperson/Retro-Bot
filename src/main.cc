#include "features/button.cpp"
#include "features/bot.cpp"
#include "features/levelcomplete.cpp"
#include "features/savefile.cpp"
#include <jni.h>
#include "init.hpp"
#include "save.hpp"
#include "RBotLayer.hpp"

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    button_hook();
    bot_hook();
    levelcomplete_hook();
    // savefile_hook();

    mod::init();
    mod::load();
    if(mod::module_by_id<bool>(id::restart_button)) g_restartPatch.Modify();
    rbot::getModules().paused = true;
    return JNI_VERSION_1_6;
}