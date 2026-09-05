#include "types.h"

struct BusyObject {
    u8 filler00[0x28];
    s32 busy;
};

extern void Func_020056fe(s32);

void OverlayObject_WaitUntilIdle(struct BusyObject *obj)
{
    s32 cnt;
    s32 busy;

    cnt = 60;
    while (cnt != 0) {
        Func_020056fe(1);
        busy = obj->busy;
        cnt--;
        if (busy == 0) break;
    }
}
