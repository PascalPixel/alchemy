#include "types.h"
#include "staged_actor.h"

/* map/locations/heidia/prologue/prologue_route_scene/actor_activation.c */
#define StopBlockedActorMotion Func_020002a8
#define PositionSceneActorPair Func_02001774
#define ActivateSceneActor8 Func_020019b4
#define ActivateSceneActor10 Func_020019c0
#define ActivateSceneActor11 Func_020019cc
#define ActivateSceneActor12 Func_020019d8
#define ActivateSceneActor13 Func_020019e4
#define ActivateSceneActor15 Func_020019f0
#define ActivateSceneActor17 Func_020019fc
#define ActivateSceneActor18 Func_02001a08
#define ActivateSceneActor9 Func_02001a14
#define ActivateSceneActor19 Func_02001a20
#define ActivateSceneActor14 Func_02001a2c
#define ActivateSceneActor16 Func_02001a38
typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6]; u16 unk6; s32 unk8; s32 unkC; s32 unk10;
    u8 filler14[0x10]; s32 unk24; u8 filler28[4]; s32 unk2C;
    u8 filler30[8]; s32 unk38; u8 filler3C[4]; s32 unk40;
    u8 filler44[0xC]; Sub *unk50;
} Ent;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

extern s32 Data_0200b15c[];
extern s32 Data_0200b19c[];
extern s32 Data_02000240[];

Ent *Func_02000342(Desc *, Ent *);
s32 Func_02003352(Ent *, Desc *);
Obj *Func_0200481e(s32);
Obj *Func_02004826(s32);
void Func_02004814(void);
void Func_020047ee(Obj *, s32, s32, s32);
void Func_020047de(Obj *, s32);
void Func_0200481a(Obj *, s32, s32, s32);
void Func_02004816(Obj *, s32);
void Func_02004820(Obj *, s32);
void Func_02004946(s32);
void Func_0200484c(Obj *);
void Func_02004954(s32);
void Func_020048b8(void);
void Func_020032be(int arg0);
void Func_020032ca(int arg0);
void Func_020032d6(int arg0);
void Func_020032e2(int arg0);
void Func_020032ee(int arg0);
void Func_020032fa(int arg0);
void Func_02003306(int arg0);
void Func_02003312(int arg0);
void Func_0200331e(int arg0);
void Func_0200332a(int arg0);
void Func_02003336(int arg0);
void Func_02003342(int arg0);

