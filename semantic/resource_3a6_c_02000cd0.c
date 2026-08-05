#include "types.h"



/* Give the actor at most sixty frames to descend to its target height, then
 * clamp the live height to the target so the following scene starts exact. */
extern void Func_020028fa(s32 frames);
void Func_02000cd0(u8 *object)
{
    s32 frames = 60;

    while (frames != 0) {
        Func_020028fa(1);
        frames--;
        if (*(s32 *)(object + 12) <= *(s32 *)(object + 20))
            break;
    }
    *(s32 *)(object + 12) = *(s32 *)(object + 20);
}
