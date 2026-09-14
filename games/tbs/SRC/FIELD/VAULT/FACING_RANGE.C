#include "TYPES.H"
#define SceneActor_CheckFacingAndRange Func_02001674
struct SceneActor { u8 unk_00[6]; u16 facing; s32 x, y, z; u8 unk_14[0x47]; u8 active; };
extern s32 Func_02002cd4(s32 *, s32 *);
extern s32 Func_02002fd8(s32, s32);
extern void Func_0200301e(struct SceneActor *, s32);
extern void Func_02003034(struct SceneActor *, s32);

s32 SceneActor_CheckFacingAndRange(struct SceneActor *actor, struct SceneActor *target,
                  s32 range, s32 force)
{
    s32 result = 0;
    s32 *target_pos = &target->x;
    s32 *actor_pos = &actor->x;
    if (Func_02002cd4(target_pos, actor_pos) < range || force != 0) {
        u32 angle = (u16)Func_02002fd8(target->z - actor->z,
                                       *target_pos - *actor_pos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;
        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Func_0200301e(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Func_02003034(actor, 2);
    }
    return result;
}
