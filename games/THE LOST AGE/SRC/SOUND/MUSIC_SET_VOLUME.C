#include "AUDIO_ENGINE.H"

void MusicCommand_SetVolume(s16 volume)
{
    MusicPlayer_SetVolume(&gMusicPlayerBgm, 0xff, (u16)volume);
    gMusicVolumeTarget = volume;
    gMusicVolume = volume;
}
