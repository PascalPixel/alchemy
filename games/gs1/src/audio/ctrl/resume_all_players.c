#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"
#include "abi/audio/ctrl/resume_all_players.h"

extern u8 gVal[];
extern s32 gRom[];

void Audio_ResumePlayer(s32);

void Audio_ResumeAllPlayers(void)
{
    u16 player_count = (u32)gVal;

    if (player_count != 0) {
        s32 *record = gRom;
        u32 remaining = player_count;

        do {
            Audio_ResumePlayer(*record);
            record += 3;
            remaining--;
        } while (remaining != 0);
    }
}
