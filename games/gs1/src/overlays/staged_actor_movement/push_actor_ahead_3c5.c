#include "types.h"
#include "staged_actor.h"

extern u32 Data_0200afd4[];

extern struct StagedActor *Func_02002f24(s32 arg0);
extern struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
extern struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
extern s32 Func_02002f88(struct StagedActor *arg0, s32 *arg1);
extern void Func_02002f50(struct StagedActor *arg0, s32 arg1);
extern void Func_02002f40(s32 arg0);
extern void Func_02003156(s32 arg0);
extern void Func_02002f8e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02002f9e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02002fac(struct StagedActor *arg0);
extern void SelectStagedActorSlot(void);
extern void Func_02002fbc(struct StagedActor *arg0, s32 arg1);

#define StagedActorStepTable Data_0200afd4
#define StagedActor_PushActorAhead Func_020000c4
#define GetStagedActor Func_02002f24
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02002f88
#define SetStagedActorMode Func_02002f50
#define SelectStagedActorSlot Func_02002f40
#define StartStagedActorEffect Func_02003156
#define StartNextStagedActorMove Func_02002f8e
#define StartLeadStagedActorMove Func_02002f9e
#define FinishStagedActorMove Func_02002fac
#define FinishStagedActorEffect Func_02003178
#define SetStagedActorTransition Func_02002fbc

void StagedActor_PushActorAhead(void) {
    s32 dest[3];
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
    dest[0] = lead->x.value + (step & 0xffff0000);
    dest[1] = lead->y;
    step <<= 16;
    dest[2] = lead->z.value + step;
    next = FindNextStagedActor(dest, lead);
    if (next == 0) return;

    step = StagedActorStepTable[dir];
    dest[0] = next->x.value + (step & 0xffff0000);
    dest[1] = next->y;
    step <<= 16;
    dest[2] = next->z.value + step;
    block = FindBlockingStagedActor(dest, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

    dest[0] = next->x.value;
    dest[1] = next->y + 0x100000;
    dest[2] = next->z.value;
    block = FindElevatedBlockingStagedActor(dest, next);
    if (block != 0 && (block->collision_flags & 1) != 0) return;

    next->transition_mode = 2;
    step = StagedActorStepTable[dir];
    dest[0] = next->x.value + (step & 0xffff0000);
    dest[1] = next->y;
    step <<= 16;
    dest[2] = next->z.value + step;
    if (CanStartStagedActorMove(next, dest) > 0) return;

    zero = next->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead, 8);
    rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next->move_rate_x = rate;
    next->move_rate_z = rate;
    StartNextStagedActorMove(next, dest[0], dest[1], dest[2]);
    lead->move_rate_x = rate;
    lead->move_rate_z = rate;
    StartLeadStagedActorMove(lead, dest[0], dest[1], dest[2]);
    FinishStagedActorMove(next);
    FinishStagedActorEffect();
    next->x.value = dest[0];
    next->z.value = dest[2];
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
