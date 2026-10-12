/* Near miss of Func_080e9db4 (first written 2026-10-12): differ 13 of 90, two places. The native enters the first
 * wait loop (60 frames) with a branch to its height test at the bottom (b; body; cmp r6, #59; bgt; ldr; ldr; cmp;
 * bgt), where this for loop copies the test in front as the two 20-frame loops do in both; measured against it: the
 * bound joined to the test with &&, a block-scoped y, do-while and while forms (13, 13, 19, 19) and a compound
 * break condition. At the end the native finishes the gravity store before it finishes the work offset 0x102c
 * (movs r2; str; movs r3; lsls r3; str; lsls r2; adds r2), the draft finishes the offset first. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "callback_scheduler.h"

s32 EventRuntime_GetControlledOwner(void);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void WaitFrames(s32 frames);
s32 Resource_ResetEntry(u32 entry);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_080e9940(void);

void Func_080e9db4(void)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];
    struct ObjectRuntime *object = Object_GetById(EventRuntime_GetControlledOwner());
    s32 i;

    Scheduler_RemoveCallback(Func_080e9940);
    Object_SetPositionAndResetMotionFar(object, object->x, object->y, object->z);
    object->motion_flags = 3;
    gPartyState.render_mode = 0;
    for (i = 0; i < 60; i++) {
        if (object->y <= object->terrain_height)
            break;
        WaitFrames(1);
    }
    Audio_PlayCue(132);
    WaitFrames(1);
    for (i = 0; i < 20; i++) {
        if (object->y <= object->terrain_height)
            break;
        WaitFrames(1);
    }
    WaitFrames(1);
    for (i = 0; i < 20; i++) {
        if (object->y <= object->terrain_height)
            break;
        WaitFrames(1);
    }
    WaitFrames(1);
    object->bounce = 0;
    object->velocity_y = 0;
    object->y = object->terrain_height;
    object->gravity = 0x10000;
    Resource_ResetEntry(*(s16 *)(work + 0x102c));
    Runtime_ReleaseHeapBlock(92);
}
