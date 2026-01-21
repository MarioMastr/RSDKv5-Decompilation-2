#pragma once

#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02

struct SteamAchievements : SKU::UserAchievements {
    void TryUnlockAchievement(SKU::AchievementID *id)
    {
        if (SKU::SteamUserStatsReceived && CheckAchievementsEnabled())
        {
            SteamUserStats()->SetAchievement(id->identifier);
            SteamUserStats()->StoreStats();
        }
    }
};

#endif
