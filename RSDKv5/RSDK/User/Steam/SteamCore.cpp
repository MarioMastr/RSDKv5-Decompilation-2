#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02

SKU::SteamCallbacks* SteamCallbacksInstance = nullptr;
bool32 SteamUserStatsReceived = false;
bool32 EnabledDLC[DLC_COUNT];

void CheckDLCs()
{
    AppId_t gameID;
#if RETRO_REV0U
    switch (engine.version) {
        default:
        case 5:
            gameID = 845640;
            break;
        case 4:
        case 3:
            gameID = 2343200;
            break;
    }
#else
    gameID = 845640;
#endif
    for (int i = 0; i < DLC_COUNT; ++i)
        EnabledDLC[i] = SteamApps()->BIsSubscribedApp(gameID);
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
            case 5:
                gameID = "584400\n";
                break;
            case 4:
            case 3:
                gameID = "1794960\n";
                break;
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
    } else {
        SKU::SteamCallbacksInstance = new SteamCallbacks();
        if (!SteamUserStats()->RequestUserStats(SteamUser()->GetSteamID()))
            PrintLog(PRINT_NORMAL, "Failed to request current stats from Steam.");
    }

    CheckDLCs();

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
