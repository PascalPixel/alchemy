#include "AUDIO_ENGINE.H"

void MusicTrack_SetTailLevel(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->tail_level = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetTailLength(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->tail_length = *track->cursor;
    track->cursor++;
}