s32 StopBlockedActorMotion(Ent *actor)
{
    Desc destination;
    u32 direction_index;
    s32 step;
    Ent *blocking_actor;

    direction_index = actor->unk6 >> 12;
    step = Data_0200b15c[direction_index];
    destination.unk0 = actor->unk8 + (step & 0xffff0000);
    destination.unk4 = actor->unkC;
    step = step << 16;
    destination.unk8 = actor->unk10 + step;
    blocking_actor = Func_02000342(&destination, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = *blocking_actor->unk50->unk28;
        s32 *allowed_kinds = Data_0200b19c;

        do {
            if (blocking_kind == *allowed_kinds++) {
                goto done;
            }
            kind_index++;
        } while (kind_index <= 5);
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
    step = Data_0200b15c[direction_index];
    destination.unk0 = actor->unk8 + (step & 0xffff0000);
    destination.unk4 = actor->unkC;
    step = step << 16;
    destination.unk8 = actor->unk10 + step;
    if (Func_02003352(actor, &destination) > 0) {
        actor->unk24 = 0;
        actor->unk2C = 0;
        actor->unk38 = 0x80000000;
        actor->unk40 = 0x80000000;
    }
done:
    return 0;
}

void PositionSceneActorPair(s32 actor_id, s32 x_offset, s32 z_offset)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_0200481e(Data_02000240[125]);
    q = Func_02004826(actor_id);
    Func_02004814();
    {
        x = ((p->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_020047ee(p, x, p->f0c, y);
    }
    Func_020047de(p, 27);
    {
        x = ((q->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_0200481a(q, x, q->f0c, y);
    }
    if (x_offset < 0 || z_offset < 0) {
        Func_02004816(q, 4);
    } else {
        Func_02004820(q, 3);
    }
    Func_02004946(226);
    Func_0200484c(p);
    Func_02004954(288);
    Func_020048b8();
}

void ActivateSceneActor8(void)
{
    Func_020032be(8);
}

void ActivateSceneActor10(void)
{
    Func_020032ca(10);
}

void ActivateSceneActor11(void)
{
    Func_020032d6(11);
}

void ActivateSceneActor12(void)
{
    Func_020032e2(12);
}

void ActivateSceneActor13(void)
{
    Func_020032ee(13);
}

void ActivateSceneActor15(void)
{
    Func_020032fa(15);
}

void ActivateSceneActor17(void)
{
    Func_02003306(17);
}

void ActivateSceneActor18(void)
{
    Func_02003312(18);
}

void ActivateSceneActor9(void)
{
    Func_0200331e(9);
}

void ActivateSceneActor19(void)
{
    Func_0200332a(19);
}

void ActivateSceneActor14(void)
{
    Func_02003336(14);
}

void ActivateSceneActor16(void)
{
    Func_02003342(16);
}
#undef StopBlockedActorMotion
#undef PositionSceneActorPair
#undef ActivateSceneActor8
#undef ActivateSceneActor10
#undef ActivateSceneActor11
#undef ActivateSceneActor12
#undef ActivateSceneActor13
#undef ActivateSceneActor15
#undef ActivateSceneActor17
#undef ActivateSceneActor18
#undef ActivateSceneActor9
#undef ActivateSceneActor19
#undef ActivateSceneActor14
#undef ActivateSceneActor16

/* map/locations/heidia/prologue/prologue_route_scene/actor_motion.c */
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
#undef StagedActorStepTable
#undef GetStagedActor
#undef FindNextStagedActor
#undef FindBlockingStagedActor
#undef FindElevatedBlockingStagedActor
#undef CanStartStagedActorMove
#undef SetStagedActorMode
#undef SelectStagedActorSlot
#undef StartStagedActorEffect
#undef StartNextStagedActorMove
#undef StartLeadStagedActorMove
#undef FinishStagedActorMove
#undef FinishStagedActorEffect
#undef SetStagedActorTransition
#undef AdvanceStagedActorPair
#undef AdvanceEffectMotion
#undef UpdateSwayingSceneObject

/* map/locations/heidia/prologue/prologue_route_scene/actor_routes.c */
#define CheckActorPathSouth Func_02001b14
#define CheckActorPathNorth Func_02001b40
#define CheckActorPathWest Func_02001b68
#define CheckActorPathEast Func_02001b94
#define UpdateEscapeRouteForActorPositions Func_02001bbc
#define UpdateActor8ReturnRoute Func_02001c84
#define UpdateActor10RetreatRoute Func_02001d2c
#define UpdateActor10AdvanceRoute Func_02001de0
#define UpdateActor11WestRoute Func_02001e5c
#define UpdateActor11EastRoute Func_02001ef4
#define UpdateActor12WestRoute Func_02001f78
#define UpdateActor12EastRoute Func_02002004
#define UpdateActor13WestRoute Func_02002080
#define UpdateActor13EastRoute Func_0200216c
#define UpdateActor15WestRoute Func_02002200
#define UpdateActor15EastRoute Func_020022c8
#define UpdateActor17SouthRoute Func_020023c4
#define UpdateActor17NorthRoute Func_02002450
#define AdvanceActor18AlongEscapeRoute Func_020025f0
#define AdvanceActor9AlongEscapeRoute Func_02002848
#define RetreatActor14AlongEscapeRoute Func_02002b80
#define AdvanceActor14AlongEscapeRoute Func_02002c4c
#define RetreatActor16AlongEscapeRoute Func_02002d0c
#define AdvanceActor16AlongEscapeRoute Func_02002dd0
struct S { s32 pad[2]; s32 f08; s32 f0c; s32 f10; };

struct V { s32 a; s32 b; s32 c; };

struct S_02001bbc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001c84 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001d2c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001de0 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001e5c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001ef4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001f78 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002004 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002080 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_0200216c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002200 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020022c8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020023c4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002450 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S *Func_02004ba4(s32 arg0);
s32 Func_02003576(struct S *arg0, struct V *arg1);
struct S *Func_02004bd0(s32 arg0);
s32 Func_020035a4(struct S *arg0, struct V *arg1);
struct S *Func_02004bf8(s32 arg0);
s32 Func_020035ca(struct S *arg0, struct V *arg1);
struct S *Func_02004c24(s32 arg0);
s32 Func_020035f8(struct S *arg0, struct V *arg1);
struct S *Func_02004c4c(s32 arg0);
struct S *Func_02004c56(s32 arg0);
struct S *Func_02004c60(s32 arg0);
struct S *Func_02004c6a(s32 arg0);
struct S *Func_02004cda(s32 arg0);
void Func_02003376(s32 arg0, s32 arg1, s32 arg2);
void Func_02003386(s32 arg0, s32 arg1, s32 arg2);
void Func_020033b2(s32 arg0, s32 arg1, s32 arg2);
void Func_02003758(void);
void Func_02004c04(s32 arg0);
void Func_02004c90(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004ca0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02004d14(s32 arg0);
struct S *Func_02004d1e(s32 arg0);
struct S *Func_02004d28(s32 arg0);
struct S *Func_02004d84(s32 arg0);
void Func_0200342a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003436(s32 arg0, s32 arg1, s32 arg2);
void Func_02003440(s32 arg0, s32 arg1, s32 arg2);
void Func_02003454(s32 arg0, s32 arg1, s32 arg2);
void Func_02003464(s32 arg0, s32 arg1, s32 arg2);
void Func_02004cae(s32 arg0);
void Func_02004d3a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004d4a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02004dbc(s32 arg0);
struct S *Func_02004dc6(s32 arg0);
struct S *Func_02004dd0(s32 arg0);
struct S *Func_02004dda(s32 arg0);
struct S *Func_02004e38(s32 arg0);
void Func_020034f0(s32 arg0, s32 arg1, s32 arg2);
void Func_02003512(s32 arg0, s32 arg1, s32 arg2);
void Func_02004d62(s32 arg0);
void Func_02004dee(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004dfe(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02004e70(s32 arg0);
struct S *Func_02004e7a(s32 arg0);
struct S *Func_02004eb2(s32 arg0);
void Func_0200357c(s32 arg0, s32 arg1, s32 arg2);
void Func_02003588(s32 arg0, s32 arg1, s32 arg2);
void Func_02003592(s32 arg0, s32 arg1, s32 arg2);
void Func_02004ddc(s32 arg0);
void Func_02004e68(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004e78(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02004eec(s32 arg0);
struct S *Func_02004ef6(s32 arg0);
struct S *Func_02004f08(s32 arg0);
struct S *Func_02004f1e(s32 arg0);
struct S *Func_02004f4c(s32 arg0);
void Func_0200361e(s32 arg0, s32 arg1, s32 arg2);
void Func_0200362c(s32 arg0, s32 arg1, s32 arg2);
void Func_02004e76(s32 arg0);
void Func_02004f02(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004f12(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02004f84_a(s32 arg0);
struct S *Func_02004f8e(s32 arg0);
struct S *Func_02004fa0(s32 arg0);
struct S *Func_02004fce(s32 arg0);
void Func_0200369e(s32 arg0, s32 arg1, s32 arg2);
void Func_020036ae(s32 arg0, s32 arg1, s32 arg2);
void Func_02004ef8(s32 arg0);
void Func_02004f84_b(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02004f94(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005008(s32 arg0);
struct S *Func_02005012_a(s32 arg0);
struct S *Func_0200505c(s32 arg0);
void Func_02003714(s32 arg0, s32 arg1, s32 arg2);
void Func_0200372a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003736(s32 arg0, s32 arg1, s32 arg2);
void Func_02004f86(s32 arg0);
void Func_02005012_b(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02005022(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005094(s32 arg0);
struct S *Func_0200509e(s32 arg0);
struct S *Func_020050d6(s32 arg0);
void Func_0200379c(s32 arg0, s32 arg1, s32 arg2);
void Func_020037b0(s32 arg0, s32 arg1, s32 arg2);
void Func_02005000(s32 arg0);
void Func_0200508c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_0200509c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005110(s32 arg0);
struct S *Func_0200511a(s32 arg0);
struct S *Func_02005124(s32 arg0);
struct S *Func_0200512e(s32 arg0);
struct S *Func_020051c4(s32 arg0);
void Func_02003846(s32 arg0, s32 arg1, s32 arg2);
void Func_02003870(s32 arg0, s32 arg1, s32 arg2);
void Func_0200389e(s32 arg0, s32 arg1, s32 arg2);
void Func_020050ee(s32 arg0);
void Func_0200517a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_0200518a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_020051fc(s32 arg0);
struct S *Func_02005206(s32 arg0);
void Func_02005210(s32 arg0);
struct S *Func_02005258(s32 arg0);
void Func_0200390a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003932(s32 arg0, s32 arg1, s32 arg2);
void Func_02005182(s32 arg0);
void Func_0200520e(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_0200521e(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005290(s32 arg0);
struct S *Func_0200529a(s32 arg0);
struct S *Func_020052a4(s32 arg0);
struct S *Func_020052ae(s32 arg0);
struct S *Func_02005320(s32 arg0);
void Func_020039be(s32 arg0, s32 arg1, s32 arg2);
void Func_020039d6(s32 arg0, s32 arg1, s32 arg2);
void Func_020039fa(s32 arg0, s32 arg1, s32 arg2);
void Func_0200524a(s32 arg0);
void Func_020052d6(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_020052e6(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005358(s32 arg0);
struct S *Func_02005362(s32 arg0);
struct S *Func_0200536c(s32 arg0);
struct S *Func_02005376(s32 arg0);
struct S *Func_0200541c(s32 arg0);
void Func_02003a86(s32 arg0, s32 arg1, s32 arg2);
void Func_02003a9a(s32 arg0, s32 arg1, s32 arg2);
void Func_02003aaa(s32 arg0, s32 arg1, s32 arg2);
void Func_02003af6(s32 arg0, s32 arg1, s32 arg2);
void Func_02005346(s32 arg0);
void Func_020053d2(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_020053e2(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_02005454(s32 arg0);
struct S *Func_0200545e(s32 arg0);
struct S *Func_02005468(s32 arg0);
struct S *Func_020054a6(s32 arg0);
void Func_02003b80(s32 arg0, s32 arg1, s32 arg2);
void Func_020053d0(s32 arg0);
void Func_0200545c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_0200546c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct S *Func_020054e0(s32 arg0);
struct S *Func_020054ea(s32 arg0);
struct S *Func_0200551e(s32 arg0);
void Func_02003be8(s32 arg0, s32 arg1, s32 arg2);
void Func_02003bf8(s32 arg0, s32 arg1, s32 arg2);
void Func_02005448(s32 arg0);
void Func_020054d4(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_020054e4(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
u8 *Func_0200568a();
u8 *Func_02005694();
u8 *Func_0200569e();
u8 *Func_02005758();
void Func_0200571e();
void Func_02003da8();
void Func_02003db2();
void Func_02003dce();
void Func_02003dda();
void Func_02003df0();
void Func_02003dfc();
void Func_02003e12();
void Func_02003e22();
void Func_02003e32();
u8 *Func_020058e8();
u8 *Func_020058f2();
u8 *Func_020058fc();
u8 *Func_02005906();
u8 *Func_020059d4();
void Func_0200599c();
void Func_0200401a();
void Func_02004024();
void Func_02004044();
void Func_02004050();
void Func_02004070();
void Func_0200407c();
void Func_02004094();
void Func_020040a4();
void Func_020040b4();
u8 *Func_02005c1a();
u8 *Func_02005c24();
u8 *Func_02005c2e();
u8 *Func_02005ca4();
void Func_02005c6a();
void Func_0200437e();
u8 *Func_02005ce6();
u8 *Func_02005cf0();
u8 *Func_02005cfa();
u8 *Func_02005d62();
void Func_02005d28();
void Func_0200443c();
u8 *Func_02005da6();
u8 *Func_02005db0();
u8 *Func_02005dba();
u8 *Func_02005e28();
void Func_02005dee();
void Func_02004502();
u8 *Func_02005e6a();
u8 *Func_02005e74();
u8 *Func_02005ec8();
void Func_02005e8e();
void Func_020045a2();

/* Advance actor 18 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */

/* This overlay's transition starter at 0x02001774, one name per site. */

/* Advance actor 9 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

/* Return leg of the slot-14 transition beat: negated ids. */

/* This overlay's transition starter at 0x02001774. */

/* Outbound leg of the slot-14 transition beat. */

/* Return leg of the slot-16 transition beat: negated ids. */

/* Outbound leg of the slot-16 transition beat. */

u8 *Func_02005680();                    /* scene entity by selector */

void Func_02005682();                   /* wait N frames */

void Func_0200570e();                   /* collision repaint, six arguments */

u8 *Func_020058dc();                    /* scene entity by selector */

void Func_020058fe();                   /* wait N frames */

void Func_0200598c();                   /* collision repaint, six arguments */

u8 *Func_02005c10();                    /* scene entity by selector */

void Func_02005bce();                   /* wait N frames */

void Func_02005c5a();                   /* collision repaint, six arguments */

u8 *Func_02005cdc();                    /* scene entity by selector */

void Func_02005c8c();                   /* wait N frames */

void Func_02005d18();                   /* collision repaint, six arguments */

u8 *Func_02005d9c();                    /* scene entity by selector */

void Func_02005d52();                   /* wait N frames */

void Func_02005dde();                   /* collision repaint, six arguments */

u8 *Func_02005e60();                    /* scene entity by selector */

void Func_02005df2();                   /* wait N frames */

void Func_02005e7e();                   /* collision repaint, six arguments */

s32 CheckActorPathSouth(void)
{
    struct S *actor = Func_02004ba4(0);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + -0x200000;
    return Func_02003576(actor, &destination);
}

s32 CheckActorPathNorth(void)
{
    struct S *actor = Func_02004bd0(0);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + 0x200000;
    return Func_020035a4(actor, &destination);
}

s32 CheckActorPathWest(void)
{
    struct S *actor = Func_02004bf8(0);
    struct V destination;

    destination.a = actor->f08 + -0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return Func_020035ca(actor, &destination);
}

s32 CheckActorPathEast(void)
{
    struct S *actor = Func_02004c24(0);
    struct V destination;

    destination.a = actor->f08 + 0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return Func_020035f8(actor, &destination);
}

void UpdateEscapeRouteForActorPositions(void)
{
    s32 actor_x = Func_02004c4c(8)->f08 >> 20;
    s32 actor_z = Func_02004c56(8)->f10 >> 20;
    s32 actor_12_x = Func_02004c60(12)->f08 >> 20;
    s32 actor_15_x = Func_02004c6a(15)->f08 >> 20;

    if (actor_z == 19) {
        if (actor_12_x == 24) {
            Func_020033b2(8, 0, -80);
        } else if (actor_15_x == 24) {
            Func_02003376(8, 0, -112);
            Func_020033b2(8, 0, -32);
        } else {
            Func_02003386(8, 0, -80);
            Func_020033b2(8, 0, -112);
        }
    } else if (actor_z == 14) {
        if (actor_12_x == 24) {
            return;
        }
        if (actor_15_x == 24) {
            Func_020033b2(8, 0, -64);
        } else {
            Func_020033b2(8, 0, -112);
        }
    } else if (actor_z == 10) {
        if (actor_15_x == 24) {
            return;
        }
        Func_020033b2(8, 0, -48);
    } else {
        Func_02003758();
        return;
    }
    Func_02004c04(2);
    {
        s32 route_end_z = Func_02004cda(8)->f10 >> 20;
        s32 route_x = actor_x - 1;

        Func_02004c90(route_x, actor_z, 3, 1, route_x, route_end_z);
    }
    Func_02004ca0(0, 0, 3, 1, actor_x - 1, actor_z);
}

void UpdateActor8ReturnRoute(void)
{
    s32 x = Func_02004d14(8)->f08 >> 20;
    s32 y = Func_02004d1e(8)->f10 >> 20;
    s32 z = Func_02004d28(12)->f08 >> 20;

    if (y == 7) {
        if (z == 24) {
            Func_0200342a(8, 0, 48);
        } else {
            Func_02003436(8, 0, 80);
            Func_02003440(8, 0, 112);
        }
    } else if (y == 10) {
        if (z == 24) {
            return;
        }
        Func_02003454(8, 0, 144);
    } else if (y == 14) {
        Func_02003464(8, 0, 80);
    } else {
        return;
    }
    Func_02004cae(2);
    {
        s32 k = Func_02004d84(8)->f10 >> 20;
        s32 m = x - 1;

        Func_02004d3a(m, y, 3, 1, m, k);
    }
    Func_02004d4a(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10RetreatRoute(void)
{
    s32 x = Func_02004dbc(10)->f08 >> 20;
    s32 y = Func_02004dc6(10)->f10 >> 20;
    s32 z = Func_02004dd0(13)->f08 >> 20;
    s32 w = Func_02004dda(15)->f08 >> 20;

    if (y == 18) {
        if (w >= 31 && w <= 33) {
            Func_02003512(10, 0, -128);
        } else if (z >= 31 && z <= 33) {
            Func_02003512(10, 0, -128);
        } else {
            Func_020034f0(10, 0, -112);
            Func_02003512(10, 0, -64);
        }
    } else if (y == 10) {
        if (w >= 31 && w <= 33) {
            return;
        }
        if (z >= 31 && z <= 33) {
            return;
        }
        Func_02003512(10, 0, -48);
    } else if (y == 7) {
        return;
    }
    Func_02004d62(2);
    {
        s32 k = Func_02004e38(10)->f10 >> 20;
        s32 m = x - 1;

        Func_02004dee(m, y, 3, 1, m, k);
    }
    Func_02004dfe(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10AdvanceRoute(void)
{
    s32 x = Func_02004e70(10)->f08 >> 20;
    s32 y = Func_02004e7a(10)->f10 >> 20;

    if (y != 18) {
        if (y == 10) {
            Func_0200357c(10, 0, 128);
        } else {
            Func_02003588(10, 0, 112);
            Func_02003592(10, 0, 64);
        }
        Func_02004ddc(2);
        {
            s32 k = Func_02004eb2(10)->f10 >> 20;
            s32 m = x - 1;

            Func_02004e68(m, y, 3, 1, m, k);
        }
        Func_02004e78(0, 0, 3, 1, x - 1, y);
    }
}

void UpdateActor11WestRoute(void)
{
    s32 x = Func_02004eec(11)->f08 >> 20;
    s32 y = Func_02004ef6(11)->f10 >> 20;

    if (x != 30) {
        if (x == 34) {
            if ((Func_02004f08(10)->f10 >> 20) == 18) {
                return;
            }
            Func_0200361e(11, -64, 0);
        } else if (x == 36) {
            if ((Func_02004f1e(10)->f10 >> 20) == 18) {
                Func_0200361e(11, -32, 0);
            } else {
                Func_0200362c(11, -96, 0);
            }
        }
        Func_02004e76(2);
        {
            s32 k = Func_02004f4c(11)->f08 >> 20;
            s32 m = y - 1;

            Func_02004f02(x, m, 1, 3, k, m);
        }
        Func_02004f12(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor11EastRoute(void)
{
    s32 x = Func_02004f84_a(11)->f08 >> 20;
    s32 y = Func_02004f8e(11)->f10 >> 20;

    if (x != 36) {
        if (x == 30) {
            if ((Func_02004fa0(10)->f10 >> 20) == 18) {
                return;
            }
            Func_0200369e(11, 96, 0);
        } else if (x == 34) {
            Func_020036ae(11, 32, 0);
        }
        Func_02004ef8(2);
        {
            s32 k = Func_02004fce(11)->f08 >> 20;
            s32 m = y - 1;

            Func_02004f84_b(x, m, 1, 3, k, m);
        }
        Func_02004f94(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor12WestRoute(void)
{
    s32 x = Func_02005008(12)->f08 >> 20;
    s32 y = Func_02005012_a(12)->f10 >> 20;

    if (x == 36) {
        Func_02003714(12, -96, 0);
        Func_02003736(12, -96, 0);
    } else if (x == 34) {
        Func_0200372a(12, -96, 0);
        Func_02003736(12, -64, 0);
    } else if (x == 24) {
        return;
    }
    Func_02004f86(2);
    {
        s32 k = Func_0200505c(12)->f08 >> 20;
        s32 m = y - 1;

        Func_02005012_b(x, m, 1, 3, k, m);
    }
    Func_02005022(0, 0, 1, 3, x, y - 1);
}

void UpdateActor12EastRoute(void)
{
    s32 x = Func_02005094(12)->f08 >> 20;
    s32 y = Func_0200509e(12)->f10 >> 20;

    if (x == 24) {
        Func_0200379c(12, 96, 0);
        Func_020037b0(12, 96, 0);
    } else if (x == 34) {
        Func_020037b0(12, 32, 0);
    } else if (x == 36) {
        return;
    }
    Func_02005000(2);
    {
        s32 k = Func_020050d6(12)->f08 >> 20;
        s32 m = y - 1;

        Func_0200508c(x, m, 1, 3, k, m);
    }
    Func_0200509c(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13WestRoute(void)
{
    s32 x = Func_02005110(13)->f08 >> 20;
    s32 y = Func_0200511a(13)->f10 >> 20;
    s32 z = Func_02005124(10)->f10 >> 20;
    s32 w = Func_0200512e(15)->f08 >> 20;

    if (x == 36) {
        if (w == 34) {
            Func_0200389e(13, -16, 0);
        } else if (z == 7) {
            Func_0200389e(13, -32, 0);
        } else if (w == 30) {
            Func_0200389e(13, -80, 0);
        } else {
            Func_02003846(13, -96, 0);
            Func_0200389e(13, -80, 0);
        }
    } else if (x == 35) {
        if (w == 34) {
            return;
        } else if (z == 7) {
            Func_0200389e(13, -16, 0);
        } else if (w == 30) {
            Func_0200389e(13, -64, 0);
        } else {
            Func_02003870(13, -80, 0);
            Func_0200389e(13, -80, 0);
        }
    } else if (x == 34) {
        if (z == 7) {
            return;
        }
        if (w == 30) {
            Func_0200389e(13, -48, 0);
        } else {
            Func_0200389e(13, -144, 0);
        }
    } else if (x == 31) {
        if (w == 30) {
            return;
        }
        Func_0200389e(13, -96, 0);
    } else if (x == 25) {
        return;
    }
    Func_020050ee(2);
    {
        s32 k = Func_020051c4(13)->f08 >> 20;
        s32 m = y - 1;

        Func_0200517a(x, m, 1, 3, k, m);
    }
    Func_0200518a(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13EastRoute(void)
{
    s32 x = Func_020051fc(13)->f08 >> 20;
    s32 y = Func_02005206(13)->f10 >> 20;

    Func_02005210(15);
    if (x == 25) {
        Func_0200390a(13, 96, 0);
        Func_02003932(13, 80, 0);
    } else if (x == 31) {
        Func_02003932(13, 80, 0);
    } else if (x == 34) {
        Func_02003932(13, 32, 0);
    } else if (x == 35) {
        Func_02003932(13, 16, 0);
    } else if (x == 36) {
        return;
    }
    Func_02005182(2);
    {
        s32 k = Func_02005258(13)->f08 >> 20;
        s32 m = y - 1;

        Func_0200520e(x, m, 1, 3, k, m);
    }
    Func_0200521e(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15WestRoute(void)
{
    s32 x = Func_02005290(15)->f08 >> 20;
    s32 y = Func_0200529a(15)->f10 >> 20;
    s32 z = Func_020052a4(8)->f10 >> 20;
    s32 w = Func_020052ae(10)->f10 >> 20;

    if (x == 35) {
        if (w == 7) {
            Func_020039fa(15, -16, 0);
        } else if (z == 7) {
            Func_020039fa(15, -112, 0);
        } else {
            Func_020039be(15, -96, 0);
            Func_020039fa(15, -80, 0);
        }
    } else if (x == 34) {
        if (w == 7) {
            return;
        }
        Func_020039d6(15, -96, 0);
        Func_020039fa(15, -64, 0);
    } else if (x == 33) {
        Func_020039fa(15, -144, 0);
    } else if (x == 31) {
        Func_020039fa(15, -80, 0);
    } else if (x == 30) {
        Func_020039fa(15, -96, 0);
    } else if (x == 24) {
        return;
    }
    Func_0200524a(2);
    {
        s32 k = Func_02005320(15)->f08 >> 20;
        s32 m = y - 1;

        Func_020052d6(x, m, 1, 3, k, m);
    }
    Func_020052e6(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15EastRoute(void)
{
    s32 x = Func_02005358(15)->f08 >> 20;
    s32 y = Func_02005362(15)->f10 >> 20;
    s32 z = Func_0200536c(10)->f10 >> 20;
    s32 w = Func_02005376(13)->f08 >> 20;

    if (x == 24) {
        if (z == 7 || w == 31) {
            Func_02003af6(15, 96, 0);
        } else if (w == 34) {
            Func_02003a86(15, 64, 0);
            Func_02003af6(15, 80, 0);
        } else if (w == 35) {
            Func_02003a9a(15, 80, 0);
            Func_02003af6(15, 80, 0);
        } else {
            Func_02003aaa(15, 80, 0);
            Func_02003af6(15, 96, 0);
        }
    } else if (x == 30 || w == 31) {
        if (z == 7) {
            return;
        }
        if (w == 34) {
            Func_02003af6(15, 48, 0);
        } else if (w == 35) {
            Func_02003af6(15, 64, 0);
        } else {
            Func_02003af6(15, 80, 0);
        }
    } else if (x == 33) {
        if (w == 34) {
            return;
        }
        if (w == 35) {
            Func_02003af6(15, 16, 0);
        } else {
            Func_02003af6(15, 32, 0);
        }
    } else if (x == 34) {
        Func_02003af6(15, 16, 0);
    } else if (x == 35) {
        return;
    }
    Func_02005346(2);
    {
        s32 k = Func_0200541c(15)->f08 >> 20;
        s32 m = y - 1;

        Func_020053d2(x, m, 1, 3, k, m);
    }
    Func_020053e2(0, 0, 1, 3, x, y - 1);
}

void UpdateActor17SouthRoute(void)
{
    s32 x = Func_02005454(17)->f08 >> 20;
    s32 y = Func_0200545e(17)->f10 >> 20;
    s32 z = Func_02005468(19)->f08 >> 20;

    if (y == 19) {
        if (z >= 3 && z <= 5) {
            Func_02003b80(17, 0, -16);
        } else {
            Func_02003b80(17, 0, -64);
        }
    } else if (y == 18) {
        if (z >= 3 && z <= 5) {
            return;
        }
        Func_02003b80(17, 0, -48);
    } else if (y == 15) {
        return;
    }
    Func_020053d0(2);
    {
        s32 k = Func_020054a6(17)->f10 >> 20;
        s32 m = x - 1;

        Func_0200545c(m, y, 3, 1, m, k);
    }
    Func_0200546c(0, 0, 3, 1, x - 1, y);
}

void UpdateActor17NorthRoute(void)
{
    s32 x = Func_020054e0(17)->f08 >> 20;
    s32 y = Func_020054ea(17)->f10 >> 20;

    if (y == 15) {
        Func_02003be8(17, 0, 64);
    } else if (y == 18) {
        Func_02003bf8(17, 0, 16);
    } else if (y == 19) {
        return;
    }
    Func_02005448(2);
    {
        s32 k = Func_0200551e(17)->f10 >> 20;

        s32 m = x - 1;

        Func_020054d4(m, y, 3, 1, m, k);
    }
    Func_020054e4(0, 0, 3, 1, x - 1, y);
}

void AdvanceActor18AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;

    s32 permuted_5;
    permuted_5 = *(s32 *)(Func_02005680(18) + 8) >> 20;
    row = *(s32 *)(Func_0200568a(18) + 16) >> 20;
    column  = permuted_5;
    companion19Column = *(s32 *)(Func_02005694(19) + 8) >> 20;
    companion14Column = *(s32 *)(Func_0200569e(14) + 8) >> 20;

    if (row == 9) {
        if ((u32)(companion14Column - 6) <= 2) {
            goto transition32;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            goto transition80;
        }
        /* This arm runs two transitions back to back. */
        Func_02003da8(18, 0, 64);
        Func_02003db2(18, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 6) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            Func_02003dce(18, 0, 48);
        } else {
            Func_02003dda(18, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion19Column - 6) <= 2) {
transition32:
            Func_02003df0(18, 0, 32);
        } else {
            Func_02003dfc(18, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 6) <= 2) {
            return;
        }
transition80:
        Func_02003e12(18, 0, 80);
    } else if (row == 15) {
        Func_02003e22(18, 0, 64);
    } else if (row == 18) {
        Func_02003e32(18, 0, 16);
    } else if (row == 19) {
        return;
    }

    Func_02005682(2);

    column -= 1;
    Func_0200570e(column, row, 3, 1,
                  column, *(s32 *)(Func_02005758(18) + 16) >> 20);
    Func_0200571e(0, 0, 3, 1, column, row);
}

void AdvanceActor9AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;
    s32 companion16Column;

    s32 permuted_6;
    permuted_6 = *(s32 *)(Func_020058dc(9) + 8) >> 20;
    row = *(s32 *)(Func_020058e8(9) + 16) >> 20;
    column  = permuted_6;
    companion19Column = *(s32 *)(Func_020058f2(19) + 8) >> 20;
    companion14Column = *(s32 *)(Func_020058fc(14) + 8) >> 20;
    companion16Column = *(s32 *)(Func_02005906(16) + 8) >> 20;

    if (row == 8) {
        if ((u32)(companion16Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion14Column - 9) <= 2) {
            goto transition48;
        }
        if ((u32)(companion19Column - 9) > 2) {
            Func_0200401a(9, 0, 80);
        }
        /* Falls through into the id-96 site from both paths. */
        Func_02004024(9, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
transition48:
            Func_02004044(9, 0, 48);
        } else {
            Func_02004050(9, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
            Func_02004070(9, 0, 32);
        } else {
            Func_0200407c(9, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 9) <= 2) {
            return;
        }
        Func_02004094(9, 0, 80);
    } else if (row == 15) {
        Func_020040a4(9, 0, 64);
    } else if (row == 18) {
        Func_020040b4(9, 0, 16);
    }

    Func_020058fe(2);

    column -= 1;
    Func_0200598c(column, row, 3, 1,
                  column, *(s32 *)(Func_020059d4(9) + 16) >> 20);
    Func_0200599c(0, 0, 3, 1, column, row);
}

void RetreatActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005c10(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005c1a(14) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005c24(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005c2e(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 12) <= 2) {
            Func_0200437e(14, -16, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            Func_0200437e(14, -64, 0);
        } else {
            Func_0200437e(14, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 12) <= 2) {
            Func_0200437e(14, -48, 0);
        } else {
            Func_0200437e(14, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        Func_0200437e(14, -48, 0);
    } else if (column == 8) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        Func_0200437e(14, -32, 0);
    } else if (column == 6) {
        return;
    }

    Func_02005bce(2);

    rowM1 = row - 1;
    Func_02005c5a(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005ca4(14) + 8) >> 20, rowM1);
    Func_02005c6a(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005cdc(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005ce6(14) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005cf0(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005cfa(9) + 16) >> 20;

    if (column == 6) {
        if ((u32)(companion9Row - 12) <= 2) {
            Func_0200443c(14, 32, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            Func_0200443c(14, 64, 0);
        } else {
            Func_0200443c(14, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        Func_0200443c(14, 80, 0);
    } else if (column == 9) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        Func_0200443c(14, 64, 0);
    } else if (column == 12) {
        Func_0200443c(14, 16, 0);
    } else if (column == 13) {
        return;
    }

    Func_02005c8c(2);

    rowM1 = row - 1;
    Func_02005d18(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005d62(14) + 8) >> 20, rowM1);
    Func_02005d28(0, 0, 1, 3, column, rowM1);
}

void RetreatActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Func_02005d9c(16) + 8) >> 20;
    permuted_7 = *(s32 *)(Func_02005da6(16) + 16) >> 20;
    companion18Row = *(s32 *)(Func_02005db0(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Func_02005dba(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 9) <= 2) {
            Func_02004502(16, -16, 0);
        } else if ((u32)(companion18Row - 9) <= 2) {
            Func_02004502(16, -64, 0);
        } else {
            Func_02004502(16, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 9) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 9) <= 2) {
            Func_02004502(16, -48, 0);
        } else {
            Func_02004502(16, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 9) <= 2) {
            return;
        }
        Func_02004502(16, -48, 0);
    } else if (column == 8) {
        Func_02004502(16, -32, 0);
    } else if (column == 6) {
        return;
    }

    Func_02005d52(2);

    rowM1 = row - 1;
    Func_02005dde(column, rowM1, 1, 3,
                  *(s32 *)(Func_02005e28(16) + 8) >> 20, rowM1);
    Func_02005dee(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companionRow;

    s32 permuted_6;
    column = *(s32 *)(Func_02005e60(16) + 8) >> 20;
    permuted_6 = *(s32 *)(Func_02005e6a(16) + 16) >> 20;
    companionRow = *(s32 *)(Func_02005e74(9) + 16) >> 20;
    row  = permuted_6;

    if (column == 6) {
        if ((u32)(companionRow - 9) <= 2) {
            Func_020045a2(16, 32, 0);
        } else {
            Func_020045a2(16, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companionRow - 9) <= 2) {
            return;
        }
        Func_020045a2(16, 80, 0);
    } else if (column == 9) {
        Func_020045a2(16, 64, 0);
    } else if (column == 12) {
        Func_020045a2(16, 16, 0);
    } else if (column == 13) {
        return;
    }

    Func_02005df2(2);

    row -= 1;
    Func_02005e7e(column, row, 1, 3,
                  *(s32 *)(Func_02005ec8(16) + 8) >> 20, row);
    Func_02005e8e(0, 0, 1, 3, column, row);
}
#undef CheckActorPathSouth
#undef CheckActorPathNorth
#undef CheckActorPathWest
#undef CheckActorPathEast
#undef UpdateEscapeRouteForActorPositions
#undef UpdateActor8ReturnRoute
#undef UpdateActor10RetreatRoute
#undef UpdateActor10AdvanceRoute
#undef UpdateActor11WestRoute
#undef UpdateActor11EastRoute
#undef UpdateActor12WestRoute
#undef UpdateActor12EastRoute
#undef UpdateActor13WestRoute
#undef UpdateActor13EastRoute
#undef UpdateActor15WestRoute
#undef UpdateActor15EastRoute
#undef UpdateActor17SouthRoute
#undef UpdateActor17NorthRoute
#undef AdvanceActor18AlongEscapeRoute
#undef AdvanceActor9AlongEscapeRoute
#undef RetreatActor14AlongEscapeRoute
#undef AdvanceActor14AlongEscapeRoute
#undef RetreatActor16AlongEscapeRoute
#undef AdvanceActor16AlongEscapeRoute

/* map/locations/heidia/prologue/prologue_route_scene/actor_setup.c */
#define ConfigureSceneActor11 Func_02001508
#define ConfigureSceneActor12 Func_02001548
#define ConfigureSceneActor13 Func_0200158c
#define ConfigureSceneActor14 Func_020015d0
u8 *Func_02004598(s32 arg0);
s32 Func_020045aa(s32 arg0);
void Func_02004560(s32 arg0, s32 arg1);
void Func_02004580(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020045a0(s32 arg0);
u8 *Func_020045d8(s32 arg0);
s32 Func_020045ea(s32 arg0);
void Func_020045a0_a(s32 arg0, s32 arg1);
void Func_020045c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020045de(s32 arg0);
u8 *Func_0200461c(s32 arg0);
s32 Func_0200462e(s32 arg0);
void Func_020045e4(s32 arg0, s32 arg1);
void Func_02004604(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02004622(s32 arg0);
u8 *Func_02004660(s32 arg0);
s32 Func_02004672(s32 arg0);
void Func_02004628(s32 arg0, s32 arg1);
void Func_02004648(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02004658(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02004676(s32 arg0);

void ConfigureSceneActor11(s32 actor_id)
{
    s32 a = 0x1300000;
    s32 b = 0x1700000;
    u8 *p = Func_02004598(11);

    if (p != 0) {
        p[89] = 0;
    }
    Func_02004560(Func_020045aa(actor_id), 0);
    Func_02004580(0, a, b, 253);
    Func_020045a0(576);
}

void ConfigureSceneActor12(s32 actor_id)
{
    s32 a = 0x500000;
    s32 b = 0x1700000;
    u8 *p = Func_020045d8(12);

    if (p != 0) {
        p[89] = 0;
    }
    Func_020045a0_a(Func_020045ea(actor_id), 0);
    Func_020045c0(0, a, b, 253);
    Func_020045de(577);
}

void ConfigureSceneActor13(s32 actor_id)
{
    s32 a = 0x600000;
    s32 b = 0x1500000;
    u8 *p = Func_0200461c(13);

    if (p != 0) {
        p[89] = 0;
    }
    Func_020045e4(Func_0200462e(actor_id), 0);
    Func_02004604(0, a, b, 253);
    Func_02004622(578);
}

void ConfigureSceneActor14(s32 actor_id)
{
    s32 a = 0x900000;
    s32 b = 0x1400000;
    s32 c = 0x2f00000;
    s32 d = 0x1400000;
    u8 *p = Func_02004660(14);

    if (p != 0) {
        p[89] = 0;
    }
    Func_02004628(Func_02004672(actor_id), 0);
    Func_02004648(0, a, b, 253);
    Func_02004658(0, c, d, 253);
    Func_02004676(579);
}
#undef ConfigureSceneActor11
#undef ConfigureSceneActor12
#undef ConfigureSceneActor13
#undef ConfigureSceneActor14

/* map/locations/heidia/prologue/prologue_route_scene/effect_spawn.c */
#define SpawnMode14Effect Func_020009f4
#define SpawnMode15Effect Func_02000a4c
#define SpawnConfiguredSceneEffect Func_02000ae8
#define SpawnRadialEffectBurst Func_02000e00
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Options {
    u8 mode_bits;
    u8 pad01[3];
    s32 mode;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct Descriptor {
    s32 pad00[3];
    s32 duration;
};

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    u8 filler00[0x24];
    s32 callback;
};

extern struct Descriptor *Data_0200b2d4[];

u8 *Func_02003a0e(s32 kind, s32 x, s32 y, s32 z);
void Func_02003a64(u8 *object, s32 mode);
void Func_02003afc(u8 *object, s32 mode);
void Func_02003a7c(u8 *object, s32 mode);
u8 *Func_02003a66(s32 kind, s32 x, s32 y, s32 z);
void Func_02003ac0(u8 *object, s32 mode);
void Func_02003b58(u8 *object, s32 mode);
struct Effect *Func_02003b92();
struct Effect *Func_02003b38();
void Func_02003b42();
void Func_02003b5c();
void Func_02003c8e();
s32 Func_02003bd8();
s32 Func_02003bf0();
s32 Func_02003bfe();
void Func_02003c5c();
void Func_02003c6c();
struct SceneObject *Func_02003e96(void);
s32 Func_02003df8(s32);
s32 Func_02003dfc_a(s32);
s32 Func_02003df2(s32, s32);
void Func_0200194a(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);

/* Spawn and configure the mode-14 object variant. */

/* Spawn and configure the mode-15 object variant. */

/* Returns the party record; only its presentation block at +80 is read. */

u8 *SpawnMode14Effect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *object = Func_02003a0e(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        s32 mask = ~12;
        mask &= owner[9];
        owner[9] = mask;
        object[85] = 0;
        object[89] = 8;
        Func_02003a64(object, 0);
        Func_02003afc(object, 14);
        Func_02003a7c(object, 1);
        return object;
    }
    return 0;
}

u8 *SpawnMode15Effect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *object = Func_02003a66(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        s32 mask = ~12;
        mask &= owner[9];
        mask |= 4;
        owner[9] = mask;
        object[85] = 0;
        object[89] = 8;
        Func_02003ac0(object, 0);
        Func_02003b58(object, 15);
        {
            s32 mask2 = 254;
            mask2 &= object[35];
            mask2 |= 2;
            object[35] = mask2;
        }
        return object;
    }
    return 0;
}

/* Creates the effect record and returns it, or 0 on failure. */

/* Relocated IWRAM helper: turns a distance and a descriptor duration into a
 * per-frame step. */
void SpawnConfiguredSceneEffect(s32 x, s32 y,
                   s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
                   const struct Options *options)
{
    u32 table_offset;
    struct Effect *party;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *mode_block;
    u32 option_bits;
    u16 *tag;
    s32 duration;
    s32 first_delta;
    s32 accumulated;
    party = Func_02003b92(0);

    /* 128 << 13.  With that bit set and an options block present the effect's
     * kind comes from the options rather than from the default 222. */
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_02003b38(options->kind, x, y, z);
    } else {
        effect = Func_02003b38(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    mode_block = block;

    Func_02003b42(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Func_02003b5c(effect, Data_0200b2d4[table_offset >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;

    /* 0x02008ab1 is Func_02000ab0 with the Thumb bit: the per-frame
     * integrator. */
    effect->callback = 0x02008ab1;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    /* Bits 2 and 3 of the effect's mode byte are copied from the party's. */
    copied_bits = party->sprite->flags9 & 12;
    block_bits = *(volatile u8 *)&block->flags9;
    flag_mask = ~12;
    block->flags9 = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    /* Everything below is optional detail: the whole block is skipped unless
     * some high flag bit is set and an options record was supplied. */
    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {                   /* 128 << 9 */
        Func_02003c8e(effect, options->mode);
    }

    if ((flags & 0x20000) != 0) {                   /* 128 << 10 */
        effect->flags23 &= 0xfe;
        option_bits = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)mode_block + 9) & flag_mask)
                             | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {                   /* 128 << 12 */
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {                   /* 128 << 11 */
        const struct Descriptor *descriptor =
            Data_0200b2d4[table_offset >> 2];
        s32 delta;

        /* The 0x80000 test is the same register the previous block left live:
         * with a destination supplied the step is measured from it, otherwise
         * the target is biased by -1.0 in 16.16. */
        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = Func_02003bd8(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = Func_02003bf0(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        /* Only the FIRST call is per-arm.  The `b.n 0x02000c4c` at the end of
         * the first arm joins both arms onto the single second call site, so
         * the second delta is computed in each arm and the call is spelled
         * once. */
        effect->rate34 = Func_02003bfe(delta, duration);
    }

    if ((flags & 0x200000) != 0) {                  /* 128 << 14 */
        Func_02003c5c(effect, 1);
        Func_02003c6c(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0) {                  /* 128 << 15 */
        block->angle = options->angle;
    }

    if ((flags & 0x800000) != 0) {                  /* 128 << 16 */
        *tag = options->step;
    }

    if ((flags & 0x1000000) != 0) {                 /* 128 << 17 */
        effect->callback = options->callback;
    }
}

void SpawnRadialEffectBurst(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 angle_step;
    s32 angle;
    s32 x;
    s32 z;

    object = Func_02003e96();
    params.callback = 0x02008da5;
    for (angle_step = 0; angle_step <= 16; angle_step += 2) {
        angle = angle_step << 12;
        vec.x = Func_02003df8(angle);
        vec.y = 0;
        z = Func_02003dfc_a(angle);
        x = vec.x;
        vec.z = z;
        x = x + Func_02003df2(x, 3);
        vec.x = x;
        Func_0200194a(object->x, object->y, object->z, x, vec.y, z, 0x01000001, &params);
    }
}
#undef SpawnMode14Effect
#undef SpawnMode15Effect
#undef SpawnConfiguredSceneEffect
#undef SpawnRadialEffectBurst

/* map/locations/heidia/prologue/prologue_route_scene/messages.c */
#define StartScriptedSceneMessage Func_02000f3c
#define ShowForgetEverythingMessage Func_02001624
#define ShowHelpYouForgetMessage Func_0200164c
#define ShowDamagedDoorMessage Func_0200165c
#define ShowSaveMyLifeMessage Func_0200166c
void Func_02003fb0_a(s32 arg0);
void Func_02003fb0_b(s32 arg0);
s32 Func_02004002(s32 arg0, s32 arg1, s32 arg2);
void Func_02004058(s32 arg0, s32 arg1);
s32 Func_02003fe0(s32 arg0, s32 arg1);
void Func_02003fdc(void);
void Func_0200468c(s32 arg0);
void Func_02004668(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void Func_020046b2(int arg0);
void Func_020046c2(int arg0);
void Func_020046d2(int arg0);

void StartScriptedSceneMessage(s32 message_id)
{
    Func_02003fb0_a(message_id);
    Func_02004002(8, 0, 0);
    Func_02003fb0_b(4055);
    Func_02004058(181, 3);
    Func_02003fe0(181, 0);
    Func_02003fdc();
}

void ShowForgetEverythingMessage(void)
{
    Func_0200468c(2244);
    {
        s32 k4 = 8, k5 = 21;

        Func_02004668(0, 0, 1, 1, k4, k5);
    }
}

void ShowHelpYouForgetMessage(void)
{
    Func_020046b2(2245);
}

void ShowDamagedDoorMessage(void)
{
    Func_020046c2(2246);
}

void ShowSaveMyLifeMessage(void)
{
    Func_020046d2(2247);
}
#undef StartScriptedSceneMessage
#undef ShowForgetEverythingMessage
#undef ShowHelpYouForgetMessage
#undef ShowDamagedDoorMessage
#undef ShowSaveMyLifeMessage

/* map/locations/heidia/prologue/prologue_route_scene/orbiting_effect.c */
#define GetOrbitingSceneObject Func_02005f6c
#define NormalizeOrbitingSceneObject Func_02005f4e
#define IsGameFlagSet Func_02005f84
#define AllocateEffectTransfer Func_02005f3c
#define LoadEffectResource Func_02005fb4
#define ConfigurePaletteTransfer Func_02005f64
#define ReleaseEffectTransfer Func_02005f62
#define UpdateOrbitingSceneObject Value_0200ae71
#define InitializeSwayingSceneObject Func_02002ed8

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern u8 Value_0200ae71;

OrbitingSceneObject *Func_02005f6c(void);
void Func_02005f4e(OrbitingSceneObject *, s32);
s32 Func_02005f84(s32);
u8 *Func_02005f3c(s32, s32);
void Func_02005fb4(s32);
s32 Func_02005f64(u8, s32, u8 *);
void Func_02005f62(s32);

void InitializeSwayingSceneObject(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    NormalizeOrbitingSceneObject(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (IsGameFlagSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    LoadEffectResource(181);
    transfer += 0x400;
    ConfigurePaletteTransfer(sprite->palette, 128, transfer);
    ReleaseEffectTransfer(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}
#undef GetOrbitingSceneObject
#undef NormalizeOrbitingSceneObject
#undef IsGameFlagSet
#undef AllocateEffectTransfer
#undef LoadEffectResource
#undef ConfigurePaletteTransfer
#undef ReleaseEffectTransfer
#undef UpdateOrbitingSceneObject
#undef InitializeSwayingSceneObject

/* map/locations/heidia/prologue/prologue_route_scene/overlay_object.c */
#define OvObj_IntegrateVelocities Func_02000ab0
void OvObj_IntegrateVelocities(void *arg0)
{
    u8 *a = arg0;

    *(volatile s32 *)(a + 0x08) += *(s32 *)(a + 0x44);
    *(volatile s32 *)(a + 0x0C) += *(s32 *)(a + 0x48);
    *(volatile s32 *)(a + 0x10) += *(s32 *)(a + 0x4C);
    *(volatile s32 *)(a + 0x18) += *(s32 *)(a + 0x30);
    *(volatile s32 *)(a + 0x1C) += *(s32 *)(a + 0x34);
    *(volatile u16 *)(*(u8 **)(a + 0x50) + 0x1E) += *(u16 *)(a + 0x64);
}
#undef OvObj_IntegrateVelocities

/* map/locations/heidia/prologue/prologue_route_scene/prologue.c */
#define FindClearActorPosition Func_02000474
#define NoOpEffectCallback Func_02000cc0
#define RunPrologueSceneSetup Func_02001374
#define StartSceneScript37 Func_0200138c
#define RunSceneVectorTransition Func_02001740
#define TryPushBlockingSceneActor Func_02001a44
struct V6 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
};

struct S {
    s32 f00;
    u16 f04;
    u16 f06;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[20];
    s32 f28;
    u8 pad2c[4];
    s32 f30;
    s32 f34;
    u8 pad38[29];
    u8 f55;
};

struct V {
    s32 a;
    s32 b;
    s32 c;
};

extern s32 Data_0200b1b4[];
extern s32 Data_0200b15c[];
extern u8 Data_0200b2bc[];

s32 Func_020007de(s32 *, s32 *, s32 *);
s32 Func_0200358c(u8 *, s32 *);
void Func_020043e8(void);
void Func_02001440(void);
void Func_020043f8(void);
void Func_02002638(void);
s32 Func_020043b6(u8 *arg0, s32 arg1, s32 arg2);
void Func_020044b4(s32 arg0);
void Func_0200447a(s32 arg0);
void Func_020047b6(void);
s32 Func_02001bc2(struct V6 *arg0);
void Func_02001d6e(struct V6 arg0);
void Func_020047e2(void);
void Func_02004a60(s32 arg0, s32 arg1, struct V *arg2);
s32 Func_02004ac0(struct S *arg0, struct V *arg1);
void Func_02004b08(void);
void Func_02004a98(struct S *arg0, s32 arg1);
void Func_02004a5e(s32 arg0);
void Func_02004bc4(s32 arg0);
void Func_02004aac(struct S *arg0, s32 arg1);
void Func_02004b0e(struct S *arg0, s32 arg1);
void Func_02004b7c(s32 arg0, s32 arg1, s32 arg2);
void Func_02004ae4(struct S *arg0, s32 arg1);
void Func_02004b2c(struct S *arg0, s32 arg1);
void Func_02004b74(void);

s32 FindClearActorPosition(s32 *a)
{
    s32 sel;
    s32 buf[3];
    u8 *obj;
    s32 cnt;
    s32 ny, nx;
    u8 *p;
    s32 off;
    s32 t, u;
    u8 *E;
    s32 *b;
    s32 ret;
    a[5] = 0;
    obj = (u8 *)Func_020007de(&sel, a + 1, a);
    if (obj == 0)
        return 0;
    p = obj + 0x22;
    *p = 2;
    cnt = 0;
    E = (u8 *)Data_0200b1b4;
    off = a[0] << 4;
    {
        s32 o = off + 4;
        t = *(s32 *)(E + o);
        if (t < 0)
            t = -t;
        o = off;
        o += 12;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        ny = (t + u) >> 4;
        t = *(s32 *)(E + off);
        if (t < 0)
            t = -t;
        o = off;
        o += 8;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        nx = (t + u) >> 4;
    }
    {
        u8 *oc;
        s32 w;
        s32 d;
        b = buf;
        d = Data_0200b15c[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (Data_0200b15c[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)Data_0200b1b4;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)Data_0200b1b4;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Func_0200358c(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (Data_0200b15c[sel] & 0xffff0000);
        buf[2] = buf[2] + (Data_0200b15c[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = Data_0200b15c[sel];
        s32 m = d & 0xffff0000;
        s32 v1 = m *cnt;
        s32 v2 = (d << 16) * cnt;
        u8 *oc = obj;
        a[2] = *(s32 *)(oc + 8) + v1;
        a[3] = *(s32 *)(oc + 12);
        a[4] = *(s32 *)(oc + 16) + v2;
        ret = 1;
    }
    return ret;
}

/* Deliberate no-op callback. */
void NoOpEffectCallback(void) {}

void RunPrologueSceneSetup(void)
{
    Func_020043e8();
    Func_02001440();
    Func_020043f8();
    Func_02002638();
}

void StartSceneScript37(void)
{
    Func_020043b6(Data_0200b2bc, 37, 7);
    Func_020044b4(183);
    Func_0200447a(4);
}

void RunSceneVectorTransition(void)
{
    struct V6 transition;

    Func_020047b6();
    if (Func_02001bc2(&transition) != 0) {
        Func_02001d6e(transition);
    }
    Func_020047e2();
}

s32 TryPushBlockingSceneActor(struct S *actor)
{
    u8 *state = &actor->f55;
    s32 saved_state = *state;
    struct V destination;

    destination.a = (actor->f08 & 0xfff00000) + 0x80000;
    destination.b = actor->f0c;
    destination.c = (actor->f10 & 0xfff00000) + 0x80000;
    {
        s32 direction = (actor->f06 + 0x2000) & 0xc000;

        Func_02004a60(0x200000, direction, &destination);
    }
    if (Func_02004ac0(actor, &destination) == 0) {
        s32 t;

        Func_02004b08();
        Func_02004a98(actor, 6);
        Func_02004a5e(6);
        Func_02004bc4(152);
        Func_02004aac(actor, 7);
        actor->f30 = 0x30000;
        actor->f34 = 0x20000;
        actor->f28 = 0x40000;
        t = 126;
        t &= *state;
        *state = (u8)t;
        Func_02004b0e(actor, 0);
        {
            s16 *coordinates = (s16 *)&destination;

            Func_02004b7c(0, coordinates[1], coordinates[5]);
        }
        Func_02004ae4(actor, 6);
        Func_02004b2c(actor, 1);
        *state = (u8)saved_state;
        Func_02004b74();
        return 1;
    }
    return 0;
}
#undef FindClearActorPosition
#undef NoOpEffectCallback
#undef RunPrologueSceneSetup
#undef StartSceneScript37
#undef RunSceneVectorTransition
#undef TryPushBlockingSceneActor

/* map/locations/heidia/prologue/prologue_route_scene/scene_data.c */
#define SelectPrimarySceneData Func_02000cc4
#define GetEmptySceneData Func_02000d3c
#define GetDefaultSceneData Func_02000d40
#define SelectSecondarySceneData Func_02000d48
#define SelectTertiarySceneData Func_02000ec4
extern s16 Data_02000240[];
extern u8 Value_00000071;
extern u8 Value_00000072;
extern u8 Value_0000007b;
extern u8 Value_0000007c;
extern u8 Value_0000007d;
extern u8 Data_0200b310[];
extern u8 Data_0200b358[];
extern u8 Data_0200b3a0[];
extern u8 Data_0200b400[];
extern u8 Data_0200b448[];
extern u8 Data_0200b478[];
extern u8 Value_00000086;
extern u8 Value_0000007e;
extern u8 Data_0200b610[];
extern u8 Data_0200b718[];
extern u8 Data_0200b850[];
extern u8 Data_0200b5f8[];
extern u8 Data_0200b904[];
extern u8 Data_0200b8e0[];
extern u8 Data_0200b9f4[];
extern u8 Data_0200bd48[];
extern u8 Data_0200bd6c[];
extern u8 Data_0200b880[];

s32 SelectPrimarySceneData(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000071) {
        return (s32)Data_0200b310;
    }
    if (v == (s32)&Value_00000072) {
        return (s32)Data_0200b358;
    }
    if (v == (s32)&Value_0000007b) {
        return (s32)Data_0200b3a0;
    }
    if (v == (s32)&Value_0000007c) {
        return (s32)Data_0200b400;
    }
    if (v == (s32)&Value_0000007d) {
        return (s32)Data_0200b448;
    }
    return (s32)Data_0200b478;
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 GetEmptySceneData(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *GetDefaultSceneData(void)
{
    return (u8 *)0x0200b508;
}

s32 SelectSecondarySceneData(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000071) {
        return (s32)Data_0200b610;
    }
    if (v == (s32)&Value_0000007b) {
        return (s32)Data_0200b718;
    }
    if (v <= (s32)&Value_00000086 && v >= (s32)&Value_0000007e) {
        return (s32)Data_0200b850;
    }
    return (s32)Data_0200b5f8;
}

s32 SelectTertiarySceneData(void)
{
    s16 scene_id = Data_02000240[224];

    if (scene_id == (s32)&Value_00000071) {
        return (s32)Data_0200b904;
    }
    if (scene_id == (s32)&Value_00000072) {
        return (s32)Data_0200b8e0;
    }
    if (scene_id == (s32)&Value_0000007b) {
        return (s32)Data_0200b9f4;
    }
    if (scene_id == (s32)&Value_0000007c) {
        return (s32)Data_0200bd48;
    }
    if (scene_id == (s32)&Value_0000007d) {
        return (s32)Data_0200bd6c;
    }
    return (s32)Data_0200b880;
}
#undef SelectPrimarySceneData
#undef GetEmptySceneData
#undef GetDefaultSceneData
#undef SelectSecondarySceneData
#undef SelectTertiarySceneData

/* map/locations/heidia/prologue/prologue_route_scene/scene_primary_script.c */
#define Scene_RunScene3b2SequenceA Func_02001214
#define FieldScene_RunScene3b2_02001494 Func_02001494
#define FieldScene_RunScene3b2_0200167c Func_0200167c
typedef union {
    s32 w;
    s16 h[2];
} RecWord;

extern u8 Data_00000071[];
extern u8 Data_0000007b[];
extern u8 Data_0000007d[];
extern u8 Data_0000007e[];
extern u8 Data_00000086[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];

void Func_02001914();
void Func_0200191a();
void Func_020023cc();
void Func_0200265e();
void Func_020026be();
void Func_020028b4();
void Func_02003fe6();
void Func_02003ff8();
s32 Func_02003ffe();
void Func_02004044_a();
void Func_02004044_b();
s32 Func_02004054();
void Func_02004068();
void Func_02004078();
void Func_020040b4_a();
s32 Func_020040ba();
void Func_020040c0();
s32 Func_020040d0();
void Func_020040e0();
void Func_020040e6();
void Func_020040f0();
s32 Func_020040f4();
s32 Func_020040f6();
void Func_020040fc();
void Func_0200411c();
void Func_0200412c();
s32 Func_02004130();
s32 Func_02004132_a();
s32 Func_02004132_b();
void Func_02004138();
void Func_02004158();
s32 Func_0200416c();
s32 Func_0200416e();
void Func_02004174();
void Func_02004194();
void Func_020041a4();
s32 Func_020041a8();
s32 Func_020041ba_a();
s32 Func_020041ba_b();
s32 Func_020041d2();
void Func_020042a8();
s32 Func_0200428e();
void Func_02004290();
u8 *Func_020042a4();
void Func_020042b0();
s32 Func_020042da();
void Func_02004300();
s32 Func_02004318();
void Func_02004330();
void Func_020044f2();
void Func_02004508();
void Func_02004522();
void Func_02004530();
void Func_02004532();
void Func_02004560_a();
void Func_0200457e();
void Func_02004596();
void Func_020045a2_a();
void Func_020045bc();
void Func_020046c4();
s32 Func_020046dc();
s32 Func_02004714();
s32 Func_02004738();
s32 Func_0200473a();
s32 Func_0200475c();
s32 Func_0200475e();
void Func_02004762();
s32 Func_02004782();
void Func_02004786();
s32 Func_020047a6();
void Func_020047aa();
void Func_020047ce();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02001214(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02001494(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_0200167c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

s32 Func_02000f70(void)
{
    u8 *rec7;
    s32 scene;
    s32 cur;
    s32 lo;

    ((RecWord *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))->w = 0x204;
    scene = Data_02000240_t[224][0];
    if (scene == (s32)Data_0000007b) {
        Func_020028b4();
        return 0;
    }
    if (scene == (s32)Data_0000007d) {
        if (Value1(Func_02003ffe, 0xef7) == 0) {
            Call6(Func_02003fe6, 0, 3, 1, 1, 13, 40);
            Call6(Func_02003ff8, 0, 2, 1, 1, 15, 40);
            Call3(Func_020040e6, 101, 0xd80000, 0x2880000);
        }
        if (Data_02000240_t[224][0] == scene) {
            if (Data_02000240_t[225][0] != 5) {
                if (Value1(Func_02004054, 0x8d1) == 0) {
                    return 0;
                }
            }
            Call1(Func_02004068, 0x8d1);
            Call6(Func_02004044_a, 0, 1, 1, 1, 13, 30);
            Call3(Func_02004132_a, 100, 0xd80000, 0x1e80000);
            return 0;
        }
    }
    cur = Data_02000240_t[224][0];
    if (cur == (s32)Data_00000071) {
        s32 rec;

        Func_020026be();
        rec = Func_020040d0(8);
        *(volatile s32 *)(rec + 56) = 0x810000;
        Func_02001914(9);
        Func_0200191a(10);
        if (Value1(Func_020040ba, 0x240) != 0) {
            rec7 = Value1(Func_020040f4, 11);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Func_02004078((s32)rec7, 4);
                Func_020040c0((s32)rec7, 0);
            }
            Call4(Func_020040e0, 0, 0x1300000, 0x1700000, 253);
        }
        if (Value1(Func_020040f6, 0x241) != 0) {
            rec7 = Value1(Func_02004130, 12);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Func_020040b4_a((s32)rec7, 4);
                Func_020040fc((s32)rec7, 0);
            }
            Call4(Func_0200411c, 0, 0x500000, 0x1700000, 253);
        }
        if (Value1(Func_02004132_b, 0x242) != 0) {
            rec7 = Value1(Func_0200416c, 13);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Func_020040f0((s32)rec7, 4);
                Func_02004138((s32)rec7, 0);
            }
            Call4(Func_02004158, 0, 0x600000, 0x1500000, 253);
        }
        if (Value1(Func_0200416e, 0x243) != 0) {
            rec7 = Value1(Func_020041a8, 14);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Func_0200412c((s32)rec7, 4);
                Func_02004174((s32)rec7, 0);
            }
            Call4(Func_02004194, 0, 0x900000, 0x1400000, 253);
            Call4(Func_020041a4, 0, 0x2f00000, 0x1400000, 253);
        }
        if (Value1(Func_020041ba_a, 0xfd7) != 0) {
            return 0;
        }
        Func_02004044_b(8);
        return 0;
    }
    lo = (s32)Data_0000007e;
    if (cur == lo) {
        if (Value1(Func_020041d2, 0xef4) == 0) {
            Call6(Func_020041ba_b, 0, 0, 1, 1, 37, 10);
            Call3(Func_020042a8, 100, 0x2580000, 0xa80000);
        }
    }
    cur = Data_02000240_t[224][0];
    if (cur >= lo) {
        if (cur <= (s32)Data_00000086) {
            Func_020023cc();
            if (Data_02000240_t[225][0] == 5) {
                Func_0200265e();
            }
        }
    }
    return 0;
}

void Scene_RunScene3b2SequenceA(void)
{
    u32 i;
    s32 rec7;
    u8 *rec8;
    s32 record;

    rec8 = Func_020042a4(8);
    rec7 = Value1_02001214(Func_0200428e, (Data_02000240_t[224][0] + (0x8d2 - (s32)Data_0000007e)));
    if (rec7 != 0) {
        Call3(Func_02004300, 8, 0x28a0000, 0xa80000);
        *(volatile s32 *)((s32)rec8 + 12) = -0x200000;
        record = Func_020042da(8);
        Func_02004290(record, 0);
        Func_02004330(8, 3);
        rec8[85] = 0;
        {
            u8 value = *(volatile u8 *)&rec8[35];

            rec8[35] = (u8)(value | 2);
        }
        Call6(Func_020042b0, 42, 10, 1, 1, 40, 10);
    } else {
        *(u8 *)(Func_02004318(8) + 85) = rec7;
    }
}

void FieldScene_RunScene3b2_02001494(void)
{
    u32 i;
    s32 record;

    Func_02004508();
    Call3_02001494(Func_02004532, 0, 0x6666, 0x3333);
    Func_02004596();
    Func_020045a2_a();
    Func_02004522((Data_02000240_t[224][0] + (0x8c8 - (s32)Data_0000007e)));
    Func_02004530(30);
    Call3_02001494(Func_020044f2, 0x200b2bc, 44, 7);
    Call3_02001494(Func_0200457e, 0, 3, -16);
    Func_020045bc(3);
    Func_02004560_a();
}

void FieldScene_RunScene3b2_0200167c(void)
{
    u32 i;
    s32 record;

    if (Value1_0200167c(Func_020046dc, 0x8c4) != 0) {
        Call6(Func_020046c4, 0, 0, 1, 1, 8, 21);
        Call3(Func_02004762, 15, 0x3c80000, 0x3c80000);
    } else {
        record = Func_0200473a(15);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Value1_0200167c(Func_02004714, 0x8c5) != 0) {
        Call3(Func_02004786, 16, 0x3c80000, 0x3c80000);
    } else {
        record = Func_0200475e(16);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Value1_0200167c(Func_02004738, 0x8c6) != 0) {
        Call3(Func_020047aa, 17, 0x3c80000, 0x3c80000);
    } else {
        record = Func_02004782(17);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Value1_0200167c(Func_0200475c, 0x8c7) != 0) {
        Call3(Func_020047ce, 18, 0x3c80000, 0x3c80000);
    } else {
        record = Func_020047a6(18);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
}
#undef Scene_RunScene3b2SequenceA
#undef FieldScene_RunScene3b2_02001494
#undef FieldScene_RunScene3b2_0200167c

/* map/locations/heidia/prologue/prologue_route_scene/scene_state.c */
#define SetEffectOwnerMode Func_020009dc
#define InitializePrologueSceneState Func_02000e88
#define MarkGridLeftOfSceneActor Func_0200185c
#define MarkGridAboveSceneActor Func_020018b0
#define SetSceneActorModes Func_02001904
#define InitializeEscapeSceneActors Func_0200191c
struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[15];
    u8 f23;
};

extern u8 Data_02000240[];

void Func_02003efc(void);
void Func_02003f86(s32 arg0, s32 arg1);
void Func_02003f8e(s32 arg0, s32 arg1);
void Func_02003f2a(void);
struct S *Func_020048f4(void);
void Func_0200494a(s32 arg0, s32 arg1);
void Func_020048c8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
struct S *Func_02004948(void);
void Func_0200499e(s32 arg0, s32 arg1);
void Func_0200491c(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void Func_020049cc(int arg0, int arg1);
void Func_020049d4(int arg0, int arg1);
void Func_02003184(s32, s32, s32, s32);
void Func_02003190(s32, s32, s32, s32);
void Func_020031f0(s32, s32, s32, s32);
void Func_020031fc(s32, s32, s32, s32);
void Func_02003208(s32, s32, s32, s32);
void Func_02003214(s32, s32, s32, s32);
void Func_020031cc(s32, s32, s32, s32);
void Func_020031d8(s32, s32, s32, s32);
void Func_020031e4(s32, s32, s32, s32);
void Func_02003244(s32, s32, s32, s32);
void Func_02003250(s32, s32, s32, s32);
void Func_0200325c(s32, s32, s32, s32);

/* Copy the low two mode bits into the object's owner record. */

void SetEffectOwnerMode(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}

void InitializePrologueSceneState(void)
{
    u8 *base;

    Func_02003efc();
    base = Data_02000240;
    Func_02003f86(*(s16 *)(base + 448), 5);
    base[555] = 3;
    Func_02003f8e(84, 5);
    Func_02003f2a();
}

void MarkGridLeftOfSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute)
{
    struct S *p = Func_020048f4();

    if (p != 0) {
        s32 v;

        Func_0200494a(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k5 = p->f10 >> 20;
            s32 k4 = (p->f08 >> 20) - 1;

            Func_020048c8(grid_value, grid_attribute, 3, 1, k4, k5);
        }
    }
}

void MarkGridAboveSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute)
{
    struct S *p = Func_02004948();

    if (p != 0) {
        s32 v;

        Func_0200499e(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k4 = p->f08 >> 20;
            s32 k5 = (p->f10 >> 20) - 1;

            Func_0200491c(grid_value, grid_attribute, 1, 3, k4, k5);
        }
    }
}

void SetSceneActorModes(int actor_id)
{
    Func_020049cc(actor_id, 1);
    Func_020049d4(actor_id, 2);
}

void InitializeEscapeSceneActors(void)
{
    Func_02003184(8, 0x11, 0x1E, 0x15);
    Func_02003190(0xA, 0x11, 0x1F, 0x16);
    Func_020031f0(0xB, 0x14, 0x1E, 0x17);
    Func_020031fc(0xC, 0x15, 0x1E, 0x18);
    Func_02003208(0xD, 0x16, 0x1E, 0x19);
    Func_02003214(0xF, 0x17, 0x1E, 0x1A);
    Func_020031cc(0x11, 0, 0x1E, 0x1F);
    Func_020031d8(0x12, 0, 0x1F, 0x20);
    Func_020031e4(9, 0, 0x20, 0x21);
    Func_02003244(0x13, 4, 0x1E, 0x22);
    Func_02003250(0xE, 5, 0x1E, 0x23);
    Func_0200325c(0x10, 6, 0x1E, 0x24);
}
#undef SetEffectOwnerMode
#undef InitializePrologueSceneState
#undef MarkGridLeftOfSceneActor
#undef MarkGridAboveSceneActor
#undef SetSceneActorModes
#undef InitializeEscapeSceneActors
