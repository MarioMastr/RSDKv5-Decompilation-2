#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02

SKU::SteamCallbacks *SteamCallbacksInstance = nullptr;
bool32 SteamUserStatsReceived               = false;
bool32 EnabledDLC[DLC_COUNT];

void SKU::SteamCallbacks::OnUserStatsReceived(UserStatsReceived_t *pCallback)
{
    SteamUserStatsReceived = pCallback->m_eResult == k_EResultOK;
    PrintLog(PRINT_NORMAL, "Logged into Steam account %lld", pCallback->m_steamIDUser.GetAccountID());
}

void SKU::SteamCallbacks::GameOverlayActivated(GameOverlayActivated_t *pCallback)
{
    // Check for DLC when the overlay closes
    if (!pCallback->m_bActive)
        CheckDLCs();
}

void CheckDLCs()
{
    AppId_t gameID;
#if RETRO_REV0U
    switch (engine.version) {
        default:
        case 5: gameID = 845640; break;
        case 4:
        case 3: gameID = 2343200; break;
    }
#else
    gameID = 845640;
#endif
    for (int i = 0; i < DLC_COUNT; ++i) EnabledDLC[i] = SteamApps()->BIsSubscribedApp(gameID);
}

// chinese isn't supported by steam
int32 LanguageValue(const char *steamLang)
{
    if (!strcmp(steamLang, "english"))
        return LANGUAGE_EN;
    else if (!strcmp(steamLang, "french"))
        return LANGUAGE_FR;
    else if (!strcmp(steamLang, "italian"))
        return LANGUAGE_IT;
    else if (!strcmp(steamLang, "german"))
        return LANGUAGE_GE;
    else if (!strcmp(steamLang, "spanish"))
        return LANGUAGE_SP;
    else if (!strcmp(steamLang, "japanese"))
        return LANGUAGE_JP;
    else if (!strcmp(steamLang, "korean"))
        return LANGUAGE_KO;
    // else if (!strcmp(steamLang, "simp-chinese"))
    // return LANGUAGE_SC;
    // else if (!strcmp(steamLang, "trad-chinese"))
    // return LANGUAGE_TC;
    else
        return 0;
}

SKU::SteamCore::SteamCore() { CheckDLCs(); }

void SKU::SteamCore::Shutdown() { SteamAPI_Shutdown(); }

bool32 SKU::SteamCore::CheckAPIInitialized()
{
    // check if steam is running
    return SteamAPI_IsSteamRunning();
}

bool32 SKU::SteamCore::CheckFocusLost()
{
    // return field_38;
    return focusState != 0;
}

void SKU::SteamCore::FrameInit()
{
    UserCore::StageLoad();
    SteamAPI_RunCallbacks();
}

int32 SKU::SteamCore::GetUserLanguage()
{
    // gets the language from steam
    return LanguageValue(SteamApps()->GetCurrentGameLanguage());
}
int32 SKU::SteamCore::GetUserRegion() { return REGION_US; }

int32 SKU::SteamCore::GetUserPlatform() { return PLATFORM_PC; }

bool32 SKU::SteamCore::GetConfirmButtonFlip() { return false; }

void SKU::SteamCore::LaunchManual() { SteamFriends()->ActivateGameOverlayToWebPage("http://www.sonicthehedgehog.com/mania/manual"); }

void SKU::SteamCore::ExitGame() { RenderDevice::isRunning = false; }

bool32 SKU::SteamCore::CheckDLC(uint8 id)
{
    if (id >= 0 && id <= 8)
        return EnabledDLC[id];
    return false;
}

bool32 SKU::SteamCore::IsOverlayEnabled(uint32 overlay)
{
    for (int32 i = 0; i < inputDeviceCount; ++i) {
        if (inputDeviceList[i] && inputDeviceList[i]->id == overlay) {
            if (((inputDeviceList[i]->gamepadType >> 16) & 0xFF) != DEVICE_API_STEAM)
                return false;

            return SteamUtils()->IsOverlayEnabled();
        }
    }

    return false;
}

bool32 SKU::SteamCore::ShowExtensionOverlay(uint8 overlay)
{
    SteamFriends()->ActivateGameOverlayToWebPage("https://store.steampowered.com/app/845640/Sonic_Mania__Encore_DLC/");
    return true;
}

SKU::SteamCore *InitSteamCore()
{
    using namespace SKU;

    char rootDir[0x80];
    char pathBuffer[0x80];

    sprintf(rootDir, "%s", SKU::userFileDir);
    sprintf(pathBuffer, "%s%s", rootDir, "steam_appid.txt");

    if (fOpen(pathBuffer, "r") == NULL) {
        FileIO *f;
        if ((f = fOpen(pathBuffer, "w")) == NULL) {
            PrintLog(PRINT_ERROR, "ERROR: Couldn't open file '%s' for writing!", "steam_appid.txt");
            return nullptr;
        }

        const char *gameID;

#if RETRO_REV0U
        switch (engine.version) {
            default:
            case 5: gameID = "584400\n"; break;
            case 4:
            case 3: gameID = "1794960\n"; break;
        }
#else
        gameID = "584400\n";
#endif

        fWrite(gameID, 1, strlen(gameID), f);
        fClose(f);
    }

    SteamErrMsg errMsg;
    if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK) {
        PrintLog(PRINT_ERROR, "Failed to initialise the Steam API: %s", errMsg);
        return nullptr;
    }
    else {
        SKU::SteamCallbacksInstance = new SteamCallbacks();
        if (!SteamUserStats()->RequestUserStats(SteamUser()->GetSteamID()))
            PrintLog(PRINT_NORMAL, "Failed to request current stats from Steam.");
    }

    // Initalize API subsystems
    SteamCore *core = new SteamCore;

    if (achievements)
        delete achievements;
    achievements = new SteamAchievements;

    if (leaderboards)
        delete leaderboards;
    leaderboards = new SteamLeaderboards;

    if (richPresence)
        delete richPresence;
    richPresence = new SteamRichPresence;

    if (stats)
        delete stats;
    stats = new SteamStats;

    if (userStorage)
        delete userStorage;
    userStorage = new SteamUserStorage;

    core->initialized = true;

    return core;
}
#endif
