// TBS original: games/tbs/src/sound/tail.c
#include "sound_driver.h"

void ply_xtype(struct SoundPlayer *player,
                             struct SoundTrack *track) {
  track->voice.kind = *track->cursor;
  track->cursor++;
}
