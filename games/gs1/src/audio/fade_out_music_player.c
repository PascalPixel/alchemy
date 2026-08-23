#include "audio_engine_symbols.h"
#include "types.h"

s32 MusicPlayer_BeginFadeOut(s32, u16);

void MusicPlayer_FadeOut(s32 player, u16 interval)
{
    MusicPlayer_BeginFadeOut(player, interval);
}
