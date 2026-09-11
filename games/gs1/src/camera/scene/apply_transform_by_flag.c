#include "types.h"
#include "scene.h"
#include "abi/camera/scene/apply_transform_by_flag.h"
#include "global_cells.h"

s32 GameFlag_IsSet(s32);

/*
 * The call in the taken branch is a typed indirect call to the relocated
 * routine at 0x030002C0, whose argument count is not established.
 * Sys_Do names the bx rN veneer slot that reaches it, not a routine
 * at that address.
 */

extern u8 gRom[];

s32 Camera_ApplyTransformByFlag(void)
{
    u8 *state = *(u8 **)ADDR_03001E80;
    Sys_Run();
    if (GameFlag_IsSet(0x16B) != 0) {
        ((void (*)(void *))0x030002C0)(gRom);
        return Sys_Apply(state, state + 0xC);
    } else {
        return Sys_Apply2(state, state + 0xC);
    }
}
