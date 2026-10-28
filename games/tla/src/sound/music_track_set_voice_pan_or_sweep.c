// TBS original: games/tbs/src/sound/tail.c
#include "audio_engine.h"

void ply_xswee(struct SoundPlayer *player,
                                   struct SoundTrack *track) {
  track->voice.panOrSweep = *track->cursor;
  track->cursor++;
}
