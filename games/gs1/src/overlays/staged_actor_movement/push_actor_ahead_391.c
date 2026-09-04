#include "types.h"
#include "staged_actor.h"

extern u32 Data_0200ad68[];

extern struct StagedActor *Func_02002cf8(s32 actor_index);
extern struct StagedActor *Func_02000176(s32 *position, struct StagedActor *actor);
extern struct StagedActor *Func_020001a2(s32 *position, struct StagedActor *actor);
extern struct StagedActor *Func_020001ce(s32 *position, struct StagedActor *actor);
extern s32 Func_02002d84(struct StagedActor *actor, s32 *position);
extern void Func_02002d5c(struct StagedActor *actor, s32 mode);
extern void Func_02002d44(s32 actor_index);
extern void Func_02002eea(s32 effect_id);
extern void Func_02002da2(struct StagedActor *actor, s32 x, s32 y, s32 z);
extern void Func_02002db2(struct StagedActor *actor, s32 x, s32 y, s32 z);
extern void Func_02002dc0(struct StagedActor *actor);
extern void Func_02002f0c(void);
extern void Func_02002dc8(struct StagedActor *actor, s32 mode);

#define StagedActorStepTable Data_0200ad68
#define StagedActor_PushActorAhead Func_020000c4
#define GetStagedActor Func_02002cf8
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02002d84
#define SetStagedActorMode Func_02002d5c
#define SelectStagedActorSlot Func_02002d44
#define StartStagedActorEffect Func_02002eea
#define StartNextStagedActorMove Func_02002da2
#define StartLeadStagedActorMove Func_02002db2
#define FinishStagedActorMove Func_02002dc0
#define FinishStagedActorEffect Func_02002f0c
#define SetStagedActorTransition Func_02002dc8

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
