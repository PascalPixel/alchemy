#include "types.h"
#include "scene.h"
#include "abi/battle/effects/misc/get_cycled_table_word.h"

extern u32 gIw;
extern u16 gRom[];

u16 BattleFx_GetCycledTableWord(void)
{
    return gRom[(gIw >> 4) & 15];
}
