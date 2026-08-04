#include "types.h"

struct BusyObject {
    u8 filler00[0x28];
    s32 busy;
};

extern void Func_020056fe(s32);

void Func_0200094c(struct BusyObject *object)
{
    s32 remaining;
    s32 busy;

    remaining = 60;
    while (remaining != 0) {
        Func_020056fe(1);
        busy = object->busy;
        remaining--;
        if (busy == 0) break;
    }
}
