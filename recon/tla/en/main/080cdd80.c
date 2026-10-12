/* Near miss: four reordered rows of one kind. The native starts a split constant or an argument copy
 * before the next position word is loaded (movs r2, #128; ldr r3, [r5, #12]; lsls r2, r2, #11 and
 * adds r2, r7, #0; ldr r3, [r5, #16]); the draft loads first. Same opening as Func_080cdc74 before it. */
#include "types.h"
#include "party_state.h"
#include "object_runtime.h"

#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

struct ObjectRuntime *Object_GetById(s32 id);
void Vector_AddPolarOffset(s32 length, s32 angle, s32 *position);
struct ObjectRuntime *Func_080eaf28(s32 *position, s32 mode);
s32 Func_08020210(struct ObjectRuntime *object, s32 *position);
void WaitFrames(s32 frames);
void Event_NoOp(void);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

void Func_080cdd80(void)
{
    s32 position[3];
    struct ObjectRuntime *owner = Object_GetById(gPartyState.current_owner);
    struct ObjectRuntime *target;
    s32 terrain;
    s32 result;
    s32 speed;
    s32 acceleration;
    s32 angle = owner->angle >> 12;

    angle = ((angle + 2) & 12) << 12;
    position[0] = owner->x;
    position[1] = (owner->y + 0x40000) & 0xfff80000;
    position[2] = owner->z;
    Vector_AddPolarOffset(0x100000, angle, position);
    target = Func_080eaf28(position, 1);
    position[0] = target->x;
    position[1] = target->y;
    position[2] = target->z;
    Vector_AddPolarOffset(0x100000, angle, position);
    terrain = target->terrain_id;
    target->terrain_id = 2;
    result = Func_08020210(target, position);
    target->terrain_id = terrain;
    if (result <= 0) {
        Object_SetMode(owner, 8);
        WaitFrames(15);
        Audio_PlayCue(185);
        target->speed_limit = 0x3333;
        target->acceleration = 0x3333;
        Object_SetPosition(target, position[0], position[1], position[2]);
        WaitFrames(2);
        acceleration = owner->acceleration;
        owner->acceleration = 0x3333;
        speed = owner->speed_limit;
        owner->speed_limit = 0x3333;
        Object_SetPosition(owner, position[0], position[1], position[2]);
        Object_CommitPosition(target);
        WaitFrames(1);
        Event_NoOp();
        Object_SetPositionAndResetMotionFar(target, position[0], position[1], position[2]);
        Object_SetPositionAndResetMotionFar(owner, S16_AT(owner, 10) << 16, owner->y, S16_AT(owner, 18) << 16);
        Object_SetMode(owner, 1);
        owner->speed_limit = speed;
        owner->acceleration = acceleration;
    }
}
