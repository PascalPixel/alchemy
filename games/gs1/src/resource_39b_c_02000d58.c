#include "types.h"

extern u8 *Func_0200324c(s32 id);
extern u16 Func_0200317c(s32 deltaZ, s32 deltaX);
extern void Func_020032f2(s32 id, s32 angle, s32 frames);

/* Keep this object facing actor 0 while the actor remains near ground level. */
s32 Func_02000d58(u8 *object)
{
    u8 *leader = Func_0200324c(0);

    if ((*(s32 *)(leader + 16) >> 19) <= 22) {
        *(u16 *)(object + 6) = Func_0200317c(
            *(s32 *)(leader + 16) - *(s32 *)(object + 16),
            *(s32 *)(leader + 8) - *(s32 *)(object + 8));
    } else if (*(u16 *)(object + 6) != 0xc000) {
        Func_020032f2(3, 0xc000, 0);
    }
    return 0;
}
