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
extern void CheckDLCs();
extern int32 LanguageValue(const char *steamLang);

struct SteamCore : SKU::UserCore {
    bool32 initialized = false;

    SteamCore();
    void Shutdown();
    bool32 CheckAPIInitialized();
    bool32 CheckFocusLost();
    void FrameInit();
    int32 GetUserLanguage();
    int32 GetUserRegion();
    int32 GetUserPlatform();
    bool32 GetConfirmButtonFlip();
    void LaunchManual();
    void ExitGame();
    bool32 CheckDLC(uint8 id);
    bool32 IsOverlayEnabled(uint32 overlay);
    bool32 ShowExtensionOverlay(uint8 overlay);
};

SteamCore *InitSteamCore();
#endif
