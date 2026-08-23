#include "audio_engine_symbols.h"
#include "types.h"

extern u8 Data_00000008[];
extern u8 Data_080fc624[];
void MusicPlayer_Stop(u8 *player);

void Audio_StopAllPlayers(void)
{
    u32 player_count = (u16)(u32)Data_00000008;

    if (player_count != 0) {
        u8 *record = Data_080fc624;
        u32 remaining = player_count;

        do {
            MusicPlayer_Stop(*(void **)record);
            record += 12;
            remaining--;
        } while (remaining != 0);
    }
}
