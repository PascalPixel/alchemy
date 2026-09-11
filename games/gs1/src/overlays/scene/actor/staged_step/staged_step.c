#include "types.h"
#include "scene.h"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"
#include "find_clear_actor_position.h"
#include "find_clear_actor_position_body.inc"
#include "staged_actor.h"

/* overlays/scene/actor/staged_step/actor_facing.c */
u8 *Actor_unk11_4();

/*
 * Complete 40-byte heading update: face the supplied entity towards entity 0,
 * store the resulting angle in its +6 halfword and report zero.
 */
s32 Actor_FaceTowardActorZero(u8 *obj)
{
    u8 *p = Actor_unk11_4(0);
    *(u16 *)(obj + 6) = (u16)Actor_unk42(
        *(s32 *)(p + 16) - *(s32 *)(obj + 16),
        *(s32 *)(p + 8) - *(s32 *)(obj + 8));
    return 0;
}

/* overlays/scene/actor/staged_step/actor_position.c */
struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern u8 gWork[];
extern s32 gOv[];

s32 *Actor_unk12_4(s32);

s32 *Actor_unk13_4(s32);

struct Actor *Actor_unk14_4(s32);
struct Actor *Actor_unk15_4(s32);
struct Frame *Actor_unk16_4(s32);

void Actor_unk17_4(void)
{
    extern u8 RuntimeSelectorTable[];

    s32 dst;
    s32 src;
    s32 idx;
    s32 tbl;
    s32 idx4;
    s32 off24a;
    u8 *work;

    work = *(volatile s32 *)gWork;
    if (Actor_Check(0x9ca) != 0) {
        off24a = 0x24a;
        if (*(s16 *)((s32)RuntimeSelectorTable + off24a) != 15) {
            idx = *(s16 *)((s32)work + 0x16c);
            dst = Actor_unk2(15);
            src = Actor_unk3(0);
            *(volatile s32 *)(dst + 48) = *(volatile s32 *)(src + 48);
            dst = Actor_unk4(15);
            src = Actor_unk5(0);
            *(volatile s32 *)(dst + 52) = *(volatile s32 *)(src + 48);
            idx -= 30;
            tbl = 0x0200a808;
            idx <<= 3;
            idx4 = idx + 4;
            Actor_unk18_4(15, *(volatile s32 *)(tbl + idx), *(volatile s32 *)(tbl + idx4));
        }
    }
}

void Actor_TrackOriginColumnForSlot(s32 no)
{
    extern s16 RuntimeSelectorTable[];

    s32 *pos = Actor_unk12_4(0);
    s32 col = pos[2] / 0x100000;   /* +8  */
    s32 row = pos[4] / 0x100000;      /* +16 */
    s32 slot = no + 10;

    if (RuntimeSelectorTable[293] == slot) return;
    if (col == gOv[no]) return;

    Actor_unk52_3(slot, 0x48000, 0x24000);
    Actor_unk11_2(188);
    Actor_unk53_3(slot, (col << 4) + 8, 360);

    gOv[no] = col;

    if (row <= 22) {
        Actor_unk54_3(0, 0, 8);
    }
    Actor_unk12_2(0);
}

void Actor_RunWhenActor9AtTile45x43(void)
{
    s32 *slot = Actor_unk13_4(9);
    s32 x = slot[2] / 0x100000;
    s32 z = slot[4] / 0x100000;

    if (x == 45 && z == 43) {
        Actor_unk19_4();
    }
}

void ActorDraw_PlaceActorFourteenOnActorNine(void)
{
    struct Actor *target = Actor_unk14_4(14);
    struct Actor *source = Actor_unk15_4(9);

    target->f0c = 0x200000;
    target->f08 = source->f08;
    target->f10 = source->f10 + 0x10000;
}

void Actor_PassActorNinePositionWithId107(void)
{
    struct Frame *frame = Actor_unk16_4(9);

    Actor_unk55_3(107, frame->f08, frame->f10 + 0x10000);
}

/* overlays/scene/actor/staged_step/actor_presentation.c */

u8 *Actor_unk20_4(s32);

s32 *Actor_unk21_4();

s32 *Actor_unk22_4();

s32 *Actor_unk23_4();
s32 *Actor_unk24_4();
s32 *Actor_unk25_4();
s32 *Actor_unk26_4();
s32 *Actor_unk27_4();
s32 *Actor_unk28_4();
s32 *Actor_unk29_4();

