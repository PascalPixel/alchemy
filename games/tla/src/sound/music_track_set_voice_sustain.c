// TBS original: games/tbs/src/sound/tail.c
#include "sound_driver.h"

void ply_xsust(struct SoundPlayer *player,
                                struct SoundTrack *track) {
  track->voice.sustain = *track->cursor;
  track->cursor++;
}
