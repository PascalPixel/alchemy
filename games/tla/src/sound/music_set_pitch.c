#include "sound_driver.h"

void SoundDriver_Update(void);

void Func_081c0f3c(void) { SoundDriver_Update(); }

void Sound_SetBgmTempo(u16 scale) {
  m4aMPlayTempoControl(&gMusicPlayerBgm, scale);
}

void Sound_SetBgmPitch(s16 pitch) {
  s32 value;

  /* FAKEMATCH(temporary): the pitch widened into its own variable; as
   * m4aMPlayPitchControl(&gMusicPlayerBgm, 0xff, pitch), 2 of 8 instructions
   * differ in tla-en. */
  value = pitch;
  m4aMPlayPitchControl(&gMusicPlayerBgm, 0xff, value);
}
