// TBS original: games/tbs/src/sound/tail.c
#include "audio_engine.h"

void ply_xleng(struct SoundPlayer *player,
                               struct SoundTrack *track) {
  track->voice.length = *track->cursor;
  track->cursor++;
}
