/* Draft, differ 2: the listing loads r1 (37) before r0 (15) for the third Func_02000b24 call. */
#include "../../../../games/tla/src/field/overlay_67c/local.h"

void Func_02001170(void)
{
    s32 i;
    s32 result;

    if (GameFlag_Test(0x24b)) {
        Func_02000b24(12, 38);
        Func_02000b24(13, 39);
        Func_02000b24(15, 37);
        Func_02003628();
        Func_02003688(38, 2, 19, 1);
        for (i = 8; i >= 0; i--)
            result = Func_020036b8(33, 55);
        GameFlag_SetBit(0x24a);
        if (result == 1)
            GameFlag_SetBit(0xa65);
    }
}
