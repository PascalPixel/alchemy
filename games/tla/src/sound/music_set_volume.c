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

void Sound_SetBgmVolume(s16 volume) {
  m4aMPlayVolumeControl(&gMusicPlayerBgm, 0xff, volume);
  gMusicVolumeTarget = volume;
  gMusicVolume = volume;
}

void Sound_SetBgmVolumeTarget(u16 primary, u16 secondary) {
  gMusicVolumeTarget = primary;
  gMusicVolumeStep = secondary;
}

void Sound_StopAllMusicPlayers(void) { m4aMPlayAllStop(); }

void Sound_ResumeAllMusicPlayers(void) { m4aMPlayAllContinue(); }

u8 Sound_GetBgmRestoreDelay(void) { return gMusicRestoreDelay; }
