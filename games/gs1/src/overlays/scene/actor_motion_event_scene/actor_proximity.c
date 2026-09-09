#include "types.h"

#define SceneActor_UpdateProximity Func_02000350
struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[71];
    u8 active;
};

s32 Func_0200068c(s32 *, s32 *);
u32 Func_020013fc(s32, s32);
void Func_02001442(struct SceneActor *, s32);
void Func_02001458(struct SceneActor *, s32);

s32 SceneActor_UpdateProximity(struct SceneActor *actor, struct SceneActor *target,
                  s32 range, s32 force)
{
    s32 result = 0;
    s32 *targetPos = &target->x;
    s32 *actorPos = &actor->x;

    if (Func_0200068c(targetPos, actorPos) < range || force != 0) {
        u32 angle = (u16)Func_020013fc(target->z - actor->z,
                                      *targetPos - *actorPos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;

        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Func_02001442(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Func_02001458(actor, 2);
    }
    return result;
}
