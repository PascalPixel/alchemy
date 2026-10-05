#include "PROTO.H"
#include "AUDIO_ENGINE.H"


void MusicCommand_SetVolume(s16 volume)
{
    MusicPlayer_SetVolume(&gMusicPlayerBgm, 0xff, volume);
    gMusicVolumeTarget = volume;
    gMusicVolume = volume;
}

void Audio_SetMusicVolumeTargetAndStep(u16 primary, u16 secondary)
{
    gMusicVolumeTarget = primary;
    gMusicVolumeStep = secondary;
}

void AudioCommand_StopAllPlayers(void)
{
    Audio_StopAllPlayers();
}

void AudioCommand_ResumeAllPlayers(void)
{
    Audio_ResumeAllPlayers();
}

u8 AudioCommand_GetStateByte(void)
{
    return gMusicRestoreDelay;
}
