#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02

SKU::SteamCallbacks* SteamCallbacksInstance = nullptr;
bool32 SteamUserStatsReceived = false;

bool32 CheckDLCs()
{
    AppId_t gameID;
    switch (engine.version) {
        default:
        case 5:
            gameID = 845640;
            break;
#if RETRO_REV0U
        case 4:
        case 3:
            gameID = 2343200;
            break;
#endif
    }

    return SteamApps()->BIsSubscribedApp(gameID);
}

void SKU::SteamCallbacks::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
    SteamUserStatsReceived = pCallback->m_eResult == k_EResultOK;
    PrintLog(PRINT_NORMAL, "Logged into Steam account %lld", pCallback->m_steamIDUser.GetAccountID());
}

void SKU::SteamCallbacks::GameOverlayActivated(GameOverlayActivated_t* pCallback)
{
    // Check for DLC when the overlay closes
    if (!pCallback->m_bActive)
        CheckDLCs();
}

SKU::SteamCore *InitSteamCore()
{
    using namespace SKU;

    char rootDir[0x80];
    char pathBuffer[0x80];
#if RETRO_PLATFORM == RETRO_UWP
    if (!usingCWD)
        sprintf(rootDir, "%s/", getResourcesPath());
    else
        sprintf(rootDir, "%s", "");
#elif RETRO_PLATFORM == RETRO_OSX
    sprintf(rootDir, "%s/", gamePath);
#else
    sprintf(rootDir, "%s", "");
#endif

#if RETRO_USERCORE_STEAM
    sprintf(pathBuffer, "%s%s", rootDir, "steam_appid.txt");

    if (fOpen(pathBuffer, "r") == NULL) {
        FileIO *f;
        if ((f = fOpen(pathBuffer, "w")) == NULL) { 
            PrintLog(PRINT_ERROR, "ERROR: Couldn't open file '%s' for writing!", "steam_appid.txt");
            return nullptr;
        }

        const char *gameID;

        switch (engine.version) {
            default:
            case 5:
                gameID = "584400\n";
                break;
#if RETRO_REV0U
            case 4:
            case 3:
                gameID = "1794960\n";
                break;
#endif
        }

        fWrite(gameID, 1, strlen(gameID), f);

        fClose(f);
    }
#endif

    SteamErrMsg errMsg;
    if (SteamAPI_InitEx(&errMsg) != k_ESteamAPIInitResult_OK) {
        PrintLog(PRINT_ERROR, "Failed to initialise the Steam API: %s", errMsg);
        return nullptr;
    } else {
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
