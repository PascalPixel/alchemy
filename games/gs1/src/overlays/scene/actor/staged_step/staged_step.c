#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/staged_step/staged_step.h"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"
#include "find_clear_actor_position.h"
#include "find_clear_actor_position_body.inc"
#include "staged_actor.h"

/* overlays/scene/actor/staged_step/actor_facing.c */
u8 *Actor_Run11();

/*
 * Complete 40-byte heading update: face the supplied entity towards entity 0,
 * store the resulting angle in its +6 halfword and report zero.
 */
s32 Actor_FaceTowardActorZero(u8 *obj)
{
    u8 *p = Actor_Run11(0);
    *(u16 *)(obj + 6) = (u16)Actor_Check42(
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

s32 *Actor_Run12(s32);

s32 *Actor_Run13(s32);

struct Actor *Actor_Run14(s32);
struct Actor *Actor_Run15(s32);
struct Frame *Actor_Run16(s32);

void Actor_Run17(void)
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
            dst = Actor_Check2(15);
            src = Actor_Check3(0);
            *(volatile s32 *)(dst + 48) = *(volatile s32 *)(src + 48);
            dst = Actor_Check4(15);
            src = Actor_Check5(0);
            *(volatile s32 *)(dst + 52) = *(volatile s32 *)(src + 48);
            idx -= 30;
            tbl = 0x0200a808;
            idx <<= 3;
            idx4 = idx + 4;
            Actor_Run18(15, *(volatile s32 *)(tbl + idx), *(volatile s32 *)(tbl + idx4));
        }
    }
}

void Actor_TrackOriginColumnForSlot(s32 no)
{
    extern s16 RuntimeSelectorTable[];

    s32 *pos = Actor_Run12(0);
    s32 col = pos[2] / 0x100000;   /* +8  */
    s32 row = pos[4] / 0x100000;      /* +16 */
    s32 slot = no + 10;

    if (RuntimeSelectorTable[293] == slot) return;
    if (col == gOv[no]) return;

    Actor_Place52(slot, 0x48000, 0x24000);
    Actor_Do11(188);
    Actor_Place53(slot, (col << 4) + 8, 360);

    gOv[no] = col;

    if (row <= 22) {
        Actor_Place54(0, 0, 8);
    }
    Actor_Do12(0);
}

void Actor_RunWhenActor9AtTile45x43(void)
{
    s32 *slot = Actor_Run13(9);
    s32 x = slot[2] / 0x100000;
    s32 z = slot[4] / 0x100000;

    if (x == 45 && z == 43) {
        Actor_Run19();
    }
}

void ActorDraw_PlaceActorFourteenOnActorNine(void)
{
    struct Actor *target = Actor_Run14(14);
    struct Actor *source = Actor_Run15(9);

    target->f0c = 0x200000;
    target->f08 = source->f08;
    target->f10 = source->f10 + 0x10000;
}

void Actor_PassActorNinePositionWithId107(void)
{
    struct Frame *frame = Actor_Run16(9);

    Actor_Place55(107, frame->f08, frame->f10 + 0x10000);
}

/* overlays/scene/actor/staged_step/actor_presentation.c */

u8 *Actor_Run20(s32);

s32 *Actor_Run21();

s32 *Actor_Run22();

s32 *Actor_Run23();
s32 *Actor_Run24();
s32 *Actor_Run25();
s32 *Actor_Run26();
s32 *Actor_Run27();
s32 *Actor_Run28();
s32 *Actor_Run29();

s32 *Actor_Run30();
s32 *Actor_Run31();
s32 *Actor_Run32();

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
    Actor_Place56(15, 0, 6);
}

