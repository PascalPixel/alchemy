// TBS original: games/tbs/src/sound/tail.c
#include "audio_engine.h"

void ply_xiecv(struct SoundPlayer *player,
                             struct SoundTrack *track) {
  track->tailLevel = *track->cursor;
  track->cursor++;
}

void ply_xiecl(struct SoundPlayer *player,
                              struct SoundTrack *track) {
  track->tailLength = *track->cursor;
  track->cursor++;
}
