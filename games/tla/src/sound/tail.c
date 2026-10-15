/* TBS original: games/tbs/src/sound/tail.c */
#include "audio_engine.h"

void MusicTrack_SetTailLevel(struct SoundPlayer *player,
                             struct SoundTrack *track) {
  track->tailLevel = *track->cursor;
  track->cursor++;
}

void MusicTrack_SetTailLength(struct SoundPlayer *player,
                              struct SoundTrack *track) {
  track->tailLength = *track->cursor;
  track->cursor++;
}
