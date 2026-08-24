#include "types.h"



/* Wait at most sixty frames for the object to reach the requested height. */
extern void Func_02000b98(s32 frames);
void Func_020004d4(u8 *object, s32 height)
{
    s32 frames = 60;

    while (frames != 0) {
        Func_02000b98(1);
        frames--;
        if (*(s32 *)(object + 12) <= height)
            break;
    }
}
