#pragma once
#include "mod.hpp"
#include "../robtop/FLAlertLayer.hpp"
#include "../KittyMemory/KittyMemory.h"
#include "../KittyMemory/MemoryPatch.h"
#include <vector>
#include "offsets.hpp"

class PatchManager {
private:
    std::vector<MemoryPatch> patches;
public:
    void addPatch(const char *libraryName, uintptr_t address,std::string hex){
        patches.push_back(MemoryPatch::createWithHex(libraryName,address,hex));
    }

    void Modify(){
        for(int k = 0; k < patches.size(); k++){
            patches[k].Modify();
        }
    }

    void Restore(){
        for(int k = 0; k < patches.size(); k++){
            patches[k].Restore();
        }
    }

};

inline PatchManager g_restartPatch;

// taken from my mod template :)
namespace mod
{
    inline void patch_init()
    {
        #if GAME_VERSION < V1P7
        g_restartPatch.addPatch("libgame.so", pause_restart_button, "00 BF");
        #else
        g_restartPatch.addPatch("libcocos2dcpp.so", pause_restart_button, "00 BF");
        #endif
    }

    inline void init()
    {

        mod::module<bool>(
            id::speedhack,
            category::menu,
            "Speedhack",
            "Modifies the speed of the game.",
            false,
            true
        );

        mod::module<bool>(
            id::noclip,
            category::menu,
            "No-Clip",
            "Disables player death.",
            false,
            true
        );

        mod::module<bool>(
            id::restart_button,
            category::menu,
            "Restart Button",
            "",
            false,
            false,
            []()
            {
                if(mod::module_by_id<bool>(id::restart_button)) g_restartPatch.Modify();
                else g_restartPatch.Restore();
            }
        );

        mod::module<bool>(
            id::playback_label,
            category::menu,
            "Playback Label",
            "Shows a label when playing your macro",
            true,
            false
        );

        mod::module<float>(
            id::speedhack_val,
            category::values,
            "",
            "",
            1.0f,
            false
        );

        mod::module<bool>(
            id::update_on_steps,
            category::menu,
            "Update on Steps",
            "Runs the game at a fixed <cy>240 FPS</c>. Do <cr>not</c> turn it off if you want the most performance out of the bot.",
            true,
            false,
            []()
            {
                if(!mod::module_by_id<bool>(id::update_on_steps))
                {
                    FLAlertLayer::create(
                        nullptr,
                        "Warning!!",
                        "Turning off this option <cr>WILL</c> make macros inaccurate which ruins the experience of retro bot. \n I am only keeping this option for more freedom.",
                        "OK ...",
                        nullptr,
                        300.f
                    )->show();
                }
            }
        );

        mod::module<bool>(
            id::show_hitboxes,
            category::menu,
            "Show Hitboxes",
            "",
            false,
            true
        );

        mod::module<bool>(
            id::disable_mirror_portals,
            category::menu,
            "Disable Mirror Portals",
            "Prevents the level from flipping, this is a temporary fix to a fatal bug so turn this option on if you have issues botting a level with mirror portals.",
            true,
            true
        );

        patch_init();
    }
}