void ActorDraw_SetSceneCell58AndMarkActorEight(void)
{
    s32 extent = 42;
    u8 *entry;

    Actor_SetRect7(58, 41, 1, 1, extent, extent);
    entry = Actor_Run20(8) + 35;
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

    Actor_Run33();

    slot = Actor_Run21(11);
    tile = slot[2] / 0x100000;
    Actor_SetRect8(53, 55, 1, 1, tile, 55);
    slot = Actor_Run22(12);
    tile = slot[2] / 0x100000;
    Actor_SetRect9(53, 55, 1, 1, tile, 55);
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
    s32 *slot0 = Actor_Run23(0);
    u8 marker;

    if (slot0[3] > 0x100000) {                 /* +12 */
        marker = 2;
        ((u8 *)Actor_Run24(8))[35] = marker;
        if (Actor_Run25(10)[3] == 0) {
            ((u8 *)Actor_Run26(10))[35] = marker;
        }
        ((u8 *)Actor_Run27(11))[35] = marker;
    } else {
        if (Actor_Run28(10)[3] == 0 &&
            Actor_Run29(0)[4] / 0x100000 > 56) {   /* +16 */
            Actor_Run34(10, 3);
        } else {
            Actor_Run35(10, 1);
            ((u8 *)Actor_Run30(10))[35] = 1;
        }
        marker = 0;
        ((u8 *)Actor_Run31(11))[35] = marker;
    }

    ((u8 *)Actor_Run32(12))[35] = marker;
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

struct Slot *Actor_Run36(s32);

s32 *Actor_Run37();
s32 *Actor_Run38();

struct Slot *Actor_Run39(s32);

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
    struct Slot *slot = Actor_Run36(0);

    if (slot->f06 == 0) {
        Actor_Run40();
    } else {
        Actor_Run41();
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
    s32 *slot0 = Actor_Run37(0);
    s32 *slot8 = Actor_Run38(8);
    s32 x0 = slot0[2] / 0x100000;
    s32 x8 = slot8[2] / 0x100000;

    if (x0 == 38 && x8 != 38) {
        u16 facing = ((u16 *)slot0)[3];

        /*
         * The facing value is still in r0 at both branches, but whether
         * either callee reads it is unverified, so no argument is passed.
         */
        if (facing == 0xc000) {
            Actor_Run42();
            return;
        }
        if (facing == 0x4000) {
            Actor_Run43();
            return;
        }
    }

    Actor_Run44();
    Actor_Run45();
    Actor_Run46();
}

void Scene_DispatchByActorZeroFacing(void)
{
    struct Slot *slot = Actor_Run39(0);

    if (slot->f06 == 0x4000) {
        Actor_Place57(0, 6, 0);
    } else {
        Actor_Run47();
    }
}

/* overlays/scene/actor/staged_step/move_and_redraw.c */

#define STAGED_ACTOR_MOVEMENT_UNTYPED_LOCATION_CALL

void Actor_Run48(StagedActorMovementRequest request)
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

    if (Actor_Check6(0x9c8) == 0) {
        Actor_Do(0x9c8);
        Actor_Run49();
        Actor_Run(0x20000, 0x4000);
        Actor_Run50(15, 1);
        Actor_Run51();
        Actor_Run52(15, 0, 20);
        Actor_Run2(15, 0x102);
        Actor_Run53(15, 2);
        Actor_Run54(20);
        Actor_Place(15, 0x10000, 0x8000);
        Actor_Run55(152);
        record = Actor_Check43(15);
        *(volatile s32 *)(record + 40) = 0x80000;
        Actor_Place2(15, 0x248, 0x2a8);
        Actor_Place3(15, 0x4000, 20);
        Actor_Run56();
    }
}

