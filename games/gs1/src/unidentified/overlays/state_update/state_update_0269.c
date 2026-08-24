#include "staged_actor_probe.h"

#define FindStagedActorProbePosition Func_02000474

s32 FindStagedActorProbePosition(struct StagedActorProbe *probe) {
    struct StagedActorProbePosition position;
    s32 direction;
    struct StagedActor *actor;
    s32 step_count;
    s32 footprint_height;
    u8 *transition_mode;
    s32 footprint_width;
    s32 row;
    s32 column;
    s32 lower_bound;
    s32 upper_bound;
    s32 current_y;
    s32 unused_offset;
    u8 *unused_table;
    s32 footprint_index;

    probe->unknown_14 = 0;
    actor = FindStagedActorProbeTarget(
        &direction, &probe->actor_slot, probe);
    if (actor == 0) return 0;
    transition_mode = &actor->transition_mode;
    *transition_mode = 2;
    step_count = 0;

    footprint_index = probe->footprint_index;
    lower_bound = StagedActorFootprints[footprint_index].z0;
    if (lower_bound < 0) lower_bound = -lower_bound;
    upper_bound = StagedActorFootprints[footprint_index].z1;
    if (upper_bound < 0) upper_bound = -upper_bound;
    footprint_height = (lower_bound + upper_bound) >> 4;

    lower_bound = StagedActorFootprints[footprint_index].x0;
    if (lower_bound < 0) lower_bound = -lower_bound;
    upper_bound = StagedActorFootprints[footprint_index].x1;
    if (upper_bound < 0) upper_bound = -upper_bound;
    footprint_width = (lower_bound + upper_bound) >> 4;

    position.x = actor->x.value
        + (StagedActorDirectionSteps[direction] & 0xffff0000);
    current_y = actor->y;
    position.y = current_y;
    position.z = actor->z.value + (StagedActorDirectionSteps[direction] << 16);
    probe->position_y = current_y;

    for (;;) {
        probe->position_z = position.z
            + (StagedActorFootprints[probe->footprint_index].z0 << 16);
        for (row = 0; row < footprint_height; row++) {
            probe->position_x = position.x
                + (StagedActorFootprints[probe->footprint_index].x0 << 16);
            for (column = 0; column < footprint_width; column++) {
                if (ClassifyStagedActorProbePosition(actor, &probe->position_x) == 2) goto hit;
                probe->position_x += 0x100000;
            }
            probe->position_z += 0x100000;
        }
        step_count++;
        position.x += StagedActorDirectionSteps[direction] & 0xffff0000;
        position.z += StagedActorDirectionSteps[direction] << 16;
    }
hit:
    *transition_mode = 0;
    if (step_count == 0) return 0;
    probe->position_x = actor->x.value
        + (s32)(StagedActorDirectionSteps[direction] & 0xffff0000) * step_count;
    probe->position_y = actor->y;
    probe->position_z = actor->z.value
        + step_count * (s32)(StagedActorDirectionSteps[direction] << 16);
    return 1;
}
