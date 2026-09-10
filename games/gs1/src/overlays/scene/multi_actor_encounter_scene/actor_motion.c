#include "types.h"

#define SceneActor_OscillateHeightBetweenLimits Func_02000514
#define SceneActor_SetWord28RandomlyOneIn40 Func_0200089c
s32 Func_02004792(void);
s32 Func_020047b0(void);
s32 Func_02004b0e(void);

s32 SceneActor_OscillateHeightBetweenLimits(u8 *obj)
{
    s16 *flag = (s16 *)(obj + 0x66);
    s32 val;
    s32 tmp;

    if (*flag != 0) {
        val = *(s32 *)(obj + 0xc) - (((u32)(Func_02004792() << 15)) >> 16) - 0x8000;
        *(s32 *)(obj + 0xc) = val;
        if (val >= 0x40000)
            goto done;
        tmp = 0;
    } else {
        val = *(s32 *)(obj + 0xc) + (((u32)(Func_020047b0() << 15)) >> 16) + 0x8000;
        *(s32 *)(obj + 0xc) = val;
        if (val <= 0xC0000)
            goto done;
        tmp = 1;
    }
    *flag = tmp;
done:
    return 1;
}

s32 SceneActor_SetWord28RandomlyOneIn40(u8 *obj)
{
    if ((((u32)(Func_02004b0e() * 40)) >> 16) == 0)
        *(s32 *)(obj + 0x28) = 0x40000;
    return 1;
}
