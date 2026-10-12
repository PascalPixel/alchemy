#include "types.h"
#include "iwram_call.h"

struct CameraSceneParameters {
    s32 result;
    s32 param1;
    s32 param2;
};

extern struct CameraSceneParameters gCameraSceneParameters;
s32 Trig_Sin(s32);

void Camera_SetAngleParameters(s32 value, s32 param1, s32 param2)
{
    s32 half;
    s32 first;
    s32 result;

    half = value / 2;
    first = Trig_Sin(half);
    result = Iwram_RatioMulQ14(first, Trig_Sin(half + 0x4000) * 0x50);
    gCameraSceneParameters.param1 = param1;
    gCameraSceneParameters.result = result;
    gCameraSceneParameters.param2 = param2;
}
