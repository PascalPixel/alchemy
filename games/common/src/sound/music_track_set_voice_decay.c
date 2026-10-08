#include "audio_engine.h"

void MusicTrack_SetVoiceDecay(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.decay = *track->cursor;
    track->cursor++;
}
