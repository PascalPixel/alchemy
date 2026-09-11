#include "types.h"
#include "scene.h"

extern void *Runtime_GetObject(s32);

u32 Battle_GetObjectTableValue(s32 id)
{
    u32 value;
    u8 no;

    no = FIELD_AT_OFFSET(Runtime_GetObject(id), u8 *, 0x128);
    value = (u32)(Battle_Check((s32)no) << 0x18) >> 8;
    if (value == 0) {
        no = FIELD_AT_OFFSET(Runtime_GetObject(id), u8 *, 0x128);
        if (Summon_IsEntryFlagged((s32)no) != 0) {
            value = 0x180000;
        } else {
            value = 0x300000;
        }
    }
    return value;
}
