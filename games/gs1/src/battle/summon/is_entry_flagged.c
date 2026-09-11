#include "types.h"
#include "scene.h"
#include "abi/battle/summon/is_entry_flagged.h"

struct Entry {
    u16 value;
    u8 flags0;
    u8 flags1;
    u8 rest[4];
};

extern struct Entry gRom[];

s32 Summon_IsEntryFlagged(s32 index)
{
    s32 result;

    if ((u32)index > 171)
        return 0;
    result = 0;
    if ((u32)gRom[index].flags0 << 31)
        result = 1;
    return result;
}
