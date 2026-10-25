#include "audio_engine.h"

void SoundDriver_Update(void);

void Func_081c0f3c(void) { SoundDriver_Update(); }

void MusicCommand_SetPitchAndUpdateFrequency(u16 scale) {
  MusicPlayer_SetPitchAndUpdateFrequency(&gMusicPlayerBgm, scale);
}

void MusicCommand_SetPitch(s16 pitch) {
  s32 value;

  /* FAKEMATCH(temporary): the pitch widened into its own variable; as
   * MusicPlayer_SetPitch(&gMusicPlayerBgm, 0xff, pitch), 2 of 8 instructions
   * differ in tla-en. */
  value = pitch;
  MusicPlayer_SetPitch(&gMusicPlayerBgm, 0xff, value);
}