s32 *Actor_unk30_4();
s32 *Actor_unk31_4();
s32 *Actor_unk32_4();

/* resource_3b4 actor presentation: cell repaints for slots 11 and 12. */

/*
 * Func_ names below are loader-relocated call words in this overlay's import
 * veneer table, not runtime addresses.  The declarations are old-style
 * because the same imports are reached with differing argument counts from
 * different call sites.
 */

/*
 * Actor presentation for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the mode imports. */

/* Complete 16-byte actor-15 mode wrapper before the no-op leaf at 0x9ec. */
void Actor_SetActor15ModeZero(void)
{
    Actor_unk56_3(15, 0, 6);
}

void ActorDraw_SetSceneCell58AndMarkActorEight(void)
{
    s32 extent = 42;
    u8 *entry;

    Actor_unk7_5(58, 41, 1, 1, extent, extent);
    entry = Actor_unk20_4(8) + 35;
    *entry = 2;
}

/*
 * Repaint the four cells, then one cell for each of slots 11 and 12 at that
 * slot's own X tile.  The 92-byte owner at 0x02001df8 includes two bytes of
 * alignment and the pool word 0x000fffff.  The tile divisions must stay
 * spelled `/ 0x100000': the reference biases a negative value before the
 * arithmetic shift, which is exactly this truncating signed division.
 */
void ActorDraw_RepaintCellsAtActorsElevenAndTwelve(void)
{
    s32 *slot;
    s32 tile;

    Actor_unk33_4();

    slot = Actor_unk21_4(11);
    tile = slot[2] / 0x100000;
    Actor_unk8_5(53, 55, 1, 1, tile, 55);
    slot = Actor_unk22_4(12);
    tile = slot[2] / 0x100000;
    Actor_unk9_5(53, 55, 1, 1, tile, 55);
}

/*
 * Publish one marker byte at +35 to slots 8, 10, 11 and 12 according to slot
 * 0's height word at +12.  The 156-byte owner includes its one pool word,
 * 0x000fffff, read by the tile division.  The marker local is what carries
 * the value 2 across the high path, which branches over the clear to 0.
 * Slot 11's record is fetched once on each path rather than once before
 * them, and that duplication is what reproduces the reference.
 */
void Actor_PublishMarkerBySlotZeroHeight(void)
{
    s32 *slot0 = Actor_unk23_4(0);
    u8 marker;

    if (slot0[3] > 0x100000) {                 /* +12 */
        marker = 2;
        ((u8 *)Actor_unk24_4(8))[35] = marker;
        if (Actor_unk25_4(10)[3] == 0) {
            ((u8 *)Actor_unk26_4(10))[35] = marker;
        }
        ((u8 *)Actor_unk27_4(11))[35] = marker;
    } else {
        if (Actor_unk28_4(10)[3] == 0 &&
            Actor_unk29_4(0)[4] / 0x100000 > 56) {   /* +16 */
            Actor_unk34_4(10, 3);
        } else {
            Actor_unk35_4(10, 1);
            ((u8 *)Actor_unk30_4(10))[35] = 1;
        }
        marker = 0;
        ((u8 *)Actor_unk31_4(11))[35] = marker;
    }

    ((u8 *)Actor_unk32_4(12))[35] = marker;
}

/* overlays/scene/actor/staged_step/display.c */
void SetBlendAlphaCoefficients(void)
{
    u32 coefficient = 208;

    coefficient <<= 4;
    *(u16 *)0x04000052 = coefficient;
}

/* Complete blend-alpha setter through return and its two pool words. */
void Effect_SetBlendAlpha0607(void)
{
    u16 value = 0x0607;

    *(volatile u16 *)0x04000052 = value;
}

/* overlays/scene/actor/staged_step/facing_target_scene.c */
struct Slot {
    u16 f00;
    u16 f02;
    u16 f04;
    u16 f06;
};

struct Slot *Actor_unk36_4(s32);

s32 *Actor_unk37_4();
s32 *Actor_unk38_4();

struct Slot *Actor_unk39_4(s32);

/*
 * Facing target scene for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the notification and step imports. */

void State_BranchOnActorZeroFacing(void)
{
    struct Slot *slot = Actor_unk36_4(0);

    if (slot->f06 == 0) {
        Actor_unk40_4();
    } else {
        Actor_unk41_4();
    }
}

