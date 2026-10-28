// TBS original: games/tbs/src/sound/tail.c
#include "audio_engine.h"

void ply_xdeca(struct SoundPlayer *player,
                              struct SoundTrack *track) {
  track->voice.decay = *track->cursor;
  track->cursor++;
}
