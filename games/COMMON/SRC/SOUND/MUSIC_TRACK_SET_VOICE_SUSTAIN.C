#include "AUDIO_ENGINE.H"

void MusicTrack_SetVoiceSustain(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.sustain = *track->cursor;
    track->cursor++;
}
