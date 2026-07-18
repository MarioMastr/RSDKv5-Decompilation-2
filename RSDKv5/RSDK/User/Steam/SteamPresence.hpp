#if RETRO_REV02

struct SteamRichPresence : SKU::UserRichPresence {
    void SetPresence(int32 id, String *message);
};

#endif
