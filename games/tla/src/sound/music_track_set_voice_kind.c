// TBS original: games/tbs/src/sound/tail.c
#include "audio_engine.h"

void ply_xtype(struct SoundPlayer *player,
                             struct SoundTrack *track) {
  track->voice.kind = *track->cursor;
  track->cursor++;
}
