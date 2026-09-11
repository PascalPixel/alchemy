#include "types.h"
#include "scene.h"
#include "spawn_configured_effect.h"
#include "spawn_configured_effect_body.inc"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"
#include "find_clear_actor_position.h"
#include "find_clear_actor_position_body.inc"

/* overlays/scene/story/branching_event/branching_event.c */
/* overlays/scene/story/branching_event/actor_facing.c */
struct Obj {
    u8 pad00[6];
    u16 f06;
};

extern u8 RuntimeSelectorTable[];

struct Obj *Story_unk18_4();

void Actor_RunSlotZeroFacingCheck(void)
{
    struct Obj *p = Story_unk18_4(0);
    s32 x = Story_unk22();
    s32 m = (p->f06 + 0x2000) & 0xc000;
    s32 r = -1;

    if (RuntimeSelectorTable[498] == 1 || x == 0) {
        if (m == 0xc000) {
            r = Story_unk23();
        }
        if (m == 0x4000) {
            r = Story_unk24();
        }
    }
    if (r != 0) {
        if (RuntimeSelectorTable[498] != 1) {
            Story_unk19_4();
        }
    }
}

/* overlays/scene/story/branching_event/actor_motion.c */
typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    Sub *unk50;
} Ent;

struct LevelCheckRecord {
    u8 unknown_00[12];
    s32 y;                          /* +0x0c */
};

extern s32 Story_Far2[];
extern s32 gOv[];
extern struct LevelCheckRecord *Story_unk20_4();   /* record by slot index */
extern struct LevelCheckRecord *Story_unk21_4();   /* occupant one square ahead */
extern void Story_unk22_4();                       /* push the object ahead */

Ent *Story_unk23_4(Desc *, Ent *);

