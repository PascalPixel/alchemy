#include "audio_engine.h"

void MusicTrack_SetVoiceSustain(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.sustain = *track->cursor;
    track->cursor++;
}
