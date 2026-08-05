#include "types.h"



/* Wait at most sixty frames for the object to reach the requested height. */
extern void Func_02001e54(s32 frames);
void Func_02000db8(u8 *object, s32 height)
{
    s32 frames = 60;
    while (frames != 0) {
        Func_02001e54(1);
        frames--;
        if (*(s32 *)(object + 12) <= height)
            break;
    }
}