void Scene_RunScene3b4(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    if (Actor_Check7(0x9c8) == 0) {
    } else {
        rec8 = Actor_Check8(0x9c9);
        if (rec8 != 0) {
        } else {
            Actor_Do2(0x9c9);
            Actor_Run57();
            Actor_Run3(0x20000, 0x4000);
            Actor_Run58(15, 1);
            Actor_Run59();
            Actor_Place4(15, 0x4000, 20);
            Actor_Run4(15, 0x102);
            Actor_Run60(15, 2);
            Actor_Run61(20);
            Actor_Place5(15, 0x10000, 0x8000);
            Actor_Run62(152);
            record = Actor_Check44(15);
            *(volatile s32 *)(record + 40) = 0xa0000;
            Actor_Place6(15, 0x248, 0x298);
            Actor_Place7(15, 0x4000, 20);
            Actor_Run5(15, 0x102);
            Actor_Run63(30);
            Actor_Place8(15, 0x80000, 0x4000);
            Actor_Place9(15, 0x298, 0x298);
            Actor_Place10(15, 0x2e8, 0x298);
            Actor_Place11(15, 0x338, 0x298);
            Actor_Run64(10);
            Actor_Run65(208);
            Actor_Place12(0x40000, 0x20000, 0x10000);
            Actor_Run66(20);
            Actor_Place13(-1, -1, 0xe666);
            Actor_Run67(30);
            Actor_Place14(15, 0x3780000, 0x2980000);
            rec7 = Actor_Check9(15);
            {
                s32 target = *(volatile s32 *)(rec7 + 80);
                s32 shown = 0xf800;

                *(volatile u16 *)(target + 30) = shown;
            }
            *(volatile u16 *)(rec7 + 6) = rec8;
            Actor_Run68(rec7, 0);
            Actor_Check10(rec7, 0x200a6fc);
            Actor_Run69();
        }
    }
}

void Scene_RunScene3b4(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (Actor_Check11(0x9c9) == 0) {
    } else {
        rec7 = Actor_Check12(0x9ca);
        if (rec7 != 0) {
        } else {
            Actor_Do3(0x9ca);
            Actor_Run70();
            record = Actor_Check13(15);
            *(volatile u16 *)(*(volatile s32 *)(record + 80) + 30) = rec7;
            Actor_Run71(record, 16);
            Actor_Run72(152);
            record = Actor_Check45(15);
            *(volatile s32 *)(record + 40) = 0x80000;
            Actor_Place15(15, 0x8000, 30);
            Actor_Run6(15, 0x102);
            Actor_Run73(15, 2);
            Actor_Run74(20);
            Actor_Place16(15, 0x10000, 0x8000);
            Actor_Run75(152);
            record = Actor_Check46(15);
            *(volatile s32 *)(record + 40) = 0x40000;
            Actor_Place17(15, 0x370, 0x2a8);
            Actor_Run76(10);
            Actor_Run7(15, 0x101);
            Actor_Place18(15, 0x20000, 0x10000);
            Actor_Place19(15, 0x370, 0x2b8);
            Actor_Place20(15, 0x372, 0x2c0);
            Actor_Place21(15, 0x370, 0x2c8);
            Actor_Place22(15, 0x36e, 0x2d0);
            Actor_Place23(15, 0x370, 0x2d8);
            Actor_Place24(15, 0x372, 0x2e0);
            Actor_Place25(15, 0x370, 0x2e8);
            Actor_Place26(15, 0x36e, 0x2f0);
            Actor_Place27(15, 0x370, 0x2f8);
            Actor_Place28(15, 0x3580000, 0x3380000);
            Actor_Run77(10);
            Actor_Place29(15, 0xc000, 20);
            Actor_Run8(15, 0x100);
            record = Actor_Check47(15);
            *(volatile s32 *)(record + 108) = 0x2008aa9;
            Actor_Run78();
        }
    }
}

void Scene_RunScene3b4(s32 a0)
{
    u32 i;
    s32 record;

    if ((a0 & 0x100) != 0) {
        Actor_Run79(157);
        Actor_Place30(0x20000, 0x20000, 0x10000);
        Actor_Place31(-1, -1, 0xe666);
        Actor_SetRect(84, 29, 1, 3, 70, 49);
        Actor_Run80(60);
    }
    Actor_SetRect2(85, 29, 1, 3, 70, 49);
    Actor_SetRect3(6, 49, 1, 1, 6, 50);
    Actor_SetRect4(6, 49, 1, 1, 6, 51);
}

