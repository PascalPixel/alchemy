#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "AUDIO_ENGINE.H"

extern u16 Data_02005834;
extern u16 Data_0200580c;

void Func_081c0f70(u16 first, u16 second)
{
    Data_02005834 = first;
    Data_0200580c = second;
}

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
