#include "AUDIO_ENGINE.H"

void MusicTrack_SetVoiceKind(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.kind = *track->cursor;
    track->cursor++;
}
