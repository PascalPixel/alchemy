#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"
#include "abi/audio/ctrl/stop_all_players.h"

extern u8 gVal[];
extern u8 gRom[];
void MusicPlayer_Stop(u8 *player);

void Audio_StopAllPlayers(void)
{
    u32 player_count = (u16)(u32)gVal;

    if (player_count != 0) {
        u8 *record = gRom;
        u32 remaining = player_count;

        do {
            MusicPlayer_Stop(*(void **)record);
            record += 12;
            remaining--;
        } while (remaining != 0);
    }
}
