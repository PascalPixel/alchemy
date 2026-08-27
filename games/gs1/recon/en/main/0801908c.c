#include "types.h"

struct SubObject {
    u8 field4;
    u8 field5;
    u16 field6;
    u8 field7;
};

struct Object {
    u8 field0[5];
    u8 mode;
    u16 counter;
    u8 field8;
    struct SubObject sub;
};

extern u16 Data_080366f8[];
extern s32 Func_08003d28(s32 packed);

void Func_0801908c(struct Object *obj)
{
    struct SubObject *sub = &obj->sub;
    s32 value = 256;
    s32 counter;
    s32 idx;

    switch (obj->mode) {
    case 9:
        counter = obj->counter;
        obj->counter = (u16)(counter + 1);
        idx = counter & 31;
        value = Data_080366f8[idx];
        break;
    case 10:
        counter = obj->counter;
        obj->counter = (u16)(counter + 1);
        idx = counter & 31;
        value = Data_080366f8[idx] >> 1;
        break;
    case 11:
        counter = obj->counter;
        if (counter <= 7) {
            obj->counter = (u16)(counter + 1);
            value = Data_080366f8[8 + counter];
        }
        break;
    case 12:
        counter = obj->counter;
        if (counter <= 7) {
            obj->counter = (u16)(counter + 1);
            value = Data_080366f8[8 + counter] >> 1;
        }
        break;
    default:
        break;
    }

    if (value == 256) {
        sub->field7 &= ~0x3f;
        sub->field5 &= ~4;
    } else {
        s32 packed = value | (value << 16);
        (void)packed;
        idx = Func_08003d28(value) & 31;
        sub->field7 = (u8)((sub->field7 & ~0x3f) | (idx << 1));
        if (value > 256) {
            sub->field5 |= 3;
            sub->field6 = (obj->counter + 0xfff8) & 0xffff;
            sub->field4 = obj->field8 + 248;
            return;
        }
        sub->field5 = (sub->field5 & ~4) | 1;
    }
    sub->field6 = obj->counter;
    sub->field4 = obj->field8;
}
