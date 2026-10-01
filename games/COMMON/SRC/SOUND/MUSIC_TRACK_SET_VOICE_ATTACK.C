#include "AUDIO_ENGINE.H"

void MusicTrack_SetVoiceAttack(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.attack = *track->cursor;
    track->cursor++;
}
