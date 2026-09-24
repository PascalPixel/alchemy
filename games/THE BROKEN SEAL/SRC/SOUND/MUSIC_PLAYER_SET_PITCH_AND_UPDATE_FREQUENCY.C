#include "AUDIO_ENGINE.H"

void MusicPlayer_SetPitchAndUpdateFrequency(struct SoundPlayer *player, u16 scale)
{
    if (player->lock == SOUND_LOCK) {
        player->lock++;
        player->tempo_scale = scale;
        player->tempo_step = (player->tempo * player->tempo_scale) >> 8;
        player->lock = SOUND_LOCK;
    }
}
