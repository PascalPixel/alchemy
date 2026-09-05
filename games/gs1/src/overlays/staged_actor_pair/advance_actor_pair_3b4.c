#include "types.h"
#include "staged_actor.h"

extern u32 Data_0200a644[];

extern struct StagedActor *Func_02002614(s32 arg0);
extern struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
extern s32 Func_02002680(struct StagedActor *arg0, s32 *arg1);
extern void Func_02002660(struct StagedActor *arg0, s32 arg1);
extern void Func_02002628(s32 arg0);
extern void Func_020027c6(s32 arg0);
extern void Func_02002696(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020026a6(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020026b4(struct StagedActor *arg0);
extern void Func_020027e8(void);
extern void Func_020026cc(struct StagedActor *arg0, s32 arg1);

#define StagedActorStepTable Data_0200a644
#define GetStagedActor Func_02002614
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02002680
#define SetStagedActorMode Func_02002660
#define SelectStagedActorSlot Func_02002628
#define StartStagedActorEffect Func_020027c6
#define StartNextStagedActorMove Func_02002696
#define StartLeadStagedActorMove Func_020026a6
#define FinishStagedActorMove Func_020026b4
#define FinishStagedActorEffect Func_020027e8
#define SetStagedActorTransition Func_020026cc

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
