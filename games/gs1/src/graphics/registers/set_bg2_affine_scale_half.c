#include "types.h"

#define Graphics_SetBg2AffineScaleHalf Func_080ec0f0

void Graphics_SetBg2AffineScaleHalf(void)
{
    *(u16 *)0x04000020 = 0x80;
}
