#include "features/button.cpp"
#include "features/bot.cpp"
#include <jni.h>
#include "init.hpp"
#include "save.hpp"

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    button_hook();
    bot_hook();
    mod::init();
    mod::load();
    if(mod::module_by_id<bool>(id::restart_button)) g_restartPatch.Modify();
    return JNI_VERSION_1_6;
}