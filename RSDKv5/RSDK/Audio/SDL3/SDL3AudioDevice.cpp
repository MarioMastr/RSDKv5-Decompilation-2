
uint8 AudioDevice::contextInitialized;
SDL_AudioStream *AudioDevice::stream;
SDL_AudioDeviceID AudioDevice::device;
SDL_AudioSpec AudioDevice::deviceSpec;

bool32 AudioDevice::Init()
{
    SDL_InitSubSystem(SDL_INIT_AUDIO);
    if (!contextInitialized) {
        contextInitialized = true;
        InitAudioChannels();
    }

    SDL_AudioSpec want = {SDL_AUDIO_F32, AUDIO_CHANNELS, AUDIO_FREQUENCY};

    audioState = false;
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want, AudioDevice::AudioCallback, NULL);
    if (stream) {
        device = SDL_GetAudioStreamDevice(stream);
        SDL_ResumeAudioDevice(device);
        audioState = true;
    }
    else {
        PrintLog(PRINT_NORMAL, "ERROR: Unable to open audio device!");
        PrintLog(PRINT_NORMAL, "ERROR: %s", SDL_GetError());
    }

    return true;
}

void AudioDevice::Release()
{
    SDL_PauseAudioDevice(device);

    AudioDeviceBase::Release();

    SDL_CloseAudioDevice(device);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void AudioDevice::InitAudioChannels()
{
    AudioDeviceBase::InitAudioChannels();
}

void SDLCALL AudioDevice::AudioCallback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount)
{
    (void)userdata; // Unused

    if (additional_amount > 0) {
        Uint8 *data = SDL_stack_alloc(Uint8, additional_amount);
        if (data) {
            AudioDevice::ProcessAudioMixing(data, additional_amount / sizeof(SAMPLE_FORMAT));
            SDL_PutAudioStreamData(stream, data, additional_amount);
            SDL_stack_free(data);
        }
    }
}
