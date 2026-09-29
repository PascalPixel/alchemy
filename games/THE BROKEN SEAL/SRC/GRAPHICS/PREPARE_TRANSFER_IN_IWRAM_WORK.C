#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "TRANSFORM.H"
s32 Trig_Sin(s32);
s32 Trig_Cos(s32);
extern u8 Data_03001ce0[];
void Graphics_PrepareTransfer(void *src, void *dst, void *work);

/* graphics/prepare_transfer_in_iwram_work.c */
/* graphics/prepare_transfer_in_iwram_work.c */
void Graphics_PrepareTransferInIwramWork(s32 src, s32 dst)
{
    Graphics_PrepareTransfer((void *)src, (void *)dst, gTransform);
}

/* graphics/prepare_transfer_and_run.c */
typedef void (*WorkFunc)(void *);

void Graphics_PrepareTransferAndRun(void *src, void *dst)
{
    u8 work[48];

    Graphics_PrepareTransfer(src, dst, work);
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
    first = Trig_Sin(half);
    result = ((CameraWorkFn)0x0300013C)(
        first,
        Trig_Cos(half)* 0x50
    );
    ((struct CameraWork *)((u32)&Data_03001ce0))->param1 = param1;
    ((struct CameraWork *)((u32)&Data_03001ce0))->result = result;
    ((struct CameraWork *)((u32)&Data_03001ce0))->param2 = param2;
}

/* camera/scene/store_parameters.c */
void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2)
{
    u32 *work = (u32 *)((u32)&Data_03001ce0);

    work[0] = value0;
    work[1] = value1;
    work[2] = value2;
}
