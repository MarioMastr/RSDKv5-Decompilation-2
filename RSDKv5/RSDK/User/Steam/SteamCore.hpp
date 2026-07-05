#if RETRO_REV02
#define DLC_COUNT 8

class SteamCallbacks
{
public:
    STEAM_CALLBACK(SteamCallbacks, OnUserStatsReceived, UserStatsReceived_t);
    STEAM_CALLBACK(SteamCallbacks, GameOverlayActivated, GameOverlayActivated_t);
};

extern SteamCallbacks* SteamCallbacksInstance;
extern bool32 SteamUserStatsReceived;
extern bool32 EnabledDLC[DLC_COUNT];

// chinese isn't supported by steam
int32 LanguageValue(const char *steamLang)
{
    if (strcmp(steamLang, "english"))
        return LANGUAGE_EN;
    else if (strcmp(steamLang, "french"))
        return LANGUAGE_FR;
    else if (strcmp(steamLang, "italian"))
        return LANGUAGE_IT;
    else if (strcmp(steamLang, "german"))
        return LANGUAGE_GE;
    else if (strcmp(steamLang, "spanish"))
        return LANGUAGE_SP;
    else if (strcmp(steamLang, "japanese"))
        return LANGUAGE_JP;
    else if (strcmp(steamLang, "korean"))
        return LANGUAGE_KO;
    // else if (strcmp(steamLang, "simp-chinese"))
        // return LANGUAGE_SC;
    // else if (strcmp(steamLang, "trad-chinese"))
        // return LANGUAGE_TC;
    else
        return 0;
}

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
        return LanguageValue(SteamApps()->GetCurrentGameLanguage());
    }
    int32 GetUserRegion() { return REGION_US; }
    int32 GetUserPlatform() { return PLATFORM_PC; }
    bool32 GetConfirmButtonFlip() { return false; }
    void LaunchManual() {}
    void ExitGame() { RenderDevice::isRunning = false; }
    bool32 CheckDLC(uint8 id)
    {
        if (id >= 0 && id <= 8)
            return EnabledDLC[id];
        return false;
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
        SteamFriends()->ActivateGameOverlayToWebPage("https://store.steampowered.com/app/845640/Sonic_Mania__Encore_DLC/");
        return true;
    }

    bool32 initialized = false;
};

SteamCore *InitSteamCore();
#endif
