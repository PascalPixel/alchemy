#include "types.h"
#include "staged_actor.h"

extern u32 Data_0200d0e8[];

extern struct StagedActor *Func_02005230(s32 arg0);
extern struct StagedActor *Func_020006e6(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_02000712(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
extern s32 Func_0200528c(struct StagedActor *arg0, s32 *arg1);
extern void Func_0200523c(struct StagedActor *arg0, s32 arg1);
extern void Func_02005214(s32 arg0);
extern void Func_02005472(s32 arg0);
extern void Func_0200528a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0200529a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020052a8();
extern void Func_02005494(void);

#define StagedActorDirectionSteps Data_0200d0e8
#define GetStagedActor Func_02005230
#define FindActorAtPosition Func_020006e6
#define FindActorAtForwardPosition Func_02000712
#define FindActorAbovePosition Func_0200073e
#define CheckStagedActorMove Func_0200528c
#define SetStagedActorMode Func_0200523c
#define WaitSceneFrames Func_02005214
#define PlaySoundCue Func_02005472
#define SetStagedActorMoveTarget Func_0200528a
#define SetLeadActorMoveTarget Func_0200529a
#define UpdateStagedActorState Func_020052a8
#define FinalizeStagedActorUpdate Func_02005494

void RunStagedActorTransition(void) {
    s32 target_position[3];
    struct StagedActor *leader;
    struct StagedActor *actor;
    struct StagedActor *blocking_actor;
    s32 direction_index;
    u32 packed_step;
    s32 move_rate;
    s32 transition_busy;

    leader = GetStagedActor(0);
    direction_index = leader->direction_and_kind >> 12;
    packed_step = StagedActorDirectionSteps[direction_index];
    target_position[0] = leader->x.value + (packed_step & 0xffff0000);
    target_position[1] = leader->y;
    packed_step <<= 16;
    target_position[2] = leader->z.value + packed_step;
    actor = FindActorAtPosition(target_position, leader);
    if (actor == 0) return;

    packed_step = StagedActorDirectionSteps[direction_index];
    target_position[0] = actor->x.value + (packed_step & 0xffff0000);
    target_position[1] = actor->y;
    packed_step <<= 16;
    target_position[2] = actor->z.value + packed_step;
    blocking_actor = FindActorAtForwardPosition(target_position, actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    target_position[0] = actor->x.value;
    target_position[1] = actor->y + 0x100000;
    target_position[2] = actor->z.value;
    blocking_actor = FindActorAbovePosition(target_position, actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    actor->transition_mode = 2;
    packed_step = StagedActorDirectionSteps[direction_index];
    target_position[0] = actor->x.value + (packed_step & 0xffff0000);
    target_position[1] = actor->y;
    packed_step <<= 16;
    target_position[2] = actor->z.value + packed_step;
    if (CheckStagedActorMove(actor, target_position) > 0) return;

    transition_busy = actor->transition_busy;
    if (transition_busy != 0) return;

    SetStagedActorMode(leader, 8);
    move_rate = 0x3333;
    WaitSceneFrames(15);
    PlaySoundCue(185);
    actor->move_rate_x = move_rate;
    actor->move_rate_z = move_rate;
    SetStagedActorMoveTarget(actor, target_position[0], target_position[1], target_position[2]);
    leader->move_rate_x = move_rate;
    leader->move_rate_z = move_rate;
    SetLeadActorMoveTarget(leader, target_position[0], target_position[1], target_position[2]);
    UpdateStagedActorState(actor);
    FinalizeStagedActorUpdate();
    actor->x.value = target_position[0];
    actor->z.value = target_position[2];
    actor->unknown_24 = transition_busy;
    actor->unknown_2c = transition_busy;
    leader->unknown_38 = 0x80000000;
    leader->unknown_40 = 0x80000000;
    leader->unknown_24 = transition_busy;
    leader->unknown_2c = transition_busy;
    leader->x.value = leader->x.parts.cell << 16;
    leader->z.value = leader->z.parts.cell << 16;
    UpdateStagedActorState(leader, 1);
}