s32 Actor_ResetMotionWhenAheadBlocked(Ent *a)
{
    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Story_Far2[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Story_unk23_4(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = gOv;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Story_Far2[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Story_EntOp(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

void Actor_PushObjectAheadIfLevel(void)
{
    struct LevelCheckRecord *p = Story_unk20_4(0);
    struct LevelCheckRecord *q = Story_unk21_4(p);
    s32 diff;

    if (q == 0) {
        return;
    }

    diff = q->y - p->y;

    if (diff >= 0) {
        /* Written with an empty arm on purpose: the reference branches away on
         * the *return* condition (`bge`), and spelling this as a plain
         * `if (diff >= 0x80000) return;` inverts it to `blt`. Arm order
         * decides the branch sense; no flag moves it. */
        if (diff < 0x80000) {
        } else {
            return;
        }
    } else if (p->y - q->y >= 0x80000) {
        return;
    }

    Story_unk22_4();
}

/* overlays/scene/story/branching_event/actor_presentation.c */

s32 *Story_unk24_4();

void ActorDraw_SetSceneCellByFlag985(void)
{
    if (Story_unk25(0x985) == 0) {
        s32 k5 = 17, k6 = 78;

        Story_unk25_4(36, 78, 1, 2, k5, k6);
    } else {
        s32 k5 = 17, k6 = 78;

        Story_unk26_4(34, 78, 1, 2, k5, k6);
    }
}

void ActorDraw_PlaceActorTwelveAtTile20And12(void)
{
    s32 *p = Story_unk24_4(12);
    s32 a = p[2] >> 20;

    if (a == 20) {
        s32 b = p[4] >> 20;

        if (b == 12) {
            ((u8 *)p)[85] = 2;
            p[5] = 0x300000;
            ((u8 *)p)[35] = 2;
            {
                s32 k5 = a, k6 = b;

                Story_unk27_4(38, 12, 1, 1, k5, k6);
            }
        }
    }
}

/* overlays/scene/story/branching_event/conditional_scene_setup.c */
s32 *Story_unk28_4();
s32 *Story_unk29_4();
s32 *Story_unk30_4();

void State_BranchOnActorEightOrNineTile(void)
{
    s32 *p = Story_unk28_4(9);

    if ((Story_unk29_4(0)[2] >> 20) <= 12) {
        p = Story_unk30_4(8);
        if ((p[2] >> 20) == 6) {
            if ((p[4] >> 20) == 20) {
                Story_unk31_4();
                return;
            }
        }
    } else {
        if ((p[2] >> 20) == 18) {
            if ((p[4] >> 20) == 20) {
                Story_unk31_4();
                return;
            }
        }
    }
    Story_unk32_4();
}

/* overlays/scene/story/branching_event/configurable_effect_spawn.c */

void Effect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct Options *options)
{
}

/* overlays/scene/story/branching_event/effect_motion_variant_b.c */
/*
 * Per-frame integrator for an effect record -- resource_3c5. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

/* Every field is named by offset; the layout is not verified. */
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

/*
 * A loader-relocated call word rather than a runtime address; it turns a value
 * and a duration into a per-frame step. Declared without a prototype, and the
 * call site passes two arguments.
 */

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void advance_effect_motion(struct Effect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Story_unk26(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

/* overlays/scene/story/branching_event/event_script.c */

extern u8 gWork[];

/* Scene calls are bound to loader-runtime addresses by the translation unit. */

/* The translation unit binds scene calls at the loader-runtime base. */

void Scene_RunBranchingActorSequence(void)
{
    s32 record;

    Story_Do(0x988);
    Story_unk2_2(0x98a);
    Story_unk34_4();
    Story_unk35_4();
    Story_unk3_2(0x2702);
    Story_Place(0, 0x10000, 0x8000);
    Story_unk2_3(0, 0x128, 0x160);
    Story_unk3_3(0, 0xc000, 0);
    Story_unk36_4(10);
    Story_Run(10, 16, 0, 0xc000);
    Story_unk2_4(1, -8, 16, 0xc000);
    Story_unk3_4(2, 8, 16, 0xc000);
    Story_unk4_4(3, 24, 16, 0xc000);
    Story_unk37_4(3);
    Story_unk38_4(20);
    Story_unk39_4(14, 0);
    Story_unk40_4(10);
    Story_unk5_4(0x30000, 0x6000);
    Story_unk6_4(0x1180000, -1, 0x1200000, 1);
    Story_unk41_4();
    Story_unk42_4(20);
    Story_unk43_4(11, 3);
    Story_unk44_4(30);
    Story_unk45_4(11, 0);
    Story_unk46_4(10);
    Story_unk4_3(14, 0xc000, 0);
    Story_unk5_3(11, 0xc000, 0);
    Story_unk47_4(30);
    Story_unk48_4(10);
    Story_unk49_4(14, 2);
    Story_unk50_4(20);
    Story_unk51_4(14, 0);
    Story_unk52_3(10);
    Story_unk53_3(13, 2);
    Story_unk54_3(12, 2);
    Story_unk55_3(40);
    Story_unk56_3(13, 2);
    Story_unk57_3(12, 2);
    Story_unk58_3(40);
    Story_unk59_3(13, 2);
    Story_unk60_3(12, 2);
    Story_unk61_3(40);
    Story_unk6_3(12, 0x102, 50);
    Story_unk62_3(12, 0);
    Story_unk63_3(10);
    Story_unk64_3(13, 0, 0);
    Story_unk65_3(25);
    Story_unk66_3(13, 2);
    Story_unk67_3(20);
    Story_unk68_3(13, 0);
    Story_unk69_3(10);
    Story_unk7_3(12, 0x8000, 0);
    Story_unk70_3(20);
    Story_unk71_3(13, 4);
    Story_unk72_3(20);
    Story_unk73_3(13, 0);
    Story_unk74_3(10);
    Story_unk75_3(11, 4);
    Story_unk76_3(20);
    Story_unk77_3(11, 0);
    Story_unk78_3(20);
    Story_unk8_3(13, 0x107, 40);
    Story_unk79_3(10);
    Story_unk9_3(13, 0x4000, 0);
    Story_unk80_3(20);
    Story_unk81_3(13, 0);
    Story_unk82_3(20);
    Story_unk10_3(13, 0x101, 75);
    Story_unk11_3(14, 0x101, 60);
    Story_unk12_3(12, 0x4000, 0);
    Story_unk83_3(20);
    Story_unk13_3(11, 0x4000, 0);
    Story_unk84_3(20);
    Story_unk14_3(14, 0x4000, 0);
    Story_unk85_3(30);
    Story_unk86_3(14, 0);
    Story_unk7_4(0x1180000, -1, 0x1400000, 1);
    Story_unk87_3();
    Story_unk88_3(20);
    Story_unk89_3(10, 3);
    Story_unk90_3(30);
    Story_unk91_3(10, 0);
    Story_unk92_3(10);
    Story_unk93_3(10, 4);
    Story_unk94_3(20);
    Story_unk95_3(10, 0);
    Story_unk96_3(20);
    Story_unk15_3(14, 0x105, 60);
    Story_unk97_3(14, 0);
    Story_unk98_3(10);
    Story_unk99_3(3, 4);
    Story_unk100_3(20);
    Story_unk101_3(3, 0);
    Story_unk102_3(10);
    Story_unk103_3(2, 2);
    Story_unk104_3(20);
    Story_unk105_3(2, 0);
    Story_unk106_3(10);
    Story_unk107_3(1, 2);
    Story_unk108_3(20);
    Story_Check(1, 0);
    if (Story_unk2(0, 0) == 0) {
        Story_unk109_3(30);
        Story_unk16_3(10, 0x8000, 0);
        Story_unk110_3(30);
        Story_unk111_3(10, 3);
        Story_unk112_3(30);
        Story_unk113_3(10, 0);
        bump_step(1);
    } else {
        Story_unk114_3(30);
        Story_unk17_3(10, 0x8000, 0);
        Story_unk115_3(30);
        Story_unk116_3(10, 4);
        Story_unk117_3(20);
        bump_step(1);
        Story_unk118_3(10, 0);
    }
    Story_unk119_3(10);
    Story_unk18_3(14, 0x101, 60);
    Story_unk19_3(14, 0x10000, 0x8000);
    Story_unk120_3(14, 0, 16);
    Story_unk121_3(20);
    Story_unk122_3(14, 0);
    Story_unk123_3(10);
    Story_unk20_3(10, 0xc000, 0);
    Story_unk124_3(35);
    Story_unk125_3(10, 0);
    Story_unk126_3(10);
    Story_unk127_3(14, 2);
    Story_unk128_3(20);
    Story_unk129_3(14, 0);
    Story_unk130_3(20);
    Story_unk21_3(3, 0x102, 40);
    Story_unk131_3(3, 0);
    Story_unk132_3(10);
    Story_unk133_3(2, 2);
    Story_unk134_3(20);
    Story_unk135_3(2, 0);
    Story_unk136_3(10);
    Story_unk137_3(1, 3);
    Story_unk138_3(30);
    Story_unk139_3(1, 0);
    Story_unk140_3(20);
    Story_unk22_3(14, 0x100, 40);
    Story_unk3(14, 0);
    if (Story_unk4(0, 0) == 0) {
        Story_unk141_3(30);
        Story_unk142_3(14, 4);
        ((void (*)())Story_unk27)(20);
        Story_unk143_3(14, 0);
        bump_step(1);
    } else {
        Story_unk144_3(30);
        ((void (*)())Story_unk28)(14, 4);
        Story_unk145_3(20);
        bump_step(1);
        Story_unk146_3(14, 0);
    }
    Story_unk147_3(10);
    Story_unk23_3(10, 0x102, 50);
    Story_unk148_3(10, 0);
    Story_unk149_3(10);
    Story_unk150_3(13, 2);
    Story_unk151_3(20);
    Story_unk24_3(13, 0x14ccc, 0xa666);
    Story_unk152_3(13, 0, 16);
    Story_unk153_3(20);
    Story_unk154_3(13, 0);
    Story_unk155_3(10);
    Story_unk25_3(12, 0x14ccc, 0xa666);
    Story_unk156_3(12, 0, 16);
    Story_unk157_3(20);
    Story_unk26_3(12, 0x107, 50);
    Story_unk158_3(12, 0);
    Story_unk159_3(10);
    Story_unk160_3(14, 3);
    Story_unk161_3(30);
    Story_unk162_3(10);
    Story_unk27_3(10, 0x102, 60);
    Story_unk28_3(10, 0x8000, 0);
    Story_unk163_3(25);
    Story_unk164_3(10, 0);
    Story_unk165_3(10);
    Story_unk166_3(10, 2);
    Story_unk167_3(20);
    Story_unk5(10, 0);
    if (Story_unk6(0, 0) == 0) {
        Story_unk168_3(30);
        Story_unk169_3(10, 3);
        ((void (*)())Story_unk29)(30);
        Story_unk170_3(10, 0);
        bump_step(1);
    } else {
        Story_unk171_3(30);
        Story_unk172_3(10, 4);
        Story_unk173_3(20);
        bump_step(1);
        Story_unk174_3(10, 0);
    }
    Story_unk175_3(10);
    Story_unk29_3(10, 0xc000, 0);
    Story_unk176_3(35);
    Story_unk177_3(14, 3);
    Story_unk178_3(30);
    Story_unk30_3(14, 0xb000, 0);
    Story_unk179_3(40);
    Story_unk180_3(14, 0);
    Story_unk181_3(10);
    Story_unk182_3(12, 13, 50);
    Story_unk183_3(12, 3);
    Story_unk184_3(13, 3);
    Story_unk185_3(30);
    Story_unk31_3(12, 0x4000, 0);
    Story_unk32_3(13, 0x4000, 0);
    Story_unk186_3(20);
    Story_unk187_3(12, 3);
    Story_unk188_3(13, 3);
    Story_unk189_3(30);
    Story_unk33_3(12, 0x10000, 0x8000);
    Story_unk34_3(13, 0x10000, 0x8000);
    Story_unk190_3(12, 32, 0);
    Story_unk191_2(13, 32, 0);
    Story_unk192_2(12, 0, 16);
    Story_unk193_2(13, 16, 0);
    Story_unk35_3(13, 0x158, 0x138);
    Story_unk36_3(12, 0x158, 0x150);
    Story_unk194_2(13, 1);
    Story_unk37_3(12, 0x8000, 0);
    Story_unk38_3(13, 0x8000, 0);
    Story_unk195_2(10);
    Story_unk39_3(14, 0x4000, 0);
    Story_unk196_2(20);
    Story_unk197_2(14, 0);
    Story_unk198_2(10);
    Story_unk40_3(14, 0x10000, 0x8000);
    Story_unk41_3(14, 0x148, 0x138);
    Story_unk42_3(14, 0x8000, 0);
    Story_unk199_2(30);
    Story_unk43_3(11, 0x10000, 0x8000);
    Story_unk44_3(11, 0x148, 0x148);
    Story_unk45_3(11, 0x8000, 0);
    Story_unk200_2(20);
    Story_unk46_3(1, 0x13333, 0x9999);
    Story_unk47_3(2, 0x13333, 0x9999);
    Story_unk48_3(3, 0x13333, 0x9999);
    Story_unk201_2(1, 2);
    record = Story_unk7(0);
    if (record != 0) {
        Story_unk202_2(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk203_2(1);
    Story_unk204_2(1, 0, 0);
    Story_unk205_2(2, 2);
    record = Story_unk8(0);
    if (record != 0) {
        Story_unk206_2(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk207_2(2);
    Story_unk208_2(2, 0, 0);
    Story_unk209_2(3, 2);
    record = Story_unk9(0);
    if (record != 0) {
        Story_unk210_2(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk211_2(3);
    Story_unk212_2(3, 0, 0);
    Story_unk213_2(10);
    Story_unk214_2();
}

void Scene_RunActorEventSequence(void)
{
    s32 record;

    Story_unk4_2(0x989);
    Story_unk215_2();
    Story_unk216_2();
    Story_unk5_2(0x272f);
    Story_unk49_3(0, 0x10000, 0x8000);
    Story_unk50_3(0, 0x128, 0x138);
    Story_unk217_2(0, 0, 0);
    Story_unk218_2(10);
    Story_unk219_2(1, 0, 16, 0);
    Story_unk8_4(2, -16, -8, 0);
    Story_unk9_4(3, -16, 24, 0);
    Story_unk220_2(3);
    Story_unk221_2(20);
    Story_unk10_4(0x30000, 0x6000);
    Story_unk11_4(0x1180000, -1, 0x1480000, 1);
    Story_unk222_2();
    Story_unk223_2(10);
    Story_unk224_2(10);
    Story_unk51_3(10, 0xb000, 0);
    Story_unk225_2(10);
    Story_unk52_2(10, 0x100, 40);
    Story_unk226_2(10, 0);
    Story_unk227_2(10, 4, 13);
    Story_unk228_2(10, 4, 30);
    Story_unk229_2(10);
    Story_unk53_2(11, 0x100, 0);
    Story_unk54_2(12, 0x100, 0);
    Story_unk55_2(13, 0x100, 0);
    Story_unk56_2(14, 0x100, 40);
    Story_unk230_2(10);
    Story_unk57_2(14, 0xb000, 0);
    Story_unk58_2(11, 0xb000, 0);
    Story_unk59_2(12, 0xb000, 0);
    Story_unk60_2(13, 0xb000, 0);
    Story_unk231_2(30);
    Story_unk232_2(14, 2);
    Story_unk233_2(20);
    Story_unk234_2(14, 0);
    Story_unk235_2(20);
    Story_unk61_2(13, 0x102, 40);
    Story_unk236_2(13, 0);
    Story_unk237_2(20);
    Story_unk62_2(12, 0x101, 50);
    Story_unk63_2(12, 0x8000, 0);
    Story_unk238_2(25);
    Story_unk239_2(12, 0);
    Story_unk240_2(10);
    Story_unk64_2(14, 0x8000, 0);
    Story_unk65_2(11, 0x8000, 0);
    Story_unk66_2(13, 0x8000, 0);
    Story_unk241_2(30);
    Story_unk242_2(10);
    Story_unk243_2(3, 2);
    Story_unk244_2(20);
    Story_unk245_2(3, 0);
    Story_unk246_2(10);
    Story_unk247_2(2, 2);
    Story_unk248_2(20);
    Story_unk249_2(2, 0);
    Story_unk250_2(10);
    Story_unk67_2(1, 0x102, 40);
    Story_unk251_2(1, 0);
    Story_unk252_2(20);
    Story_unk253_2(10, 4);
    Story_unk254_2(20);
    Story_unk255_2(10, 0);
    Story_unk256_2(10);
    Story_unk257_2(14, 2);
    Story_unk258_2(25);
    Story_unk259_2(14, 0);
    Story_unk260_2(10);
    Story_unk261_2(10, 2);
    Story_unk262_2(20);
    Story_unk68_2(10, 0x10000, 0x8000);
    Story_unk69_2(10, 0, -40);
    Story_unk263_2(10, 0, 0);
    Story_unk264_2(20);
    Story_unk265_2(10, 0);
    Story_unk266_2(10);
    Story_unk267_2(14, 3);
    Story_unk268_2(30);
    Story_unk269_2(10);
    Story_unk70_2(13, 0x4000, 0);
    Story_unk71_2(12, 0xc000, 0);
    Story_unk270_2(30);
    Story_unk271_2(12, 3);
    Story_unk272_2(13, 3);
    Story_unk273_2(30);
    Story_unk72_2(13, 0x8000, 0);
    Story_unk73_2(12, 0x8000, 0);
    Story_unk274_2(20);
    Story_unk275_2(10, 3);
    Story_unk276_2(30);
    Story_unk277_2(10, 1);
    Story_unk74_2(10, 0, -32);
    Story_unk278_2();
    Story_unk6_2(0x301);
    Story_unk12_4(-1, -1, -1, 0);
    Story_unk279_2();
    Story_unk75_2(10, 0x102, 40);
    Story_unk280_2(10, 0);
    Story_unk281_2(10);
    Story_unk76_2(14, 0xb000, 0);
    Story_unk77_2(11, 0xb000, 0);
    Story_unk78_2(12, 0xb000, 0);
    Story_unk79_2(13, 0xb000, 0);
    Story_unk80_2(0, 0xc000, 0);
    Story_unk81_2(1, 0xc000, 0);
    Story_unk82_2(2, 0xc000, 0);
    Story_unk83_2(3, 0xc000, 0);
    Story_unk282_2(30);
    Story_unk84_2(11, 0x100, 0);
    Story_unk85_2(12, 0x100, 0);
    Story_unk86_2(13, 0x100, 0);
    Story_unk87_2(14, 0x100, 70);
    Story_unk13_4(0x1180000, -1, 0x1380000, 1);
    Story_unk283_2();
    Story_unk284_2(10);
    Story_unk88_2(12, 0x102, 40);
    Story_unk285_2(12, 0);
    Story_unk286_2(10);
    Story_unk287_2(13, 4);
    Story_unk288_2(20);
    Story_unk289_2(13, 0);
    Story_unk290_2(10);
    Story_unk291_2(2, 2);
    Story_unk292_2(30);
    Story_unk89_2(2, 0x2000, 0);
    Story_unk293_2(20);
    Story_unk294_2(2, 0);
    Story_unk295_2(10);
    Story_unk90_2(10, 0x4000, 0);
    Story_unk296_2(30);
    Story_unk91_2(3, 0xe000, 0);
    Story_unk297_2(30);
    Story_unk92_2(3, 0x102, 40);
    Story_unk93_2(0, 0x3000, 0);
    Story_unk298_2(20);
    Story_unk299_2(3, 0);
    Story_unk300_2(10);
    Story_unk301_2(1, 4);
    Story_unk302_2(20);
    Story_unk303_2(1, 0);
    Story_unk304_2(20);
    Story_unk94_2(14, 0x103, 50);
    Story_unk95_2(14, 0x8000, 0);
    Story_unk305_2(20);
    Story_unk10(14, 0);
    Story_unk306_2(0, 0, 0);
    Story_unk96_2(11, 0x8000, 0);
    Story_unk97_2(12, 0x8000, 0);
    Story_unk98_2(13, 0x8000, 0);
    Story_unk99_2(2, 0x1000, 0);
    Story_unk100_2(3, 0xe000, 0);
    if (Story_unk11(0, 0) == 0) {
        Story_unk307(30);
        Story_unk308(14, 2);
        Story_unk309(20);
        Story_unk310(14, 0);
        bump_step(1);
    } else {
        Story_unk311(30);
        Story_unk312(14, 2);
        Story_unk313(20);
        bump_step(1);
        Story_unk314(14, 0);
    }
    Story_unk315(10);
    Story_unk316(10, 2);
    Story_unk317(20);
    Story_unk318(10, 0, 16);
    Story_unk101_2(10, 0x2000, 0);
    Story_unk319(20);
    Story_unk320(10, 0);
    Story_unk321(10);
    Story_unk102_2(14, 0xa000, 0);
    Story_unk322(20);
    Story_unk323(14, 4);
    Story_unk324(20);
    Story_unk325(14, 0);
    Story_unk326(10);
    Story_unk327(10, 3);
    Story_unk328(30);
    Story_unk329(10, 0);
    Story_unk330(10);
    Story_unk331(14, 2);
    Story_unk332(20);
    Story_unk12(14, 0);
    Story_unk333(40);
    Story_unk103_2(10, 0x5000, 0);
    Story_unk334(20);
    Story_unk104_2(10, 0x101, 60);
    if (Story_unk13(0, 0) == 0) {
        Story_unk335(30);
        Story_unk105_2(10, 0x2000, 0);
        Story_unk336(20);
        Story_unk337(10, 3);
        Story_unk338(30);
        Story_unk339(10, 0);
        bump_step(1);
    } else {
        Story_unk340(30);
        Story_unk106_2(10, 0x2000, 0);
        Story_unk341(20);
        Story_unk342(10, 4);
        Story_unk343(20);
        bump_step(1);
        Story_unk344(10, 0);
    }
    Story_unk345(10);
    Story_unk107_2(14, 0x102, 50);
    Story_unk346(14, 0);
    Story_unk347(15);
    Story_unk348(10, 3);
    Story_unk349(30);
    Story_unk350(20);
    Story_unk351(14, 2);
    Story_unk352(40);
    Story_unk108_2(14, 0x8000, 0);
    Story_unk353(20);
    Story_unk354(14, 0);
    Story_unk355(10);
    Story_unk356(14, 3);
    Story_unk357(30);
    Story_unk358(14, 0);
    Story_unk359(10);
    Story_unk109_2(10, 0x5000, 0);
    Story_unk360(25);
    Story_unk361(10, 2);
    Story_unk362(20);
    Story_unk110_2(10, 0x138, 0x138);
    Story_unk363(0, 0, 0);
    Story_unk111_2(10, 0x8000, 0);
    Story_unk364(25);
    Story_unk365(10, 0);
    Story_unk366(10);
    Story_unk367(0, 1, 0);
    Story_unk368(3, 2, 0);
    Story_unk369(30);
    Story_unk370(0, 3);
    Story_unk371(1, 3);
    Story_unk372(3, 3);
    Story_unk373(2, 3);
    Story_unk374(30);
    Story_unk112_2(1, 0x13333, 0x9999);
    Story_unk113_2(2, 0x13333, 0x9999);
    Story_unk114_2(3, 0x13333, 0x9999);
    Story_unk375(1, 2);
    record = Story_unk14(0);
    if (record != 0) {
        Story_unk376(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk377(1);
    Story_unk378(1, 0, 0);
    Story_unk379(2, 2);
    record = Story_unk15(0);
    if (record != 0) {
        Story_unk380(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk381(2);
    Story_unk382(2, 0, 0);
    Story_unk383(3, 2);
    record = Story_unk16(0);
    if (record != 0) {
        Story_unk384(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_unk385(3);
    Story_unk386(3, 0, 0);
    Story_unk387(10);
    Story_unk115_2(0, 0xc000, 0);
    Story_unk116_2(10, 0xb000, 0);
    Story_unk117_2(14, 0xb000, 0);
    Story_unk118_2(11, 0xb000, 0);
    Story_unk119_2(12, 0xb000, 0);
    Story_unk120_2(13, 0xb000, 0);
    Story_unk388(30);
    Story_unk389();
}

/* overlays/scene/story/branching_event/move_and_redraw.c */

u8 *Story_unk390(s32 kind, s32 x, s32 y, s32 z);

/*
 * Spawns an overlay object and puts it into its configured state --
 * resource_3c5.
 */

/*
 * The four callees are declared at the in-overlay entry points that are
 * called directly, not at their veneer addresses.
 */

/*
 * Spawn an object, clear the low bits of its owner record, and issue command
 * 15.  The three callees are declared at their in-overlay entry points, not at
 * veneers.  Each read-modify-write takes its mask from a local that is
 * initialised first and ANDed with the loaded byte afterwards, which is what
 * puts the mask in r3 and the byte in r2.  The +0x23 mask stays byte-wide as
 * 0xfe rather than ~1, which would widen it to a negated 2.
 */

u8 *OvObj_SpawnConfiguredObject(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Story_unk391(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        /*
         * The mask is a local initialised first and ANDed with the loaded
         * byte afterwards; folding the clear into one expression swaps the
         * two registers.
         */
        u32 masked = ~12;
        masked = masked & owner[9];
        owner[9] = (u8)masked;
        object[85] = 0;
        object[89] = 8;
        Story_Apply(object, 0);
        Story_Apply2(object, 14);
        Story_Apply3(object, 1);
        result = object;
    } else {
        result = 0;
    }
    return result;
}

u8 *OvObj_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Story_unk390(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        u32 mask = ~12;
        u32 low;
        mask = mask & owner[9];
        owner[9] = (u8)(mask | 4);
        object[85] = 0;
        object[89] = 8;
        Story_Apply4(object, 0);
        Story_Apply5(object, 15);
        low = 0xfe;
        low = low & object[35];
        object[35] = (u8)(low | 2);
        result = object;
    } else {
        result = 0;
    }
    return result;
}

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

void OvObj_WaitUntilIdle(s32 *obj)
{
    s32 i = 60;

    while (i != 0) {
        Story_unk392(1);
        i--;
        if (obj[10] == 0) {
            break;
        }
    }
}

/* overlays/scene/story/branching_event/scene_data_head.c */
#define PrimaryRuntimeSelector Value_000000b0
#define SecondaryRuntimeSelector Value_000000af
#define TertiaryRuntimeSelector Value_000000ae

extern s16 RuntimeSelectorTable[];
extern u8 Value_000000b0;
extern u8 gOv2[];

/* Contiguous unnamed leaf-owner run for resource_3c5. */

/* Return this overlay's state block. */

s32 SceneData_SelectByRuntimeSelector(void)
{
}

s32 SceneData_SelectTableB5b8ByState(void)
{
    if (RuntimeSelectorTable[224] == (s32)&Value_000000b0) {
        return (s32)gOv2;
    }
    return 0;
}

u8 *SceneData_GetTableb5f8(void) { return (u8 *)0x0200b5f8; }

/* overlays/scene/story/branching_event/scene_primary_script.c */

/* Contiguous unnamed leaf-owner run for resource_3c5. */

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    extern u8 gWork[];

    u32 i;
    s32 rec4;
    s32 zero;
    s32 t;
    s32 nv;
    u8 *p8;
    u8 *p6;
    u8 slot28[40];
    u8 slot16[12];

    Story_unk393(a0);
    Story_unk14_4(-1, -1, -1, 0);
    Story_unk394();
    Story_unk395(1);
    *(volatile s32 *)(Story_unk30(0) + 12) = 0x820000;
    *(volatile s32 *)(Story_unk31(0) + 72) = 0x8000;
    zero = 0;
    *(volatile s32 *)(Story_unk32(0) + 68) = zero;
    *(u8 *)(Story_unk33(0) + 85) = zero;
    Story_unk396();
    Story_unk397();
    Story_unk398(30);
    Story_unk399(204);
    *(u8 *)(Story_unk34(0) + 85) = 3;
    Story_unk400(24);
    rec4 = Story_unk17(0);
    p8 = slot28;
    *(s32 *)(p8 + 4) = 7;
    *(s32 *)(p8 + 36) = 0x2008cf1;
    *(s32 *)(p8 + 8) = 0xcccc;
    *(s32 *)(p8 + 12) = 0xcccc;
    i = 0;
    p6 = slot16;
    for (; i < 17; i++) {
        *(s32 *)(p6) = Story_unk35(i << 12);
        *(s32 *)(p6 + 4) = 0;
        t = Story_unk36(i << 12);
        nv = *(s32 *)(p6);
        nv = nv + nv / 2;
        *(s32 *)(p6 + 8) = t;
        *(s32 *)(p6) = nv;
        Story_unk401(*(s32 *)(rec4 + 8), *(s32 *)(rec4 + 12),
              *(s32 *)(rec4 + 16), nv,
              *(s32 *)(p6 + 4), t, 0x1090001, (s32)p8);
    }
    Story_unk402(188);
    Story_unk15_4(0, 0x101);
    Story_unk403(0, 22);
    Story_unk121_2(0x50000, 0x50000, 0x10000);
    Story_unk122_2(-1, -1, 0xe666);
    Story_unk404();
    Story_unk16_4(0, 0x100);
    *(volatile s32 *)(Story_unk37(0) + 72) = 0x10000;
    *(volatile s32 *)(Story_unk38(0) + 68) = 0x4000;
    Story_unk405();
}

void Scene_RunScene3c5SequenceA(s32 a0)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 v5;

    Story_unk406();
    Story_unk17_4(-1, -1, -1, 0);
    Story_unk407();
    Story_unk408(1);
    record = Story_unk39(0);
    *(volatile s32 *)(record + 12) = 0x820000;
    record = Story_unk40(0);
    *(volatile s32 *)(record + 72) = 0x4000;
    v5 = 0;
    record = Story_unk41(0);
    *(volatile s32 *)(record + 68) = v5;
    *(u8 *)(Story_unk42(0) + 85) = v5;
    record = Story_unk43(0);
    Story_unk409(record, 0);
    Story_unk410();
    Story_unk411();
    Story_unk412(10);
    Story_unk413(204);
    *(u8 *)(Story_unk44(0) + 85) = 3;
    record = Story_unk45(0);
    *(volatile s32 *)(record + 40) = -0x50000;
    Story_unk414(0);
    Story_unk415();
    Story_unk416(0, 15);
    Story_unk417(a0);
    Story_unk418();
}

void Scene_RunStep11(void)
{
    Story_unk419();
    Story_unk420();
    Story_unk421(11);
    Story_unk422();
}

void Scene_RunStep12WithPosition(void)
{
    Story_unk423();
    Story_unk424(0x1d00000, 0, 0x1220000, 223);
    Story_unk425();
    Story_unk426(12);
    Story_unk427();
}

void Scene_RunStep13WithTwoPositions(void)
{
    Story_unk428();
    Story_unk46(0x8f0000, 0, 0x1220000, 223);
    Story_unk429(0x790000, 0, 0x11e0000, 253);
    Story_unk430();
    Story_unk431(13);
    Story_unk432();
}

void Scene_RunStep15(void)
{
    Story_unk433();
    Story_unk434();
    Story_unk435(15);
    Story_unk436();
}

void Scene_RunStepWithValue2693(void)
{
    Story_unk437();
    Story_unk438(0, 1);
    Story_unk439(0x2693, 1);
    Story_unk440();
}

void Scene_RunScene3c5(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Story_unk18(0x985) == 0) {
        Story_unk7_2(0x985);
        Story_unk441(157);
        Story_unk442();
        Story_unk123_2(8, 0x118, 240);
        Story_unk124_2(9, 0x148, 240);
        Story_unk443(8);
        Story_unk444(9);
        Story_SetRect(81, 14, 4, 1, 17, 14);
        Story_unk445();
        if (Story_unk19(0x989) == 0) {
            Story_unk446();
        }
    }
}

void Scene_RunScene3c5(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Story_unk20(0x985) != 0) {
        Story_unk8_2(0x985);
        Story_unk447(157);
        Story_unk448();
        Story_unk125_2(8, 0x128, 240);
        Story_unk126_2(9, 0x138, 240);
        Story_unk449(8);
        Story_unk450(9);
        Story_unk2_5(0, 14, 4, 1, 17, 14);
        Story_unk451();
        if (Story_unk21(0x301) != 0) {
            Story_unk9_2(0x301);
        } else {
            Story_unk10_2(0x301);
        }
    }
}

void Scene_RunFlag985DialogueBranch(void)
{
    extern u8 *gWork;

    u8 *base = gWork;
    s16 *h;

    Story_unk452();
    h = (s16 *)(base + 0xcb8);
    if (h[0] != 0) {
        if (Story_unk47(0x985) == 0) {
            s32 k5 = 17, k6 = 78;

            Story_unk453(0x1528, 1);
            Story_unk454(155);
            Story_unk455(35, 78, 1, 2, k5, k6);
            Story_unk456(10);
            Story_unk457(34, 78, 1, 2, k5, k6);
            Story_unk458(10);
            Story_unk459();
        }
    } else {
        Story_unk460(0x2756);
        Story_unk461(-1, 0);
    }
    Story_unk462();
}

/* overlays/scene/story/branching_event/scene_setup.c */

/*
 * Configure this overlay's scene parameters, enable the configuration, and
 * apply the final magnitude.  The complete owner is the saved-link function
 * at 0x020027f4-0x02002820; its stack frame holds arguments five and six for
 * the first call.
 */

static __inline__ void ResetSceneParameters(s32 a, s32 b, s32 c, s32 mode)
{
    Story_unk463(a, b, c, mode);
}

void Scene_RunFourCallSequence(void)
{
    Story_unk464();
    Story_unk465();
    Story_unk466();
    Story_unk467();
}

/* resource_3c5 owner at 0x02001158, 42 bytes. */
void ResetSceneParametersAndFinishSetup(void)
{
    ResetSceneParameters(-1, -1, -1, 0);
    Story_unk468(0, 6, 0);
    Story_unk469();
    Story_unk470();
}

void State_ConfigureRegion82_7AndApply768(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 18;
    s32 b = 7;

    Story_unk3_5(82, 7, 1, 2, a, b);
    Story_unk11_2(1);
    Story_unk12_2(768);
}

/* overlays/scene/story/branching_event/staged_actor.c */

s32 StagedActor_FindClearPosition(s32 *a)
{
}

/* overlays/scene/story/branching_event/state_update.c */

s32 *Story_unk471();

s32 *Story_unk472();

void State_SetValue8Mode66(void)
{
    Story_unk473(8, 66);
}

void State_ApplyRectAt32x78(void)
{
    {
        s32 k5 = 17, k6 = 78;

        Story_unk474(32, 78, 1, 2, k5, k6);
    }
}

void State_SetRuntimeByte34(void)
{
    FIELD_AT_OFFSET(*(void **)0x03001F30, s8 *, 0x34) = 1;
}

void State_ApplyRectsAtActors8And9(void)
{
    s32 *p = Story_unk471(8);

    Story_unk475(8, 1);
    Story_unk476(9, 1);
    {
        s32 k5 = 5, k6 = 19;

        Story_unk477(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = 17, k6 = 19;

        Story_unk478(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = p[2] >> 20, k6 = p[4] >> 20;

        Story_unk479(3, 3, 1, 1, k5, k6);
    }
    {
        s32 *q = Story_unk472(9);
        s32 k5 = q[2] >> 20, k6 = q[4] >> 20;

        Story_unk480(3, 3, 1, 1, k5, k6);
    }
}

/* overlays/scene/story/branching_event/select_data_by_runtime_selector.c */
#define PrimaryRuntimeSelector Value_000000b0
#define SecondaryRuntimeSelector Value_000000af
#define TertiaryRuntimeSelector Value_000000ae

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

/* overlays/scene/story/branching_event/select_table_b91c_by_runtime_selector.c */
#define PrimaryRuntimeSelector Value_000000b0
#define SecondaryRuntimeSelector Value_000000af
#define TertiaryRuntimeSelector Value_000000ae

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectTableB91cByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