/*
 * Compare the X tiles of slots 0 and 8, each the word at +8 divided by
 * 0x100000.  The 100-byte owner includes its one pool word, 0x000fffff,
 * which that truncating signed division reads.  The special cases apply only
 * when slot 0 sits on tile 38 and slot 8 does not; then slot 0's halfword at
 * +6 selects one of two notifications.  Every other case, an unrecognised
 * halfword included, runs the three-step ordinary path.
 */
void Actor_BranchOnSlotZeroAtTile38(void)
{
    s32 *slot0 = Actor_unk37_4(0);
    s32 *slot8 = Actor_unk38_4(8);
    s32 x0 = slot0[2] / 0x100000;
    s32 x8 = slot8[2] / 0x100000;

    if (x0 == 38 && x8 != 38) {
        u16 facing = ((u16 *)slot0)[3];

        /*
         * The facing value is still in r0 at both branches, but whether
         * either callee reads it is unverified, so no argument is passed.
         */
        if (facing == 0xc000) {
            Actor_unk42_4();
            return;
        }
        if (facing == 0x4000) {
            Actor_unk43_4();
            return;
        }
    }

    Actor_unk44_4();
    Actor_unk45_4();
    Actor_unk46_4();
}

void Scene_DispatchByActorZeroFacing(void)
{
    struct Slot *slot = Actor_unk39_4(0);

    if (slot->f06 == 0x4000) {
        Actor_unk57_3(0, 6, 0);
    } else {
        Actor_unk47_4();
    }
}

/* overlays/scene/actor/staged_step/move_and_redraw.c */

#define STAGED_ACTOR_MOVEMENT_UNTYPED_LOCATION_CALL

void Actor_unk48_4(StagedActorMovementRequest request)
{
}

/* overlays/scene/actor/staged_step/scene_data_head.c */
#define PrimaryRuntimeSelector Value_00000075
#define SecondaryRuntimeSelector Value_00000076
#define TertiaryRuntimeSelector Value_00000078

/* Contiguous unnamed leaf-owner run for resource_3b4. */

