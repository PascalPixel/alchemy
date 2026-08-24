#include "audio_engine_symbols.h"
#include "types.h"

void Audio_Initialize(void);
extern u8 Data_02003000;
extern u8 Data_02003004;
extern u16 Data_02003008;
extern u16 Data_0200300c;
extern u16 Data_02003010;
extern u8 Data_02003014;
extern u16 Data_02003020;
extern u16 Data_02003030;
extern u16 Data_02003034;
extern u16 Data_02003038;
extern u8 Data_0200303c;
extern u8 Data_02003040;

void Audio_InitializeRuntimeDefaults(void)
{
    s16 *player_volume;
    s32 remaining;

    Audio_Initialize();
    Data_0200303c = 0xff;
    Data_02003000 = 0;
    Data_02003034 = 0x100;
    Data_02003008 = 0x100;
    Data_02003010 = 4;
    Data_02003030 = 0x100;
    Data_02003038 = 0x100;
    Data_0200300c = 4;
    Data_02003014 = 0;
    Data_02003040 = 0;
    player_volume = &Data_02003020;
    Data_02003004 = 0;
    remaining = 7;
    do {
        remaining--;
        *player_volume = 0;
        player_volume += 1;
    } while (remaining >= 0);
}
