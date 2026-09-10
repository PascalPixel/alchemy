#include "types.h"

#define SceneActor_UpdatePlayerProximity Func_0200007c
struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[71];
    u8 active;
};

s32 SceneActor_GetPositionDistance(s32 *, s32 *);
u32 SceneActor_GetAngle(s32, s32);
void SceneActor_SetMode(struct SceneActor *, s32);
struct SceneActor *SceneActor_Find(s32);

s32 SceneActor_UpdatePlayerProximity(struct SceneActor *actor,
                                    struct SceneActor *target,
                                    s32 range, s32 force)
{
    s32 result = 0;
    s32 *targetPos = &target->x;
    s32 *actorPos = &actor->x;

    if (SceneActor_GetPositionDistance(targetPos, actorPos) < range || force != 0) {
        u32 angle = (u16)SceneActor_GetAngle(target->z - actor->z,
                                            *targetPos - *actorPos);
        u32 farLeft = (angle - 0x2000) & 0xf000;
        u32 farRight = (angle + 0x2000) & 0xf000;
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;

        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            SceneActor_SetMode(actor, 1);
            result = 1;
        }
        if (target == SceneActor_Find(0) && (farRight == facing || farLeft == facing)) {
            actor->active = 1;
            SceneActor_SetMode(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        SceneActor_SetMode(actor, 2);
    }
    return result;
}
