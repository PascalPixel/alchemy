#include "types.h"

#define Render_ResetTransformState Func_080049ac

void Render_ResetTransformState(void)
{
    s32 *transforms = (s32 *)0x03000350;
    s32 allocation = Func_080048f4(2, 48);

    *(s32 *)0x03001cc4 = 0;
    *(s32 *)0x03001d2c = allocation;
    transforms[0] = 0x10000;
    transforms[1] = 0;
    transforms[2] = 0;
    transforms[3] = 0;
    transforms[4] = 0x10000;
    transforms[5] = 0;
    transforms[6] = 0;
    transforms[7] = 0;
    transforms[8] = 0x10000;
    transforms[9] = 0;
    transforms[10] = 0;
    transforms[11] = 0;
}
