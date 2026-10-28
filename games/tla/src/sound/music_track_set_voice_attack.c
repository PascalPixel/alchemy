// TBS original: games/tbs/src/sound/tail.c
#include "audio_engine.h"

void ply_xatta(struct SoundPlayer *player,
                               struct SoundTrack *track) {
  track->voice.attack = *track->cursor;
  track->cursor++;
}
