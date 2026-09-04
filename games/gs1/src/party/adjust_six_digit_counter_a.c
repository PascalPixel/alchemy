#include "types.h"

#define Party_AdjustSixDigitCounterA Func_08079700

struct Work_08079700 {
    u8 unknown_00[0x10];
    s32 value;
};

s32 Party_AdjustSixDigitCounterA(s32 amount)
{
    s32 value;
    struct Work_08079700 *work;
    struct Work_08079700 *store;

    work = (struct Work_08079700 *)0x02000240;
    value = work->value;
    value = (s32)((u32)value + (u32)amount);
    store = work;
    if (value > 0xF423F) {
        value = 0xF423F;
    }
    if (value < 0) {
        value = 0;
    }
    work = store;
    work->value = value;
    return value;
}
