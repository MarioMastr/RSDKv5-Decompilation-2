#if RETRO_REV02

struct SteamAchievements : SKU::UserAchievements {
    void TryUnlockAchievement(SKU::AchievementID *id);
};

#endif
