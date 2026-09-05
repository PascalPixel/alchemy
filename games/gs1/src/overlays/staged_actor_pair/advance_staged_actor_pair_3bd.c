#include "types.h"
#include "staged_actor.h"

extern u32 Data_0200beb4[];

extern struct StagedActor *Func_02003d9c(s32 arg0);
extern struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
extern s32 Func_02003df8(struct StagedActor *arg0, s32 *arg1);
extern void Func_02003de0(struct StagedActor *arg0, s32 arg1);
extern void Func_02003dc0(s32 arg0);
extern void Func_02004036(s32 arg0);
extern void Func_02003e0e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02003e1e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02003e2c(struct StagedActor *arg0);
extern void Func_02004048(void);
extern void Func_02003e4c(struct StagedActor *arg0, s32 arg1);

#define StagedActorStepTable Data_0200beb4
#define GetStagedActor Func_02003d9c
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02003df8
#define SetStagedActorMode Func_02003de0
#define SelectStagedActorSlot Func_02003dc0
#define StartStagedActorEffect Func_02004036
#define StartNextStagedActorMove Func_02003e0e
#define StartLeadStagedActorMove Func_02003e1e
#define FinishStagedActorMove Func_02003e2c
#define FinishStagedActorEffect Func_02004048
#define SetStagedActorTransition Func_02003e4c

void StagedActor_AdvancePair(void) {
    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead_actor = GetStagedActor(0);
    dir = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[dir];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[dir];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = StagedActorStepTable[dir];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead_actor, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = rate;
    next_actor->move_rate_z = rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = rate;
    lead_actor->move_rate_z = rate;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = zero;
    next_actor->unknown_2c = zero;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = zero;
    lead_actor->unknown_2c = zero;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}
