#include "types.h"
#include "scene.h"
#include "abi/battle/summon/get_entry_byte4.h"

extern u8 gRom[];

s32 Summon_GetEntryByte4(s32 index)
{
    if ((u32)index > 171)
        return 0;
    return gRom[index * 8 + 4];
}
