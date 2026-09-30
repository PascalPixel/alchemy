#include "AUDIO_ENGINE.H"

void MusicTrack_SetVoiceLength(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.length = *track->cursor;
    track->cursor++;
}
