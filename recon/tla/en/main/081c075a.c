// Blocked, 5 November 2026: a track command handler that clears the track's
// modulation and flags the pitch or volume update its target needs, as ClearModM
// (games/tla/src/sound/driver.c) with the handler's player argument. It starts on
// a halfword boundary, 2 bytes past a word, where old_agbcc and the game compiler
// both align a function to 4, so neither route can place C there. Through the
// sound route this C gives 14 instructions to the listing's 12; game route 15 of
// 12.
#include "types.h"
#include "sound_driver.h"

void Func_081c075a(struct SoundPlayer *player, struct SoundTrack *track) {
  track->modAmount = 0;
  track->lfoPhase = 0;
  if (track->modTarget == 0)
    track->flags |= SOUND_TRACK_UPDATE_PITCH;
  else
    track->flags |= SOUND_TRACK_UPDATE_VOLUME;
}
