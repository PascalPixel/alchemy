#include "types.h"
#include "staged_actor.h"

extern u32 Data_02009e48[];

extern struct StagedActor *Func_02002074(s32 arg0);
extern struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_0200076a(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_02000796(s32 *arg0, struct StagedActor *arg1);
extern s32 Func_020020f8(struct StagedActor *arg0, s32 *arg1);
extern void Func_020020d0(struct StagedActor *arg0, s32 arg1);
extern void Func_020020c0(s32 arg0);
extern void Func_02002206(s32 arg0);
extern void Func_0200210e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0200211e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0200212c(struct StagedActor *arg0);
extern void Func_02002228(void);
extern void Func_0200213c(struct StagedActor *arg0, s32 arg1);

#define StagedActorStepTable Data_02009e48
#define GetStagedActor Func_02002074
#define FindNextStagedActor Func_0200073e
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define CanStartStagedActorMove Func_020020f8
#define SetStagedActorMode Func_020020d0
#define SelectStagedActorSlot Func_020020c0
#define StartStagedActorEffect Func_02002206
#define StartNextStagedActorMove Func_0200210e
#define StartLeadStagedActorMove Func_0200211e
#define FinishStagedActorMove Func_0200212c
#define FinishStagedActorEffect Func_02002228
#define SetStagedActorTransition Func_0200213c
#define AdvanceStagedActorPair Func_020003a8

void AdvanceStagedActorPair(void) {
    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 zero;

    lead_actor = GetStagedActor(0);
    facing_index = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[facing_index];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[facing_index];
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
    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_rate;
    next_actor->move_rate_z = move_rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_rate;
    lead_actor->move_rate_z = move_rate;
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
