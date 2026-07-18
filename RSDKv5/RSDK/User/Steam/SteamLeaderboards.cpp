#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02

void SKU::SteamLeaderboards::FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser) {}
void SKU::SteamLeaderboards::LoadLeaderboards() {}
void SKU::SteamLeaderboards::TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank)) {}
int32 SKU::SteamLeaderboards::GetStatus() { return status; }

#endif
