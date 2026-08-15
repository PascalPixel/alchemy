#ifndef ALCHEMY_STAGED_ACTOR_PROBE_H
#define ALCHEMY_STAGED_ACTOR_PROBE_H

#include "staged_actor.h"

/* Six-word record exchanged by the staged-actor target finder and sweep. */
struct StagedActorProbe {
    s32 footprint_index;
    s32 actor_slot;
    s32 position_x;
    s32 position_y;
    s32 position_z;
    s32 unknown_14;
};

struct StagedActorProbePosition {
    s32 x;
    s32 y;
    s32 z;
};

/* The x/z bounds used to sweep one staged actor footprint. */
struct StagedActorFootprint {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
};

extern struct StagedActorFootprint Data_0200adc0[];
extern u32 Data_0200ad68[];

extern struct StagedActor *Func_020007de(
    s32 *direction_out, s32 *actor_slot_out, struct StagedActorProbe *probe);
extern s32 Func_02003140(struct StagedActor *actor, s32 *position);

#define StagedActorFootprints Data_0200adc0
#define StagedActorDirectionSteps Data_0200ad68
#define FindStagedActorProbeTarget Func_020007de
#define ClassifyStagedActorProbePosition Func_02003140

#define STAGED_ACTOR_PROBE_OFFSET(field) \
    ((u32)&(((struct StagedActorProbe *)0)->field))
typedef char StagedActorProbe_size[
    sizeof(struct StagedActorProbe) == 0x18 ? 1 : -1
];
typedef char StagedActorProbe_position_x_offset[
    STAGED_ACTOR_PROBE_OFFSET(position_x) == 0x08 ? 1 : -1
];
typedef char StagedActorProbe_position_z_offset[
    STAGED_ACTOR_PROBE_OFFSET(position_z) == 0x10 ? 1 : -1
];
#undef STAGED_ACTOR_PROBE_OFFSET

typedef char StagedActorFootprint_size[
    sizeof(struct StagedActorFootprint) == 0x10 ? 1 : -1
];

#endif
