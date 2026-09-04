#include "audio_engine_symbols.h"
#include "types.h"

#define AUDIO_ENGINE_IDENT 0x68736D53

struct MusicPlayerFadeView {
    u8 unknown_00[36];
    u16 fade_interval;
    u16 fade_counter;
    u16 fade_volume;
    u8 unknown_2a[10];
    u32 ident;
};

void MusicPlayer_BeginFadeOut(
    struct MusicPlayerFadeView *player,
    u32 interval)
{
    u16 narrowed_interval = (u16)interval;

    if (player->ident != AUDIO_ENGINE_IDENT)
        return;

    player->fade_counter = narrowed_interval;
    player->fade_interval = narrowed_interval;
    player->fade_volume = 0x100;
}
