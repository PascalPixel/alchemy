// Draft, differ 3 of 12 (tla-en, 2026-11-08, -mtune=gba), the same in all six
// editions, from 5: the volatile modAmount store and modTarget read keep the
// read below the two zero stores, the update bit is chosen into a local and
// or-ed in through a second local, and the zero is the same local before its
// reuse. Left: push {lr} / pop {pc} where the listing ends with bx lr, all 3.
// The game compiler saves lr in every leaf with a conditional branch (its
// far-jump check sees the arg pointer live and scans before branch lengths
// are known; no game C object of tla-en returns with bx lr after a
// conditional branch), so no C on the game route closes them. old_agbcc ends
// with bx lr but takes r0 for both loads and or-s into r2 (5 by hand,
// 2026-11-08). Tried without gain (2026-11-08): all three accesses volatile
// (5: the store's latency lets movs #3 run before the read), the lfoPhase
// store volatile instead (5: the stores swap), the zero as a constant or
// flags or-ed in place (7 to 14); earlier, a once-only block after or around
// the zero stores (5), the update chosen by if/else (11).
// Blocked, 5 November 2026: a track command handler that clears the track's
// modulation and flags the pitch or volume update its target needs, as ClearModM
// (games/tla/src/sound/driver.c) with the handler's player argument. It starts on
// a halfword boundary, 2 bytes past a word, between the handwritten driver
// objects channel_unlink.s and handle_note.s, where old_agbcc and the game
// compiler both align a function to 4, so neither route can place C there.
#include "types.h"
#include "sound_driver.h"

void Func_081c075a(struct SoundPlayer *player, struct SoundTrack *track) {
  u8 update;
  u8 flags;

  // A local reused for a second value: the zero, then the update bit. The
  // volatile store and read keep the target's read after the clear.
  update = 0;
  *(volatile s8 *)&track->modAmount = update;
  track->lfoPhase = update;
  update = SOUND_TRACK_UPDATE_VOLUME;
  if (*(volatile u8 *)&track->modTarget == 0)
    update = SOUND_TRACK_UPDATE_PITCH;
  flags = track->flags;
  flags |= update;
  track->flags = flags;
}
