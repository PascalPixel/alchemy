#include "types.h"

struct MapActor {
    u8 reserved_00[6];
    u16 heading;
    s32 x;
    u8 reserved_0c[4];
    s32 z;
    u8 reserved_14[71];
    u8 tracking_state;
    u8 reserved_5c[8];
    s16 tracking_mode;
};

s32 Func_0200082a(const s32 *target_position, const s32 *actor_position);
void Func_020017a2(struct MapActor *actor, s32 mode);
u16 Func_020017cc(s32 z_delta, s32 x_delta);
void Func_02001814(struct MapActor *actor, s32 mode);
void Func_02001828(struct MapActor *actor, s32 mode);
void Func_02001842(struct MapActor *actor, s32 mode);

s32 Func_02000408(
    struct MapActor *actor,
    struct MapActor *target,
    s32 distance_limit,
    s32 force_tracking)
{
    s32 result = 0;

    if (actor->tracking_state == 1 && actor->tracking_mode == 0) {
        Func_020017a2(actor, 1);
        return 1;
    }

    if (Func_0200082a(&target->x, &actor->x) < distance_limit ||
        force_tracking != 0) {
        u16 angle = Func_020017cc(target->z - actor->z, target->x - actor->x);
        u16 heading = actor->heading & 0xf000;

        if ((angle & 0xf000) != heading &&
            ((angle + 0x1000) & 0xf000) != heading &&
            ((angle + 0xfa7ef001) & 0xf000) != heading &&
            force_tracking == 0) {
            actor->tracking_state = 0;
            Func_02001814(actor, 2);
            actor->tracking_mode = 0;
        } else {
            actor->tracking_state = 1;
            Func_02001828(actor, 1);
            actor->tracking_mode = 1;
            result = 1;
        }
    } else {
        actor->tracking_state = 0;
        Func_02001842(actor, 2);
        actor->tracking_mode = 0;
    }

    return result;
}
