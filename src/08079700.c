#include "types.h"

struct Work_08079700 {
    u8 unknown_00[0x10];
    s32 value;
};

s32 Func_08079700(s32 amount)
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
