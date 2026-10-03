#include "AUDIO_ENGINE.H"

void MusicCommand_SetPitch(s16 pitch)
{
    MusicPlayer_SetPitch(&gMusicPlayerBgm, 0xff, pitch);
}
