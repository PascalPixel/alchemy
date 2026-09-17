#ifndef ALCHEMY_STAGED_ACTOR_H
#define ALCHEMY_STAGED_ACTOR_H

#include "TYPES.H"

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
    s32 unknown_28;
    s32 unknown_2c;
    s32 move_rate_x;
    s32 move_rate_z;
    s32 unknown_38;
    s32 unknown_3c;
    s32 unknown_40;
    u8 unknown_44[0x15];
    u8 collision_flags;
    u8 unknown_5a[8];
    u8 transition_busy;
    u8 unknown_63[3];
    s16 vertical_motion_direction;
    u8 unknown_68[4];
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
typedef char StagedActor_vertical_motion_direction_offset[
    STAGED_ACTOR_OFFSET(vertical_motion_direction) == 0x66 ? 1 : -1
];
typedef char StagedActor_movement_callback_offset[
    STAGED_ACTOR_OFFSET(movement_callback) == 0x6c ? 1 : -1
];
#undef STAGED_ACTOR_OFFSET

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

/* The object record and request SceneActor_MoveAndRedraw reads. */
typedef struct StagedActorRecord {
    u8 padding_00[6];
    u16 orientation;
    s32 x;
    s32 depth;
    s32 y;
    u8 padding_14[16];
    s32 horizontal_velocity;
    u8 padding_28[4];
    s32 vertical_velocity;
    s32 movement_rate;
    s32 movement_step;
    u8 padding_38[52];
    u32 callback;
} StagedActorRecord;

typedef struct StagedActorPosition {
    s32 x;
    u8 padding_04[4];
    s32 y;
} StagedActorPosition;

typedef struct StagedActorMovementRequestTail {
    s32 target_y;
    void (*callback)(void);
} StagedActorMovementRequestTail;

typedef struct StagedActorMovementRequest {
    s32 movement_index;
    s32 actor_id;
    s32 target_x;
    s32 target_depth;
    StagedActorMovementRequestTail tail;
} StagedActorMovementRequest;

/* The staged-actor module, linked whole into each field overlay that stages
   actors (FIELD/COMMON/OBJECT/STAGED_ACTOR.C). */
s32 FixedPoint_Distance(s32 *first_position, s32 *second_position);
struct StagedActor *StagedActor_FindAtTile(s32 *position, struct StagedActor *self);
void StagedActor_AdvancePair(void);
s32 StagedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);
s32 StagedActor_StopBlockedMotion(struct StagedActor *actor);
u8 *FieldScene_FindActorRegion(s32 *direction, s32 *slot, s32 *footprint);
s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);
void SceneActor_MoveAndRedraw(StagedActorMovementRequest request);
s32 FieldScene_RedrawActorFootprint(s32 id);

#endif
