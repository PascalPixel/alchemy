#include "types.h"

struct Slot {
    s32 tag;
    s32 padding04_field04;
    u8 padding[0];
};

extern void *Data_03001e8c;
extern u8 Data_00000000[];

struct Entry {
    s32 owner;
    s16 field04;
    s16 field06;
    s16 field08[4];
    s16 field10;
    s16 field12;
    s16 field14;
    s16 field16;
    s16 field18;
    s16 field1a;
    s16 field1c;
    s16 field1e;
    s16 field20;
    s16 field22;
    s16 field24;
    s16 field26;
    s16 field28;
    s16 field2a;
    s16 field2c;
    s16 field2e;
};

void Func_080165d8(s32 owner, s32 arg1, s32 x, s32 y, u16 *copy_source, s32 arg5)
{
    struct Entry *base;
    struct Entry *entry;
    struct Entry *found;
    s32 i;

    base = (struct Entry *)((u8 *)Data_03001e8c + 1568);
    entry = base;
    found = 0;
    i = 0;
    while (entry->owner != 0) {
        i++;
        entry = (struct Entry *)((u8 *)entry + 40);
        if (i == 3)
            break;
    }
    if (entry->owner == 0)
        found = entry;

    if (found != 0) {
        found->field1e = (s16)(x << 8);
        found->field04 = (s16)(x << 8);
        found->field06 = (s16)(y << 8);
        found->field12 = (s16)arg1;
        found->field16 = 15;
        found->field1a = 10;
        found->owner = owner;
        found->field14 = 0;
        found->field18 = 0;
        found->field20 = 0;
        found->field24 = (s16)arg5;
        if (copy_source != 0) {
            for (i = 0; i < 4; i++)
                found->field08[i] = copy_source[i];
        } else {
            for (i = 0; i < 4; i++)
                found->field08[i] = (u16)(u32)Data_00000000;
        }
        found->field10 = 0;
    }
}
