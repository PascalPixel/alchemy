#include "AUDIO_ENGINE.H"

void MusicCommand_SetPitch(s16 pitch)
{
    s32 player;
    s32 mask;
    s16 value;

    /* FAKEMATCH: retain the original one-pass block and local lifetimes;
       both direct-call and local-only forms move the player literal load
       after signed narrowing in this 24-byte wrapper. */
    player = (u32)&gMusicPlayerBgm;
    do {
        value = pitch;
        mask = 0xff;
        MusicPlayer_SetPitch((struct SoundPlayer *)player, mask, value);
    } while (0);
}
