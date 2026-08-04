#include "types.h"

struct EventTable {
    s16 header[32];
    s16 entries[1][33];
};

extern struct EventTable Data_080b41ac;

s32 Func_080b2764(s32 index)
{
    return Data_080b41ac.entries[index][0];
}
