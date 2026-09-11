#include "types.h"
#include "scene.h"
#include "staged_actor.h"

/* map/locations/heidia/prologue/prologue_route_scene/actor_activation.c */
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

extern s32 StagedActorStepTable[];
extern s32 gOv[];
extern s32 gCell[];

Ent *Map_unk6_4(Desc *, Ent *);

Obj *Map_unk7_4(s32);
Obj *Map_unk8_4(s32);

s32 StopBlockedActorMotion(Ent *actor)
{
    Desc destination;
    u32 direction_index;
    s32 step;
    Ent *blocking_actor;

    direction_index = actor->unk6 >> 12;
    step = StagedActorStepTable[direction_index];
    destination.unk0 = actor->unk8 + (step & 0xffff0000);
    destination.unk4 = actor->unkC;
    step = step << 16;
    destination.unk8 = actor->unk10 + step;
    blocking_actor = Map_unk6_4(&destination, actor);
    if (blocking_actor != 0) {
        u32 kind_index = 0;
        s32 blocking_kind = *blocking_actor->unk50->unk28;
        s32 *allowed_kinds = gOv;

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
    step = StagedActorStepTable[direction_index];
    destination.unk0 = actor->unk8 + (step & 0xffff0000);
    destination.unk4 = actor->unkC;
    step = step << 16;
    destination.unk8 = actor->unk10 + step;
    if (Map_EntOp(actor, &destination) > 0) {
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

    p = Map_unk7_4(gCell[125]);
    q = Map_unk8_4(actor_id);
    Map_unk9_4();
    {
        x = ((p->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Map_SetMode(p, x, p->f0c, y);
    }
    Map_Apply(p, 27);
    {
        x = ((q->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Map_SetMode2(q, x, q->f0c, y);
    }
    if (x_offset < 0 || z_offset < 0) {
        Map_Apply2(q, 4);
    } else {
        Map_Apply3(q, 3);
    }
    Map_unk2_2(226);
    Map_unk3_2(p);
    Map_unk4_2(288);
    Map_unk10_4();
}

void ActivateSceneActor8(void)
{
    Map_unk5_2(8);
}

void ActivateSceneActor10(void)
{
    Map_unk6_2(10);
}

void ActivateSceneActor11(void)
{
    Map_unk7_2(11);
}

void ActivateSceneActor12(void)
{
    Map_unk8_2(12);
}

void ActivateSceneActor13(void)
{
    Map_unk9_2(13);
}

void ActivateSceneActor15(void)
{
    Map_unk10_2(15);
}

void ActivateSceneActor17(void)
{
    Map_unk11_2(17);
}

void ActivateSceneActor18(void)
{
    Map_unk12_2(18);
}

void ActivateSceneActor9(void)
{
    Map_unk13_2(9);
}

void ActivateSceneActor19(void)
{
    Map_unk14_2(19);
}

void ActivateSceneActor14(void)
{
    Map_unk15_2(14);
}

void ActivateSceneActor16(void)
{
    Map_unk16_2(16);
}

/* map/locations/heidia/prologue/prologue_route_scene/actor_motion.c */

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

extern u32 StagedActorStepTable[];

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

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

    effect->velocity[0] = velocity_x - Map_unk18(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

s32 UpdateSwayingSceneObject(struct S *object)
{
    struct T *sprite = object->f50;
    s32 vertical_offset = Map_unk19(object->f30) * 2;
    s32 random_b;
    s32 random_a;

    if (vertical_offset > 0) {
        vertical_offset = -vertical_offset;
    }
    object->f08 = object->f38 + Map_unk20(object->f30) * 2;
    object->f0c = object->f3c + vertical_offset;
    sprite->f1e = (u16)(Map_unk21(object->f30 + 0x8000) / 8);
    random_a = Map_unk22();
    random_b = Map_unk23();
    object->f30 += (((u32)(random_a << 9)) >> 16) + (((u32)(random_b << 9)) >> 16) + 0x400;
    return 0;
}

/* map/locations/heidia/prologue/prologue_route_scene/actor_routes.c */
struct S { s32 pad[2]; s32 f08; s32 f0c; s32 f10; };

struct V { s32 a; s32 b; s32 c; };

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S2 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S3 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S5 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S6 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S7 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S9 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S10 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S11 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S12 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S13 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S14 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S *Map_unk11_4(s32 arg0);

struct S *Map_unk12_4(s32 arg0);

struct S *Map_unk13_4(s32 arg0);

struct S *Map_unk14_4(s32 arg0);

struct S *Map_unk15_4(s32 arg0);
struct S *Map_unk16_4(s32 arg0);
struct S *Map_unk17_4(s32 arg0);
struct S *Map_unk18_4(s32 arg0);
struct S *Map_unk19_4(s32 arg0);

struct S *Map_unk20_4(s32 arg0);
struct S *Map_unk21_4(s32 arg0);
struct S *Map_unk22_4(s32 arg0);
struct S *Map_unk23_4(s32 arg0);

struct S *Map_unk24_4(s32 arg0);
struct S *Map_unk25_4(s32 arg0);
struct S *Map_unk26_4(s32 arg0);
struct S *Map_unk27_4(s32 arg0);
struct S *Map_unk28_4(s32 arg0);

struct S *Map_unk29_4(s32 arg0);
struct S *Map_unk30_4(s32 arg0);
struct S *Map_unk31_4(s32 arg0);

struct S *Map_unk32_4(s32 arg0);
struct S *Map_unk33_4(s32 arg0);
struct S *Map_unk34_4(s32 arg0);
struct S *Map_unk35_4(s32 arg0);
struct S *Map_unk36_4(s32 arg0);

struct S *Map_unk37_4(s32 arg0);
struct S *Map_unk38_4(s32 arg0);
struct S *Map_unk39_4(s32 arg0);
struct S *Map_unk40_4(s32 arg0);

struct S *Map_unk41_4(s32 arg0);
struct S *Map_unk42_4(s32 arg0);
struct S *Map_unk43_4(s32 arg0);

struct S *Map_unk44_4(s32 arg0);
struct S *Map_unk45_4(s32 arg0);
struct S *Map_unk46_4(s32 arg0);

struct S *Map_unk47_4(s32 arg0);
struct S *Map_unk48_4(s32 arg0);
struct S *Map_unk49_4(s32 arg0);
struct S *Map_unk50_4(s32 arg0);
struct S *Map_unk51_4(s32 arg0);

struct S *Map_unk52_4(s32 arg0);
struct S *Map_unk53_4(s32 arg0);

struct S *Map_unk54_4(s32 arg0);

struct S *Map_unk55_4(s32 arg0);
struct S *Map_unk56_4(s32 arg0);
struct S *Map_unk57_4(s32 arg0);
struct S *Map_unk58_4(s32 arg0);
struct S *Map_unk59_4(s32 arg0);

struct S *Map_unk60_4(s32 arg0);
struct S *Map_unk61_4(s32 arg0);
struct S *Map_unk62_4(s32 arg0);
struct S *Map_unk63_4(s32 arg0);
struct S *Map_unk64_4(s32 arg0);

struct S *Map_unk65_4(s32 arg0);
struct S *Map_unk66_4(s32 arg0);
struct S *Map_unk67_4(s32 arg0);
struct S *Map_unk68_4(s32 arg0);

struct S *Map_unk69_4(s32 arg0);
struct S *Map_unk70_4(s32 arg0);
struct S *Map_unk71_4(s32 arg0);

u8 *Map_unk72_4();
u8 *Map_unk73_4();
u8 *Map_unk74_4();
u8 *Map_unk75_4();

u8 *Map_unk76_4();
u8 *Map_unk77_4();
u8 *Map_unk78_4();
u8 *Map_unk79_4();
u8 *Map_unk80_4();

u8 *Map_unk81_4();
u8 *Map_unk82_3();
u8 *Map_unk83_3();
u8 *Map_unk84_3();

u8 *Map_unk85_3();
u8 *Map_unk86_3();
u8 *Map_unk87_3();
u8 *Map_unk88_3();

u8 *Map_unk89_3();
u8 *Map_unk90_3();
u8 *Map_unk91_3();
u8 *Map_unk92_3();

u8 *Map_unk93_3();
u8 *Map_unk94_3();
u8 *Map_unk95_3();

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

u8 *Map_unk96_3();                    /* scene entity by selector */

void Map_unk97_3();                   /* wait N frames */

void Map_unk98_3();                   /* collision repaint, six arguments */

u8 *Map_unk99_3();                    /* scene entity by selector */

void Map_unk100_3();                   /* wait N frames */

void Map_unk101_3();                   /* collision repaint, six arguments */

u8 *Map_unk102_3();                    /* scene entity by selector */

void Map_unk103_3();                   /* wait N frames */

void Map_unk104_3();                   /* collision repaint, six arguments */

u8 *Map_unk105_3();                    /* scene entity by selector */

void Map_unk106_3();                   /* wait N frames */

void Map_unk107_3();                   /* collision repaint, six arguments */

u8 *Map_unk108_3();                    /* scene entity by selector */

void Map_unk109_3();                   /* wait N frames */

void Map_unk110_3();                   /* collision repaint, six arguments */

u8 *Map_unk111_3();                    /* scene entity by selector */

void Map_unk112_3();                   /* wait N frames */

void Map_unk113_3();                   /* collision repaint, six arguments */

s32 CheckActorPathSouth(void)
{
    struct S *actor = Map_unk11_4(0);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + -0x200000;
    return Map_Apply4(actor, &destination);
}

s32 CheckActorPathNorth(void)
{
    struct S *actor = Map_unk12_4(0);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + 0x200000;
    return Map_Apply5(actor, &destination);
}

s32 CheckActorPathWest(void)
{
    struct S *actor = Map_unk13_4(0);
    struct V destination;

    destination.a = actor->f08 + -0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return Map_Apply6(actor, &destination);
}

s32 CheckActorPathEast(void)
{
    struct S *actor = Map_unk14_4(0);
    struct V destination;

    destination.a = actor->f08 + 0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return Map_Apply7(actor, &destination);
}

void UpdateEscapeRouteForActorPositions(void)
{
    s32 actor_x = Map_unk15_4(8)->f08 >> 20;
    s32 actor_z = Map_unk16_4(8)->f10 >> 20;
    s32 actor_12_x = Map_unk17_4(12)->f08 >> 20;
    s32 actor_15_x = Map_unk18_4(15)->f08 >> 20;

    if (actor_z == 19) {
        if (actor_12_x == 24) {
            Map_unk12_3(8, 0, -80);
        } else if (actor_15_x == 24) {
            Map_unk13_3(8, 0, -112);
            Map_unk12_3(8, 0, -32);
        } else {
            Map_unk14_3(8, 0, -80);
            Map_unk12_3(8, 0, -112);
        }
    } else if (actor_z == 14) {
        if (actor_12_x == 24) {
            return;
        }
        if (actor_15_x == 24) {
            Map_unk12_3(8, 0, -64);
        } else {
            Map_unk12_3(8, 0, -112);
        }
    } else if (actor_z == 10) {
        if (actor_15_x == 24) {
            return;
        }
        Map_unk12_3(8, 0, -48);
    } else {
        Map_unk114_3();
        return;
    }
    Map_unk17_2(2);
    {
        s32 route_end_z = Map_unk19_4(8)->f10 >> 20;
        s32 route_x = actor_x - 1;

        Map_unk7_5(route_x, actor_z, 3, 1, route_x, route_end_z);
    }
    Map_unk8_5(0, 0, 3, 1, actor_x - 1, actor_z);
}

void UpdateActor8ReturnRoute(void)
{
    s32 x = Map_unk20_4(8)->f08 >> 20;
    s32 y = Map_unk21_4(8)->f10 >> 20;
    s32 z = Map_unk22_4(12)->f08 >> 20;

    if (y == 7) {
        if (z == 24) {
            Map_unk15_3(8, 0, 48);
        } else {
            Map_unk16_3(8, 0, 80);
            Map_unk17_3(8, 0, 112);
        }
    } else if (y == 10) {
        if (z == 24) {
            return;
        }
        Map_unk18_3(8, 0, 144);
    } else if (y == 14) {
        Map_unk19_3(8, 0, 80);
    } else {
        return;
    }
    Map_unk18_2(2);
    {
        s32 k = Map_unk23_4(8)->f10 >> 20;
        s32 m = x - 1;

        Map_unk9_5(m, y, 3, 1, m, k);
    }
    Map_unk10_5(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10RetreatRoute(void)
{
    s32 x = Map_unk24_4(10)->f08 >> 20;
    s32 y = Map_unk25_4(10)->f10 >> 20;
    s32 z = Map_unk26_4(13)->f08 >> 20;
    s32 w = Map_unk27_4(15)->f08 >> 20;

    if (y == 18) {
        if (w >= 31 && w <= 33) {
            Map_unk20_3(10, 0, -128);
        } else if (z >= 31 && z <= 33) {
            Map_unk20_3(10, 0, -128);
        } else {
            Map_unk21_3(10, 0, -112);
            Map_unk20_3(10, 0, -64);
        }
    } else if (y == 10) {
        if (w >= 31 && w <= 33) {
            return;
        }
        if (z >= 31 && z <= 33) {
            return;
        }
        Map_unk20_3(10, 0, -48);
    } else if (y == 7) {
        return;
    }
    Map_unk19_2(2);
    {
        s32 k = Map_unk28_4(10)->f10 >> 20;
        s32 m = x - 1;

        Map_unk11_5(m, y, 3, 1, m, k);
    }
    Map_unk12_5(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10AdvanceRoute(void)
{
    s32 x = Map_unk29_4(10)->f08 >> 20;
    s32 y = Map_unk30_4(10)->f10 >> 20;

    if (y != 18) {
        if (y == 10) {
            Map_unk22_3(10, 0, 128);
        } else {
            Map_unk23_3(10, 0, 112);
            Map_unk24_3(10, 0, 64);
        }
        Map_unk20_2(2);
        {
            s32 k = Map_unk31_4(10)->f10 >> 20;
            s32 m = x - 1;

            Map_unk13_5(m, y, 3, 1, m, k);
        }
        Map_unk14_5(0, 0, 3, 1, x - 1, y);
    }
}

void UpdateActor11WestRoute(void)
{
    s32 x = Map_unk32_4(11)->f08 >> 20;
    s32 y = Map_unk33_4(11)->f10 >> 20;

    if (x != 30) {
        if (x == 34) {
            if ((Map_unk34_4(10)->f10 >> 20) == 18) {
                return;
            }
            Map_unk25_3(11, -64, 0);
        } else if (x == 36) {
            if ((Map_unk35_4(10)->f10 >> 20) == 18) {
                Map_unk25_3(11, -32, 0);
            } else {
                Map_unk26_3(11, -96, 0);
            }
        }
        Map_unk21_2(2);
        {
            s32 k = Map_unk36_4(11)->f08 >> 20;
            s32 m = y - 1;

            Map_unk15_5(x, m, 1, 3, k, m);
        }
        Map_unk16_5(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor11EastRoute(void)
{
    s32 x = Map_unk37_4(11)->f08 >> 20;
    s32 y = Map_unk38_4(11)->f10 >> 20;

    if (x != 36) {
        if (x == 30) {
            if ((Map_unk39_4(10)->f10 >> 20) == 18) {
                return;
            }
            Map_unk27_3(11, 96, 0);
        } else if (x == 34) {
            Map_unk28_3(11, 32, 0);
        }
        Map_unk22_2(2);
        {
            s32 k = Map_unk40_4(11)->f08 >> 20;
            s32 m = y - 1;

            Map_unk17_5(x, m, 1, 3, k, m);
        }
        Map_unk18_5(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor12WestRoute(void)
{
    s32 x = Map_unk41_4(12)->f08 >> 20;
    s32 y = Map_unk42_4(12)->f10 >> 20;

    if (x == 36) {
        Map_unk29_3(12, -96, 0);
        Map_unk30_3(12, -96, 0);
    } else if (x == 34) {
        Map_unk31_3(12, -96, 0);
        Map_unk30_3(12, -64, 0);
    } else if (x == 24) {
        return;
    }
    Map_unk23_2(2);
    {
        s32 k = Map_unk43_4(12)->f08 >> 20;
        s32 m = y - 1;

        Map_unk19_5(x, m, 1, 3, k, m);
    }
    Map_unk20_5(0, 0, 1, 3, x, y - 1);
}

void UpdateActor12EastRoute(void)
{
    s32 x = Map_unk44_4(12)->f08 >> 20;
    s32 y = Map_unk45_4(12)->f10 >> 20;

    if (x == 24) {
        Map_unk32_3(12, 96, 0);
        Map_unk33_3(12, 96, 0);
    } else if (x == 34) {
        Map_unk33_3(12, 32, 0);
    } else if (x == 36) {
        return;
    }
    Map_unk24_2(2);
    {
        s32 k = Map_unk46_4(12)->f08 >> 20;
        s32 m = y - 1;

        Map_unk21_5(x, m, 1, 3, k, m);
    }
    Map_unk22_5(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13WestRoute(void)
{
    s32 x = Map_unk47_4(13)->f08 >> 20;
    s32 y = Map_unk48_4(13)->f10 >> 20;
    s32 z = Map_unk49_4(10)->f10 >> 20;
    s32 w = Map_unk50_4(15)->f08 >> 20;

    if (x == 36) {
        if (w == 34) {
            Map_unk34_3(13, -16, 0);
        } else if (z == 7) {
            Map_unk34_3(13, -32, 0);
        } else if (w == 30) {
            Map_unk34_3(13, -80, 0);
        } else {
            Map_unk35_3(13, -96, 0);
            Map_unk34_3(13, -80, 0);
        }
    } else if (x == 35) {
        if (w == 34) {
            return;
        } else if (z == 7) {
            Map_unk34_3(13, -16, 0);
        } else if (w == 30) {
            Map_unk34_3(13, -64, 0);
        } else {
            Map_unk36_3(13, -80, 0);
            Map_unk34_3(13, -80, 0);
        }
    } else if (x == 34) {
        if (z == 7) {
            return;
        }
        if (w == 30) {
            Map_unk34_3(13, -48, 0);
        } else {
            Map_unk34_3(13, -144, 0);
        }
    } else if (x == 31) {
        if (w == 30) {
            return;
        }
        Map_unk34_3(13, -96, 0);
    } else if (x == 25) {
        return;
    }
    Map_unk25_2(2);
    {
        s32 k = Map_unk51_4(13)->f08 >> 20;
        s32 m = y - 1;

        Map_unk23_5(x, m, 1, 3, k, m);
    }
    Map_unk24_5(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13EastRoute(void)
{
    s32 x = Map_unk52_4(13)->f08 >> 20;
    s32 y = Map_unk53_4(13)->f10 >> 20;

    Map_unk26_2(15);
    if (x == 25) {
        Map_unk37_3(13, 96, 0);
        Map_unk38_3(13, 80, 0);
    } else if (x == 31) {
        Map_unk38_3(13, 80, 0);
    } else if (x == 34) {
        Map_unk38_3(13, 32, 0);
    } else if (x == 35) {
        Map_unk38_3(13, 16, 0);
    } else if (x == 36) {
        return;
    }
    Map_unk27_2(2);
    {
        s32 k = Map_unk54_4(13)->f08 >> 20;
        s32 m = y - 1;

        Map_unk25_5(x, m, 1, 3, k, m);
    }
    Map_unk26_5(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15WestRoute(void)
{
    s32 x = Map_unk55_4(15)->f08 >> 20;
    s32 y = Map_unk56_4(15)->f10 >> 20;
    s32 z = Map_unk57_4(8)->f10 >> 20;
    s32 w = Map_unk58_4(10)->f10 >> 20;

    if (x == 35) {
        if (w == 7) {
            Map_unk39_3(15, -16, 0);
        } else if (z == 7) {
            Map_unk39_3(15, -112, 0);
        } else {
            Map_unk40_3(15, -96, 0);
            Map_unk39_3(15, -80, 0);
        }
    } else if (x == 34) {
        if (w == 7) {
            return;
        }
        Map_unk41_3(15, -96, 0);
        Map_unk39_3(15, -64, 0);
    } else if (x == 33) {
        Map_unk39_3(15, -144, 0);
    } else if (x == 31) {
        Map_unk39_3(15, -80, 0);
    } else if (x == 30) {
        Map_unk39_3(15, -96, 0);
    } else if (x == 24) {
        return;
    }
    Map_unk28_2(2);
    {
        s32 k = Map_unk59_4(15)->f08 >> 20;
        s32 m = y - 1;

        Map_unk27_5(x, m, 1, 3, k, m);
    }
    Map_unk28_5(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15EastRoute(void)
{
    s32 x = Map_unk60_4(15)->f08 >> 20;
    s32 y = Map_unk61_4(15)->f10 >> 20;
    s32 z = Map_unk62_4(10)->f10 >> 20;
    s32 w = Map_unk63_4(13)->f08 >> 20;

    if (x == 24) {
        if (z == 7 || w == 31) {
            Map_unk42_3(15, 96, 0);
        } else if (w == 34) {
            Map_unk43_3(15, 64, 0);
            Map_unk42_3(15, 80, 0);
        } else if (w == 35) {
            Map_unk44_3(15, 80, 0);
            Map_unk42_3(15, 80, 0);
        } else {
            Map_unk45_3(15, 80, 0);
            Map_unk42_3(15, 96, 0);
        }
    } else if (x == 30 || w == 31) {
        if (z == 7) {
            return;
        }
        if (w == 34) {
            Map_unk42_3(15, 48, 0);
        } else if (w == 35) {
            Map_unk42_3(15, 64, 0);
        } else {
            Map_unk42_3(15, 80, 0);
        }
    } else if (x == 33) {
        if (w == 34) {
            return;
        }
        if (w == 35) {
            Map_unk42_3(15, 16, 0);
        } else {
            Map_unk42_3(15, 32, 0);
        }
    } else if (x == 34) {
        Map_unk42_3(15, 16, 0);
    } else if (x == 35) {
        return;
    }
    Map_unk29_2(2);
    {
        s32 k = Map_unk64_4(15)->f08 >> 20;
        s32 m = y - 1;

        Map_unk29_5(x, m, 1, 3, k, m);
    }
    Map_unk30_5(0, 0, 1, 3, x, y - 1);
}

void UpdateActor17SouthRoute(void)
{
    s32 x = Map_unk65_4(17)->f08 >> 20;
    s32 y = Map_unk66_4(17)->f10 >> 20;
    s32 z = Map_unk67_4(19)->f08 >> 20;

    if (y == 19) {
        if (z >= 3 && z <= 5) {
            Map_unk46_3(17, 0, -16);
        } else {
            Map_unk46_3(17, 0, -64);
        }
    } else if (y == 18) {
        if (z >= 3 && z <= 5) {
            return;
        }
        Map_unk46_3(17, 0, -48);
    } else if (y == 15) {
        return;
    }
    Map_unk30_2(2);
    {
        s32 k = Map_unk68_4(17)->f10 >> 20;
        s32 m = x - 1;

        Map_unk31_5(m, y, 3, 1, m, k);
    }
    Map_unk32_5(0, 0, 3, 1, x - 1, y);
}

void UpdateActor17NorthRoute(void)
{
    s32 x = Map_unk69_4(17)->f08 >> 20;
    s32 y = Map_unk70_4(17)->f10 >> 20;

    if (y == 15) {
        Map_unk47_3(17, 0, 64);
    } else if (y == 18) {
        Map_unk48_3(17, 0, 16);
    } else if (y == 19) {
        return;
    }
    Map_unk31_2(2);
    {
        s32 k = Map_unk71_4(17)->f10 >> 20;

        s32 m = x - 1;

        Map_unk33_5(m, y, 3, 1, m, k);
    }
    Map_unk34_5(0, 0, 3, 1, x - 1, y);
}

void AdvanceActor18AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;

    s32 permuted_5;
    permuted_5 = *(s32 *)(Map_unk96_3(18) + 8) >> 20;
    row = *(s32 *)(Map_unk72_4(18) + 16) >> 20;
    column  = permuted_5;
    companion19Column = *(s32 *)(Map_unk73_4(19) + 8) >> 20;
    companion14Column = *(s32 *)(Map_unk74_4(14) + 8) >> 20;

    if (row == 9) {
        if ((u32)(companion14Column - 6) <= 2) {
            goto transition32;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            goto transition80;
        }
        /* This arm runs two transitions back to back. */
        Map_unk115_3(18, 0, 64);
        Map_unk116_3(18, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 6) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            Map_unk117_3(18, 0, 48);
        } else {
            Map_unk118_3(18, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion19Column - 6) <= 2) {
transition32:
            Map_unk119_3(18, 0, 32);
        } else {
            Map_unk120_3(18, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 6) <= 2) {
            return;
        }
transition80:
        Map_unk121_3(18, 0, 80);
    } else if (row == 15) {
        Map_unk122_3(18, 0, 64);
    } else if (row == 18) {
        Map_unk123_3(18, 0, 16);
    } else if (row == 19) {
        return;
    }

    Map_unk97_3(2);

    column -= 1;
    Map_unk98_3(column, row, 3, 1,
                  column, *(s32 *)(Map_unk75_4(18) + 16) >> 20);
    Map_unk124_3(0, 0, 3, 1, column, row);
}

void AdvanceActor9AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;
    s32 companion16Column;

    s32 permuted_6;
    permuted_6 = *(s32 *)(Map_unk99_3(9) + 8) >> 20;
    row = *(s32 *)(Map_unk76_4(9) + 16) >> 20;
    column  = permuted_6;
    companion19Column = *(s32 *)(Map_unk77_4(19) + 8) >> 20;
    companion14Column = *(s32 *)(Map_unk78_4(14) + 8) >> 20;
    companion16Column = *(s32 *)(Map_unk79_4(16) + 8) >> 20;

    if (row == 8) {
        if ((u32)(companion16Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion14Column - 9) <= 2) {
            goto transition48;
        }
        if ((u32)(companion19Column - 9) > 2) {
            Map_unk125_3(9, 0, 80);
        }
        /* Falls through into the id-96 site from both paths. */
        Map_unk126_3(9, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
transition48:
            Map_unk127_3(9, 0, 48);
        } else {
            Map_unk128_3(9, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
            Map_unk129_3(9, 0, 32);
        } else {
            Map_unk130_3(9, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 9) <= 2) {
            return;
        }
        Map_unk131_3(9, 0, 80);
    } else if (row == 15) {
        Map_unk132_3(9, 0, 64);
    } else if (row == 18) {
        Map_unk133_3(9, 0, 16);
    }

    Map_unk100_3(2);

    column -= 1;
    Map_unk101_3(column, row, 3, 1,
                  column, *(s32 *)(Map_unk80_4(9) + 16) >> 20);
    Map_unk134_3(0, 0, 3, 1, column, row);
}

void RetreatActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Map_unk102_3(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Map_unk81_4(14) + 16) >> 20;
    companion18Row = *(s32 *)(Map_unk82_3(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Map_unk83_3(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 12) <= 2) {
            Map_unk135_3(14, -16, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            Map_unk135_3(14, -64, 0);
        } else {
            Map_unk135_3(14, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 12) <= 2) {
            Map_unk135_3(14, -48, 0);
        } else {
            Map_unk135_3(14, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        Map_unk135_3(14, -48, 0);
    } else if (column == 8) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        Map_unk135_3(14, -32, 0);
    } else if (column == 6) {
        return;
    }

    Map_unk103_3(2);

    rowM1 = row - 1;
    Map_unk104_3(column, rowM1, 1, 3,
                  *(s32 *)(Map_unk84_3(14) + 8) >> 20, rowM1);
    Map_unk136_3(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Map_unk105_3(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Map_unk85_3(14) + 16) >> 20;
    companion18Row = *(s32 *)(Map_unk86_3(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Map_unk87_3(9) + 16) >> 20;

    if (column == 6) {
        if ((u32)(companion9Row - 12) <= 2) {
            Map_unk137_3(14, 32, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            Map_unk137_3(14, 64, 0);
        } else {
            Map_unk137_3(14, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        Map_unk137_3(14, 80, 0);
    } else if (column == 9) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        Map_unk137_3(14, 64, 0);
    } else if (column == 12) {
        Map_unk137_3(14, 16, 0);
    } else if (column == 13) {
        return;
    }

    Map_unk106_3(2);

    rowM1 = row - 1;
    Map_unk107_3(column, rowM1, 1, 3,
                  *(s32 *)(Map_unk88_3(14) + 8) >> 20, rowM1);
    Map_unk138_3(0, 0, 1, 3, column, rowM1);
}

void RetreatActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Map_unk108_3(16) + 8) >> 20;
    permuted_7 = *(s32 *)(Map_unk89_3(16) + 16) >> 20;
    companion18Row = *(s32 *)(Map_unk90_3(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Map_unk91_3(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 9) <= 2) {
            Map_unk139_3(16, -16, 0);
        } else if ((u32)(companion18Row - 9) <= 2) {
            Map_unk139_3(16, -64, 0);
        } else {
            Map_unk139_3(16, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 9) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 9) <= 2) {
            Map_unk139_3(16, -48, 0);
        } else {
            Map_unk139_3(16, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 9) <= 2) {
            return;
        }
        Map_unk139_3(16, -48, 0);
    } else if (column == 8) {
        Map_unk139_3(16, -32, 0);
    } else if (column == 6) {
        return;
    }

    Map_unk109_3(2);

    rowM1 = row - 1;
    Map_unk110_3(column, rowM1, 1, 3,
                  *(s32 *)(Map_unk92_3(16) + 8) >> 20, rowM1);
    Map_unk140_3(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companionRow;

    s32 permuted_6;
    column = *(s32 *)(Map_unk111_3(16) + 8) >> 20;
    permuted_6 = *(s32 *)(Map_unk93_3(16) + 16) >> 20;
    companionRow = *(s32 *)(Map_unk94_3(9) + 16) >> 20;
    row  = permuted_6;

    if (column == 6) {
        if ((u32)(companionRow - 9) <= 2) {
            Map_unk141_3(16, 32, 0);
        } else {
            Map_unk141_3(16, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companionRow - 9) <= 2) {
            return;
        }
        Map_unk141_3(16, 80, 0);
    } else if (column == 9) {
        Map_unk141_3(16, 64, 0);
    } else if (column == 12) {
        Map_unk141_3(16, 16, 0);
    } else if (column == 13) {
        return;
    }

    Map_unk112_3(2);

    row -= 1;
    Map_unk113_3(column, row, 1, 3,
                  *(s32 *)(Map_unk95_3(16) + 8) >> 20, row);
    Map_unk142_3(0, 0, 1, 3, column, row);
}

/* map/locations/heidia/prologue/prologue_route_scene/actor_setup.c */
u8 *Map_unk143_3(s32 arg0);

u8 *Map_unk144_3(s32 arg0);

u8 *Map_unk145_3(s32 arg0);

u8 *Map_unk146_3(s32 arg0);

void ConfigureSceneActor11(s32 actor_id)
{
    s32 a = 0x1300000;
    s32 b = 0x1700000;
    u8 *p = Map_unk143_3(11);

    if (p != 0) {
        p[89] = 0;
    }
    Map_Apply8(Map_unk24(actor_id), 0);
    Map_SetMode3(0, a, b, 253);
    Map_unk32_2(576);
}

void ConfigureSceneActor12(s32 actor_id)
{
    s32 a = 0x500000;
    s32 b = 0x1700000;
    u8 *p = Map_unk144_3(12);

    if (p != 0) {
        p[89] = 0;
    }
    Map_Apply9(Map_unk25(actor_id), 0);
    Map_SetMode4(0, a, b, 253);
    Map_unk33_2(577);
}

void ConfigureSceneActor13(s32 actor_id)
{
    s32 a = 0x600000;
    s32 b = 0x1500000;
    u8 *p = Map_unk145_3(13);

    if (p != 0) {
        p[89] = 0;
    }
    Map_Apply10(Map_unk26(actor_id), 0);
    Map_SetMode5(0, a, b, 253);
    Map_unk34_2(578);
}

void ConfigureSceneActor14(s32 actor_id)
{
    s32 a = 0x900000;
    s32 b = 0x1400000;
    s32 c = 0x2f00000;
    s32 d = 0x1400000;
    u8 *p = Map_unk146_3(14);

    if (p != 0) {
        p[89] = 0;
    }
    Map_Apply11(Map_unk27(actor_id), 0);
    Map_SetMode6(0, a, b, 253);
    Map_SetMode7(0, c, d, 253);
    Map_unk35_2(579);
}

/* map/locations/heidia/prologue/prologue_route_scene/effect_spawn.c */
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

extern struct Descriptor *gOv2[];

u8 *Map_unk147_3(s32 kind, s32 x, s32 y, s32 z);

u8 *Map_unk148_3(s32 kind, s32 x, s32 y, s32 z);

struct Effect *Map_unk149_3();
struct Effect *Map_unk150_3();

struct SceneObject *Map_unk151_3(void);

/* Spawn and configure the mode-14 object variant. */

/* Spawn and configure the mode-15 object variant. */

/* Returns the party record; only its presentation block at +80 is read. */

u8 *SpawnMode14Effect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *object = Map_unk147_3(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        s32 mask = ~12;
        mask &= owner[9];
        owner[9] = mask;
        object[85] = 0;
        object[89] = 8;
        Map_Apply12(object, 0);
        Map_Apply13(object, 14);
        Map_Apply14(object, 1);
        return object;
    }
    return 0;
}

u8 *SpawnMode15Effect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *object = Map_unk148_3(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        s32 mask = ~12;
        mask &= owner[9];
        mask |= 4;
        owner[9] = mask;
        object[85] = 0;
        object[89] = 8;
        Map_Apply15(object, 0);
        Map_Apply16(object, 15);
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
    party = Map_unk149_3(0);

    /* 128 << 13.  With that bit set and an options block present the effect's
     * kind comes from the options rather than from the default 222. */
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Map_unk150_3(options->kind, x, y, z);
    } else {
        effect = Map_unk150_3(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    mode_block = block;

    Map_unk152_3(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Map_unk153_3(effect, gOv2[table_offset >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;

    /* 0x02008ab1 is OvObj_IntegrateVelocities with the Thumb bit: the per-frame
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
        Map_unk154_3(effect, options->mode);
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
            gOv2[table_offset >> 2];
        s32 delta;

        /* The 0x80000 test is the same register the previous block left live:
         * with a destination supplied the step is measured from it, otherwise
         * the target is biased by -1.0 in 16.16. */
        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = Map_unk28(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = Map_unk29(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        /* Only the FIRST call is per-arm.  The `b.n 0x02000c4c` at the end of
         * the first arm joins both arms onto the single second call site, so
         * the second delta is computed in each arm and the call is spelled
         * once. */
        effect->rate34 = Map_unk30(delta, duration);
    }

    if ((flags & 0x200000) != 0) {                  /* 128 << 14 */
        Map_unk155_3(effect, 1);
        Map_unk156_3(effect, options->callback_arg);
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

    object = Map_unk151_3();
    params.callback = 0x02008da5;
    for (angle_step = 0; angle_step <= 16; angle_step += 2) {
        angle = angle_step << 12;
        vec.x = Map_unk31(angle);
        vec.y = 0;
        z = Map_unk32(angle);
        x = vec.x;
        vec.z = z;
        x = x + Map_Apply17(x, 3);
        vec.x = x;
        Map_unk35_5(object->x, object->y, object->z, x, vec.y, z, 0x01000001, &params);
    }
}

/* map/locations/heidia/prologue/prologue_route_scene/messages.c */

void StartScriptedSceneMessage(s32 message_id)
{
    Map_unk36_2(message_id);
    Map_unk49_3(8, 0, 0);
    Map_unk37_2(4055);
    Map_Apply18(181, 3);
    Map_Apply19(181, 0);
    Map_unk157_3();
}

void ShowForgetEverythingMessage(void)
{
    Map_unk38_2(2244);
    {
        s32 k4 = 8, k5 = 21;

        Map_unk36_5(0, 0, 1, 1, k4, k5);
    }
}

void ShowHelpYouForgetMessage(void)
{
    Map_unk39_2(2245);
}

void ShowDamagedDoorMessage(void)
{
    Map_unk40_2(2246);
}

void ShowSaveMyLifeMessage(void)
{
    Map_unk41_2(2247);
}

/* map/locations/heidia/prologue/prologue_route_scene/orbiting_effect.c */
#define UpdateOrbitingSceneObject Value

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

extern u8 Value;

OrbitingSceneObject *GetOrbitingSceneObject(void);

u8 *AllocateEffectTransfer(s32, s32);

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

/* map/locations/heidia/prologue/prologue_route_scene/overlay_object.c */
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

/* map/locations/heidia/prologue/prologue_route_scene/prologue.c */
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

extern s32 gOv3[];
extern s32 StagedActorStepTable[];
extern u8 gOv4[];

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
    obj = (u8 *)Map_unk50_3(&sel, a + 1, a);
    if (obj == 0)
        return 0;
    p = obj + 0x22;
    *p = 2;
    cnt = 0;
    E = (u8 *)gOv3;
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
        d = StagedActorStepTable[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (StagedActorStepTable[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)gOv3;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)gOv3;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Map_Apply20(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (StagedActorStepTable[sel] & 0xffff0000);
        buf[2] = buf[2] + (StagedActorStepTable[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = StagedActorStepTable[sel];
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
    Map_unk158_3();
    Map_unk159_3();
    Map_unk160_3();
    Map_unk161_3();
}

void StartSceneScript37(void)
{
    Map_unk51_3(gOv4, 37, 7);
    Map_unk42_2(183);
    Map_unk43_2(4);
}

void RunSceneVectorTransition(void)
{
    struct V6 transition;

    Map_unk162_3();
    if (Map_unk33(&transition) != 0) {
        Map_unk44_2(transition);
    }
    Map_unk163_3();
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

        Map_unk52_3(0x200000, direction, &destination);
    }
    if (Map_Apply21(actor, &destination) == 0) {
        s32 t;

        Map_unk164_3();
        Map_Apply22(actor, 6);
        Map_unk45_2(6);
        Map_unk46_2(152);
        Map_Apply23(actor, 7);
        actor->f30 = 0x30000;
        actor->f34 = 0x20000;
        actor->f28 = 0x40000;
        t = 126;
        t &= *state;
        *state = (u8)t;
        Map_Apply24(actor, 0);
        {
            s16 *coordinates = (s16 *)&destination;

            Map_unk53_3(0, coordinates[1], coordinates[5]);
        }
        Map_Apply25(actor, 6);
        Map_Apply26(actor, 1);
        *state = (u8)saved_state;
        Map_unk165_3();
        return 1;
    }
    return 0;
}

/* map/locations/heidia/prologue/prologue_route_scene/scene_data.c */
extern s16 gCell[];
extern u8 Value_00000071;
extern u8 Value_00000072;
extern u8 Value_0000007b;
extern u8 Value_0000007c;
extern u8 Value_0000007d;
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 Value_00000086;
extern u8 Value_0000007e;
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 gOv15[];
extern u8 gOv16[];
extern u8 gOv17[];
extern u8 gOv18[];
extern u8 gOv19[];
extern u8 gOv20[];

s32 SelectPrimarySceneData(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000071) {
        return (s32)gOv5;
    }
    if (v == (s32)&Value_00000072) {
        return (s32)gOv6;
    }
    if (v == (s32)&Value_0000007b) {
        return (s32)gOv7;
    }
    if (v == (s32)&Value_0000007c) {
        return (s32)gOv8;
    }
    if (v == (s32)&Value_0000007d) {
        return (s32)gOv9;
    }
    return (s32)gOv10;
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
    s16 v = gCell[224];

    if (v == (s32)&Value_00000071) {
        return (s32)gOv11;
    }
    if (v == (s32)&Value_0000007b) {
        return (s32)gOv12;
    }
    if (v <= (s32)&Value_00000086 && v >= (s32)&Value_0000007e) {
        return (s32)gOv13;
    }
    return (s32)gOv14;
}

s32 SelectTertiarySceneData(void)
{
    s16 scene_id = gCell[224];

    if (scene_id == (s32)&Value_00000071) {
        return (s32)gOv15;
    }
    if (scene_id == (s32)&Value_00000072) {
        return (s32)gOv16;
    }
    if (scene_id == (s32)&Value_0000007b) {
        return (s32)gOv17;
    }
    if (scene_id == (s32)&Value_0000007c) {
        return (s32)gOv18;
    }
    if (scene_id == (s32)&Value_0000007d) {
        return (s32)gOv19;
    }
    return (s32)gOv20;
}

/* map/locations/heidia/prologue/prologue_route_scene/scene_primary_script.c */
typedef union {
    s32 w;
    s16 h[2];
} RecWord;

extern u8 gVal[];
extern u8 gVal2[];
extern u8 gVal3[];
extern u8 gVal4[];
extern u8 gVal5[];
extern u8 gCell[];
extern u8 gWork[];
extern s16 gCell2[][1];

u8 *Map_unk166_3();

s32 Map_unk167_3(void)
{
    u8 *rec7;
    s32 scene;
    s32 cur;
    s32 lo;

    ((RecWord *)(*(u8 *volatile *)gWork + 0x1c0))->w = 0x204;
    scene = gCell2[224][0];
    if (scene == (s32)gVal2) {
        Map_unk168_3();
        return 0;
    }
    if (scene == (s32)gVal3) {
        if (Map_Check(0xef7) == 0) {
            Map_SetRect(0, 3, 1, 1, 13, 40);
            Map_unk2_5(0, 2, 1, 1, 15, 40);
            Map_Place(101, 0xd80000, 0x2880000);
        }
        if (gCell2[224][0] == scene) {
            if (gCell2[225][0] != 5) {
                if (Map_unk2(0x8d1) == 0) {
                    return 0;
                }
            }
            Map_Do(0x8d1);
            Map_unk3_5(0, 1, 1, 1, 13, 30);
            Map_unk2_3(100, 0xd80000, 0x1e80000);
            return 0;
        }
    }
    cur = gCell2[224][0];
    if (cur == (s32)gVal) {
        s32 rec;

        Map_unk169_3();
        rec = Map_unk34(8);
        *(volatile s32 *)(rec + 56) = 0x810000;
        Map_unk170_3(9);
        Map_unk171_3(10);
        if (Map_unk3(0x240) != 0) {
            rec7 = Map_unk4(11);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Map_unk172_3((s32)rec7, 4);
                Map_unk173_3((s32)rec7, 0);
            }
            Map_Run(0, 0x1300000, 0x1700000, 253);
        }
        if (Map_unk5(0x241) != 0) {
            rec7 = Map_unk6(12);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Map_unk174_3((s32)rec7, 4);
                Map_unk175_3((s32)rec7, 0);
            }
            Map_unk2_4(0, 0x500000, 0x1700000, 253);
        }
        if (Map_unk7(0x242) != 0) {
            rec7 = Map_unk8(13);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Map_unk176_3((s32)rec7, 4);
                Map_unk177_3((s32)rec7, 0);
            }
            Map_unk3_4(0, 0x600000, 0x1500000, 253);
        }
        if (Map_unk9(0x243) != 0) {
            rec7 = Map_unk10(14);
            if ((s32)rec7 != 0) {
                rec7[89] = 0;
                Map_unk178_3((s32)rec7, 4);
                Map_unk179_3((s32)rec7, 0);
            }
            Map_unk4_4(0, 0x900000, 0x1400000, 253);
            Map_unk5_4(0, 0x2f00000, 0x1400000, 253);
        }
        if (Map_unk11(0xfd7) != 0) {
            return 0;
        }
        Map_unk180_3(8);
        return 0;
    }
    lo = (s32)gVal4;
    if (cur == lo) {
        if (Map_unk12(0xef4) == 0) {
            Map_unk4_5(0, 0, 1, 1, 37, 10);
            Map_unk3_3(100, 0x2580000, 0xa80000);
        }
    }
    cur = gCell2[224][0];
    if (cur >= lo) {
        if (cur <= (s32)gVal5) {
            Map_unk181_3();
            if (gCell2[225][0] == 5) {
                Map_unk182_3();
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

    rec8 = Map_unk166_3(8);
    rec7 = Map_unk13((gCell2[224][0] + (0x8d2 - (s32)gVal4)));
    if (rec7 != 0) {
        Map_unk4_3(8, 0x28a0000, 0xa80000);
        *(volatile s32 *)((s32)rec8 + 12) = -0x200000;
        record = Map_unk35(8);
        Map_unk183_3(record, 0);
        Map_unk184_3(8, 3);
        rec8[85] = 0;
        {
            u8 value = *(volatile u8 *)&rec8[35];

            rec8[35] = (u8)(value | 2);
        }
        Map_unk5_5(42, 10, 1, 1, 40, 10);
    } else {
        *(u8 *)(Map_unk36(8) + 85) = rec7;
    }
}

void Scene_RunScene3b2(void)
{
    u32 i;
    s32 record;

    Map_unk185_3();
    Map_unk5_3(0, 0x6666, 0x3333);
    Map_unk186_3();
    Map_unk187_3();
    Map_unk188_3((gCell2[224][0] + (0x8c8 - (s32)gVal4)));
    Map_unk189_3(30);
    Map_unk6_3(0x200b2bc, 44, 7);
    Map_unk7_3(0, 3, -16);
    Map_unk190_3(3);
    Map_unk191_3();
}

void Scene_RunScene3b2(void)
{
    u32 i;
    s32 record;

    if (Map_unk14(0x8c4) != 0) {
        Map_unk6_5(0, 0, 1, 1, 8, 21);
        Map_unk8_3(15, 0x3c80000, 0x3c80000);
    } else {
        record = Map_unk37(15);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Map_unk15(0x8c5) != 0) {
        Map_unk9_3(16, 0x3c80000, 0x3c80000);
    } else {
        record = Map_unk38(16);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Map_unk16(0x8c6) != 0) {
        Map_unk10_3(17, 0x3c80000, 0x3c80000);
    } else {
        record = Map_unk39(17);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Map_unk17(0x8c7) != 0) {
        Map_unk11_3(18, 0x3c80000, 0x3c80000);
    } else {
        record = Map_unk40(18);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
}

/* map/locations/heidia/prologue/prologue_route_scene/scene_state.c */
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

extern u8 gCell[];

struct S *Map_unk192_3(void);

struct S *Map_unk193_3(void);

/* Copy the low two mode bits into the object's owner record. */

void SetEffectOwnerMode(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}

void InitializePrologueSceneState(void)
{
    u8 *base;

    Map_unk194_3();
    base = gCell;
    Map_Apply27(*(s16 *)(base + 448), 5);
    base[555] = 3;
    Map_Apply28(84, 5);
    Map_unk195_3();
}

void MarkGridLeftOfSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute)
{
    struct S *p = Map_unk192_3();

    if (p != 0) {
        s32 v;

        Map_Apply29(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k5 = p->f10 >> 20;
            s32 k4 = (p->f08 >> 20) - 1;

            Map_unk37_5(grid_value, grid_attribute, 3, 1, k4, k5);
        }
    }
}

void MarkGridAboveSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute)
{
    struct S *p = Map_unk193_3();

    if (p != 0) {
        s32 v;

        Map_Apply30(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k4 = p->f08 >> 20;
            s32 k5 = (p->f10 >> 20) - 1;

            Map_unk38_5(grid_value, grid_attribute, 1, 3, k4, k5);
        }
    }
}

void SetSceneActorModes(int actor_id)
{
    Map_Apply31(actor_id, 1);
    Map_Apply32(actor_id, 2);
}

void InitializeEscapeSceneActors(void)
{
    Map_SetMode8(8, 0x11, 0x1E, 0x15);
    Map_SetMode9(0xA, 0x11, 0x1F, 0x16);
    Map_SetMode10(0xB, 0x14, 0x1E, 0x17);
    Map_SetMode11(0xC, 0x15, 0x1E, 0x18);
    Map_SetMode12(0xD, 0x16, 0x1E, 0x19);
    Map_SetMode13(0xF, 0x17, 0x1E, 0x1A);
    Map_SetMode14(0x11, 0, 0x1E, 0x1F);
    Map_SetMode15(0x12, 0, 0x1F, 0x20);
    Map_SetMode16(9, 0, 0x20, 0x21);
    Map_SetMode17(0x13, 4, 0x1E, 0x22);
    Map_SetMode18(0xE, 5, 0x1E, 0x23);
    Map_SetMode19(0x10, 6, 0x1E, 0x24);
}
