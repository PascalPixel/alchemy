#include "audio_engine_symbols.h"
#include "types.h"

#define AUDIO_ENGINE_IDENT 0x68736D53

struct MusicPlayerFadeView {
    u8 unknown_00[4];
    u32 status;
    u8 unknown_08[28];
    u16 fade_interval;
    u16 fade_counter;
    u16 fade_volume;
    u8 unknown_2a[10];
    u32 ident;
};

void MusicPlayer_FadeOutPause(
    struct MusicPlayerFadeView *player,
    u16 interval)
{
    if (player->ident != AUDIO_ENGINE_IDENT)
        return;

    player->fade_counter = interval;
    player->fade_interval = interval;
    player->fade_volume = 0x101;
}

void MusicPlayer_FadeIn(
    struct MusicPlayerFadeView *player,
    u16 interval)
{
    if (player->ident != AUDIO_ENGINE_IDENT)
        return;

    player->fade_counter = interval;
    player->fade_interval = interval;
    player->fade_volume = 2;
    player->status &= 0x7FFFFFFF;
}
