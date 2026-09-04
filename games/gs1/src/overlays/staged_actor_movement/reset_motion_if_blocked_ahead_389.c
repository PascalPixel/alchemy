#include "types.h"
#include "staged_actor_probe_state.h"

extern s32 Data_02009554[];
extern s32 Data_02009594[];
extern struct StagedActor *Func_02000342(
    struct StagedActorProbePoint *, struct StagedActor *);
extern s32 Func_0200171a(
    struct StagedActor *, struct StagedActorProbePoint *);

#define StagedActorDirectionSteps Data_02009554
#define StagedActorProbeValues Data_02009594
#define StagedActor_ResetMotionIfBlockedAhead Func_020002a8
#define FindStagedActorAtProbe Func_02000342
#define CheckStagedActorProbePosition Func_0200171a

s32 StagedActor_ResetMotionIfBlockedAhead(struct StagedActor *actor)
{
    struct StagedActorProbePoint probe;
    u32 dir;
    s32 step;
    struct StagedActor *target;

    dir = actor->direction_and_kind >> 12;
    step = StagedActorDirectionSteps[dir];
    probe.x = actor->x.value + (step & 0xffff0000);
    probe.y = actor->y;
    step = step << 16;
    probe.z = actor->z.value + step;
    target = FindStagedActorAtProbe(&probe, actor);
    if (target != 0) {
        u32 i = 0;
        s32 v =
            *(STAGED_ACTOR_PROBE_DETAILS(target)->unknown_28);
        s32 *p = StagedActorProbeValues;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        actor->unknown_24 = 0;
        actor->unknown_2c = 0;
        actor->unknown_38 = 0x80000000;
        actor->unknown_40 = 0x80000000;
    }
    step = StagedActorDirectionSteps[dir];
    probe.x = actor->x.value + (step & 0xffff0000);
    probe.y = actor->y;
    step = step << 16;
    probe.z = actor->z.value + step;
    if (CheckStagedActorProbePosition(actor, &probe) > 0) {
        actor->unknown_24 = 0;
        actor->unknown_2c = 0;
        actor->unknown_38 = 0x80000000;
        actor->unknown_40 = 0x80000000;
    }
done:
    return 0;
}
