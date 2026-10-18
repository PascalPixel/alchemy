// Draft, differ 15 of 12: a track command handler that clears the track's
// modulation and flags the pitch or volume update its target needs, as
// MusicTrack_ClearModulation (games/tla/src/sound/engine.c) with the
// handler's player argument. A sound-library function: the draft compiles
// with the game profile, where the build routes the library through
// old_agbcc. Other: the game compiler saves lr and keeps a flags load in each
// arm; the listing (old_agbcc's shape) picks the constant, then one or.
#include "types.h"
#include "audio_engine.h"

void Func_081c075a(struct SoundPlayer *player, struct SoundTrack *track) {
  track->modAmount = 0;
  track->lfoPhase = 0;
  if (track->modTarget == 0)
    track->flags |= SOUND_TRACK_UPDATE_PITCH;
  else
    track->flags |= SOUND_TRACK_UPDATE_VOLUME;
}
