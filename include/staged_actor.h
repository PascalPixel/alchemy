#ifndef ALCHEMY_STAGED_ACTOR_H
#define ALCHEMY_STAGED_ACTOR_H

#include "types.h"

/* Shared actor layout proved by the staged probe/move/redraw overlay family. */
union StagedActorCoordinate {
    s32 value;
    struct {
        s16 fraction;
        s16 cell;
    } parts;
};

struct StagedActor {
    u8 unknown_00[6];
    u16 direction_and_kind;
    union StagedActorCoordinate x;
    s32 y;
    union StagedActorCoordinate z;
    u8 unknown_14[0x0e];
    u8 transition_mode;
    u8 unknown_23;
    s32 unknown_24;
    u8 unknown_28[4];
    s32 unknown_2c;
    s32 move_rate_x;
    s32 move_rate_z;
    s32 unknown_38;
    u8 unknown_3c[4];
    s32 unknown_40;
    u8 unknown_44[0x15];
    u8 collision_flags;
    u8 unknown_5a[8];
    u8 transition_busy;
    u8 unknown_63[9];
    u32 movement_callback;
};

struct StagedActorMoveArgs {
    s32 displacement_row;
    s32 actor_slot;
    s32 target_x;
    s32 elevation;
    s32 target_z;
    void (*callback)(void);
};

#define STAGED_ACTOR_OFFSET(field) ((u32)&(((struct StagedActor *)0)->field))
typedef char StagedActor_size[sizeof(struct StagedActor) == 0x70 ? 1 : -1];
typedef char StagedActor_x_offset[
    STAGED_ACTOR_OFFSET(x) == 0x08 ? 1 : -1
];
typedef char StagedActor_transition_mode_offset[
    STAGED_ACTOR_OFFSET(transition_mode) == 0x22 ? 1 : -1
];
typedef char StagedActor_collision_flags_offset[
    STAGED_ACTOR_OFFSET(collision_flags) == 0x59 ? 1 : -1
];
typedef char StagedActor_transition_busy_offset[
    STAGED_ACTOR_OFFSET(transition_busy) == 0x62 ? 1 : -1
];
typedef char StagedActor_movement_callback_offset[
    STAGED_ACTOR_OFFSET(movement_callback) == 0x6c ? 1 : -1
];
#undef STAGED_ACTOR_OFFSET

#endif
