#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"

s32 SuharaSabaku_FollowLeaderWithSparks(struct FieldActor *actor)
{
    struct EffectOptions options;
    struct FieldActor *leader;
    s32 dx;
    s32 dz;
    s32 phase;

    leader = Engine_ActorGet(gGameState.selected_actor);
    actor->acceleration = 0x4000;
    actor->speed = 0x18000;
    actor->motion_flags = 0;
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->active ^= 1;
    if (Engine_GameFlagIsSet(0x104)) {
        actor->target_x = ACTOR_NO_TARGET;
        actor->target_y = ACTOR_NO_TARGET;
        actor->target_z = ACTOR_NO_TARGET;
    } else {
        actor->target_x = leader->x.fixed;
        actor->target_y = *(s32 *)leader->unknown_14;
        actor->target_z = leader->z.fixed;
        dx = actor->x.fixed - leader->x.fixed;
        if (dx < 0) {
            dx = leader->x.fixed - actor->x.fixed;
        }
        dz = actor->z.fixed - leader->z.fixed;
        if (dx + (dz >= 0 ? actor->z.fixed - leader->z.fixed : leader->z.fixed - actor->z.fixed) < 0x80000) {
            struct EventWork *work = gEventWork;

            if (leader->motion_flags != 0) {
                work->raised_trigger = 55;
            }
            actor->motion_flags = 3;
            actor->target_x = leader->x.fixed;
            actor->target_y = leader->y.fixed;
            actor->target_z = leader->z.fixed;
        }
    }
    phase = gFrameCount & 7;
    if (phase == 0) {
        options.start_scale_x = 0xcccc;
        options.start_scale_y = 0xcccc;
        options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, phase, phase, 0x880001, &options);
    }
    return 1;
}
