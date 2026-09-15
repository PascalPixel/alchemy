#include "TYPES.H"

void Graphics_SetBg2AffineScaleHalf(void)
{
    *(u16 *)0x04000020 = 0x80;
}
