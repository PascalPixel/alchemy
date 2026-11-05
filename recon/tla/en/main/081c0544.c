// Measured 2026-11-05 (tla-en): differ 3 of 4.
// Near miss, differ 3 of 4 (tla-en, 2026-10-31): only the return, the game
// profile's pop {pc} against the interworking pop {r0}; bx r0. Compiled by
// hand through the sound library's route (cpp0, then old_agbcc
// -mthumb-interwork -O2), this same C emits the listing's four instructions.
// alchemy-gcc without -mthumb-interwork pops the return address straight into
// pc (thumb_exit); only varargs or __builtin_eh_return return through bx, and
// neither gives pop {r0}; bx r0, so no reshaping reaches it on the game route.
// The work copy of the mixer frame entry, as ☀️'s m4aSoundMain
// (games/tbs/src/sound/audio.c) and ⚓️'s own in games/tla/src/sound/sound.c.
// Its call reaches a stale address before the far-call banks, which the draft
// names as the driver's SoundMain.
#include "types.h"

void SoundMain(void);

void Func_081c0544(void) { SoundMain(); }
