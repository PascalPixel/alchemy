#include "types.h"
#include "scene.h"
#include "global_cells.h"
void Sys_Place(void *src, void *dst, void *work);

/* graphics/prepare_transfer_in_iwram_work.c */
/* graphics/prepare_transfer_in_iwram_work.c */
void Graphics_PrepareTransferInIwramWork(s32 src, s32 dst)
{
    Sys_Place((void *)src, (void *)dst, (void *)0x03000350);
}

/* graphics/prepare_transfer_and_run.c */
typedef void (*WorkFunc)(void *);

void Graphics_PrepareTransferAndRun(void *src, void *dst)
{
    u8 work[48];

    Sys_Place(src, dst, work);
    ((WorkFunc)0x030002c0)(work);
}

/* camera/scene/set_angle_parameters.c */
/* camera/scene/set_angle_parameters.c */
typedef s32 (*CameraWorkFn)(s32, s32);

struct CameraWork {
    s32 result;
    s32 param1;
    s32 param2;
};

void Camera_SetAngleParameters(u32 value, s32 param1, s32 param2)
{
    s32 half;
    s32 first;
    s32 result;

    half = (s32)(value + (value >> 31)) >> 1;
    first = FixedSin(half);
    result = ((CameraWorkFn)0x0300013C)(
        first,
        FixedCos(half)* 0x50
    );
    ((struct CameraWork *)ADDR_03001CE0)->param1 = param1;
    ((struct CameraWork *)ADDR_03001CE0)->result = result;
    ((struct CameraWork *)ADDR_03001CE0)->param2 = param2;
}

/* camera/scene/store_parameters.c */
void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2)
{
    u32 *work = (u32 *)ADDR_03001CE0;

    work[0] = value0;
    work[1] = value1;
    work[2] = value2;
}
