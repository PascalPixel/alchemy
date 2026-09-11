#include "types.h"
#include "scene.h"
#include "abi/battle/summon/get_entry_flag1_field.h"

struct Entry {
    u16 value;
    u8 flags0;
    u8 flags1;
    u8 rest[4];
};

extern struct Entry gRom[];

s32 Summon_GetEntryFlag1Field(s32 index)
{
    if ((u32)index > 171)
        return gRom[0].value;
    return ((u32)gRom[index].flags1 << 27) >> 28;
}
