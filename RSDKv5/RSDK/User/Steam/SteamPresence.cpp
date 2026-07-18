#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02

void SKU::SteamRichPresence::SetPresence(int32 id, String *message)
{
	char buffer[256];
	GetCString(buffer, message);

	SteamFriends()->SetRichPresence("status", buffer);
}

#endif

