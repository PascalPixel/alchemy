#include "AUDIO_ENGINE.H"

void Audio_StopAllPlayers(void);
void Audio_ResumeAllPlayers(void);

void MusicCommand_SetVolume(s16 volume)
{
    MusicPlayer_SetVolume(&gMusicPlayerBgm, 0xff, (u16)volume);
    gMusicVolumeTarget = volume;
    gMusicVolume = volume;
}

void Audio_SetWorkPairA(u16 primary, u16 secondary)
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
