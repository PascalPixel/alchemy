// Differ 5 of 12 (tla-en, 2026-11-08, -mtune=gba), from 15: the update bit is
// chosen into a local and or-ed in through a second local, and the zero is the
// same local before its reuse. Left: the game route schedules the modTarget
// load above the two zero stores and wraps the leaf in push {lr} / pop {pc}
// where the listing ends with bx lr (2 of the 5): the game compiler saves lr
// in any function with a branch. old_agbcc ends with bx lr and keeps the
// load below the stores, but takes r0 for the loads (measured by hand,
// 2026-11-08). Tried without gain (2026-11-08): a once-only block after or
// around the zero stores (5), the update chosen by if/else (11), modTarget
// read through a volatile cast (5; volatile byte stores add an ldrb each).
// Blocked, 5 November 2026: a track command handler that clears the track's
// modulation and flags the pitch or volume update its target needs, as ClearModM
// (games/tla/src/sound/driver.c) with the handler's player argument. It starts on
// a halfword boundary, 2 bytes past a word, where old_agbcc and the game compiler
// both align a function to 4, so neither route can place C there.
#include "types.h"
#include "sound_driver.h"

void Func_081c075a(struct SoundPlayer *player, struct SoundTrack *track) {
  u8 update;
  u8 flags;

  // A local reused for a second value: the zero, then the update bit.
  update = 0;
  track->modAmount = update;
  track->lfoPhase = update;
  update = SOUND_TRACK_UPDATE_VOLUME;
  if (track->modTarget == 0)
    update = SOUND_TRACK_UPDATE_PITCH;
  flags = track->flags;
  flags |= update;
  track->flags = flags;
}
