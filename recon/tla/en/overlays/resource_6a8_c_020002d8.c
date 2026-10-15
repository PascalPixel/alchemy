#include "types.h"
#include "ram_buffer.h"
#include "eventwrk.h"

struct FacingActor {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x47];
    u8 paused;
    u8 unknown_5c[7];
    u8 unknown_63;
};

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);
s32 ArcTan2(s32 y, s32 x);
void Func_020043c4(struct FacingActor *actor, s32 mode);

s32 Func_020002d8(struct FacingActor *actor, struct FacingActor *target, s32 range, s32 force)
{
    s32 result = 0;
    s32 *target_pos;
    s32 *actor_pos;

    if (actor->unknown_63 != 0) {
        actor->paused = 1;
        return 1;
    }
    target_pos = &target->x;
    actor_pos = &actor->x;
    if (SceneActor_CalculateFixedPointDistance(target_pos, actor_pos) < range || force != 0) {
        u32 angle = (u16)ArcTan2(target->z - actor->z, *target_pos - *actor_pos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;

        if (forward == facing || right == facing || left == facing || force != 0) {
            if (((struct EventWork *)Ram_HeapSlots->event_work)->unknown_184 > 120) {
                actor->paused = 0;
                Func_020043c4(actor, 2);
            } else {
                actor->paused = 1;
                Func_020043c4(actor, 1);
                result = 1;
            }
        }
    } else {
        actor->paused = 0;
        Func_020043c4(actor, 2);
    }
    return result;
}
