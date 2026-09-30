#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
s32 Trig_Sin(s32);
s32 Trig_Cos(s32);
extern u8 gProjection[];
void Graphics_PrepareTransfer(void *src, void *dst, void *work);

/* graphics/prepare_transfer_in_iwram_work.c */
/* graphics/prepare_transfer_in_iwram_work.c */

void Camera_SetAngleParameters(u32 value, s32 param1, s32 param2)
{
    s32 half;
    s32 first;
    s32 result;

    half = (s32)(value + (value >> 31)) >> 1;
    first = Trig_Sin(half);
    result = Iwram_RatioMulQ14(
        first,
        Trig_Cos(half)* 0x50
    );
    ((struct CameraWork *)((u32)&gProjection))->param1 = param1;
    ((struct CameraWork *)((u32)&gProjection))->result = result;
    ((struct CameraWork *)((u32)&gProjection))->param2 = param2;
}
