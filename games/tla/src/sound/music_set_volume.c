#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "audio_engine.h"

extern u16 gData_02005834;
extern u16 gData_0200580c;

void Func_081c0f70(u16 first, u16 second) {
  gData_02005834 = first;
  gData_0200580c = second;
}

void MusicCommand_SetVolume(s16 volume) {
  MusicPlayer_SetVolume(&gMusicPlayerBgm, 0xff, volume);
  gMusicVolumeTarget = volume;
  gMusicVolume = volume;
}

void Audio_SetMusicVolumeTargetAndStep(u16 primary, u16 secondary) {
  gMusicVolumeTarget = primary;
  gMusicVolumeStep = secondary;
}

void AudioCommand_StopAllPlayers(void) { Audio_StopAllPlayers(); }

void AudioCommand_ResumeAllPlayers(void) { Audio_ResumeAllPlayers(); }

u8 AudioCommand_GetStateByte(void) { return gMusicRestoreDelay; }
