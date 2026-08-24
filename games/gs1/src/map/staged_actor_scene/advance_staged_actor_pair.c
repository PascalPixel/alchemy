#include "types.h"
#include "staged_actor.h"

extern u32 Data_0200e190[];

extern struct StagedActor *Func_020060b8(s32 arg0);
extern struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
extern s32 Func_020060e4(struct StagedActor *arg0, s32 *arg1);
extern void Func_020060ac(struct StagedActor *arg0, s32 arg1);
extern void Func_0200604c(s32 arg0);
extern void Func_02006312(s32 arg0);
extern void Func_020060f2(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02006102(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02006110(struct StagedActor *arg0);
extern void Func_02006334(void);
extern void Func_02006118(struct StagedActor *arg0, s32 arg1);

#define StagedActorStepTable Data_0200e190
#define GetStagedActor Func_020060b8
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_020060e4
#define SetStagedActorMode Func_020060ac
#define SelectStagedActorSlot Func_0200604c
#define StartStagedActorEffect Func_02006312
#define StartNextStagedActorMove Func_020060f2
#define StartLeadStagedActorMove Func_02006102
#define FinishStagedActorMove Func_02006110
#define FinishStagedActorEffect Func_02006334
#define SetStagedActorTransition Func_02006118
void MapStagedActor_AdvancePair(void) {
    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 direction_index;
    u32 direction_step;
    s32 move_speed;
    s32 transition_state;

    lead_actor = GetStagedActor(0);
    direction_index = lead_actor->direction_and_kind >> 12;
    direction_step = StagedActorStepTable[direction_index];
    destination[0] = lead_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = lead_actor->y;
    direction_step <<= 16;
    destination[2] = lead_actor->z.value + direction_step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    direction_step = StagedActorStepTable[direction_index];
    destination[0] = next_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = next_actor->y;
    direction_step <<= 16;
    destination[2] = next_actor->z.value + direction_step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    direction_step = StagedActorStepTable[direction_index];
    destination[0] = next_actor->x.value + (direction_step & 0xffff0000);
    destination[1] = next_actor->y;
    direction_step <<= 16;
    destination[2] = next_actor->z.value + direction_step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    transition_state = next_actor->transition_busy;
    if (transition_state != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_speed = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_speed;
    next_actor->move_rate_z = move_speed;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_speed;
    lead_actor->move_rate_z = move_speed;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = transition_state;
    next_actor->unknown_2c = transition_state;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = transition_state;
    lead_actor->unknown_2c = transition_state;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}
