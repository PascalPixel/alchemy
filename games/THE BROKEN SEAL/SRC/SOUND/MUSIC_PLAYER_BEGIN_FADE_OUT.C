#include "AUDIO_ENGINE.H"

void MusicPlayer_BeginFadeOut(struct SoundPlayer *player, u16 speed)
{
    if (player->lock == SOUND_LOCK) {
        player->lock++;
        player->fade_counter = speed;
        player->fade_period = speed;
        player->fade_volume = 0x100;
        player->lock = SOUND_LOCK;
    }
}
