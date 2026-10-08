#include "audio_engine.h"

void MusicTrack_SetVoicePanOrSweep(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.pan_or_sweep = *track->cursor;
    track->cursor++;
}
