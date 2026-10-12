/* Near misses of overlay 6AB (the same three functions close overlays 69D and 69F).
 * Func_020020b8 (47 instructions, 4 differ): the native stores the drift before it loads the
 * scale step and the scale before it loads the sprite; a one-pass block at either store moves
 * the 0x1000 constant instead.
 * Func_02003b74 (56 instructions, 2 differ): the native loads x and z from r0 before it keeps the
 * leader in r10; a second variable changes nothing and a one-pass block before the copy moves
 * the owner address load after it (4 differ).
 * Func_02003bfc (49 instructions, 2 differ): the native loads the leader x before it copies the
 * mask 64; with the mask on the left 47 differ. */
#include "../../../../games/tla/src/field/overlay_6ab/ov6ab.h"

void Func_020020b8(struct ObjectRuntime *object)
{
    s32 bounce;
    s32 drift;

    Object_GetById(gPartyState.current_owner);
    bounce = object->bounce;
    object->x += bounce;
    object->y += object->gravity;
    drift = object->unknown_4c;
    object->z += drift;
    object->bounce = bounce - bounce / 18;
    object->unknown_4c = drift - drift / 16;
    object->scale_x += object->speed_limit;
    object->scale_y += object->acceleration;
    ((struct FieldSprite *)object->animation)->rotation += 0x1000;
}

void Func_02003b74(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    s32 x = leader->x;
    s32 z = leader->z;

    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x40000, 0x20000);
    x >>= 20;
    z >>= 20;
    ObjectMotion_SetPositionAndCommit(gPartyState.current_owner, x * 16 + 8, z * 16 + 8);
    WaitFrames(1);
    Object_SetPositionAndResetMotion(leader, (x << 20) + 0x80000, leader->y, (z << 20) + 0x80000);
    Battle_WaitMode0(4);
    Func_020043dc()->y -= 0x80000;
}

void Func_02003bfc(struct PendingEvent *event)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    s32 x = leader->x >> 20 | (s16)(*(u16 *)((u8 *)event + 6) & 64);
    s32 z = leader->z >> 20 | (s16)(*(u16 *)((u8 *)event + 8) & 64);

    Func_02003b74();
    Func_0200446c(161);
    Func_02004234(*(s16 *)((u8 *)event + 6), *(s16 *)((u8 *)event + 8) + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}
