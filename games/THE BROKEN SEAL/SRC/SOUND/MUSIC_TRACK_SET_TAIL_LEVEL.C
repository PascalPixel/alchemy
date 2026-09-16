#include "AUDIO_ENGINE.H"

void MusicTrack_SetTailLevel(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->tail_level = *track->cursor;
    track->cursor++;
}
