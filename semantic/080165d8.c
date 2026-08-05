#include "types.h"

struct Slot {
    void *entry;
    u16 x;
    u16 y;
    u16 values[4];
    u16 field10;
    u16 field12;
    u16 field14;
    u16 field16;
    u16 field18;
    u16 field1a;
    u16 field1c;
    u16 field1e;
    u16 field20;
    u16 field22;
    u16 field24;
    u16 field26;
};

extern u8 *Data_03001e8c;

struct Slot *Func_080165d8(void *entry, s32 field12, s32 x, s32 y,
                           const u16 *values, s32 field24) {
    struct Slot *slot;
    struct Slot *selected;
    u16 *destination;
    void *savedEntry;
    s32 savedY;
    s32 savedField12;
    const u16 *source;
    u32 index;

    savedEntry = entry;
    savedY = y;
    savedField12 = field12;
    source = values;
    slot = (struct Slot *)(Data_03001e8c + 0x620);
    selected = 0;
    for (index = 0; index != 3; index++, slot++) {
        if (slot->entry == 0) {
            selected = slot;
            break;
        }
    }

    if (selected != 0) {
        selected->field1e = x << 8;
        selected->x = x << 8;
        selected->y = savedY << 8;
        selected->field12 = savedField12;
        selected->field16 = 15;
        selected->field1a = 10;
        selected->entry = savedEntry;
        selected->field14 = 0;
        selected->field18 = 0;
        selected->field20 = 0;
        selected->field24 = field24;
        if (source != 0) {
            destination = selected->values;
            index = 0;
            do {
                *destination++ = *source++;
                index++;
            } while (index <= 3);
        } else {
            destination = selected->values;
            index = 0;
            do {
                *destination++ = 0;
                index++;
            } while (index <= 3);
        }
        selected->field10 = 0;
    }
    return selected;
}
