#include "types.h"
#include "staged_actor.h"

extern u32 Data_02009554[];

extern struct StagedActor *Func_02001524(s32 arg0);
extern struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
extern s32 Func_02001598(struct StagedActor *arg0, s32 *arg1);
extern void Func_02001580(struct StagedActor *arg0, s32 arg1);
extern void Func_02001568(s32 arg0);
extern void Func_020016d6(s32 arg0);
extern void Func_020015b6(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020015c6(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020015d4(struct StagedActor *arg0);
extern void Func_020016f8(void);
extern void Func_020015ec(struct StagedActor *arg0, s32 arg1);

#define StagedActorStepTable Data_02009554
#define GetStagedActor Func_02001524
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02001598
#define SetStagedActorMode Func_02001580
#define SelectStagedActorSlot Func_02001568
#define StartStagedActorEffect Func_020016d6
#define StartNextStagedActorMove Func_020015b6
#define StartLeadStagedActorMove Func_020015c6
#define FinishStagedActorMove Func_020015d4
#define FinishStagedActorEffect Func_020016f8
#define SetStagedActorTransition Func_020015ec

void StagedActor_AdvanceActorPair(void) {
    s32 dst[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *blocker;
    s32 dir;
    u32 step;
    s32 rate;
    s32 zero;

    lead = GetStagedActor(0);
    dir = lead->direction_and_kind >> 12;
    step = StagedActorStepTable[dir];
    dst[0] = lead->x.value + (step & 0xffff0000);
    dst[1] = lead->y;
    step <<= 16;
    dst[2] = lead->z.value + step;
    next = FindNextStagedActor(dst, lead);
    if (next == 0) return;

    step = StagedActorStepTable[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    blocker = FindBlockingStagedActor(dst, next);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) return;

    dst[0] = next->x.value;
    dst[1] = next->y + 0x100000;
    dst[2] = next->z.value;
    blocker = FindElevatedBlockingStagedActor(dst, next);
    if (blocker != 0 && (blocker->collision_flags & 1) != 0) return;

    next->transition_mode = 2;
    step = StagedActorStepTable[dir];
    dst[0] = next->x.value + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z.value + step;
    if (CanStartStagedActorMove(next, dst) > 0) return;

    zero = next->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next->move_rate_x = rate;
    next->move_rate_z = rate;
    StartNextStagedActorMove(next, dst[0], dst[1], dst[2]);
    lead->move_rate_x = rate;
    lead->move_rate_z = rate;
    StartLeadStagedActorMove(lead, dst[0], dst[1], dst[2]);
    FinishStagedActorMove(next);
    FinishStagedActorEffect();
    next->x.value = dst[0];
    next->z.value = dst[2];
    next->unknown_24 = zero;
    next->unknown_2c = zero;
    lead->unknown_38 = 0x80000000;
    lead->unknown_40 = 0x80000000;
    lead->unknown_24 = zero;
    lead->unknown_2c = zero;
    lead->x.value = lead->x.parts.cell << 16;
    lead->z.value = lead->z.parts.cell << 16;
    SetStagedActorTransition(lead, 1);
}
