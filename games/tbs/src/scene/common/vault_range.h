#include "types.h"
#include "field_obj.h"

s32 SceneActor_CheckFacingAndRange(
    struct FieldActor *actor, struct FieldActor *target, s32 range, s32 force)
{
    s32  result = 0;
    s32 *target_pos = &target->x.fixed;
    s32 *actor_pos = &actor->x.fixed;
    if (FixedPoint_Distance(target_pos, actor_pos) < range || force != 0)
    {
        u32 angle = (u16)ArcTan2(target->z.fixed - actor->z.fixed, *target_pos - *actor_pos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;
        if (forward == facing || right == facing || left == facing || force != 0)
        {
            actor->paused = 1;
            Object_SetMode(actor, 1);
            result = 1;
        }
    }
    else
    {
        actor->paused = 0;
        Object_SetMode(actor, 2);
    }
    return result;
}
