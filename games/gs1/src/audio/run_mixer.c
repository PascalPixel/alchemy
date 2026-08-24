#include "audio_engine_symbols.h"
#include "types.h"

s32 AudioEngine_RunMixerTick(void);

void AudioEngine_RunMixer(void)
{
    AudioEngine_RunMixerTick();
}
