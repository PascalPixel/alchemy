#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

s32 GameFlag_TestFar(s32);

/*
 * The call in the taken branch is a typed indirect call to the relocated
 * routine at 0x030002C0, whose argument count is not established.
 * Sys_Do names the bx rN veneer slot that reaches it, not a routine
 * at that address.
 */

extern u8 Camera_FlagTransformWork[];

s32 Camera_ApplyTransformByFlag(void)
{
    u8 *state = *(u8 **)ADDR_03001E80;
    Render_ResetTransformState();
    if (GameFlag_TestFar(0x16B) != 0) {
        ((void (*)(void *))0x030002C0)(Camera_FlagTransformWork);
        return Graphics_PrepareTransferAndRun(state, state + 0xC);
    } else {
        return Graphics_PrepareTransferInIwramWork(state, state + 0xC);
    }
}
