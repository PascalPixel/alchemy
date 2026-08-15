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

void Func_020000c4(void) {
    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 zero;

    lead_actor = Func_02002cf8(0);
    facing_index = lead_actor->direction_and_kind >> 12;
    step = Data_0200ad68[facing_index];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = Func_02000176(destination, lead_actor);
    if (next_actor == 0) return;

    step = Data_0200ad68[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = Func_020001a2(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = Func_020001ce(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = Data_0200ad68[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (Func_02002d84(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    Func_02002d5c(lead_actor, 8);
    move_rate = 0x3333;
    Func_02002d44(15);
    Func_02002eea(185);
    next_actor->move_rate_x = move_rate;
    next_actor->move_rate_z = move_rate;
    Func_02002da2(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_rate;
    lead_actor->move_rate_z = move_rate;
    Func_02002db2(lead_actor, destination[0], destination[1], destination[2]);
    Func_02002dc0(next_actor);
    Func_02002f0c();
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
    Func_02002dc8(lead_actor, 1);
}
