#ifndef GUARD_OVERLAYS_STAGED_ACTOR_MOVEMENT_H
#define GUARD_OVERLAYS_STAGED_ACTOR_MOVEMENT_H

#include "types.h"

#ifndef RefreshStagedActor
#define RefreshStagedActor Func_020066c4
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

extern u8 *Data_03001e70;
extern s32 Data_0200e1e8[];
extern s32 Data_0200e190[];
extern u8 Value_020082a9;

extern StagedActorRecord *Func_02006610(s32);
extern StagedActorRecord *Func_0200661c(s32);
extern void Func_02000902(s32, s32, s32, s32, s32, s32);
extern void Func_020066b4(s32, s32, s32);
extern void Func_02006714();
extern void Func_0200668a(s32);
extern void Func_0200672c(s32, s32, s32);
extern StagedActorRecord *Func_020066ea(s32);
extern void Func_020066c4();
extern void Func_02006614(StagedActorRecord *, s32);
extern void Func_0200661e(StagedActorRecord *, s32);
extern void Func_0200687c();
extern void Func_02006658(StagedActorRecord *, s32, s32, s32);
extern void Func_02006776(s32);
extern void Func_0200678e(s32, s32);
extern void Func_02006740();
extern void Func_020067a4(s32, s32, s32);
extern void Func_020067bc(s32);
extern void Func_020067d4();
extern StagedActorRecord *Func_0200677a(s32);
extern void RefreshStagedActor();
extern void Func_020068f4(s32);
extern void Func_020068fa(s32);
extern void Func_020066b8();
extern void Func_02006752(s32, s32, s32, s32, s32, s32);
extern void Func_02000a6e(s32, s32, s32, s32, s32, s32);
extern void Func_02000a80(s32, s32, s32, s32, s32, s32);
extern void Func_020067ae(s32, s32, s32, s32, s32, s32);
extern void Func_02000acc(s32, s32, s32, s32, s32, s32);
extern void Func_020069d0(void);

static __inline__ void RedrawStagedActorRegion(
    s32 x, s32 y, s32 width, s32 height, s32 screen_x, s32 screen_y)
{
    Func_02006752(x, y, width, height, screen_x, screen_y);
}

static __inline__ void ConfigureStagedActorMotion(
    s32 actor_id, s32 movement_rate, s32 movement_step)
{
    Func_02006740(actor_id, movement_rate, movement_step);
}

#endif
