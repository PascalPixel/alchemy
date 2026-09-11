#include "types.h"
#include "scene.h"

/* graphics/registers/reset_bg2_pa.c */
void Graphics_ResetBg2Pa(void)
{
    *(volatile u16 *)0x04000020 = 0x100;
}

/* graphics/registers/set_bg2_affine_scale_half.c */
void Graphics_SetBg2AffineScaleHalf(void)
{
    *(u16 *)0x04000020 = 0x80;
}
