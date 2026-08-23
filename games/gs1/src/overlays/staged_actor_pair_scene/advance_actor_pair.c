#include "types.h"
#include "staged_actor.h"
#include "staged_actor_pair_scene.h"

extern u32 Data_020096c0[];

extern struct StagedActor *Func_020018ec(s32 arg0);
extern struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_0200076a(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_02000796(s32 *arg0, struct StagedActor *arg1);
extern s32 Func_02001958(struct StagedActor *arg0, s32 *arg1);
extern void Func_02001940(struct StagedActor *arg0, s32 arg1);
extern void Func_02001930(s32 arg0);
extern void Func_02001a7e(s32 arg0);
extern void Func_02001976(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02001986(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02001994(struct StagedActor *arg0);
extern void Func_02001aa0(void);
extern void Func_020019ac(struct StagedActor *arg0, s32 arg1);

#define StagedActorStepTable Data_020096c0
#define GetStagedActor Func_020018ec
#define FindNextStagedActor Func_0200073e
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define CanStartStagedActorMove Func_02001958
#define SetStagedActorMode Func_02001940
#define SelectStagedActorSlot Func_02001930
#define StartStagedActorEffect Func_02001a7e
#define StartNextStagedActorMove Func_02001976
#define StartLeadStagedActorMove Func_02001986
#define FinishStagedActorMove Func_02001994
#define FinishStagedActorEffect Func_02001aa0
#define SetStagedActorTransition Func_020019ac
void StagedActorPairScene_AdvancePair(void) {
    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 clear_value;

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

    clear_value = next_actor->transition_busy;
    if (clear_value != 0) return;

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
    next_actor->unknown_24 = clear_value;
    next_actor->unknown_2c = clear_value;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = clear_value;
    lead_actor->unknown_2c = clear_value;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}
