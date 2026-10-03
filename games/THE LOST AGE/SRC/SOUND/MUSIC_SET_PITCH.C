#include "AUDIO_ENGINE.H"

void MusicCommand_SetPitch(s16 pitch)
{
    s32 player;
    s16 value;

    /* FAKEMATCH: retain the original player-address and pitch-value
       lifetimes; a direct call moves the player literal load after the
       signed narrowing in this 24-byte wrapper. */
    player = (s32)&gMusicPlayerBgm;
    value = pitch;
    MusicPlayer_SetPitch((struct SoundPlayer *)player, 0xff, value);
}
