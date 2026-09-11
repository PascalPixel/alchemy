#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/branching_event/branching_event.h"
#include "spawn_configured_effect.h"
#include "spawn_configured_effect_body.inc"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"
#include "find_clear_actor_position.h"
#include "find_clear_actor_position_body.inc"

/* overlays/scene/story/branching_event/actor_facing.c */
struct Obj {
    u8 pad00[6];
    u16 f06;
};

extern u8 RuntimeSelectorTable[];

struct Obj *Story_Run18();

void Actor_RunSlotZeroFacingCheck(void)
{
    struct Obj *p = Story_Run18(0);
    s32 x = Story_Check22();
    s32 m = (p->f06 + 0x2000) & 0xc000;
    s32 r = -1;

    if (RuntimeSelectorTable[498] == 1 || x == 0) {
        if (m == 0xc000) {
            r = Story_Check23();
        }
        if (m == 0x4000) {
            r = Story_Check24();
        }
    }
    if (r != 0) {
        if (RuntimeSelectorTable[498] != 1) {
            Story_Run19();
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
extern struct LevelCheckRecord *Story_Run20();   /* record by slot index */
extern struct LevelCheckRecord *Story_Run21();   /* occupant one square ahead */
extern void Story_Run22();                       /* push the object ahead */

Ent *Story_Run23(Desc *, Ent *);

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
    r = Story_Run23(&d, a);
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
    struct LevelCheckRecord *p = Story_Run20(0);
    struct LevelCheckRecord *q = Story_Run21(p);
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

    Story_Run22();
}

/* overlays/scene/story/branching_event/actor_presentation.c */

s32 *Story_Run24();

void ActorDraw_SetSceneCellByFlag985(void)
{
    if (Story_Check25(0x985) == 0) {
        s32 k5 = 17, k6 = 78;

        Story_Run25(36, 78, 1, 2, k5, k6);
    } else {
        s32 k5 = 17, k6 = 78;

        Story_Run26(34, 78, 1, 2, k5, k6);
    }
}

void ActorDraw_PlaceActorTwelveAtTile20And12(void)
{
    s32 *p = Story_Run24(12);
    s32 a = p[2] >> 20;

    if (a == 20) {
        s32 b = p[4] >> 20;

        if (b == 12) {
            ((u8 *)p)[85] = 2;
            p[5] = 0x300000;
            ((u8 *)p)[35] = 2;
            {
                s32 k5 = a, k6 = b;

                Story_Run27(38, 12, 1, 1, k5, k6);
            }
        }
    }
}

/* overlays/scene/story/branching_event/conditional_scene_setup.c */
s32 *Story_Run28();
s32 *Story_Run29();
s32 *Story_Run30();

void State_BranchOnActorEightOrNineTile(void)
{
    s32 *p = Story_Run28(9);

    if ((Story_Run29(0)[2] >> 20) <= 12) {
        p = Story_Run30(8);
        if ((p[2] >> 20) == 6) {
            if ((p[4] >> 20) == 20) {
                Story_Run31();
                return;
            }
        }
    } else {
        if ((p[2] >> 20) == 18) {
            if ((p[4] >> 20) == 20) {
                Story_Run31();
                return;
            }
        }
    }
    Story_Run32();
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
void Story_Run33(struct Effect *effect)
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

    effect->velocity[0] = velocity_x - Story_Check26(velocity_x, 18);
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
    Story_Do2(0x98a);
    Story_Run34();
    Story_Run35();
    Story_Do3(0x2702);
    Story_Place(0, 0x10000, 0x8000);
    Story_Place2(0, 0x128, 0x160);
    Story_Place3(0, 0xc000, 0);
    Story_Run36(10);
    Story_Run(10, 16, 0, 0xc000);
    Story_Run2(1, -8, 16, 0xc000);
    Story_Run3(2, 8, 16, 0xc000);
    Story_Run4(3, 24, 16, 0xc000);
    Story_Run37(3);
    Story_Run38(20);
    Story_Run39(14, 0);
    Story_Run40(10);
    Story_Run5(0x30000, 0x6000);
    Story_Run6(0x1180000, -1, 0x1200000, 1);
    Story_Run41();
    Story_Run42(20);
    Story_Run43(11, 3);
    Story_Run44(30);
    Story_Run45(11, 0);
    Story_Run46(10);
    Story_Place4(14, 0xc000, 0);
    Story_Place5(11, 0xc000, 0);
    Story_Run47(30);
    Story_Run48(10);
    Story_Run49(14, 2);
    Story_Run50(20);
    Story_Run51(14, 0);
    Story_Run52(10);
    Story_Run53(13, 2);
    Story_Run54(12, 2);
    Story_Run55(40);
    Story_Run56(13, 2);
    Story_Run57(12, 2);
    Story_Run58(40);
    Story_Run59(13, 2);
    Story_Run60(12, 2);
    Story_Run61(40);
    Story_Place6(12, 0x102, 50);
    Story_Run62(12, 0);
    Story_Run63(10);
    Story_Run64(13, 0, 0);
    Story_Run65(25);
    Story_Run66(13, 2);
    Story_Run67(20);
    Story_Run68(13, 0);
    Story_Run69(10);
    Story_Place7(12, 0x8000, 0);
    Story_Run70(20);
    Story_Run71(13, 4);
    Story_Run72(20);
    Story_Run73(13, 0);
    Story_Run74(10);
    Story_Run75(11, 4);
    Story_Run76(20);
    Story_Run77(11, 0);
    Story_Run78(20);
    Story_Place8(13, 0x107, 40);
    Story_Run79(10);
    Story_Place9(13, 0x4000, 0);
    Story_Run80(20);
    Story_Run81(13, 0);
    Story_Run82(20);
    Story_Place10(13, 0x101, 75);
    Story_Place11(14, 0x101, 60);
    Story_Place12(12, 0x4000, 0);
    Story_Run83(20);
    Story_Place13(11, 0x4000, 0);
    Story_Run84(20);
    Story_Place14(14, 0x4000, 0);
    Story_Run85(30);
    Story_Run86(14, 0);
    Story_Run7(0x1180000, -1, 0x1400000, 1);
    Story_Run87();
    Story_Run88(20);
    Story_Run89(10, 3);
    Story_Run90(30);
    Story_Run91(10, 0);
    Story_Run92(10);
    Story_Run93(10, 4);
    Story_Run94(20);
    Story_Run95(10, 0);
    Story_Run96(20);
    Story_Place15(14, 0x105, 60);
    Story_Run97(14, 0);
    Story_Run98(10);
    Story_Run99(3, 4);
    Story_Run100(20);
    Story_Run101(3, 0);
    Story_Run102(10);
    Story_Run103(2, 2);
    Story_Run104(20);
    Story_Run105(2, 0);
    Story_Run106(10);
    Story_Run107(1, 2);
    Story_Run108(20);
    Story_Check(1, 0);
    if (Story_Check2(0, 0) == 0) {
        Story_Run109(30);
        Story_Place16(10, 0x8000, 0);
        Story_Run110(30);
        Story_Run111(10, 3);
        Story_Run112(30);
        Story_Run113(10, 0);
        bump_step(1);
    } else {
        Story_Run114(30);
        Story_Place17(10, 0x8000, 0);
        Story_Run115(30);
        Story_Run116(10, 4);
        Story_Run117(20);
        bump_step(1);
        Story_Run118(10, 0);
    }
    Story_Run119(10);
    Story_Place18(14, 0x101, 60);
    Story_Place19(14, 0x10000, 0x8000);
    Story_Run120(14, 0, 16);
    Story_Run121(20);
    Story_Run122(14, 0);
    Story_Run123(10);
    Story_Place20(10, 0xc000, 0);
    Story_Run124(35);
    Story_Run125(10, 0);
    Story_Run126(10);
    Story_Run127(14, 2);
    Story_Run128(20);
    Story_Run129(14, 0);
    Story_Run130(20);
    Story_Place21(3, 0x102, 40);
    Story_Run131(3, 0);
    Story_Run132(10);
    Story_Run133(2, 2);
    Story_Run134(20);
    Story_Run135(2, 0);
    Story_Run136(10);
    Story_Run137(1, 3);
    Story_Run138(30);
    Story_Run139(1, 0);
    Story_Run140(20);
    Story_Place22(14, 0x100, 40);
    Story_Check3(14, 0);
    if (Story_Check4(0, 0) == 0) {
        Story_Run141(30);
        Story_Run142(14, 4);
        ((void (*)())Story_Check27)(20);
        Story_Run143(14, 0);
        bump_step(1);
    } else {
        Story_Run144(30);
        ((void (*)())Story_Check28)(14, 4);
        Story_Run145(20);
        bump_step(1);
        Story_Run146(14, 0);
    }
    Story_Run147(10);
    Story_Place23(10, 0x102, 50);
    Story_Run148(10, 0);
    Story_Run149(10);
    Story_Run150(13, 2);
    Story_Run151(20);
    Story_Place24(13, 0x14ccc, 0xa666);
    Story_Run152(13, 0, 16);
    Story_Run153(20);
    Story_Run154(13, 0);
    Story_Run155(10);
    Story_Place25(12, 0x14ccc, 0xa666);
    Story_Run156(12, 0, 16);
    Story_Run157(20);
    Story_Place26(12, 0x107, 50);
    Story_Run158(12, 0);
    Story_Run159(10);
    Story_Run160(14, 3);
    Story_Run161(30);
    Story_Run162(10);
    Story_Place27(10, 0x102, 60);
    Story_Place28(10, 0x8000, 0);
    Story_Run163(25);
    Story_Run164(10, 0);
    Story_Run165(10);
    Story_Run166(10, 2);
    Story_Run167(20);
    Story_Check5(10, 0);
    if (Story_Check6(0, 0) == 0) {
        Story_Run168(30);
        Story_Run169(10, 3);
        ((void (*)())Story_Check29)(30);
        Story_Run170(10, 0);
        bump_step(1);
    } else {
        Story_Run171(30);
        Story_Run172(10, 4);
        Story_Run173(20);
        bump_step(1);
        Story_Run174(10, 0);
    }
    Story_Run175(10);
    Story_Place29(10, 0xc000, 0);
    Story_Run176(35);
    Story_Run177(14, 3);
    Story_Run178(30);
    Story_Place30(14, 0xb000, 0);
    Story_Run179(40);
    Story_Run180(14, 0);
    Story_Run181(10);
    Story_Run182(12, 13, 50);
    Story_Run183(12, 3);
    Story_Run184(13, 3);
    Story_Run185(30);
    Story_Place31(12, 0x4000, 0);
    Story_Place32(13, 0x4000, 0);
    Story_Run186(20);
    Story_Run187(12, 3);
    Story_Run188(13, 3);
    Story_Run189(30);
    Story_Place33(12, 0x10000, 0x8000);
    Story_Place34(13, 0x10000, 0x8000);
    Story_Run190(12, 32, 0);
    Story_Run191(13, 32, 0);
    Story_Run192(12, 0, 16);
    Story_Run193(13, 16, 0);
    Story_Place35(13, 0x158, 0x138);
    Story_Place36(12, 0x158, 0x150);
    Story_Run194(13, 1);
    Story_Place37(12, 0x8000, 0);
    Story_Place38(13, 0x8000, 0);
    Story_Run195(10);
    Story_Place39(14, 0x4000, 0);
    Story_Run196(20);
    Story_Run197(14, 0);
    Story_Run198(10);
    Story_Place40(14, 0x10000, 0x8000);
    Story_Place41(14, 0x148, 0x138);
    Story_Place42(14, 0x8000, 0);
    Story_Run199(30);
    Story_Place43(11, 0x10000, 0x8000);
    Story_Place44(11, 0x148, 0x148);
    Story_Place45(11, 0x8000, 0);
    Story_Run200(20);
    Story_Place46(1, 0x13333, 0x9999);
    Story_Place47(2, 0x13333, 0x9999);
    Story_Place48(3, 0x13333, 0x9999);
    Story_Run201(1, 2);
    record = Story_Check7(0);
    if (record != 0) {
        Story_Run202(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run203(1);
    Story_Run204(1, 0, 0);
    Story_Run205(2, 2);
    record = Story_Check8(0);
    if (record != 0) {
        Story_Run206(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run207(2);
    Story_Run208(2, 0, 0);
    Story_Run209(3, 2);
    record = Story_Check9(0);
    if (record != 0) {
        Story_Run210(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run211(3);
    Story_Run212(3, 0, 0);
    Story_Run213(10);
    Story_Run214();
}

void Scene_RunActorEventSequence(void)
{
    s32 record;

    Story_Do4(0x989);
    Story_Run215();
    Story_Run216();
    Story_Do5(0x272f);
    Story_Place49(0, 0x10000, 0x8000);
    Story_Place50(0, 0x128, 0x138);
    Story_Run217(0, 0, 0);
    Story_Run218(10);
    Story_Run219(1, 0, 16, 0);
    Story_Run8(2, -16, -8, 0);
    Story_Run9(3, -16, 24, 0);
    Story_Run220(3);
    Story_Run221(20);
    Story_Run10(0x30000, 0x6000);
    Story_Run11(0x1180000, -1, 0x1480000, 1);
    Story_Run222();
    Story_Run223(10);
    Story_Run224(10);
    Story_Place51(10, 0xb000, 0);
    Story_Run225(10);
    Story_Place52(10, 0x100, 40);
    Story_Run226(10, 0);
    Story_Run227(10, 4, 13);
    Story_Run228(10, 4, 30);
    Story_Run229(10);
    Story_Place53(11, 0x100, 0);
    Story_Place54(12, 0x100, 0);
    Story_Place55(13, 0x100, 0);
    Story_Place56(14, 0x100, 40);
    Story_Run230(10);
    Story_Place57(14, 0xb000, 0);
    Story_Place58(11, 0xb000, 0);
    Story_Place59(12, 0xb000, 0);
    Story_Place60(13, 0xb000, 0);
    Story_Run231(30);
    Story_Run232(14, 2);
    Story_Run233(20);
    Story_Run234(14, 0);
    Story_Run235(20);
    Story_Place61(13, 0x102, 40);
    Story_Run236(13, 0);
    Story_Run237(20);
    Story_Place62(12, 0x101, 50);
    Story_Place63(12, 0x8000, 0);
    Story_Run238(25);
    Story_Run239(12, 0);
    Story_Run240(10);
    Story_Place64(14, 0x8000, 0);
    Story_Place65(11, 0x8000, 0);
    Story_Place66(13, 0x8000, 0);
    Story_Run241(30);
    Story_Run242(10);
    Story_Run243(3, 2);
    Story_Run244(20);
    Story_Run245(3, 0);
    Story_Run246(10);
    Story_Run247(2, 2);
    Story_Run248(20);
    Story_Run249(2, 0);
    Story_Run250(10);
    Story_Place67(1, 0x102, 40);
    Story_Run251(1, 0);
    Story_Run252(20);
    Story_Run253(10, 4);
    Story_Run254(20);
    Story_Run255(10, 0);
    Story_Run256(10);
    Story_Run257(14, 2);
    Story_Run258(25);
    Story_Run259(14, 0);
    Story_Run260(10);
    Story_Run261(10, 2);
    Story_Run262(20);
    Story_Place68(10, 0x10000, 0x8000);
    Story_Place69(10, 0, -40);
    Story_Run263(10, 0, 0);
    Story_Run264(20);
    Story_Run265(10, 0);
    Story_Run266(10);
    Story_Run267(14, 3);
    Story_Run268(30);
    Story_Run269(10);
    Story_Place70(13, 0x4000, 0);
    Story_Place71(12, 0xc000, 0);
    Story_Run270(30);
    Story_Run271(12, 3);
    Story_Run272(13, 3);
    Story_Run273(30);
    Story_Place72(13, 0x8000, 0);
    Story_Place73(12, 0x8000, 0);
    Story_Run274(20);
    Story_Run275(10, 3);
    Story_Run276(30);
    Story_Run277(10, 1);
    Story_Place74(10, 0, -32);
    Story_Run278();
    Story_Do6(0x301);
    Story_Run12(-1, -1, -1, 0);
    Story_Run279();
    Story_Place75(10, 0x102, 40);
    Story_Run280(10, 0);
    Story_Run281(10);
    Story_Place76(14, 0xb000, 0);
    Story_Place77(11, 0xb000, 0);
    Story_Place78(12, 0xb000, 0);
    Story_Place79(13, 0xb000, 0);
    Story_Place80(0, 0xc000, 0);
    Story_Place81(1, 0xc000, 0);
    Story_Place82(2, 0xc000, 0);
    Story_Place83(3, 0xc000, 0);
    Story_Run282(30);
    Story_Place84(11, 0x100, 0);
    Story_Place85(12, 0x100, 0);
    Story_Place86(13, 0x100, 0);
    Story_Place87(14, 0x100, 70);
    Story_Run13(0x1180000, -1, 0x1380000, 1);
    Story_Run283();
    Story_Run284(10);
    Story_Place88(12, 0x102, 40);
    Story_Run285(12, 0);
    Story_Run286(10);
    Story_Run287(13, 4);
    Story_Run288(20);
    Story_Run289(13, 0);
    Story_Run290(10);
    Story_Run291(2, 2);
    Story_Run292(30);
    Story_Place89(2, 0x2000, 0);
    Story_Run293(20);
    Story_Run294(2, 0);
    Story_Run295(10);
    Story_Place90(10, 0x4000, 0);
    Story_Run296(30);
    Story_Place91(3, 0xe000, 0);
    Story_Run297(30);
    Story_Place92(3, 0x102, 40);
    Story_Place93(0, 0x3000, 0);
    Story_Run298(20);
    Story_Run299(3, 0);
    Story_Run300(10);
    Story_Run301(1, 4);
    Story_Run302(20);
    Story_Run303(1, 0);
    Story_Run304(20);
    Story_Place94(14, 0x103, 50);
    Story_Place95(14, 0x8000, 0);
    Story_Run305(20);
    Story_Check10(14, 0);
    Story_Run306(0, 0, 0);
    Story_Place96(11, 0x8000, 0);
    Story_Place97(12, 0x8000, 0);
    Story_Place98(13, 0x8000, 0);
    Story_Place99(2, 0x1000, 0);
    Story_Place100(3, 0xe000, 0);
    if (Story_Check11(0, 0) == 0) {
        Story_Run307(30);
        Story_Run308(14, 2);
        Story_Run309(20);
        Story_Run310(14, 0);
        bump_step(1);
    } else {
        Story_Run311(30);
        Story_Run312(14, 2);
        Story_Run313(20);
        bump_step(1);
        Story_Run314(14, 0);
    }
    Story_Run315(10);
    Story_Run316(10, 2);
    Story_Run317(20);
    Story_Run318(10, 0, 16);
    Story_Place101(10, 0x2000, 0);
    Story_Run319(20);
    Story_Run320(10, 0);
    Story_Run321(10);
    Story_Place102(14, 0xa000, 0);
    Story_Run322(20);
    Story_Run323(14, 4);
    Story_Run324(20);
    Story_Run325(14, 0);
    Story_Run326(10);
    Story_Run327(10, 3);
    Story_Run328(30);
    Story_Run329(10, 0);
    Story_Run330(10);
    Story_Run331(14, 2);
    Story_Run332(20);
    Story_Check12(14, 0);
    Story_Run333(40);
    Story_Place103(10, 0x5000, 0);
    Story_Run334(20);
    Story_Place104(10, 0x101, 60);
    if (Story_Check13(0, 0) == 0) {
        Story_Run335(30);
        Story_Place105(10, 0x2000, 0);
        Story_Run336(20);
        Story_Run337(10, 3);
        Story_Run338(30);
        Story_Run339(10, 0);
        bump_step(1);
    } else {
        Story_Run340(30);
        Story_Place106(10, 0x2000, 0);
        Story_Run341(20);
        Story_Run342(10, 4);
        Story_Run343(20);
        bump_step(1);
        Story_Run344(10, 0);
    }
    Story_Run345(10);
    Story_Place107(14, 0x102, 50);
    Story_Run346(14, 0);
    Story_Run347(15);
    Story_Run348(10, 3);
    Story_Run349(30);
    Story_Run350(20);
    Story_Run351(14, 2);
    Story_Run352(40);
    Story_Place108(14, 0x8000, 0);
    Story_Run353(20);
    Story_Run354(14, 0);
    Story_Run355(10);
    Story_Run356(14, 3);
    Story_Run357(30);
    Story_Run358(14, 0);
    Story_Run359(10);
    Story_Place109(10, 0x5000, 0);
    Story_Run360(25);
    Story_Run361(10, 2);
    Story_Run362(20);
    Story_Place110(10, 0x138, 0x138);
    Story_Run363(0, 0, 0);
    Story_Place111(10, 0x8000, 0);
    Story_Run364(25);
    Story_Run365(10, 0);
    Story_Run366(10);
    Story_Run367(0, 1, 0);
    Story_Run368(3, 2, 0);
    Story_Run369(30);
    Story_Run370(0, 3);
    Story_Run371(1, 3);
    Story_Run372(3, 3);
    Story_Run373(2, 3);
    Story_Run374(30);
    Story_Place112(1, 0x13333, 0x9999);
    Story_Place113(2, 0x13333, 0x9999);
    Story_Place114(3, 0x13333, 0x9999);
    Story_Run375(1, 2);
    record = Story_Check14(0);
    if (record != 0) {
        Story_Run376(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run377(1);
    Story_Run378(1, 0, 0);
    Story_Run379(2, 2);
    record = Story_Check15(0);
    if (record != 0) {
        Story_Run380(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run381(2);
    Story_Run382(2, 0, 0);
    Story_Run383(3, 2);
    record = Story_Check16(0);
    if (record != 0) {
        Story_Run384(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Story_Run385(3);
    Story_Run386(3, 0, 0);
    Story_Run387(10);
    Story_Place115(0, 0xc000, 0);
    Story_Place116(10, 0xb000, 0);
    Story_Place117(14, 0xb000, 0);
    Story_Place118(11, 0xb000, 0);
    Story_Place119(12, 0xb000, 0);
    Story_Place120(13, 0xb000, 0);
    Story_Run388(30);
    Story_Run389();
}

/* overlays/scene/story/branching_event/move_and_redraw.c */

u8 *Story_Run390(s32 kind, s32 x, s32 y, s32 z);

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
    u8 *object = Story_Run391(kind, x, y, z);

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
    u8 *object = Story_Run390(kind, x, y, z);

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
        Story_Run392(1);
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

    Story_Run393(a0);
    Story_Run14(-1, -1, -1, 0);
    Story_Run394();
    Story_Run395(1);
    *(volatile s32 *)(Story_Check30(0) + 12) = 0x820000;
    *(volatile s32 *)(Story_Check31(0) + 72) = 0x8000;
    zero = 0;
    *(volatile s32 *)(Story_Check32(0) + 68) = zero;
    *(u8 *)(Story_Check33(0) + 85) = zero;
    Story_Run396();
    Story_Run397();
    Story_Run398(30);
    Story_Run399(204);
    *(u8 *)(Story_Check34(0) + 85) = 3;
    Story_Run400(24);
    rec4 = Story_Check17(0);
    p8 = slot28;
    *(s32 *)(p8 + 4) = 7;
    *(s32 *)(p8 + 36) = 0x2008cf1;
    *(s32 *)(p8 + 8) = 0xcccc;
    *(s32 *)(p8 + 12) = 0xcccc;
    i = 0;
    p6 = slot16;
    for (; i < 17; i++) {
        *(s32 *)(p6) = Story_Check35(i << 12);
        *(s32 *)(p6 + 4) = 0;
        t = Story_Check36(i << 12);
        nv = *(s32 *)(p6);
        nv = nv + nv / 2;
        *(s32 *)(p6 + 8) = t;
        *(s32 *)(p6) = nv;
        Story_Run401(*(s32 *)(rec4 + 8), *(s32 *)(rec4 + 12),
              *(s32 *)(rec4 + 16), nv,
              *(s32 *)(p6 + 4), t, 0x1090001, (s32)p8);
    }
    Story_Run402(188);
    Story_Run15(0, 0x101);
    Story_Run403(0, 22);
    Story_Place121(0x50000, 0x50000, 0x10000);
    Story_Place122(-1, -1, 0xe666);
    Story_Run404();
    Story_Run16(0, 0x100);
    *(volatile s32 *)(Story_Check37(0) + 72) = 0x10000;
    *(volatile s32 *)(Story_Check38(0) + 68) = 0x4000;
    Story_Run405();
}

void Scene_RunScene3c5SequenceA(s32 a0)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 v5;

    Story_Run406();
    Story_Run17(-1, -1, -1, 0);
    Story_Run407();
    Story_Run408(1);
    record = Story_Check39(0);
    *(volatile s32 *)(record + 12) = 0x820000;
    record = Story_Check40(0);
    *(volatile s32 *)(record + 72) = 0x4000;
    v5 = 0;
    record = Story_Check41(0);
    *(volatile s32 *)(record + 68) = v5;
    *(u8 *)(Story_Check42(0) + 85) = v5;
    record = Story_Check43(0);
    Story_Run409(record, 0);
    Story_Run410();
    Story_Run411();
    Story_Run412(10);
    Story_Run413(204);
    *(u8 *)(Story_Check44(0) + 85) = 3;
    record = Story_Check45(0);
    *(volatile s32 *)(record + 40) = -0x50000;
    Story_Run414(0);
    Story_Run415();
    Story_Run416(0, 15);
    Story_Run417(a0);
    Story_Run418();
}

void Scene_RunStep11(void)
{
    Story_Run419();
    Story_Run420();
    Story_Run421(11);
    Story_Run422();
}

void Scene_RunStep12WithPosition(void)
{
    Story_Run423();
    Story_Run424(0x1d00000, 0, 0x1220000, 223);
    Story_Run425();
    Story_Run426(12);
    Story_Run427();
}

void Scene_RunStep13WithTwoPositions(void)
{
    Story_Run428();
    Story_Check46(0x8f0000, 0, 0x1220000, 223);
    Story_Run429(0x790000, 0, 0x11e0000, 253);
    Story_Run430();
    Story_Run431(13);
    Story_Run432();
}

void Scene_RunStep15(void)
{
    Story_Run433();
    Story_Run434();
    Story_Run435(15);
    Story_Run436();
}

void Scene_RunStepWithValue2693(void)
{
    Story_Run437();
    Story_Run438(0, 1);
    Story_Run439(0x2693, 1);
    Story_Run440();
}

void Scene_RunScene3c5(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Story_Check18(0x985) == 0) {
        Story_Do7(0x985);
        Story_Run441(157);
        Story_Run442();
        Story_Place123(8, 0x118, 240);
        Story_Place124(9, 0x148, 240);
        Story_Run443(8);
        Story_Run444(9);
        Story_SetRect(81, 14, 4, 1, 17, 14);
        Story_Run445();
        if (Story_Check19(0x989) == 0) {
            Story_Run446();
        }
    }
}

void Scene_RunScene3c5(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Story_Check20(0x985) != 0) {
        Story_Do8(0x985);
        Story_Run447(157);
        Story_Run448();
        Story_Place125(8, 0x128, 240);
        Story_Place126(9, 0x138, 240);
        Story_Run449(8);
        Story_Run450(9);
        Story_SetRect2(0, 14, 4, 1, 17, 14);
        Story_Run451();
        if (Story_Check21(0x301) != 0) {
            Story_Do9(0x301);
        } else {
            Story_Do10(0x301);
        }
    }
}

void Scene_RunFlag985DialogueBranch(void)
{
    extern u8 *gWork;

    u8 *base = gWork;
    s16 *h;

    Story_Run452();
    h = (s16 *)(base + 0xcb8);
    if (h[0] != 0) {
        if (Story_Check47(0x985) == 0) {
            s32 k5 = 17, k6 = 78;

            Story_Run453(0x1528, 1);
            Story_Run454(155);
            Story_Run455(35, 78, 1, 2, k5, k6);
            Story_Run456(10);
            Story_Run457(34, 78, 1, 2, k5, k6);
            Story_Run458(10);
            Story_Run459();
        }
    } else {
        Story_Run460(0x2756);
        Story_Run461(-1, 0);
    }
    Story_Run462();
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
    Story_Run463(a, b, c, mode);
}

void Scene_RunFourCallSequence(void)
{
    Story_Run464();
    Story_Run465();
    Story_Run466();
    Story_Run467();
}

/* resource_3c5 owner at 0x02001158, 42 bytes. */
void ResetSceneParametersAndFinishSetup(void)
{
    ResetSceneParameters(-1, -1, -1, 0);
    Story_Run468(0, 6, 0);
    Story_Run469();
    Story_Run470();
}

void State_ConfigureRegion82_7AndApply768(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 18;
    s32 b = 7;

    Story_SetRect3(82, 7, 1, 2, a, b);
    Story_Do11(1);
    Story_Do12(768);
}

/* overlays/scene/story/branching_event/staged_actor.c */

s32 StagedActor_FindClearPosition(s32 *a)
{
}

/* overlays/scene/story/branching_event/state_update.c */

s32 *Story_Run471();

s32 *Story_Run472();

void State_SetValue8Mode66(void)
{
    Story_Run473(8, 66);
}

void State_ApplyRectAt32x78(void)
{
    {
        s32 k5 = 17, k6 = 78;

        Story_Run474(32, 78, 1, 2, k5, k6);
    }
}

void State_SetRuntimeByte34(void)
{
    FIELD_AT_OFFSET(*(void **)0x03001F30, s8 *, 0x34) = 1;
}

void State_ApplyRectsAtActors8And9(void)
{
    s32 *p = Story_Run471(8);

    Story_Run475(8, 1);
    Story_Run476(9, 1);
    {
        s32 k5 = 5, k6 = 19;

        Story_Run477(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = 17, k6 = 19;

        Story_Run478(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = p[2] >> 20, k6 = p[4] >> 20;

        Story_Run479(3, 3, 1, 1, k5, k6);
    }
    {
        s32 *q = Story_Run472(9);
        s32 k5 = q[2] >> 20, k6 = q[4] >> 20;

        Story_Run480(3, 3, 1, 1, k5, k6);
    }
}