void Scene_RunScene3b4SequenceC(void)
{
    u32 i;
    s32 record;

    record = Actor_Check48(0);
    if (*(u16 *)(record + 6) == 0xc000) {
        if (Actor_Check14(0x9c4) == 0) {
            if (Actor_Check49(243) != -1) {
                Actor_Do4(0x9c4);
                Actor_Do5(0x100);
                Actor_Run81(243);
            }
        }
    }
}

void Scene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;

    Actor_Run82();
    Actor_Place32(0, 0x1e666, 0xf333);
    Actor_Place33(8, 0x1e666, 0xf333);
    Actor_Run83(188);
    record = Actor_Check15(0);
    if (record != 0) {
        Actor_Run84(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_Run85(8);
    Actor_Run86(0, 0, 24);
    Actor_Run87(4);
    Actor_Run88(188);
    Actor_Run89(8, 0, 16);
    Actor_Run90(0);
    Actor_Place34(8, 0x168, 152);
    Actor_Run91(8);
    Actor_Run92();
    Actor_Do6(0x220);
}

void Scene_RunScene3b4SequenceA(void)
{
    u32 i;
    s32 record;

    Actor_Run93();
    Actor_Place35(0, 0x1b333, 0xd999);
    Actor_Place36(9, 0x1b333, 0xd999);
    Actor_Run94(188);
    record = Actor_Check16(0);
    if (record != 0) {
        Actor_Run95(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_Run96(9);
    Actor_Run97(0, 0, 24);
    Actor_Run98(188);
    Actor_Run99(4);
    Actor_Run100(9, 0, 16);
    Actor_Run101(0);
    Actor_Place37(9, 168, 0x108);
    Actor_Run102(9);
    Actor_Run103();
    Actor_Do7(0x220);
}

void Scene_RunPrimarySequence(void)
{
    u32 i;
    s32 record;
    s32 base3_2000240;

    base3_2000240 = (s32)RuntimeSelectorTable;
    if (*(s16 *)((base3_2000240 + 0x24a)) != 10) {
        Actor_Run104();
        Actor_Place38(0, 0x1b333, 0xd999);
        Actor_Place39(10, 0x1b333, 0xd999);
        Actor_Run105(188);
        record = Actor_Check17(0);
        if (record != 0) {
            Actor_Run106(10, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_Run107(10);
        Actor_Run108(0, 0, 24);
        Actor_Run109(4);
        Actor_Run110(188);
        Actor_Run111(10, 0, 16);
        Actor_Run112(0);
        Actor_Place40(10, 0x108, 0x168);
        Actor_Run113(10);
        Actor_Run114(10);
        Actor_Run115();
    }
}

void Scene_RunScene3b4SequenceB(void)
{
    u32 i;
    s32 record;

    Actor_Run116();
    Actor_Place41(0, 0x1b333, 0xd999);
    Actor_Place42(12, 0x1b333, 0xd999);
    Actor_Run117(188);
    record = Actor_Check18(0);
    if (record != 0) {
        Actor_Run118(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_Run119(12);
    Actor_Run120(0, 0, 24);
    Actor_Run121(188);
    Actor_Run122(12, 0, 16);
    Actor_Run123(0);
    Actor_Place43(12, 0x138, 232);
    Actor_Run124(12);
    Actor_Run125();
    Actor_Do8(0x220);
}

void Scene_RunScene3b4(void)
{
    u32 i;
    s32 record;

    Actor_Do9(0x2009e95);
    Actor_Run126(14, 0, 0);
    if (Actor_Check19(0x207) != 0) {
        Actor_SetRect5(58, 36, 1, 1, 45, 43);
    } else {
        Actor_SetRect6(46, 43, 1, 1, 45, 43);
    }
    Actor_Run127();
    Actor_Do10(0x206);
}

void Scene_RunScene3b4(void)
{
    u32 i;
    s32 record;

    Actor_Run128(1);
    Actor_Run129(12, 243);
    Actor_Run130(11, 244);
    Actor_Run131(10, 244);
    Actor_Run132(9, 244);
    Actor_Run133(8, 244);
    if (Actor_Check20(0xee7) == 0) {
        Actor_Place44(8, 0xe80000, 0x3680000);
    }
    if (Actor_Check21(0xee8) == 0) {
        Actor_Place45(9, 0x1280000, 0x3380000);
    }
    if (Actor_Check22(0xee9) == 0) {
        Actor_Place46(10, 0x1480000, 0x2f80000);
    }
    if (Actor_Check23(0xeea) == 0) {
        Actor_Place47(11, 0x1680000, 0x3680000);
    }
    if (Actor_Check24(0x9c0) != 0) {
        Actor_Run134(0);
    }
    if (Actor_Check25(0x9c1) != 0) {
        Actor_Run135(1);
    }
    if (Actor_Check26(0x9c2) != 0) {
        Actor_Run136(2);
    }
    if (Actor_Check27(0x9c3) != 0) {
        Actor_Run137(3);
    }
    if (Actor_Check28(0x9c4) != 0) {
        Actor_Run138(0);
    }
}

void Scene_RunScene3b4(void)
{
    s32 record;

    *(u8 *)(Actor_Check50(8) + 89) = 1;
    *(u8 *)(Actor_Check51(9) + 89) = 1;
    *(u8 *)(Actor_Check52(10) + 89) = 1;
    *(u8 *)(Actor_Check53(11) + 89) = 1;
    record = Actor_Check54(8);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Actor_Check29(9);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Actor_Check30(10);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Actor_Check31(11);
    *(volatile s32 *)(record + 24) = 0xb333;
    record = Actor_Check55(12);
    *(volatile s32 *)(record + 24) = 0xb333;
    Actor_Run9(0x20097ad, 0xc80);
    Actor_Check32(0x200941d, 0xc80);
    Actor_Check33(0x2009309, 0xc80);
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

    *(u8 *)(Actor_Check56(14) + 85) = 0;
    Actor_Run10(0x2009e95, 0xc80);
    Actor_Check34(0x2009edd, 0xc80);
    Actor_Run139(107, 0, 0);
    if (Actor_Check35(0xed9) != 0) {
        Actor_Run140(14, 2);
    }
    Actor_Run141();
    Actor_Run142();
    Actor_Run143();
    Actor_Run144();
    Actor_Run145();
    Actor_Run146(8, 3);
    *(u8 *)(Actor_Check57(11) + 85) = 0;
    *(u8 *)(Actor_Check58(12) + 85) = 0;
    Actor_Run147();
    if (Actor_Check36(0x200) != 0) {
        Actor_Run148();
        Actor_Run149(13, 5);
    }
    if (Actor_Check37(0x109) == 0) {
        rec7 = Actor_Check38(0x9ca);
        if (rec7 != 0) {
            Actor_Place48(15, 0x3580000, 0x3380000);
            record = Actor_Check59(15);
            *(volatile s32 *)(record + 108) = 0x2008aa9;
        } else {
            if (Actor_Check39(0x9c9) != 0) {
                Actor_Place49(15, 0x3780000, 0x2980000);
                record = Actor_Check40(15);
                *(volatile u16 *)(*(volatile s32 *)(record + 80) + 30) = rec7;
                Actor_Run150(record, 16);
            } else {
                if (Actor_Check41(0x9c8) != 0) {
                    Actor_Place50(15, 0x2480000, 0x2a80000);
                } else {
                    Actor_Place51(15, 0x2480000, 0x2980000);
                }
            }
        }
    }
}

/* overlays/scene/actor/staged_step/scene_setup.c */

u8 *Actor_Run151(s32);

u8 *Actor_Run152(s32);

/* Contiguous unnamed leaf-owner run for resource_3b4. */

void Scene_RunActor15ZeroStep(void)
{
    Actor_Run153();
    Actor_Apply(15, 0);
    Actor_Run154();
}

void Scene_RunLateIndexedStep0(void)
{
    Actor_Do13(0);
}

void Scene_RunLateIndexedStep1(void)
{
    Actor_Do14(1);
}

void Scene_RunLateIndexedStep2(void)
{
    Actor_Do15(2);
}

void State_ApplyRectAndSetSlotEightByte35(void)
{
    s32 width = 40;
    s32 height = 42;
    u8 *entry;

    Actor_SetRect10(39, 42, 1, 1, width, height);
    entry = Actor_Run151(8) + 35;
    *entry = 2;
}

void Scene_RunTwoStepSequence(void)
{
    Actor_Run155();
    Actor_Run156();
}

void State_ApplyRectAndClearSlotTenByte85(void)
{
    s32 width = 38;
    s32 height = 55;
    u8 *entry;

    Actor_SetRect11(40, 54, 1, 1, width, height);
    entry = Actor_Run152(10) + 85;
    *entry = 0;
}

void Scene_RunThreeCallSequenceB(void)
{
    Actor_Run157();
    Actor_Run158();
    Actor_Run159();
}

void Scene_RunThreeCallSequence(void)
{
    Actor_Run160();
    Actor_Run161();
    Actor_Run162();
}

void Scene_RunThreeStepSequence(void)
{
    Actor_Run163();
    Actor_Run164();
    Actor_Run165();
}

/* overlays/scene/actor/staged_step/scene_state_interaction.c */
s32 *Actor_Run166();

u8 *Actor_Run167(s32);

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
    s32 *slot = Actor_Run166(0);
    u16 facing = *(u16 *)((u8 *)slot + 6);

    if (facing == 0xc000) {
        if (Actor_Check60(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Actor_Run168(46, 43, 1, 1, fifth, last); }
        }
        Actor_Run169(0x207);
        Actor_Run170();
    } else if (facing == 0x4000) {
        Actor_Run171();
    } else if (facing == 0) {
        if (Actor_Check61(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Actor_Run172(58, 36, 1, 1, fifth, last); }
        }
        Actor_Run173(0x207);
        Actor_Run174();
    } else if (facing == 0x8000) {
        if (slot[3] == 0) {          /* +12 */
            Actor_Run175();
        } else {
            Actor_Run176();
        }
    }
}

void State_ApplyRectAndSetActor8Byte35(void)
{
    s32 w = 41;
    s32 h = 42;
    u8 *p;

    Actor_SetRect12(44, 42, 1, 1, w, h);
    p = Actor_Run167(8) + 35;
    *p = 2;
}

void State_ApplyFourRectsAt48_55(void)
{
    s32 base = 55;

    Actor_SetRect13(48, 55, 1, 1, 49, base);
    Actor_SetRect14(48, 55, 1, 1, 50, base);
    Actor_SetRect15(48, 55, 1, 1, 51, base);
    Actor_SetRect16(48, 55, 1, 1, 52, base);
}

/* overlays/scene/actor/staged_step/scene_wrapper.c */

void Scene_CallHelper(void)
{
    Actor_Run177();
}

void Scene_CallHelper2(void)
{
    Actor_Run178();
}

/* overlays/scene/actor/staged_step/shared.c */

void Scene_RunSingleStep(void)
{
    Actor_Run110();
}

/* overlays/scene/actor/staged_step/staged_actor.c */

extern u8 *gWork;

u8 *Actor_Run179(s32);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_Run180(actor, x, y);
}

s32 StagedActor_FindClearPosition(s32 *a)
{
}

void Scene_RunIndexedStep0(void)
{
    Actor_Do16(0);
}

void Scene_RunIndexedStep1(void)
{
    Actor_Do16(1);
}

void State_ResetCounter412OnHeading4000B(void)
{
    u8 *slot;
    s16 *cnt;
    s32 reset;

    Actor_Do16(2);
    slot = Actor_Run179(0);
    if (*(u16 *)(slot + 6) == 0x4000) {
        cnt = (s16 *)(gWork + 412);
        if (*cnt > 12) {
            Actor_Run181();
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

    Actor_Do16(3);
    p = Actor_Run179(0);
    if (*(u16 *)(p + 6) == 0x4000) {
        cnt = (s16 *)(gWork + 412);
        if (*cnt > 12) {
            Actor_Run181();
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

    Actor_SetRect17(25, 45, 1, 2, a, b);
    if (Actor_Check62(0xeeb) == 0)
        Actor_Place58(slot, x, z);
    Actor_Do17(1);
}

void ConfigureAndPlaceActorTwelve(void)
{
    s32 a = 25, b = 48;
    Actor_Run182(24, 48, 1, 2, a, b);
    PlaceActor(12, 0x00080000, 0x00080000);
}

void Scene_RunStep8ValueEe7(void)
{
    Actor_Apply2(0xF4, 3);
    Actor_Apply3(0, 1);
    Actor_Apply4(0xF4, 0);
    Actor_Place58(8, 0, 0);
    Actor_Do18(0xEE7);
}

void Scene_RunStep9ValueEe8(void)
{
    Actor_Apply2(0xF4, 3);
    Actor_Apply3(0, 1);
    Actor_Apply4(0xF4, 0);
    Actor_Place58(9, 0, 0);
    Actor_Do18(0xEE8);
}

void Scene_RunStep10ValueEe9(void)
{
    Actor_Apply2(0xF4, 3);
    Actor_Apply3(0, 1);
    Actor_Apply4(0xF4, 0);
    Actor_Place58(0xA, 0, 0);
    Actor_Do18(0xEE9);
}

void Scene_RunStep11ValueEea(void)
{
    Actor_Apply2(0xF4, 3);
    Actor_Apply3(0, 1);
    Actor_Apply4(0xF4, 0);
    Actor_Place58(0xB, 0, 0);
    Actor_Do18(0xEEA);
}

void Scene_RunStep12ValueEeb(void)
{
    Actor_Apply2(0xF3, 3);
    Actor_Apply3(0, 1);
    Actor_Apply4(0xF3, 0);
    Actor_Place58(0xC, 0, 0);
    Actor_Do18(0xEEB);
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

Ent *Actor_Run183(Desc *, Ent *);

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
    r = Actor_Run183(&d, a);
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
u8 *Actor_Run184(s32);

u8 *Actor_Run185(s32);

u8 *Actor_Run186(s32);

u8 *Actor_Run187(s32);

u8 *Actor_Run188(s32);

/* Four sites of one import, so four names. */

void State_SetSelectorFlagWhenFacingC000(s32 selector)
{
    u8 *slot = Actor_Run184(0);
    s32 flag;

    if (*(u16 *)(slot + 6) != 0xC000) {
        return;
    }
    flag = selector + 2496;
    if (Actor_Check63(flag)!= 0) {
        return;
    }
    if (Actor_Check64(244) == -1) {
        return;
    }
    Actor_Do19(flag);
    Actor_Do20(0x100 | selector);
    Actor_Run189(244);
}

void Actor_MarkSlot13AndSetFlag200(void)
{
    u8 *slot = Actor_Run185(13);
    s32 fifth = 40;
    s32 sixth = 55;

    Actor_SetRect18(40, 54, 1, 1, fifth, sixth);
    if (slot != 0) {
        u8 *other = Actor_Run186(13) + 85;
        u8 *flags = slot + 35;

        *other = 0;
        *flags = 2;
    }
    Actor_Do21(512);
}

void State_ApplyFourRectsAndSetActor8Byte85(void)
{
    Actor_SetRect19(57, 42, 1, 1, 40, 42);
    Actor_SetRect20(57, 42, 1, 1, 41, 42);
    Actor_SetRect21(58, 42, 1, 1, 42, 42);
    Actor_SetRect22(62, 37, 3, 1, 37, 42);

    Actor_Run187(8)[85] = 1;
}

void State_ApplyTwoRectsAtRow56(void)
{
    s32 base = 55;

    Actor_SetRect23(38, 56, 1, 1, 38, base);
    Actor_SetRect24(42, 56, 1, 1, 42, base);
}

void State_ApplyRectAndClearActor10Byte85(void)
{
    s32 w = 42;
    s32 h = 55;
    u8 *p;

    Actor_SetRect25(40, 54, 1, 1, w, h);
    p = Actor_Run188(10) + 85;
    *p = 0;
}
