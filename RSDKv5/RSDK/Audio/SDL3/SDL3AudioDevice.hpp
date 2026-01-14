#define LockAudioDevice() // SDL_LockAudioDevice(AudioDevice::device)
#define UnlockAudioDevice() // SDL_UnlockAudioDevice(AudioDevice::device)

namespace RSDK
{
class AudioDevice : public AudioDeviceBase
{
public:
    static SDL_AudioStream *stream;
    static SDL_AudioDeviceID device;

    static bool32 Init();
    static void Release();

    static void FrameInit() {}

    inline static void HandleStreamLoad(ChannelInfo *channel, bool32 async)
    {
        if (async)
            SDL_CreateThread((SDL_ThreadFunction)LoadStream, "LoadStream", (void *)channel);
        else
            LoadStream(channel);
    }

private:
    static SDL_AudioSpec deviceSpec;

    static uint8 contextInitialized;

    static void InitAudioChannels();

    static void SDLCALL AudioCallback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount);
};
} // namespace RSDK