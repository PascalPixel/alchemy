#include "types.h"

extern void Func_080000c0(s32 frames);

/* Wait at most sixty frames for the object to reach the requested height. */
void Func_020004d4(u8 *object, s32 height)
{
    s32 frames = 60;

    while (frames != 0) {
        Func_080000c0(1);
        frames--;
        if (*(s32 *)(object + 12) <= height)
            break;
    }
}
