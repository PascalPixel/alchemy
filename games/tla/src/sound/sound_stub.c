#include "types.h"
#include "audio_engine_symbols.h"

extern u16 gData_02005830;
extern u16 gData_02005834;
extern u16 gData_0200583c;
extern u16 gData_0200580c;
extern u8 gData_02005814;
extern u8 gData_02005804;
extern s16 gData_02005820[8];

void m4aSoundInit(void);
void Sound_SetFixedSampleFreq(u32 mode);

void Func_081c0c18(void) {}

/// Initializes the sound driver and sets the music state to its defaults:
/// full volume and pitch, steps of 4 and the eight player volumes cleared.
// As The Broken Seal's.
void Sound_Initialize(void) {
  s16 *playerVolume;
  s32 remaining;

  m4aSoundInit();
  gData_02005830 = 0;
  gMusicRestoreDelay = 0;
  gMusicVolumeTarget = 0x100;
  gMusicVolume = 0x100;
  gMusicVolumeStep = 4;
  gData_02005834 = 0x100;
  gData_0200583c = 0x100;
  gData_0200580c = 4;
  gData_02005814 = 0;
  gSoundCommandMask = 0;
  gData_02005804 = 0;
  playerVolume = gData_02005820;
  remaining = 7;
  do {
    remaining--;
    *playerVolume = 0;
    playerVolume += 1;
  } while (remaining >= 0);
  Sound_SetFixedSampleFreq(0x90000);
}
