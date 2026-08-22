#include "types.h"

struct Entry_08003f3c {
    u16 value;
    u16 flags;
};

extern struct Entry_08003f3c Data_03001b10[];
s32 Func_08003f04(s32 arg0);

s32 Func_08003f3c(u32 index)
{
    struct Entry_08003f3c *entry = &Data_03001b10[index];

    if (index > 95) {
        return -1;
    }
    if (entry->flags != 0xffff) {
        Func_08003f04(index);
        entry->flags |= 0xffff;
        entry->value = 0;
    }
    return 0;
}
