#include "audio_engine_symbols.h"
#include "types.h"

extern u8 Data_00000008[];
extern s32 Data_080fc624[];
#define Audio_ResumePlayer Func_080fa264

void Audio_ResumePlayer(s32);

void Audio_ResumeAllPlayers(void)
{
    u16 player_count = (u32)Data_00000008;

    if (player_count != 0) {
        s32 *record = Data_080fc624;
        u32 remaining = player_count;

        do {
            Audio_ResumePlayer(*record);
            record += 3;
            remaining--;
        } while (remaining != 0);
    }
}
