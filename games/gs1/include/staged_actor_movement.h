#ifndef GUARD_OVERLAYS_STAGED_ACTOR_MOVEMENT_H
#define GUARD_OVERLAYS_STAGED_ACTOR_MOVEMENT_H

#include "types.h"

#ifndef RefreshStagedActor
#endif

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

extern u8 Value_020082a9;

#ifdef STAGED_ACTOR_MOVEMENT_UNTYPED_LOCATION_CALL
#else
#endif
extern void RefreshStagedActor();

static __inline__ void RedrawStagedActorRegion(
    s32 x, s32 y, s32 width, s32 height, s32 screen_x, s32 screen_y)
{
    StagedActor_RedrawRegion(x, y, width, height, screen_x, screen_y);
}

static __inline__ void ConfigureStagedActorMotion(
    s32 actor_id, s32 movement_rate, s32 movement_step)
{
    StagedActor_ConfigureMotion(actor_id, movement_rate, movement_step);
}

#endif
