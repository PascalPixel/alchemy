#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/push_puzzle/push_puzzle.h"
#include "facing_object.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"

/* overlays/scene/actor/push_puzzle/actor_facing.c */

s32 Actor_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        tgt = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

/* overlays/scene/actor/push_puzzle/actor_presentation.c */
extern u8 gOv[];

u8 *Actor_Run10();

/*
 * Ten (x, z) tile pairs, held in the overlay's own writable image.  Overlay
 * data lives in EWRAM and is deliberately not const.
 */

/*
 * Slot accessor: Actor_Run10(slot) returns the actor record, or NULL.
 * Typed as a byte pointer so the +0x08 and +0x10 field reads are explicit.
 */

/*
 * The six-argument renderer ABI: four register arguments plus two stack
 * words, here the tile x and tile z of the cell being repainted.  The two
 * names are separate per-site call words that reach the same renderer.
 */

/*
 * Repaint ten fixed collision cells and then actor 8's own cell.  The
 * 92-byte owner includes the alignment halfword and the single pool word
 * that follows the code; that word holds the address of gOv, which
 * is in-image data rather than a RAM global.  The two renderer calls must
 * keep their separate call words -- naming one renderer for both changes the
 * displacement emitted at each site.
 */
void ActorDraw_RepaintTenCellsAndActorEightCell(void)
{
    u8 *actor;
    s32 tx;
    s32 tz;
    u32 i;

    /*
     * Slot 8 is the scene's own actor; the accessor result is not
     * null-checked here.  The 20-bit shift is one signed arithmetic shift:
     * 16 takes the fixed-point coordinate to pixels, the further 4 take it
     * to the 16-pixel tile grid.
     */
    actor = Actor_Run10(8);
    tx = *(s32 *)(actor + 0x08) >> 20;
    tz = *(s32 *)(actor + 0x10) >> 20;

    /*
     * Ten fixed cells from the table, then the actor's own cell.  The table
     * is walked by the byte index itself rather than by a 0..9 counter
     * scaled by two, so the loop steps the byte offset directly.
     */
    for (i = 0; i < 20; i += 2) {
        s32 x = (s32)gOv[i];
        s32 z = (s32)gOv[i + 1];
        Actor_SetRect8(1, 0, 1, 1, x, z);
    }

    /*
     * The same repaint with 0 rather than 1 in the first argument.  What
     * that selector chooses is not established.
     */
    Actor_SetRect9(0, 0, 1, 1, tx, tz);
}

/* overlays/scene/actor/push_puzzle/actor_search.c */
extern u8 *gWork;

