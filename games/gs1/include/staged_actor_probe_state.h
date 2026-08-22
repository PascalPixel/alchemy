#ifndef ALCHEMY_STAGED_ACTOR_PROBE_STATE_H
#define ALCHEMY_STAGED_ACTOR_PROBE_STATE_H

#include "staged_actor.h"

/* Directional probe point and the observed nested slice of a returned actor. */
struct StagedActorProbePoint {
    s32 x;
    s32 y;
    s32 z;
};

struct StagedActorProbeDetails {
    u8 unknown_00[0x28];
    s16 *unknown_28;
};

struct StagedActorProbeActorView {
    u8 unknown_00[0x50];
    struct StagedActorProbeDetails *unknown_50;
};

#define STAGED_ACTOR_PROBE_DETAILS(actor) \
    (((struct StagedActorProbeActorView *)(actor))->unknown_50)

#define STAGED_ACTOR_PROBE_STATE_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))
typedef char StagedActorProbePoint_size[
    sizeof(struct StagedActorProbePoint) == 0x0c ? 1 : -1
];
typedef char StagedActorProbeDetails_unknown_28_offset[
    STAGED_ACTOR_PROBE_STATE_OFFSET(struct StagedActorProbeDetails, unknown_28) == 0x28 ? 1 : -1
];
typedef char StagedActorProbeDetails_size[
    sizeof(struct StagedActorProbeDetails) == 0x2c ? 1 : -1
];
typedef char StagedActorProbeActorView_unknown_50_offset[
    STAGED_ACTOR_PROBE_STATE_OFFSET(struct StagedActorProbeActorView, unknown_50) == 0x50 ? 1 : -1
];
#undef STAGED_ACTOR_PROBE_STATE_OFFSET

#endif
