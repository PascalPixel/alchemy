/* Contiguous unnamed state-owner run for resource_387. */

#include "types.h"

/* Turn the object's attached presentation state by one eighth-turn. */
#define OverlayObject_TurnStateByEighth Func_02000da8
void OverlayObject_TurnStateByEighth(u8 *obj)
{
    u8 *state = *(u8 **)(obj + 80);
    s32 v = *(u16 *)(state + 30) - 0x800;

    *(u16 *)(state + 30) = v;
}

#include "types.h"

#define OverlayObject_WaitForHeight Func_02000db8

/* Wait at most sixty frames for the object to reach the requested height. */
extern void Func_02001e54(s32 cnt);
void OverlayObject_WaitForHeight(u8 *obj, s32 height)
{
    s32 cnt = 60;
    while (cnt != 0) {
        Func_02001e54(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= height)
            break;
    }
}
