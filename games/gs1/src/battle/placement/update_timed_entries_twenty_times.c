#include "types.h"

s32 Func_080bf5a8();

s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void) {
    s32 cnt;

    cnt = 0x13;
    do {
        cnt -= 1;
        Func_080bf5a8();
    } while (cnt >= 0);
    return 0;
}
