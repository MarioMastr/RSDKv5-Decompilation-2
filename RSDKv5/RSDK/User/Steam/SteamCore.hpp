#pragma once

#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02
void CheckDLCs();

class SteamCallbacks
{
public:
    STEAM_CALLBACK(SteamCallbacks, OnUserStatsReceived, UserStatsReceived_t);
    STEAM_CALLBACK(SteamCallbacks, GameOverlayActivated, GameOverlayActivated_t);
};

extern SteamCallbacks* SteamCallbacksInstance;
extern bool32 SteamUserStatsReceived;
extern bool32 EnabledDLC[8];

struct SteamCore : SKU::UserCore {
    void Shutdown()
    {
        SteamAPI_Shutdown();
    }
    bool32 CheckAPIInitialized()
    {
        // check if steam is running
        return SteamAPI_IsSteamRunning();
    }
    bool32 CheckFocusLost()
    {
        // return field_38;
        return focusState != 0;
    }
    void FrameInit()
    {
        UserCore::StageLoad();
        SteamAPI_RunCallbacks();
    }
    int32 GetUserLanguage()
    {
        // gets the language from steam
        return (int32)*SteamApps()->GetCurrentGameLanguage();
    }
    int32 GetUserRegion() { return REGION_US; }
    int32 GetUserPlatform() { return PLATFORM_PC; }
    bool32 GetConfirmButtonFlip() { return false; }
    void LaunchManual() {}
    void ExitGame() { RenderDevice::isRunning = false; }
    bool32 CheckDLC(uint8 id)
    {
        return SteamApps()->BIsSubscribedApp(845640);
    }
    bool32 IsOverlayEnabled(uint32 overlay)
    {
        for (int32 i = 0; i < inputDeviceCount; ++i) {
            if (inputDeviceList[i] && inputDeviceList[i]->id == overlay) {
                if (((inputDeviceList[i]->gamepadType >> 16) & 0xFF) != DEVICE_API_STEAM)
                    return false;

                return false; // not implemented, sorry!
            }
        }

        return false;
    }
    bool32 ShowExtensionOverlay(uint8 overlay)
    {
        switch (overlay)
        {
        case 0:
            SteamFriends()->ActivateGameOverlayToWebPage("https://store.steampowered.com/app/845640/Sonic_Mania__Encore_DLC/");
            return true;
        default:
            return false;
        }
    }

    bool32 initialized = false;
};

SteamCore *InitSteamCore();
#endif
