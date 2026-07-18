#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02

void SKU::SteamStats::TryTrackStat(StatInfo *stat)
{
    if (SKU::SteamUserStatsReceived && GetStatsEnabled()) {
        SteamUserStats()->SetStat(stat->name, *(int*)stat->data);
        SteamUserStats()->StoreStats();
    }
}

#endif
