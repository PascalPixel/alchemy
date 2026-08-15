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
#define FindStagedActorAtProbe Func_02000342
#define CheckStagedActorProbePosition Func_0200171a
#define CheckStagedActorProbe Func_020002a8

s32 CheckStagedActorProbe(struct StagedActor *actor)
{
    struct StagedActorProbePoint probe;
    u32 direction;
    s32 packed_step;
    struct StagedActor *target_actor;

    direction = actor->direction_and_kind >> 12;
    packed_step = StagedActorDirectionSteps[direction];
    probe.x = actor->x.value + (packed_step & 0xffff0000);
    probe.y = actor->y;
    packed_step = packed_step << 16;
    probe.z = actor->z.value + packed_step;
    target_actor = FindStagedActorAtProbe(&probe, actor);
    if (target_actor != 0) {
        u32 probe_value_index = 0;
        s32 target_probe_value =
            *(STAGED_ACTOR_PROBE_DETAILS(target_actor)->unknown_28);
        s32 *probe_values = StagedActorProbeValues;

        do {
            if (target_probe_value == *probe_values++) goto done;
            probe_value_index++;
        } while (probe_value_index <= 5);
        actor->unknown_24 = 0;
        actor->unknown_2c = 0;
        actor->unknown_38 = 0x80000000;
        actor->unknown_40 = 0x80000000;
    }
    packed_step = StagedActorDirectionSteps[direction];
    probe.x = actor->x.value + (packed_step & 0xffff0000);
    probe.y = actor->y;
    packed_step = packed_step << 16;
    probe.z = actor->z.value + packed_step;
    if (CheckStagedActorProbePosition(actor, &probe) > 0) {
        actor->unknown_24 = 0;
        actor->unknown_2c = 0;
        actor->unknown_38 = 0x80000000;
        actor->unknown_40 = 0x80000000;
    }
done:
    return 0;
}
