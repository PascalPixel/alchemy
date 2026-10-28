// TBS original: games/tbs/src/sound/tail.c
#include "sound_driver.h"

void ply_xdeca(struct SoundPlayer *player,
                              struct SoundTrack *track) {
  track->voice.decay = *track->cursor;
  track->cursor++;
}
