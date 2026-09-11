#include "types.h"
#include "scene.h"
#include "abi/battle/summon/get_entry_value.h"

struct Entry {
    u16 value;
    u8 rest[6];
};

extern struct Entry gRom[];

s32 Summon_GetEntryValue(s32 index)
{
    if ((u32)index > 171)
        return gRom[0].value;
    return gRom[index].value;
}
