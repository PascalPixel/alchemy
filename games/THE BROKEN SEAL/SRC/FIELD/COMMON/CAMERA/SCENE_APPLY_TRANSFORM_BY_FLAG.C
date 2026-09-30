#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
extern u8 gCameraWork[];
void Render_ResetTransformState(void);
s32 Graphics_PrepareTransferAndRun(void *, void *);
s32 Graphics_PrepareTransferInIwramWork(void *, void *);

s32 GameFlag_TestFar(s32);

extern u8 Camera_FlagTransformWork[];

s32 Camera_ApplyTransformByFlag(void)
{
    u8 *state = *(u8 **)((u32)&gCameraWork);
    Render_ResetTransformState();
    if (GameFlag_TestFar(0x16B) != 0) {
        Iwram_TransformMatrix((s32 *)Camera_FlagTransformWork);
        return Graphics_PrepareTransferAndRun(state, state + 0xC);
    } else {
        return Graphics_PrepareTransferInIwramWork(state, state + 0xC);
    }
}