s32 *Actor_FindAtTileXZ(s32 x, s32 z)
{
    s32 **tbl = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = tbl[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/* overlays/scene/actor/push_puzzle/overlay_object.c */
extern u8 gOv2[];

/* Per-site raw names preserve the stock relocated branch operands; they
 * resolve to Actor_Run11, Object_SetCallback and Object_InitializeMode respectively. */
extern u8 *Actor_Run12();

void OvObj_SpawnKind24AtActor(u8 *actor)
{
    u8 *obj;
    u8 *child;
    s32 mask;

    obj = Actor_Run12(24,
                            *(s32 *)(actor + 0x08),
                            *(s32 *)(actor + 0x0c),
                            *(s32 *)(actor + 0x10));
    if (obj == 0)
        return;

    child = *(u8 **)(obj + 0x50);
    Actor_Run13(obj, gOv2);
    *(u8 *)(obj + 0x55) = 0;
    *(u8 *)(obj + 0x22) = 1;
    *(u8 *)(obj + 0x23) = 2;
    if (child == 0)
        return;

    Actor_Run14(child, 2);
    *(u8 *)(child + 0x26) = 0;
    mask = 13;
    mask = -mask;
    *(u8 *)(child + 5) = (u8)((*(u8 *)(child + 5) & mask) | 4);
    *(u8 *)(child + 9) |= 12;
}

/* overlays/scene/actor/push_puzzle/scene_data_head.c */
#define PrimaryRuntimeSelector Value_0000001e
#define SecondaryRuntimeSelector Value_00000023
#define TertiaryRuntimeSelector Value_00000020

extern s16 RuntimeSelectorTable[];
extern u8 Value_00000020;
extern u8 gOv3[];

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
{
}

s32 SceneData_GetTable93FCWhenState20(void)
{
    if (RuntimeSelectorTable[224] == (s32)&Value_00000020) {
        return (s32)gOv3;
    }
    return 0;
}

/*
 * Table getter for resource_38b. The owner at 0x02000104 is eight bytes and
 * includes its one pool word at 0x02000108: the pc-relative load reads that
 * word, so the word belongs to this owner. The word is an address returned
 * without being dereferenced. Many getters share this body, but each returns
 * a different address.
 */
u8 *SceneData_GetTable942c(void)
{
    return (u8 *)0x0200942c;
}

/* overlays/scene/actor/push_puzzle/scene_dialogue.c */
extern u8 gWork[];

void Scene_RunScriptedStep1472(void)
{
    Actor_Run15();
    Actor_Apply(0x1472, 1);
    Actor_Run16();
}

void Scene_RunScriptedStep146E(void)
{
    Actor_Run15();
    Actor_Apply(0x146E, 1);
    Actor_Run16();
}

void Dialogue_RunLine1470(void)
{
    Actor_Run15();
    Actor_Apply(0x1470, 1);
    Actor_Run16();
}

void Scene_RunScene38b(void)
{
    Actor_Run17();
    Actor_Do(0x13ae);
    if (Actor_Check(0x301) != 0) {
        bump_step(1);
    }
    Actor_Run18(9, 0);
    Actor_Do2(0x301);
    Actor_Run19();
}

void Dialogue_RunActorTwelveDialogue(void)
{
    Actor_Run15();
    Actor_Do11(0x13B3);
    Actor_Apply2(12, 0);
    Actor_Run16();
}

void Dialogue_RunActorFourteenDialogue(void)
{
    Actor_Run15();
    Actor_Do11(0x13B7);
    Actor_Apply2(14, 0);
    Actor_Run16();
}

void Dialogue_ShowLine16BF(void)
{
    Actor_Run15();
    Actor_Do11(0x16BF);
    Actor_Apply2(21, 0);
    Actor_Run16();
}

void Dialogue_RunActorSixteenDialogue(void)
{
    Actor_Run15();
    Actor_Do11(0x16C8);
    Actor_Apply2(16, 0);
    Actor_Run16();
}

void Dialogue_ShowLine16CC(void)
{
    Actor_Run15();
    Actor_Do11(0x16CC);
    Actor_Apply2(18, 0);
    Actor_Run16();
}

/* overlays/scene/actor/push_puzzle/scene_primary_script.c */

extern u8 gWork[];
extern u8 gOv4[];

u8 *Actor_Run20();

u8 *Actor_Run21();

u8 *Actor_Run22();

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */

static __inline__ u16 ReadU16Elem(u16 *base, s32 idx)
{
    extern u16 RuntimeSelectorTable[];

    return *(u16 *)(base + idx);
}

void Scene_RunScriptedSteps947And29DD(void)
{
    Actor_Run23();
    Actor_Apply3(0x947, 1);
    Actor_Apply4(0x29DD, 1);
    Actor_Run24();
}

void Scene_RunEarlySequence(void)
{
    u32 i;
    u8 *record;
    s32 v5;
    u8 *tbl;
    u8 *tbl2;
    s32 off;
    s32 off2;
    s32 a1;
    s32 a2;
    u8 *p7;

    p7 = *(u8 *volatile *)gWork;
    Actor_Run25();
    for (i = 8; i < 66; i++) {
        record = (u8 *)Actor_Check2(i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    v5 = (s32)((s32)(*(volatile u16 *)(p7 + 0x16c) - 3) << 16) >> 16;
    if (v5 == 6) {
        Actor_Run26(188);
    } else {
        Actor_Do3(158);
    }
    off = v5 << 2;
    tbl = (u8 *)0x02009d0c;
    a1 = *(s16 *)(tbl + off);
    off2 = off + 2;
    a2 = *(s16 *)(tbl + off2);
    tbl2 = (u8 *)0x02009cf0;
    Actor_Place(*(s32 *)(tbl2 + off), a1, a2);
    Actor_Place2(0, 0x8000, 0x4000);
    *(u8 *)(Actor_Check24(0) + 85) = 0;
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    if (v5 == 6) {
        Actor_Run27(0, 2);
        Actor_Place3(0, 0, -4);
    } else {
        Actor_Place4(0, 3, -16);
    }
    if (v5 == 4) {
        Actor_Run28(0, 3);
    } else {
        Actor_Run29(0, 2);
    }
    Actor_Run30(16);
    Actor_Run31(v5 + 3);
    Actor_Run32();
}

void Scene_RunScene38bSequenceC(void)
{
    u32 i;
    s32 rec;
    u8 *rec7;
    u8 *record;

    rec = Actor_Check3(0);
    rec7 = Actor_Check4(11);
    if ((*(volatile s32 *)((s32)rec7 + 8) >> 20) != 6) {
    } else {
        Actor_Run33();
        Actor_Run34(11, 1);
        Actor_Run35(0, 2);
        Actor_Run36(20);
        Actor_Place5(0, 0x3333, 0x1999);
        Actor_Place6(11, 0x3333, 0x1999);
        *(u8 *)(Actor_Check25(0) + 90) &= 254;
        {
            u8 zero = 0;
            u8 *dst = rec7 + 85;

            *dst = zero;
        }
        *(volatile s32 *)(rec + 24) = -0x10000;
        Actor_Run(0, 0x102);
        Actor_Run37(0, 16);
        Actor_Run38(11, 111, 196);
        *(volatile s32 *)(rec + 24) = 0x10000;
        Actor_Run39(0, 128, 185);
        Actor_Run40(20);
        *(volatile s32 *)(rec + 24) = -0x10000;
        Actor_Run2(0, 0x102);
        Actor_Run41(0, 16);
        Actor_Run42(11, 121, 190);
        *(volatile s32 *)(rec + 24) = 0x10000;
        Actor_Run43(0, 141, 189);
        Actor_Run44(20);
        *(volatile s32 *)(rec + 24) = -0x10000;
        Actor_Run3(0, 0x102);
        Actor_Run45(0, 16);
        Actor_Run46(11, 132, 186);
        *(volatile s32 *)(rec + 24) = 0x10000;
        {
            u8 *record = Actor_Run20(0);
            u8 value = *(volatile u8 *)&record[90];

            record[90] = (u8)(value | 1);
        }
        Actor_Place7(0, 0x9999, 0x4ccc);
        Actor_Run47(0, 166, 185);
        Actor_Place8(0, 0x8000, 20);
        Actor_Run48(11, 2);
        Actor_Run49(0, 11);
        Actor_Run50(10);
        Actor_Do4(0x1774);
        Actor_Run51(11, 0);
        Actor_Run52();
        Actor_Run53(10);
        Actor_Do5(0x848);
        Actor_Run54();
    }
}

void Scene_RunScene38b(void)
{
    u32 i;
    s32 rec7;
    u8 *record;
    s32 base5_20091c0;

    rec7 = Actor_Check5(0);
    if (Actor_Check6(0x845) == 0) {
    } else {
        if (Actor_Check7(0x848) == 0) {
        } else {
            Actor_Run55();
            Actor_Run4(0x26666, 0x4ccc);
            Actor_Run5(0x1070000, -1, 0xad0000, 1);
            Actor_Run56();
            record = Actor_Run21(12);
            if (*(s32 *)((s32)record + 8) > *(volatile s32 *)(rec7 + 8)) {
                Actor_Place9(13, 0x5000, 20);
                Actor_Place10(13, 0x100, 20);
                Actor_Do6(0x1775);
                Actor_Run57(13, 0, 10);
                Actor_Place11(12, 0x100, 0);
            } else {
                Actor_Place12(12, 0x3000, 20);
                Actor_Place13(12, 0x100, 20);
                Actor_Do7(0x1775);
                Actor_Run58(12, 0, 10);
                Actor_Place14(13, 0x100, 0);
            }
            Actor_Place15(14, 0x100, 0);
            Actor_Place16(14, 0x3000, 0);
            Actor_Place17(12, 0x5000, 0);
            Actor_Place18(13, 0x3000, 0);
            Actor_Place19(0, 0x10c, 184);
            Actor_Place20(0, 0xc000, 40);
            Actor_Run59(13, 2);
            Actor_Run60(13, 0, 10);
            Actor_Run61(13, 0, 0);
            Actor_Place21(14, 0x3000, 20);
            Actor_Place22(12, 0x8000, 20);
            Actor_Run62(12, 3);
            Actor_Run6(14, 0x102);
            Actor_Run63(40);
            Actor_Place23(14, 0x3000, 10);
            Actor_Place24(12, 0x5000, 0);
            Actor_Place25(13, 0x3000, 10);
            Actor_Run64(14, 1);
            Actor_Run65(14, 0, 10);
            Actor_Run66(12, 3);
            Actor_Run67(13, 3);
            Actor_Run68(20);
            Actor_Run69(14, 0);
            Actor_Place26(14, 0x9999, 0x4ccc);
            *(u8 *)(Actor_Check26(14) + 90) &= 254;
            Actor_Place27(14, 0x10a, 172);
            Actor_Run70(1);
            *(u8 *)(Actor_Check27(14) + 90) |= 1;
            Actor_Run71(10);
            Actor_Run72(14, 3);
            Actor_Run73(14, 0, 10);
            Actor_Run7(0x177a, 1);
            bump_step(1);
            Actor_Run74(194, 3);
            Actor_Check8(194, 0);
            Actor_Run75(14, 3);
            Actor_Run76(0, 1);
            Actor_Place28(0, 0xc000, 0);
            Actor_Place29(14, 0x10000, 0x8000);
            *(u8 *)(Actor_Check28(14) + 90) &= 254;
            Actor_Place30(14, 0x106, 156);
            Actor_Run77(1);
            {
                u8 *record = Actor_Run22(14);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Actor_Run78(20);
            Actor_Run79(12, 2);
            Actor_Run80(12, 0, 10);
            Actor_Run81(12, 3);
            Actor_Run82(13, 3);
            Actor_Run83(14, 3);
            base5_20091c0 = (s32)gOv4;
            Actor_Place31(12, 0x10000, base5_20091c0);
            Actor_Place32(13, 0x10000, base5_20091c0);
            Actor_Place33(14, 0x10000, base5_20091c0);
            Actor_Do8(0x849);
            Actor_Run84();
        }
    }
}

void Scene_RunScene38b(void)
{
    extern u8 RuntimeSelectorTable[];

    s32 record;
    s16 sub_state;

    if (Actor_Check9(0x845) != 0) {
        ((void (*)())Actor_Check29)(9, 0, 0);
        Actor_Place34(14, 0x3000, 0);
        Actor_Place35(15, 0x5000, 0);
    } else {
        record = Actor_Check30(9);
        Actor_Run85(record, 0);
        Actor_Run86(21, 0, 0);
    }
    record = Actor_Check31(8);
    *(volatile s32 *)(record + 28) = 0x18000;
    {
        s32 off = 450;
        sub_state = *(s16 *)(RuntimeSelectorTable + off);
    }
    if (sub_state == 10) {
        Actor_Run87(8, 0, 0);
    } else {
        if (sub_state == 9) {
            Actor_Do9(0x12f);
        }
    }
    if (Actor_Check10(0x109) == 0) {
        {
            s32 off = 450;
            sub_state = *(s16 *)(RuntimeSelectorTable + off);
        }
        if (sub_state == 11) {
            Actor_Place36(20, 0xf80000, 0xd80000);
        }
    }
    Actor_Run88();
    if (Actor_Check11(0x84a) != 0) {
        if (Actor_Check12(0x84b) == 0) {
            Actor_Do10(0x304);
        }
    }
}

void Scene_RunScene38bSequenceA(void)
{
    u32 i;
    u8 *rec;
    s32 rec7;
    u8 *rec8;
    s32 record;
    s32 target;

    rec8 = Actor_Check13(10);
    rec = Actor_Check14(11);
    record = Actor_Check32(8);
    Actor_Run89(record, 0);
    rec7 = Actor_Check15(0x845);
    if (rec7 != 0) {
        Actor_Run90(9, 0, 0);
        Actor_Run91(10, 0, 0);
        Actor_Run92(11, 0, 0);
        Actor_SetRect(56, 15, 40, 15, 1, 2);
        Actor_SetRect2(26, 15, 1, 3, 10, 15);
        if (Actor_Check16(0x849) == 0) {
            if (Actor_Check17(0x848) != 0) {
                goto L_02000c92;
            }
            ((void (*)())Actor_Check33)(14, 0, 0);
        }
        Actor_Place37(12, 0xd000, 0);
        Actor_Place38(13, 0xb000, 0);
    } else {
        Actor_Run93(12, 0, 0);
        Actor_Run94(13, 0, 0);
        Actor_Run95(14, 0, 0);
        record = Actor_Check34(9);
        Actor_Run96(record, 0);
        record = Actor_Check35(10);
        Actor_Run97(record, 0);
        record = Actor_Check36(11);
        Actor_Run98(record, 0);
        target = (s32)rec8 + 85;
        *(u8 *)target = rec7;
        record = Actor_Check18(0x881);
        if (record != 0) {
            *(u8 *)(Actor_Check37(9) + 89) |= 16;
            *(u8 *)(Actor_Check38(16) + 89) |= 16;
            *(u8 *)(Actor_Check39(11) + 89) |= 16;
            Actor_Place39(16, 0x8e0000, 0x9c0000);
            record = Actor_Check40(16);
            Actor_Run99(record, 0);
            Actor_Place40(10, 0x8e0000, 0x9c0000);
            {
                s32 shown;

                target = *(volatile s32 *)((s32)rec8 + 80);
                shown = 0x4000;
                *(volatile u16 *)(target + 30) = shown;
            }
            *(volatile s32 *)((s32)rec8 + 12) += -0x80000;
            if (Actor_Check19(0x848) != 0) {
                Actor_Place41(11, 0x840000, 0xba0000);
                goto L_02000c92;
            }
            Actor_Place42(11, 0x580000, 0xc40000);
            Actor_Run100(11, 3);
            rec[89] |= 4;
        } else {
            *(volatile s32 *)((s32)rec8 + 12) = 0x200000;
            rec[85] = record;
            *(volatile s32 *)((s32)rec + 12) = 0x300000;
        }
    }
    L_02000c92:;
    Actor_Run101();
}

void Scene_RunScene38b(void)
{
    extern u16 RuntimeSelectorTable[];

    s32 arg0;
    s32 rec7;
    s32 record;

    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    Actor_Run102();
    record = ReadU16Elem(RuntimeSelectorTable, 225);
    if ((u32)((record - 3) << 16) <= 0x10000) {
        if (Actor_Check20(0x109) == 0) {
            rec7 = Actor_Check21(0);
            Actor_Run103();
            arg0 = *(s32 *)(rec7 + 8);
            *(s32 *)(rec7 + 12) = 0x100000;
            Actor_Run8(arg0, 0x100000, *(s32 *)(rec7 + 16), 0);
            Actor_Run104();
            Actor_Run105();
            Actor_Run106(1);
        }
    }
}

/* overlays/scene/actor/push_puzzle/scene_script.c */

extern u8 Value_0000001e;
extern u8 Value_00000020;
extern u8 Value_00000023;
extern u8 gWork[];
extern s16 RuntimeSelectorTable[];

s32 Scene_DispatchPuzzleEvent(void)
{
    *(s32 *)(*(u8 **)gWork + 0x1c0) = 0x100;
    if (RuntimeSelectorTable[224] == (s32)&Value_0000001e) {
        Actor_Run107();
    } else {
        if (RuntimeSelectorTable[224] == (s32)&Value_00000023) {
            Actor_Run108();
            Actor_Run9(0x2008ed9, 0xc80);
        } else {
            if (RuntimeSelectorTable[224] == (s32)&Value_00000020) {
                Actor_Run109();
            }
        }
    }
    return 0;
}

void Scene_UpdatePuzzleActors(void)
{
    s32 p10;
    s32 p9;
    s32 rec7;
    s32 record;
    s32 p6;
    s32 row;

    rec7 = Actor_Check22(0);
    record = Actor_Check23(20);
    row = *(s32 *)(record + 16) >> 20;
    p9 = (*(s32 *)(rec7 + 8) >> 20);
    p10 = (*(s32 *)(rec7 + 16) >> 20);
    p6 = *(s32 *)(record + 8);
    Actor_SetRect3(15, 11, 3, 1, 15, 12);
    Actor_SetRect4(15, 11, 3, 1, 15, 13);
    Actor_SetRect5(15, 11, 3, 1, 15, 14);
    Actor_SetRect6(1, 0, 1, 1, (p6 >> 20), row);
    if (((s32)p6 >> 20) == 16) {
        if (row == 13) {
            goto L_02000a60;
        }
    }
    Actor_SetRect7(0, 0, 1, 1, 16, 13);
    L_02000a60:;
    if (p9 == 16) {
        if (p10 == 13) {
            Actor_Run110();
            Actor_Place43(0, 0x100, 20);
            Actor_Place44(0, 0x20000, 0x10000);
            Actor_Run111(0, 6, 0);
            if (row == 13) {
                Actor_Place45(0, 0x106, 196);
                Actor_Place46(0, 0x4000, 20);
            } else {
                Actor_Place47(0, 0x11e, 218);
                Actor_Place48(0, 0x8000, 20);
            }
            Actor_Run112();
        }
    }
}

/* overlays/scene/actor/push_puzzle/scene_setup.c */
/* Per-site raw names: the first resolves to Scene_GetRecord and the four
 * renderer sites resolve to Actor_Run113. */
extern u8 *Actor_Run114();

void Scene_DrawTilesByActor8Row(void)
{
    u8 *actor;
    actor = Actor_Run114(8);
    if (actor == 0)
        return;

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 6)
            Actor_SetRect10(2, 0, 1, 1, 14, 6);
        else {
            s32 x = 14;
            s32 z = 6;
            Actor_SetRect11(0, 0, 1, 1, x, z);
        }
    }

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 9)
            Actor_SetRect12(2, 0, 1, 1, 14, 9);
        else {
            s32 x = 14;
            s32 z = 9;
            Actor_SetRect13(1, 0, 1, 1, x, z);
        }
    }
}

/* overlays/scene/actor/push_puzzle/scene_wrapper.c */

void Scene_CallHelper(void)
{
    Actor_Run115();
}

/* overlays/scene/actor/push_puzzle/state_update.c */

void State_SetValues9_3_0(void)
{
    Actor_Place49(9, 3, 0);
}
