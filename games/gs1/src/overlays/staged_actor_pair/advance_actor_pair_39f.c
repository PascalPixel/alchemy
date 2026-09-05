#include "types.h"
#include "staged_actor.h"

#define StagedActor_AdvanceActorPair Func_020000c4

extern u32 Data_0200aef8[];

extern struct StagedActor *Func_02002e90(s32 arg0);
extern struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
extern s32 Func_02002ef4(struct StagedActor *arg0, s32 *arg1);
extern void Func_02002edc(struct StagedActor *arg0, s32 arg1);
extern void Func_02002eac(s32 arg0);
extern void Func_0200307a(s32 arg0);
extern void Func_02002f12(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02002f22(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02002f30(struct StagedActor *arg0);
extern void Func_0200309c(void);
extern void Func_02002f48(struct StagedActor *arg0, s32 arg1);

#define StagedActorStepTable Data_0200aef8
#define GetStagedActor Func_02002e90
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02002ef4
#define SetStagedActorMode Func_02002edc
#define SelectStagedActorSlot Func_02002eac
#define StartStagedActorEffect Func_0200307a
#define StartNextStagedActorMove Func_02002f12
#define StartLeadStagedActorMove Func_02002f22
#define FinishStagedActorMove Func_02002f30
#define FinishStagedActorEffect Func_0200309c
#define SetStagedActorTransition Func_02002f48

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
