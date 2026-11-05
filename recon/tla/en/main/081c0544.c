// Blocked, 5 November 2026: the work copy of the mixer frame entry, as ☀️'s
// m4aSoundMain (games/tbs/src/sound/audio.c) and ⚓️'s own in
// games/tla/src/sound/sound.c. Through the sound route (old_agbcc -O2
// -mthumb-interwork, SOUND_C) this C emits the listing's 4 instructions; the
// game route differs by 3. What stops it is not the code: the call reaches
// 0x081bf81c, in the zero fill before the sound bank, which C reaches only by a
// name at that address, which O2 forbids.
#include "types.h"

void SoundMain(void);

void Func_081c0544(void) { SoundMain(); }
