#include "types.h"
#include "scene.h"
#include "abi/battle/summon/is_entry_secondary_flagged.h"

struct Entry {
    u16 value;
    u8 flags0;
    u8 flags1;
    u8 rest[4];
};

extern struct Entry gRom[];

s32 Summon_IsEntrySecondaryFlagged(s32 index)
{
    if ((u32)index > 171)
        return 0;
    return ((u32)gRom[index].flags1 << 31) >> 31;
}
