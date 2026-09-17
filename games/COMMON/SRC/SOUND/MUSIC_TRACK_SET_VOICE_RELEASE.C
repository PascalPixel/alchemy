#include "AUDIO_ENGINE.H"

void MusicTrack_SetVoiceRelease(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.release = *track->cursor;
    track->cursor++;
}
