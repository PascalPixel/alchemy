#include "sound_driver.h"

void SoundDriver_Update(void);

void Func_081c0f3c(void) { SoundDriver_Update(); }

void Sound_SetBgmTempo(u16 scale) {
  m4aMPlayTempoControl(&gMusicPlayerBgm, scale);
}

void Sound_SetBgmPitch(u16 pitch) {
  m4aMPlayPitchControl(&gMusicPlayerBgm, 0xff, pitch);
}
