#include "types.h"

struct Work {
    u8 padding00[0x14];
    u16 state;
};

struct Slot {
    struct Work *work;
    u16 field04;
    u16 field06;
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
void Func_080167d8(void *);

struct Slot *Func_08016670(struct Work *work, s32 value, s32 preserve)
{
    struct Slot *slot;
    struct Slot *selected;
    u16 *destination;
    u16 field04Value;
    u16 zero;
    u32 index;

    slot = (struct Slot *)(Data_03001e8c + 0x620);
    selected = 0;
    for (index = 0; index != 3; slot++, index++) {
        if (slot->work == 0 || slot->work->state != 0) {
            selected = slot;
            break;
        }
    }

    if (selected != 0) {
        if (selected->work == 0) {
            selected->field06 = 0xA00;
            field04Value = 0x300;
            selected->work = work;
            goto reset_field04;
        }
        if (preserve == 0) {
            if (selected->field06 == 0) {
                selected->field06 = 0xA00;
            } else if ((u32)selected->field06 < 0xD00) {
                selected->field06 += 0xD00;
            } else {
                Func_080167d8(selected);
            }
            field04Value = 0x300;
reset_field04:
            selected->field04 = field04Value;
        }

        zero = 0;
        selected->field1e = 0x300;
        selected->work->state = zero;
        selected->field16 = 15;
        selected->field1a = 10;
        selected->field12 = value;
        selected->field14 = zero;
        selected->field18 = zero;
        selected->field10 = zero;
        selected->field20 = zero;
        index = 0;
        destination = selected->values;
        do {
            index++;
            *destination++ = zero;
        } while (index <= 3);
    }
    return selected;
}
