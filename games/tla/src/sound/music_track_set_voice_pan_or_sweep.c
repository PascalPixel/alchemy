/* TBS original: games/tbs/src/sound/tail.c */
#include "audio_engine.h"

void MusicTrack_SetVoicePanOrSweep(struct SoundPlayer *player,
                                   struct SoundTrack *track) {
  track->voice.panOrSweep = *track->cursor;
  track->cursor++;
}