s32 SceneData_SelectTableA898ByRuntimeSelector(void)
{
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTablea970(void)
{
    return (u8 *)0x0200a970;
}

/* overlays/scene/actor/staged_step/scene_primary_script.c */
extern u8 gWork[];
extern u8 RuntimeSelectorTable[];

void Scene_RunScene3b4(void)
{
    u32 i;
    s32 record;

    if (Actor_unk6(0x9c8) == 0) {
        Actor_Do(0x9c8);
        Actor_unk49_4();
        Actor_Run(0x20000, 0x4000);
        Actor_unk50_4(15, 1);
        Actor_unk51_4();
        Actor_unk52_4(15, 0, 20);
        Actor_unk2_4(15, 0x102);
        Actor_unk53_4(15, 2);
        Actor_unk54_4(20);
        Actor_Place(15, 0x10000, 0x8000);
        Actor_unk55_4(152);
        record = Actor_unk43(15);
        *(volatile s32 *)(record + 40) = 0x80000;
        Actor_unk2_3(15, 0x248, 0x2a8);
        Actor_unk3_3(15, 0x4000, 20);
        Actor_unk56_4();
    }
}

void Scene_RunScene3b4(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    if (Actor_unk7(0x9c8) == 0) {
    } else {
        rec8 = Actor_unk8(0x9c9);
        if (rec8 != 0) {
        } else {
            Actor_unk2_2(0x9c9);
            Actor_unk57_4();
            Actor_unk3_4(0x20000, 0x4000);
            Actor_unk58_4(15, 1);
            Actor_unk59_4();
            Actor_unk4_3(15, 0x4000, 20);
            Actor_unk4_4(15, 0x102);
            Actor_unk60_4(15, 2);
            Actor_unk61_4(20);
            Actor_unk5_3(15, 0x10000, 0x8000);
            Actor_unk62_4(152);
            record = Actor_unk44(15);
            *(volatile s32 *)(record + 40) = 0xa0000;
            Actor_unk6_3(15, 0x248, 0x298);
            Actor_unk7_3(15, 0x4000, 20);
            Actor_unk5_4(15, 0x102);
            Actor_unk63_4(30);
            Actor_unk8_3(15, 0x80000, 0x4000);
            Actor_unk9_3(15, 0x298, 0x298);
            Actor_unk10_3(15, 0x2e8, 0x298);
            Actor_unk11_3(15, 0x338, 0x298);
            Actor_unk64_4(10);
            Actor_unk65_4(208);
            Actor_unk12_3(0x40000, 0x20000, 0x10000);
            Actor_unk66_4(20);
            Actor_unk13_3(-1, -1, 0xe666);
            Actor_unk67_4(30);
            Actor_unk14_3(15, 0x3780000, 0x2980000);
            rec7 = Actor_unk9(15);
            {
                s32 target = *(volatile s32 *)(rec7 + 80);
                s32 shown = 0xf800;

                *(volatile u16 *)(target + 30) = shown;
            }
            *(volatile u16 *)(rec7 + 6) = rec8;
            Actor_unk68_4(rec7, 0);
            Actor_unk10(rec7, 0x200a6fc);
            Actor_unk69_4();
        }
    }
}

void Scene_RunScene3b4(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (Actor_unk11(0x9c9) == 0) {
    } else {
        rec7 = Actor_unk12(0x9ca);
        if (rec7 != 0) {
        } else {
            Actor_unk3_2(0x9ca);
            Actor_unk70_4();
            record = Actor_unk13(15);
            *(volatile u16 *)(*(volatile s32 *)(record + 80) + 30) = rec7;
            Actor_unk71_4(record, 16);
            Actor_unk72_4(152);
            record = Actor_unk45(15);
            *(volatile s32 *)(record + 40) = 0x80000;
            Actor_unk15_3(15, 0x8000, 30);
            Actor_unk6_4(15, 0x102);
            Actor_unk73_4(15, 2);
            Actor_unk74_4(20);
            Actor_unk16_3(15, 0x10000, 0x8000);
            Actor_unk75_4(152);
            record = Actor_unk46(15);
            *(volatile s32 *)(record + 40) = 0x40000;
            Actor_unk17_3(15, 0x370, 0x2a8);
            Actor_unk76_4(10);
            Actor_unk7_4(15, 0x101);
            Actor_unk18_3(15, 0x20000, 0x10000);
            Actor_unk19_3(15, 0x370, 0x2b8);
            Actor_unk20_3(15, 0x372, 0x2c0);
            Actor_unk21_3(15, 0x370, 0x2c8);
            Actor_unk22_3(15, 0x36e, 0x2d0);
            Actor_unk23_3(15, 0x370, 0x2d8);
            Actor_unk24_3(15, 0x372, 0x2e0);
            Actor_unk25_3(15, 0x370, 0x2e8);
            Actor_unk26_3(15, 0x36e, 0x2f0);
            Actor_unk27_3(15, 0x370, 0x2f8);
            Actor_unk28_3(15, 0x3580000, 0x3380000);
            Actor_unk77_4(10);
            Actor_unk29_3(15, 0xc000, 20);
            Actor_unk8_4(15, 0x100);
            record = Actor_unk47(15);
            *(volatile s32 *)(record + 108) = 0x2008aa9;
            Actor_unk78_4();
        }
    }
}

void Scene_RunScene3b4(s32 a0)
{
    u32 i;
    s32 record;

    if ((a0 & 0x100) != 0) {
        Actor_unk79_4(157);
        Actor_unk30_3(0x20000, 0x20000, 0x10000);
        Actor_unk31_3(-1, -1, 0xe666);
        Actor_SetRect(84, 29, 1, 3, 70, 49);
        Actor_unk80_4(60);
    }
    Actor_unk2_5(85, 29, 1, 3, 70, 49);
    Actor_unk3_5(6, 49, 1, 1, 6, 50);
    Actor_unk4_5(6, 49, 1, 1, 6, 51);
}

void Scene_RunScene3b4SequenceC(void)
{
    u32 i;
    s32 record;

    record = Actor_unk48(0);
    if (*(u16 *)(record + 6) == 0xc000) {
        if (Actor_unk14(0x9c4) == 0) {
            if (Actor_unk49(243) != -1) {
                Actor_unk4_2(0x9c4);
                Actor_unk5_2(0x100);
                Actor_unk81_4(243);
            }
        }
    }
}

void Scene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;

    Actor_unk82_4();
    Actor_unk32_3(0, 0x1e666, 0xf333);
    Actor_unk33_3(8, 0x1e666, 0xf333);
    Actor_unk83_4(188);
    record = Actor_unk15(0);
    if (record != 0) {
        Actor_unk84_4(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_unk85_4(8);
    Actor_unk86_4(0, 0, 24);
    Actor_unk87_4(4);
    Actor_unk88_4(188);
    Actor_unk89_4(8, 0, 16);
    Actor_unk90_4(0);
    Actor_unk34_3(8, 0x168, 152);
    Actor_unk91_4(8);
    Actor_unk92_4();
    Actor_unk6_2(0x220);
}

void Scene_RunScene3b4SequenceA(void)
{
    u32 i;
    s32 record;

    Actor_unk93_4();
    Actor_unk35_3(0, 0x1b333, 0xd999);
    Actor_unk36_3(9, 0x1b333, 0xd999);
    Actor_unk94_4(188);
    record = Actor_unk16(0);
    if (record != 0) {
        Actor_unk95_4(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_unk96_4(9);
    Actor_unk97_4(0, 0, 24);
    Actor_unk98_4(188);
    Actor_unk99_4(4);
    Actor_unk100_4(9, 0, 16);
    Actor_unk101_4(0);
    Actor_unk37_3(9, 168, 0x108);
    Actor_unk102_4(9);
    Actor_unk103_4();
    Actor_unk7_2(0x220);
}

void Scene_RunPrimarySequence(void)
{
    u32 i;
    s32 record;
    s32 base3_2000240;

    base3_2000240 = (s32)RuntimeSelectorTable;
    if (*(s16 *)((base3_2000240 + 0x24a)) != 10) {
        Actor_unk104_4();
        Actor_unk38_3(0, 0x1b333, 0xd999);
        Actor_unk39_3(10, 0x1b333, 0xd999);
        Actor_unk105_4(188);
        record = Actor_unk17(0);
        if (record != 0) {
            Actor_unk106_4(10, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_unk107_4(10);
        Actor_unk108_4(0, 0, 24);
        Actor_unk109_4(4);
        Actor_unk110_4(188);
        Actor_unk111_4(10, 0, 16);
        Actor_unk112_4(0);
        Actor_unk40_3(10, 0x108, 0x168);
        Actor_unk113_4(10);
        Actor_unk114_4(10);
        Actor_unk115_4();
    }
}

void Scene_RunScene3b4SequenceB(void)
{
    u32 i;
    s32 record;

    Actor_unk116_4();
    Actor_unk41_3(0, 0x1b333, 0xd999);
    Actor_unk42_3(12, 0x1b333, 0xd999);
    Actor_unk117_4(188);
    record = Actor_unk18(0);
    if (record != 0) {
        Actor_unk118_4(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_unk119_4(12);
    Actor_unk120_4(0, 0, 24);
    Actor_unk121_4(188);
    Actor_unk122_4(12, 0, 16);
    Actor_unk123_4(0);
    Actor_unk43_3(12, 0x138, 232);
    Actor_unk124_4(12);
    Actor_unk125_4();
    Actor_unk8_2(0x220);
}

void Scene_RunScene3b4(void)
{
    u32 i;
    s32 record;

    Actor_unk9_2(0x2009e95);
    Actor_unk126_4(14, 0, 0);
    if (Actor_unk19(0x207) != 0) {
        Actor_unk5_5(58, 36, 1, 1, 45, 43);
    } else {
        Actor_unk6_5(46, 43, 1, 1, 45, 43);
    }
    Actor_unk127_4();
    Actor_unk10_2(0x206);
}

void Scene_RunScene3b4(void)
{
    u32 i;
    s32 record;

    Actor_unk128_4(1);
    Actor_unk129_4(12, 243);
    Actor_unk130_4(11, 244);
    Actor_unk131_3(10, 244);
    Actor_unk132_3(9, 244);
    Actor_unk133_3(8, 244);
    if (Actor_unk20(0xee7) == 0) {
        Actor_unk44_3(8, 0xe80000, 0x3680000);
    }
    if (Actor_unk21(0xee8) == 0) {
        Actor_unk45_3(9, 0x1280000, 0x3380000);
    }
    if (Actor_unk22(0xee9) == 0) {
        Actor_unk46_3(10, 0x1480000, 0x2f80000);
    }
    if (Actor_unk23(0xeea) == 0) {
        Actor_unk47_3(11, 0x1680000, 0x3680000);
    }
    if (Actor_unk24(0x9c0) != 0) {
        Actor_unk134_3(0);
    }
    if (Actor_unk25(0x9c1) != 0) {
        Actor_unk135_3(1);
    }
    if (Actor_unk26(0x9c2) != 0) {
        Actor_unk136_3(2);
    }
    if (Actor_unk27(0x9c3) != 0) {
        Actor_unk137_3(3);
    }
    if (Actor_unk28(0x9c4) != 0) {
        Actor_unk138_3(0);
    }
}

void Scene_RunScene3b4(void)
{
    s32 record;

    *(u8 *)(Actor_unk50(8) + 89) = 1;
    *(u8 *)(Actor_unk51(9) + 89) = 1;
    *(u8 *)(Actor_unk52(10) + 89) = 1;
    *(u8 *)(Actor_unk53(11) + 89) = 1;
    record = Actor_unk54(8);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Actor_unk29(9);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Actor_unk30(10);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Actor_unk31(11);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Actor_unk55(12);
    *(volatile s32 *)(record + 24) = 0xb333;
    Actor_unk9_4(0x20097ad, 0xc80);
    Actor_unk32(0x200941d, 0xc80);
    Actor_unk33(0x2009309, 0xc80);
    {
        u16 t;
        t = 0x3f42;
        *(volatile u16 *)0x04000050 = t;
        t = 0x607;
        *(volatile u16 *)0x04000052 = t;
    }
}

void Scene_RunScene3b4(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    *(u8 *)(Actor_unk56(14) + 85) = 0;
    Actor_unk10_4(0x2009e95, 0xc80);
    Actor_unk34(0x2009edd, 0xc80);
    Actor_unk139_3(107, 0, 0);
    if (Actor_unk35(0xed9) != 0) {
        Actor_unk140_3(14, 2);
    }
    Actor_unk141_3();
    Actor_unk142_3();
    Actor_unk143_3();
    Actor_unk144_3();
    Actor_unk145_3();
    Actor_unk146_3(8, 3);
    *(u8 *)(Actor_unk57(11) + 85) = 0;
    *(u8 *)(Actor_unk58(12) + 85) = 0;
    Actor_unk147_3();
    if (Actor_unk36(0x200) != 0) {
        Actor_unk148_3();
        Actor_unk149_3(13, 5);
    }
    if (Actor_unk37(0x109) == 0) {
        rec7 = Actor_unk38(0x9ca);
        if (rec7 != 0) {
            Actor_unk48_3(15, 0x3580000, 0x3380000);
            record = Actor_unk59(15);
            *(volatile s32 *)(record + 108) = 0x2008aa9;
        } else {
            if (Actor_unk39(0x9c9) != 0) {
                Actor_unk49_3(15, 0x3780000, 0x2980000);
                record = Actor_unk40(15);
                *(volatile u16 *)(*(volatile s32 *)(record + 80) + 30) = rec7;
                Actor_unk150_3(record, 16);
            } else {
                if (Actor_unk41(0x9c8) != 0) {
                    Actor_unk50_3(15, 0x2480000, 0x2a80000);
                } else {
                    Actor_unk51_3(15, 0x2480000, 0x2980000);
                }
            }
        }
    }
}

/* overlays/scene/actor/staged_step/scene_setup.c */

u8 *Actor_unk151_3(s32);

u8 *Actor_unk152_3(s32);

/* Contiguous unnamed leaf-owner run for resource_3b4. */

void Scene_RunActor15ZeroStep(void)
{
    Actor_unk153_3();
    Actor_Apply(15, 0);
    Actor_unk154_3();
}

void Scene_RunLateIndexedStep0(void)
{
    Actor_unk13_2(0);
}

void Scene_RunLateIndexedStep1(void)
{
    Actor_unk14_2(1);
}

void Scene_RunLateIndexedStep2(void)
{
    Actor_unk15_2(2);
}

void State_ApplyRectAndSetSlotEightByte35(void)
{
    s32 width = 40;
    s32 height = 42;
    u8 *entry;

    Actor_unk10_5(39, 42, 1, 1, width, height);
    entry = Actor_unk151_3(8) + 35;
    *entry = 2;
}

void Scene_RunTwoStepSequence(void)
{
    Actor_unk155_2();
    Actor_unk156_2();
}

void State_ApplyRectAndClearSlotTenByte85(void)
{
    s32 width = 38;
    s32 height = 55;
    u8 *entry;

    Actor_unk11_5(40, 54, 1, 1, width, height);
    entry = Actor_unk152_3(10) + 85;
    *entry = 0;
}

void Scene_RunThreeCallSequenceB(void)
{
    Actor_unk157_2();
    Actor_unk158_2();
    Actor_unk159_2();
}

void Scene_RunThreeCallSequence(void)
{
    Actor_unk160_2();
    Actor_unk161_2();
    Actor_unk162_2();
}

void Scene_RunThreeStepSequence(void)
{
    Actor_unk163_2();
    Actor_unk164_2();
    Actor_unk165_2();
}

/* overlays/scene/actor/staged_step/scene_state_interaction.c */
s32 *Actor_unk166_2();

u8 *Actor_unk167_2(s32);

/*
 * Scene state interaction for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the scene flag test, clear and set imports. */

/*
 * Dispatch on slot 0's halfword at +6, the facing field, which takes the
 * values 0, 0x4000, 0x8000 and 0xc000.  The 160-byte owner includes its two
 * pool words, 0x206 and 0x207.  Two arms repaint one collision cell, but
 * only while scene flag 0x206 is set, and then move flag 0x207.  The 0x8000
 * arm does no flag work and branches on slot 0's height word at +12, read
 * from the record pointer already in hand rather than a fresh lookup.
 */
void Scene_DispatchBySlotZeroFacing(void)
{
    s32 *slot = Actor_unk166_2(0);
    u16 facing = *(u16 *)((u8 *)slot + 6);

    if (facing == 0xc000) {
        if (Actor_unk60(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Actor_unk168_2(46, 43, 1, 1, fifth, last); }
        }
        Actor_unk169_2(0x207);
        Actor_unk170_2();
    } else if (facing == 0x4000) {
        Actor_unk171_2();
    } else if (facing == 0) {
        if (Actor_unk61(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Actor_unk172_2(58, 36, 1, 1, fifth, last); }
        }
        Actor_unk173_2(0x207);
        Actor_unk174_2();
    } else if (facing == 0x8000) {
        if (slot[3] == 0) {          /* +12 */
            Actor_unk175_2();
        } else {
            Actor_unk176_2();
        }
    }
}

void State_ApplyRectAndSetActor8Byte35(void)
{
    s32 w = 41;
    s32 h = 42;
    u8 *p;

    Actor_unk12_5(44, 42, 1, 1, w, h);
    p = Actor_unk167_2(8) + 35;
    *p = 2;
}

void State_ApplyFourRectsAt48_55(void)
{
    s32 base = 55;

    Actor_unk13_5(48, 55, 1, 1, 49, base);
    Actor_unk14_5(48, 55, 1, 1, 50, base);
    Actor_unk15_5(48, 55, 1, 1, 51, base);
    Actor_unk16_5(48, 55, 1, 1, 52, base);
}

/* overlays/scene/actor/staged_step/scene_wrapper.c */

void Scene_CallHelper(void)
{
    Actor_unk177_2();
}

void Scene_CallHelper2(void)
{
    Actor_unk178_2();
}

/* overlays/scene/actor/staged_step/shared.c */

void Scene_RunSingleStep(void)
{
    Actor_unk110_4();
}

/* overlays/scene/actor/staged_step/staged_actor.c */

extern u8 *gWork;

u8 *Actor_unk179_2(s32);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_unk180_2(actor, x, y);
}

s32 StagedActor_FindClearPosition(s32 *a)
{
}

void Scene_RunIndexedStep0(void)
{
    Actor_unk16_2(0);
}

void Scene_RunIndexedStep1(void)
{
    Actor_unk16_2(1);
}

void State_ResetCounter412OnHeading4000B(void)
{
    u8 *slot;
    s16 *cnt;
    s32 reset;

    Actor_unk16_2(2);
    slot = Actor_unk179_2(0);
    if (*(u16 *)(slot + 6) == 0x4000) {
        cnt = (s16 *)(gWork + 412);
        if (*cnt > 12) {
            Actor_unk181_2();
            reset = 0;
            *cnt = reset;
        }
    }
}

void State_ResetCounter412OnHeading4000(void)
{
    u8 *p;
    s16 *cnt;
    s32 zero;

    Actor_unk16_2(3);
    p = Actor_unk179_2(0);
    if (*(u16 *)(p + 6) == 0x4000) {
        cnt = (s16 *)(gWork + 412);
        if (*cnt > 12) {
            Actor_unk181_2();
            zero = 0;
            *cnt = zero;
        }
    }
}

void State_ApplyRectAndPlaceSlot12(void)
{
    s32 a = 25;
    s32 b = 48;
    s32 slot = 12;
    s32 x = 0x1980000;
    s32 z = 0x3080000;

    Actor_unk17_5(25, 45, 1, 2, a, b);
    if (Actor_unk62(0xeeb) == 0)
        Actor_unk58_3(slot, x, z);
    Actor_unk17_2(1);
}

void ConfigureAndPlaceActorTwelve(void)
{
    s32 a = 25, b = 48;
    Actor_unk182_2(24, 48, 1, 2, a, b);
    PlaceActor(12, 0x00080000, 0x00080000);
}

void Scene_RunStep8ValueEe7(void)
{
    Actor_Apply2(0xF4, 3);
    Actor_Apply3(0, 1);
    Actor_Apply4(0xF4, 0);
    Actor_unk58_3(8, 0, 0);
    Actor_unk18_2(0xEE7);
}

void Scene_RunStep9ValueEe8(void)
{
    Actor_Apply2(0xF4, 3);
    Actor_Apply3(0, 1);
    Actor_Apply4(0xF4, 0);
    Actor_unk58_3(9, 0, 0);
    Actor_unk18_2(0xEE8);
}

void Scene_RunStep10ValueEe9(void)
{
    Actor_Apply2(0xF4, 3);
    Actor_Apply3(0, 1);
    Actor_Apply4(0xF4, 0);
    Actor_unk58_3(0xA, 0, 0);
    Actor_unk18_2(0xEE9);
}

void Scene_RunStep11ValueEea(void)
{
    Actor_Apply2(0xF4, 3);
    Actor_Apply3(0, 1);
    Actor_Apply4(0xF4, 0);
    Actor_unk58_3(0xB, 0, 0);
    Actor_unk18_2(0xEEA);
}

void Scene_RunStep12ValueEeb(void)
{
    Actor_Apply2(0xF3, 3);
    Actor_Apply3(0, 1);
    Actor_Apply4(0xF3, 0);
    Actor_unk58_3(0xC, 0, 0);
    Actor_unk18_2(0xEEB);
}

/* overlays/scene/actor/staged_step/staged_actor_navigation.c */
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

extern s32 Actor_Far2[];
extern s32 gOv2[];

Ent *Actor_unk183_2(Desc *, Ent *);

s32 Actor_CheckAheadTileAndStop(Ent *a)
{
    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Actor_Far2[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Actor_unk183_2(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = gOv2;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Actor_Far2[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Actor_EntOp(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

/* overlays/scene/actor/staged_step/staged_actor_pair.c */

extern u32 Actor_Far2[];

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

void StagedActor_AdvanceActorPair(void)
{
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

/* overlays/scene/actor/staged_step/state_update.c */
u8 *Actor_unk184_2(s32);

u8 *Actor_unk185_2(s32);

u8 *Actor_unk186_2(s32);

u8 *Actor_unk187_2(s32);

u8 *Actor_unk188_2(s32);

/* Four sites of one import, so four names. */

void State_SetSelectorFlagWhenFacingC000(s32 selector)
{
    u8 *slot = Actor_unk184_2(0);
    s32 flag;

    if (*(u16 *)(slot + 6) != 0xC000) {
        return;
    }
    flag = selector + 2496;
    if (Actor_unk63(flag)!= 0) {
        return;
    }
    if (Actor_unk64(244) == -1) {
        return;
    }
    Actor_unk19_2(flag);
    Actor_unk20_2(0x100 | selector);
    Actor_unk189_2(244);
}

void Actor_MarkSlot13AndSetFlag200(void)
{
    u8 *slot = Actor_unk185_2(13);
    s32 fifth = 40;
    s32 sixth = 55;

    Actor_unk18_5(40, 54, 1, 1, fifth, sixth);
    if (slot != 0) {
        u8 *other = Actor_unk186_2(13) + 85;
        u8 *flags = slot + 35;

        *other = 0;
        *flags = 2;
    }
    Actor_unk21_2(512);
}

void State_ApplyFourRectsAndSetActor8Byte85(void)
{
    Actor_unk19_5(57, 42, 1, 1, 40, 42);
    Actor_unk20_5(57, 42, 1, 1, 41, 42);
    Actor_unk21_5(58, 42, 1, 1, 42, 42);
    Actor_unk22_5(62, 37, 3, 1, 37, 42);

    Actor_unk187_2(8)[85] = 1;
}

void State_ApplyTwoRectsAtRow56(void)
{
    s32 base = 55;

    Actor_unk23_5(38, 56, 1, 1, 38, base);
    Actor_unk24_5(42, 56, 1, 1, 42, base);
}

void State_ApplyRectAndClearActor10Byte85(void)
{
    s32 w = 42;
    s32 h = 55;
    u8 *p;

    Actor_unk25_5(40, 54, 1, 1, w, h);
    p = Actor_unk188_2(10) + 85;
    *p = 0;
}
