#include "PROBE.H"
#include "RAM_BUFFER.H"

void SceneState_ApplyPair12And21(void)
{
    BattleFx_SetPhaseRequest(12, 21);
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
/* The later localizations stop the leader effect following an idle actor
 * in the narrow middle strip of this room. */
struct LeaderEffectWork {
    u8 unknown_00[24];
    struct FieldActor *target;
};

void MakyuriHeya_UpdateLeaderEffectTarget(void)
{
    struct FieldActor *actor = Object_GetById(0);
    struct LeaderEffectWork *work = *(struct LeaderEffectWork **)Ram_ActorEffectWork;

    if (actor->unknown_22 == 0 && actor->z.fixed >> 20 > 12 && actor->z.fixed >> 20 <= 18)
        work->target = NULL;
    else
        work->target = actor;
}
#endif

s32 MakyuriHeya_TrailSparks(struct FieldActor *actor)
{
    struct {
        s32 priority;
        s32 palette;
        s32 start_scale_x;
        s32 start_scale_y;
        u8 unknown_10[24];
    } options;
    s32 velocity_y;

    options.palette = 7;
    if ((gFrameCount & 1) == 0) {
        options.palette = 5;
    }
    options.start_scale_x = 0xcccc;
    options.start_scale_y = 0xcccc;
    options.priority = 0;
    velocity_y = -((((u32)Random_Next() * 8) >> 16) * 0x3333);
    Effect_Spawn(actor->x.fixed + ((8 - (gFrameCount & 15)) << 16),
                                       actor->y.fixed + 0x1a0000, actor->z.fixed, 0, velocity_y, 0,
                                       EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                                       &options);
    return 0;
}
