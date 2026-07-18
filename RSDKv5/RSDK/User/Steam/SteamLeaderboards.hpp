#if RETRO_REV02

struct SteamLeaderboards : UserLeaderboards {
    void FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser);
    void LoadLeaderboards();
    void TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank));
    int32 GetStatus();
};

#endif
