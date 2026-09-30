#include "TYPES.H"
#include "FIXED_POINT_POSITION.H"
#include "IWRAM_CALL.H"

/*
 * Whether an actor should turn toward a target: the target is within range
 * (or a forced check), and it lies in the actor's facing half of the full
 * circle. A hit activates the actor and queues the face-toward motion; a miss
 * deactivates it and queues the fall-back motion.
 *
 * The local SceneActor layout is this overlay's own copy of the field actor
 * record (facing at 0x08, x/y/z words at 0x0C, active byte at 0x5B). It is
 * not the shared STAGED_ACTOR.H type: the offsets here are what the reference
 * reads, so the spelling is kept as-is.
 */

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[0x47];
    u8 active;
};

extern s32 ArcTan2(s32, s32);
extern void Object_SetMode(struct SceneActor *, s32);

s32 SceneActor_GetPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return Iwram_Sqrt(dxsq + dysq + dzsq);
}

s32 SceneActor_CheckFacingAndRange(struct SceneActor *actor, struct SceneActor *target,
                  s32 range, s32 force)
{
    s32 result = 0;
    s32 *target_pos = &target->x;
    s32 *actor_pos = &actor->x;
    if (SceneActor_GetPositionDistance(target_pos, actor_pos) < range || force != 0) {
        u32 angle = (u16)ArcTan2(target->z - actor->z,
                                       *target_pos - *actor_pos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;
        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Object_SetMode(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Object_SetMode(actor, 2);
    }
    return result;
}
