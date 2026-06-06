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
