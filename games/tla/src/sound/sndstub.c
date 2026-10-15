#include "types.h"
#include "audio_engine_symbols.h"

extern u16 Data_02005830;
extern u16 Data_02005834;
extern u16 Data_0200583c;
extern u16 Data_0200580c;
extern u8 Data_02005814;
extern u8 Data_02005804;
extern s16 Data_02005820[8];

void Audio_Initialize(void);
void AudioEngine_SetFixedPcmRate(u32 mode);

void Func_081c0c18(void)
{
}

// Initializes the sound engine and sets the music state to its defaults:
// full volume and pitch, steps of 4 and the eight player volumes cleared, as
// The Broken Seal's.
void Audio_InitializeRuntimeDefaults(void)
{
    s16 *player_volume;
    s32 remaining;

    Audio_Initialize();
    Data_02005830 = 0;
    gMusicRestoreDelay = 0;
    gMusicVolumeTarget = 0x100;
    gMusicVolume = 0x100;
    gMusicVolumeStep = 4;
    Data_02005834 = 0x100;
    Data_0200583c = 0x100;
    Data_0200580c = 4;
    Data_02005814 = 0;
    Audio_CommandMask = 0;
    Data_02005804 = 0;
    player_volume = Data_02005820;
    remaining = 7;
    do
    {
        remaining--;
        *player_volume = 0;
        player_volume += 1;
    } while (remaining >= 0);
    AudioEngine_SetFixedPcmRate(0x90000);
}
