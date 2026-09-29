#include "AUDIO_ENGINE.H"

void Audio_ResumePlayer(struct SoundPlayer *player)
{
    if (player->lock == SOUND_LOCK) {
        player->lock++;
        player->status &= 0x7fffffff;
        player->lock = SOUND_LOCK;
    }
}
