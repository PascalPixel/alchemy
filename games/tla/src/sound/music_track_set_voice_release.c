// TBS original: games/tbs/src/sound/tail.c
#include "audio_engine.h"

void ply_xrele(struct SoundPlayer *player,
                                struct SoundTrack *track) {
  track->voice.release = *track->cursor;
  track->cursor++;
}
