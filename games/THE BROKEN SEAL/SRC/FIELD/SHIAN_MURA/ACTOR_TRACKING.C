#include "TYPES.H"

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

s32 SceneEffect_CalculatePositionDistance(const s32 *target_position, const s32 *actor_position);
void Engine_ObjectSetAnimation(struct MapActor *actor, s32 mode);
u16 ArcTan2(s32 z_delta, s32 x_delta);

/* Decides whether an actor tracks its target. An actor already tracking in
 * mode 0 carries on. Otherwise, when the target is closer than the distance
 * limit and lies in the actor's facing sector or one beside it, or tracking
 * is forced, the actor starts tracking in mode 1; if not, it stops tracking.
 * Returns 1 while tracking. */
s32 SceneActor_UpdateTracking(
    struct MapActor *actor,
    struct MapActor *target,
    s32 distance_limit,
    s32 force_tracking)
{
    s32 result = 0;

    if (actor->tracking_state == 1 && actor->tracking_mode == 0) {
        Engine_ObjectSetAnimation(actor, 1);
        return 1;
    }

    if (SceneEffect_CalculatePositionDistance(&target->x, &actor->x) < distance_limit ||
        force_tracking != 0) {
        u16 angle = ArcTan2(target->z - actor->z, target->x - actor->x);
        s32 prev = (angle - 0x1000) & 0xf000;
        s32 next = (angle + 0x1000) & 0xf000;
        s32 facing = angle & 0xf000;
        s32 heading = actor->heading & 0xf000;

        if (facing == heading || next == heading || prev == heading ||
            force_tracking != 0) {
            actor->tracking_state = 1;
            Engine_ObjectSetAnimation(actor, 1);
            result = 1;
            actor->tracking_mode = result;
        } else {
            actor->tracking_state = 0;
            Engine_ObjectSetAnimation(actor, 2);
            actor->tracking_mode = 0;
        }
    } else {
        actor->tracking_state = 0;
        Engine_ObjectSetAnimation(actor, 2);
        actor->tracking_mode = 0;
    }

    return result;
}
