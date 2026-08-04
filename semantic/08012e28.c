#include "types.h"

struct State_08012e28 {
    u8 *entries;
    u8 pad04[0x10];
    s32 value14;
};

struct Entry_08012e28 {
    u8 pad00[0x28];
    u8 *object;
};

struct Pair_08012e28 {
    s32 first;
    s32 second;
};

extern struct State_08012e28 Data_03001e60;
extern struct Pair_08012e28 Data_08013584;

void Func_0800b168(void *entry, s32 value, s32 *pair, u16 angle);

void Func_08012e28(void)
{
    struct State_08012e28 *state = &Data_03001e60;
    u8 *entry = state->entries;
    struct Entry_08012e28 *first = (struct Entry_08012e28 *)entry;
    u8 kind = first->object[4];
    struct Pair_08012e28 pair = Data_08013584;
    s32 value = state->value14;
    s32 alternate = 0;
    s32 step;
    u16 angle;
    u32 count;
    u32 index;

    switch (kind) {
    case 3:
        angle = 0;
        step = 0x2AAA;
        count = 6;
        break;
    case 4:
    case 6:
        angle = 0;
        step = 0x1999;
        count = 10;
        break;
    case 8:
    case 44:
    case 88:
        angle = 0;
        step = 0x2000;
        count = 8;
        break;
    case 20:
        angle = 0;
        step = 0;
        alternate = -0x8000;
        count = 4;
        break;
    default:
        angle = 0x2000;
        step = 0x4000;
        count = 4;
        break;
    }

    for (index = 0; index < count; index++) {
        Func_0800b168(entry, value, &pair.first, angle);
        entry += 0x38;
        angle = (s16)(angle + (u16)step);
        if (index & 1)
            angle = (s16)(angle + (u16)alternate);
        value += 0x10;
    }
}
