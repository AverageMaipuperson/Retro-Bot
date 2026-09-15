#include "rbot.hpp"
#include "tools.hpp"
#include "hooking.hpp"
#include "PlatformToolbox.hpp"

std::string (*PlatformToolbox_loadAndDecryptFileToString)(const char* name, const char* dir);
std::string PlatformToolbox_loadAndDecryptFileToString_H(const char* name, const char* dir) {
    return PlatformToolbox::loadAndDecryptFileToString(name, "/sdcard/rbot/savefiles");
}

void savefile_hook()
{
    HOOK("_ZN15PlatformToolbox26loadAndDecryptFileToStringEPKcS1_", PlatformToolbox_loadAndDecryptFileToString_H, PlatformToolbox_loadAndDecryptFileToString);
}