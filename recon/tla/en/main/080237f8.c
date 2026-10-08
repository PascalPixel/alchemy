#include "object_dispatch.h"
#include "types.h"
#include "scene.h"

s32 AnimationObjects_SelectAnimation(void *, s32);
void AnimationObjects_SetActiveTimerSteps(void *, s32);

void Graphics_EnableObjLayerAndCallbacks(void)
{
    Scheduler_EnableCallbacks((u32)ObjectSystem_UpdateCamera);
    Scheduler_EnableCallbacks((u32)ObjectSystem_UpdateCameraFixed);
    BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
    BattleFx_StartBufferInterpolationFar(1);
    WaitFrames(1);
    *(u16 *)0x04000000 = (0xF1FF & *(u16 *)0x04000000) | 0x1000;
}
