#include "audio_engine.h"

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
