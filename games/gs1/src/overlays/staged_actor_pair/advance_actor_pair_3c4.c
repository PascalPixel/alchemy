#include "types.h"
#include "staged_actor.h"

extern u32 Data_0200b1f0[];

extern struct StagedActor *Func_020031e0(s32 arg0);
extern struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
extern s32 Func_02003244(struct StagedActor *arg0, s32 *arg1);
extern void Func_0200320c(struct StagedActor *arg0, s32 arg1);
extern void Func_020031e4(s32 arg0);
extern void Func_02003372(s32 arg0);
extern void Func_0200324a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0200325a(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02003268(struct StagedActor *arg0);
extern void Func_02003394(void);
extern void Func_02003278(struct StagedActor *arg0, s32 arg1);

#define StagedActorStepTable Data_0200b1f0
#define StagedActor_AdvanceActorPair Func_020000c4
#define GetStagedActor Func_020031e0
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02003244
#define SetStagedActorMode Func_0200320c
#define SelectStagedActorSlot Func_020031e4
#define StartStagedActorEffect Func_02003372
#define StartNextStagedActorMove Func_0200324a
#define StartLeadStagedActorMove Func_0200325a
#define FinishStagedActorMove Func_02003268
#define FinishStagedActorEffect Func_02003394
#define SetStagedActorTransition Func_02003278

void StagedActor_AdvanceActorPair(void) {
    s32 dst[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *block;
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
    block = FindBlockingStagedActor(dst, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

    dst[0] = next->x.value;
    dst[1] = next->y + 0x100000;
    dst[2] = next->z.value;
    block = FindElevatedBlockingStagedActor(dst, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

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
