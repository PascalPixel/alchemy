#include "OBJECT_DISPATCH.H"
#include "TYPES.H"
#include "SCENE.H"

s32 AnimationObjects_SelectAnimation(void *, s32);
void AnimationObjects_SetField15OnActive(void *, s32);

void Graphics_EnableObjLayerAndCallbacks(void)
{
    Scheduler_EnableCallbacks((u32)ObjectSystem_UpdateCamera);
    Scheduler_EnableCallbacks((u32)ObjectSystem_UpdateCameraFixed);
    BattleFx_ApplyColorToTargetBufferFar(0x10000, 1);
    BattleFx_StartBufferInterpolationFar(1);
    WaitFrames(1);
    *(u16 *)0x04000000 = (0xF1FF & *(u16 *)0x04000000) | 0x1000;
}
