#include "types.h"

extern void Func_02004150(s32);
extern u8 *Func_020041b6(s32);
extern void Func_02004108(s32, s32, s32, s32, s32, s32);

void SceneState_SetFlag331AndConfigureRegion46_17(void)
{
    u8 *p;

    Func_02004150(0x331);
    p = Func_020041b6(20) + 85;
    *p = 0;
    {
        s32 p5 = 44;
        s32 p6 = 17;

        Func_02004108(46, 17, 1, 1, p5, p6);
    }
}
