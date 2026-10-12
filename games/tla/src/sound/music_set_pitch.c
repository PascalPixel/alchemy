#include "audio_engine.h"

void SoundDriver_Update(void);

void Func_081c0f3c(void)
{
    SoundDriver_Update();
}

void MusicCommand_SetPitchAndUpdateFrequency(u16 scale)
{
    MusicPlayer_SetPitchAndUpdateFrequency(&gMusicPlayerBgm, scale);
}

void MusicCommand_SetPitch(s16 pitch)
{
    s32 player;
    s32 mask;
    s16 value;

    /* FAKEMATCH: the one-pass block and local lifetimes keep the player load before signed narrowing. */
    player = (u32)&gMusicPlayerBgm;
    do {
        value = pitch;
        mask = 0xff;
        MusicPlayer_SetPitch((struct SoundPlayer *)player, mask, value);
    } while (0);
}
