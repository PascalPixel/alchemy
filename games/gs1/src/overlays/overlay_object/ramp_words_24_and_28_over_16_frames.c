#include "types.h"

#define OverlayObject_RampWords24And28Over16Frames Func_02001c60

s32 OverlayObject_RampWords24And28Over16Frames(u8 *obj) {
    u16 *cnt = (u16 *)(obj + 100);
    s16 n;

    *cnt = *cnt + 1;
    n = (s16)*cnt;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(obj + 24) = (n * 3) << 10;
    *(s32 *)(obj + 28) = (n * 3) << 10;
    return 1;
}
