#include "types.h"

#define StagedActorStepTable Data_0200b15c
#define GetStagedActor Func_0200315c
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_020031d0
#define SetStagedActorMode Func_020031b0
#define SelectStagedActorSlot Func_02003178
#define StartStagedActorEffect Func_020032de
#define StartNextStagedActorMove Func_020031e6
#define StartLeadStagedActorMove Func_020031f6
#define FinishStagedActorMove Func_02003204
#define FinishStagedActorEffect Func_02003300
#define SetStagedActorTransition Func_0200321c
#define AdvanceStagedActorPair Func_020000c4
#define AdvanceEffectMotion Func_02000da4
#define UpdateSwayingSceneObject Func_02002e70

#include "staged_actor.h"

/* Integrate position, velocity, rate and sprite angle for one scene effect.
   Signed division preserves decay toward zero for negative Z velocity. */
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct T {
    u8 pad00[30];
    u16 f1e;
};

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 pad10[32];
    s32 f30;
    s32 f34;
    s32 f38;
    s32 f3c;
    u8 pad40[16];
    struct T *f50;
};

extern u32 Data_0200b15c[];

struct StagedActor *Func_0200315c(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_020031d0(struct StagedActor *arg0, s32 *arg1);
void Func_020031b0(struct StagedActor *arg0, s32 arg1);
void Func_02003178(s32 arg0);
void Func_020032de(s32 arg0);
void Func_020031e6(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020031f6(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02003204(struct StagedActor *arg0);
void Func_02003300(void);
void Func_0200321c(struct StagedActor *arg0, s32 arg1);
s32 Func_02003d76();
s32 Func_02005e42(s32 arg0);
s32 Func_02005e58(s32 arg0);
s32 Func_02005e72(s32 arg0);
s32 Func_02005e70(void);
s32 Func_02005e76(void);

/* Relocated IWRAM helper: turns a value and a duration into a per-frame step. */

void AdvanceStagedActorPair(void)
{
    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 zero;

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

    zero = next_actor->transition_busy;
    if (zero != 0) return;

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
    next_actor->unknown_24 = zero;
    next_actor->unknown_2c = zero;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = zero;
    lead_actor->unknown_2c = zero;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

void AdvanceEffectMotion(struct Effect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* Macro-shaped block keeps the following Z load after the Y store. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Func_02003d76(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

s32 UpdateSwayingSceneObject(struct S *object)
{
    struct T *sprite = object->f50;
    s32 vertical_offset = Func_02005e42(object->f30) * 2;
    s32 random_b;
    s32 random_a;

    if (vertical_offset > 0) {
        vertical_offset = -vertical_offset;
    }
    object->f08 = object->f38 + Func_02005e58(object->f30) * 2;
    object->f0c = object->f3c + vertical_offset;
    sprite->f1e = (u16)(Func_02005e72(object->f30 + 0x8000) / 8);
    random_a = Func_02005e70();
    random_b = Func_02005e76();
    object->f30 += (((u32)(random_a << 9)) >> 16) + (((u32)(random_b << 9)) >> 16) + 0x400;
    return 0;
}
