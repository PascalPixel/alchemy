#include "types.h"
#include "scene.h"
#include "facing_object.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"

/* overlays/scene/actor/push_puzzle/push_puzzle.c */
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

u8 *Actor_unk10_4();

/*
 * Ten (x, z) tile pairs, held in the overlay's own writable image.  Overlay
 * data lives in EWRAM and is deliberately not const.
 */

/*
 * Slot accessor: Actor_unk10_4(slot) returns the actor record, or NULL.
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
    actor = Actor_unk10_4(8);
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
        Actor_unk8_5(1, 0, 1, 1, x, z);
    }

    /*
     * The same repaint with 0 rather than 1 in the first argument.  What
     * that selector chooses is not established.
     */
    Actor_unk9_5(0, 0, 1, 1, tx, tz);
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
 * resolve to Object_CreateFar, Object_SetCallback and Object_InitializeMode respectively. */
extern u8 *Actor_unk12_4();

void OvObj_SpawnKind24AtActor(u8 *actor)
{
    u8 *obj;
    u8 *child;
    s32 mask;

    obj = Actor_unk12_4(24,
                            *(s32 *)(actor + 0x08),
                            *(s32 *)(actor + 0x0c),
                            *(s32 *)(actor + 0x10));
    if (obj == 0)
        return;

    child = *(u8 **)(obj + 0x50);
    Actor_unk13_4(obj, gOv2);
    *(u8 *)(obj + 0x55) = 0;
    *(u8 *)(obj + 0x22) = 1;
    *(u8 *)(obj + 0x23) = 2;
    if (child == 0)
        return;

    Actor_unk14_4(child, 2);
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
    Actor_unk15_4();
    Actor_Apply(0x1472, 1);
    Actor_unk16_4();
}

void Scene_RunScriptedStep146E(void)
{
    Actor_unk15_4();
    Actor_Apply(0x146E, 1);
    Actor_unk16_4();
}

void Dialogue_RunLine1470(void)
{
    Actor_unk15_4();
    Actor_Apply(0x1470, 1);
    Actor_unk16_4();
}

void Scene_RunScene38b(void)
{
    Actor_unk17_4();
    Actor_Do(0x13ae);
    if (Actor_Check(0x301) != 0) {
        bump_step(1);
    }
    Actor_unk18_4(9, 0);
    Actor_unk2_2(0x301);
    Actor_unk19_4();
}

void Dialogue_RunActorTwelveDialogue(void)
{
    Actor_unk15_4();
    Actor_unk11_2(0x13B3);
    Actor_Apply2(12, 0);
    Actor_unk16_4();
}

void Dialogue_RunActorFourteenDialogue(void)
{
    Actor_unk15_4();
    Actor_unk11_2(0x13B7);
    Actor_Apply2(14, 0);
    Actor_unk16_4();
}

void Dialogue_ShowLine16BF(void)
{
    Actor_unk15_4();
    Actor_unk11_2(0x16BF);
    Actor_Apply2(21, 0);
    Actor_unk16_4();
}

void Dialogue_RunActorSixteenDialogue(void)
{
    Actor_unk15_4();
    Actor_unk11_2(0x16C8);
    Actor_Apply2(16, 0);
    Actor_unk16_4();
}

void Dialogue_ShowLine16CC(void)
{
    Actor_unk15_4();
    Actor_unk11_2(0x16CC);
    Actor_Apply2(18, 0);
    Actor_unk16_4();
}

/* overlays/scene/actor/push_puzzle/scene_primary_script.c */

extern u8 gWork[];
extern u8 gOv4[];

u8 *Actor_unk20_4();

u8 *Actor_unk21_4();

u8 *Actor_unk22_4();

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */

static __inline__ u16 ReadU16Elem(u16 *base, s32 idx)
{
    extern u16 RuntimeSelectorTable[];

    return *(u16 *)(base + idx);
}

void Scene_RunScriptedSteps947And29DD(void)
{
    Actor_unk23_4();
    Actor_Apply3(0x947, 1);
    Actor_Apply4(0x29DD, 1);
    Actor_unk24_4();
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
    Actor_unk25_4();
    for (i = 8; i < 66; i++) {
        record = (u8 *)Actor_unk2(i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    v5 = (s32)((s32)(*(volatile u16 *)(p7 + 0x16c) - 3) << 16) >> 16;
    if (v5 == 6) {
        Actor_unk26_4(188);
    } else {
        Actor_unk3_2(158);
    }
    off = v5 << 2;
    tbl = (u8 *)0x02009d0c;
    a1 = *(s16 *)(tbl + off);
    off2 = off + 2;
    a2 = *(s16 *)(tbl + off2);
    tbl2 = (u8 *)0x02009cf0;
    Actor_Place(*(s32 *)(tbl2 + off), a1, a2);
    Actor_unk2_3(0, 0x8000, 0x4000);
    *(u8 *)(Actor_unk24(0) + 85) = 0;
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    if (v5 == 6) {
        Actor_unk27_4(0, 2);
        Actor_unk3_3(0, 0, -4);
    } else {
        Actor_unk4_3(0, 3, -16);
    }
    if (v5 == 4) {
        Actor_unk28_4(0, 3);
    } else {
        Actor_unk29_4(0, 2);
    }
    Actor_unk30_4(16);
    Actor_unk31_4(v5 + 3);
    Actor_unk32_4();
}

void Scene_RunScene38bSequenceC(void)
{
    u32 i;
    s32 rec;
    u8 *rec7;
    u8 *record;

    rec = Actor_unk3(0);
    rec7 = Actor_unk4(11);
    if ((*(volatile s32 *)((s32)rec7 + 8) >> 20) != 6) {
    } else {
        Actor_unk33_4();
        Actor_unk34_4(11, 1);
        Actor_unk35_4(0, 2);
        Actor_unk36_4(20);
        Actor_unk5_3(0, 0x3333, 0x1999);
        Actor_unk6_3(11, 0x3333, 0x1999);
        *(u8 *)(Actor_unk25(0) + 90) &= 254;
        {
            u8 zero = 0;
            u8 *dst = rec7 + 85;

            *dst = zero;
        }
        *(volatile s32 *)(rec + 24) = -0x10000;
        Actor_Run(0, 0x102);
        Actor_unk37_4(0, 16);
        Actor_unk38_4(11, 111, 196);
        *(volatile s32 *)(rec + 24) = 0x10000;
        Actor_unk39_4(0, 128, 185);
        Actor_unk40_4(20);
        *(volatile s32 *)(rec + 24) = -0x10000;
        Actor_unk2_4(0, 0x102);
        Actor_unk41_4(0, 16);
        Actor_unk42_4(11, 121, 190);
        *(volatile s32 *)(rec + 24) = 0x10000;
        Actor_unk43_4(0, 141, 189);
        Actor_unk44_4(20);
        *(volatile s32 *)(rec + 24) = -0x10000;
        Actor_unk3_4(0, 0x102);
        Actor_unk45_4(0, 16);
        Actor_unk46_4(11, 132, 186);
        *(volatile s32 *)(rec + 24) = 0x10000;
        {
            u8 *record = Actor_unk20_4(0);
            u8 value = *(volatile u8 *)&record[90];

            record[90] = (u8)(value | 1);
        }
        Actor_unk7_3(0, 0x9999, 0x4ccc);
        Actor_unk47_4(0, 166, 185);
        Actor_unk8_3(0, 0x8000, 20);
        Actor_unk48_4(11, 2);
        Actor_unk49_4(0, 11);
        Actor_unk50_4(10);
        Actor_unk4_2(0x1774);
        Actor_unk51_4(11, 0);
        Actor_unk52_4();
        Actor_unk53_4(10);
        Actor_unk5_2(0x848);
        Actor_unk54_4();
    }
}

void Scene_RunScene38b(void)
{
    u32 i;
    s32 rec7;
    u8 *record;
    s32 base5_20091c0;

    rec7 = Actor_unk5(0);
    if (Actor_unk6(0x845) == 0) {
    } else {
        if (Actor_unk7(0x848) == 0) {
        } else {
            Actor_unk55_4();
            Actor_unk4_4(0x26666, 0x4ccc);
            Actor_unk5_4(0x1070000, -1, 0xad0000, 1);
            Actor_unk56_4();
            record = Actor_unk21_4(12);
            if (*(s32 *)((s32)record + 8) > *(volatile s32 *)(rec7 + 8)) {
                Actor_unk9_3(13, 0x5000, 20);
                Actor_unk10_3(13, 0x100, 20);
                Actor_unk6_2(0x1775);
                Actor_unk57_4(13, 0, 10);
                Actor_unk11_3(12, 0x100, 0);
            } else {
                Actor_unk12_3(12, 0x3000, 20);
                Actor_unk13_3(12, 0x100, 20);
                Actor_unk7_2(0x1775);
                Actor_unk58_4(12, 0, 10);
                Actor_unk14_3(13, 0x100, 0);
            }
            Actor_unk15_3(14, 0x100, 0);
            Actor_unk16_3(14, 0x3000, 0);
            Actor_unk17_3(12, 0x5000, 0);
            Actor_unk18_3(13, 0x3000, 0);
            Actor_unk19_3(0, 0x10c, 184);
            Actor_unk20_3(0, 0xc000, 40);
            Actor_unk59_4(13, 2);
            Actor_unk60_4(13, 0, 10);
            Actor_unk61_4(13, 0, 0);
            Actor_unk21_3(14, 0x3000, 20);
            Actor_unk22_3(12, 0x8000, 20);
            Actor_unk62_4(12, 3);
            Actor_unk6_4(14, 0x102);
            Actor_unk63_4(40);
            Actor_unk23_3(14, 0x3000, 10);
            Actor_unk24_3(12, 0x5000, 0);
            Actor_unk25_3(13, 0x3000, 10);
            Actor_unk64_4(14, 1);
            Actor_unk65_4(14, 0, 10);
            Actor_unk66_4(12, 3);
            Actor_unk67_4(13, 3);
            Actor_unk68_4(20);
            Actor_unk69_4(14, 0);
            Actor_unk26_3(14, 0x9999, 0x4ccc);
            *(u8 *)(Actor_unk26(14) + 90) &= 254;
            Actor_unk27_3(14, 0x10a, 172);
            Actor_unk70_4(1);
            *(u8 *)(Actor_unk27(14) + 90) |= 1;
            Actor_unk71_4(10);
            Actor_unk72_4(14, 3);
            Actor_unk73_4(14, 0, 10);
            Actor_unk7_4(0x177a, 1);
            bump_step(1);
            Actor_unk74_4(194, 3);
            Actor_unk8(194, 0);
            Actor_unk75_4(14, 3);
            Actor_unk76_4(0, 1);
            Actor_unk28_3(0, 0xc000, 0);
            Actor_unk29_3(14, 0x10000, 0x8000);
            *(u8 *)(Actor_unk28(14) + 90) &= 254;
            Actor_unk30_3(14, 0x106, 156);
            Actor_unk77_4(1);
            {
                u8 *record = Actor_unk22_4(14);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Actor_unk78_4(20);
            Actor_unk79_4(12, 2);
            Actor_unk80_4(12, 0, 10);
            Actor_unk81_4(12, 3);
            Actor_unk82_4(13, 3);
            Actor_unk83_4(14, 3);
            base5_20091c0 = (s32)gOv4;
            Actor_unk31_3(12, 0x10000, base5_20091c0);
            Actor_unk32_3(13, 0x10000, base5_20091c0);
            Actor_unk33_3(14, 0x10000, base5_20091c0);
            Actor_unk8_2(0x849);
            Actor_unk84_4();
        }
    }
}

void Scene_RunScene38b(void)
{
    extern u8 RuntimeSelectorTable[];

    s32 record;
    s16 sub_state;

    if (Actor_unk9(0x845) != 0) {
        ((void (*)())Actor_unk29)(9, 0, 0);
        Actor_unk34_3(14, 0x3000, 0);
        Actor_unk35_3(15, 0x5000, 0);
    } else {
        record = Actor_unk30(9);
        Actor_unk85_4(record, 0);
        Actor_unk86_4(21, 0, 0);
    }
    record = Actor_unk31(8);
    *(volatile s32 *)(record + 28) = 0x18000;
    {
        s32 off = 450;
        sub_state = *(s16 *)(RuntimeSelectorTable + off);
    }
    if (sub_state == 10) {
        Actor_unk87_4(8, 0, 0);
    } else {
        if (sub_state == 9) {
            Actor_unk9_2(0x12f);
        }
    }
    if (Actor_unk10(0x109) == 0) {
        {
            s32 off = 450;
            sub_state = *(s16 *)(RuntimeSelectorTable + off);
        }
        if (sub_state == 11) {
            Actor_unk36_3(20, 0xf80000, 0xd80000);
        }
    }
    Actor_unk88_4();
    if (Actor_unk11(0x84a) != 0) {
        if (Actor_unk12(0x84b) == 0) {
            Actor_unk10_2(0x304);
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

    rec8 = Actor_unk13(10);
    rec = Actor_unk14(11);
    record = Actor_unk32(8);
    Actor_unk89_4(record, 0);
    rec7 = Actor_unk15(0x845);
    if (rec7 != 0) {
        Actor_unk90_4(9, 0, 0);
        Actor_unk91_4(10, 0, 0);
        Actor_unk92_4(11, 0, 0);
        Actor_SetRect(56, 15, 40, 15, 1, 2);
        Actor_unk2_5(26, 15, 1, 3, 10, 15);
        if (Actor_unk16(0x849) == 0) {
            if (Actor_unk17(0x848) != 0) {
                goto L_02000c92;
            }
            ((void (*)())Actor_unk33)(14, 0, 0);
        }
        Actor_unk37_3(12, 0xd000, 0);
        Actor_unk38_3(13, 0xb000, 0);
    } else {
        Actor_unk93_4(12, 0, 0);
        Actor_unk94_4(13, 0, 0);
        Actor_unk95_4(14, 0, 0);
        record = Actor_unk34(9);
        Actor_unk96_4(record, 0);
        record = Actor_unk35(10);
        Actor_unk97_4(record, 0);
        record = Actor_unk36(11);
        Actor_unk98_4(record, 0);
        target = (s32)rec8 + 85;
        *(u8 *)target = rec7;
        record = Actor_unk18(0x881);
        if (record != 0) {
            *(u8 *)(Actor_unk37(9) + 89) |= 16;
            *(u8 *)(Actor_unk38(16) + 89) |= 16;
            *(u8 *)(Actor_unk39(11) + 89) |= 16;
            Actor_unk39_3(16, 0x8e0000, 0x9c0000);
            record = Actor_unk40(16);
            Actor_unk99_4(record, 0);
            Actor_unk40_3(10, 0x8e0000, 0x9c0000);
            {
                s32 shown;

                target = *(volatile s32 *)((s32)rec8 + 80);
                shown = 0x4000;
                *(volatile u16 *)(target + 30) = shown;
            }
            *(volatile s32 *)((s32)rec8 + 12) += -0x80000;
            if (Actor_unk19(0x848) != 0) {
                Actor_unk41_3(11, 0x840000, 0xba0000);
                goto L_02000c92;
            }
            Actor_unk42_3(11, 0x580000, 0xc40000);
            Actor_unk100_4(11, 3);
            rec[89] |= 4;
        } else {
            *(volatile s32 *)((s32)rec8 + 12) = 0x200000;
            rec[85] = record;
            *(volatile s32 *)((s32)rec + 12) = 0x300000;
        }
    }
    L_02000c92:;
    Actor_unk101_4();
}

void Scene_RunScene38b(void)
{
    extern u16 RuntimeSelectorTable[];

    s32 arg0;
    s32 rec7;
    s32 record;

    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x204;
    Actor_unk102_4();
    record = ReadU16Elem(RuntimeSelectorTable, 225);
    if ((u32)((record - 3) << 16) <= 0x10000) {
        if (Actor_unk20(0x109) == 0) {
            rec7 = Actor_unk21(0);
            Actor_unk103_4();
            arg0 = *(s32 *)(rec7 + 8);
            *(s32 *)(rec7 + 12) = 0x100000;
            Actor_unk8_4(arg0, 0x100000, *(s32 *)(rec7 + 16), 0);
            Actor_unk104_4();
            Actor_unk105_4();
            Actor_unk106_4(1);
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
        Actor_unk107_4();
    } else {
        if (RuntimeSelectorTable[224] == (s32)&Value_00000023) {
            Actor_unk108_4();
            Actor_unk9_4(0x2008ed9, 0xc80);
        } else {
            if (RuntimeSelectorTable[224] == (s32)&Value_00000020) {
                Actor_unk109_4();
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

    rec7 = Actor_unk22(0);
    record = Actor_unk23(20);
    row = *(s32 *)(record + 16) >> 20;
    p9 = (*(s32 *)(rec7 + 8) >> 20);
    p10 = (*(s32 *)(rec7 + 16) >> 20);
    p6 = *(s32 *)(record + 8);
    Actor_unk3_5(15, 11, 3, 1, 15, 12);
    Actor_unk4_5(15, 11, 3, 1, 15, 13);
    Actor_unk5_5(15, 11, 3, 1, 15, 14);
    Actor_unk6_5(1, 0, 1, 1, (p6 >> 20), row);
    if (((s32)p6 >> 20) == 16) {
        if (row == 13) {
            goto L_02000a60;
        }
    }
    Actor_unk7_5(0, 0, 1, 1, 16, 13);
    L_02000a60:;
    if (p9 == 16) {
        if (p10 == 13) {
            Actor_unk110_4();
            Actor_unk43_3(0, 0x100, 20);
            Actor_unk44_3(0, 0x20000, 0x10000);
            Actor_unk111_4(0, 6, 0);
            if (row == 13) {
                Actor_unk45_3(0, 0x106, 196);
                Actor_unk46_3(0, 0x4000, 20);
            } else {
                Actor_unk47_3(0, 0x11e, 218);
                Actor_unk48_3(0, 0x8000, 20);
            }
            Actor_unk112_4();
        }
    }
}

/* overlays/scene/actor/push_puzzle/scene_setup.c */
/* Per-site raw names: the first resolves to Scene_GetRecord and the four
 * renderer sites resolve to Actor_unk113_4. */
extern u8 *Actor_unk114_4();

void Scene_DrawTilesByActor8Row(void)
{
    u8 *actor;
    actor = Actor_unk114_4(8);
    if (actor == 0)
        return;

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 6)
            Actor_unk10_5(2, 0, 1, 1, 14, 6);
        else {
            s32 x = 14;
            s32 z = 6;
            Actor_unk11_5(0, 0, 1, 1, x, z);
        }
    }

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 9)
            Actor_unk12_5(2, 0, 1, 1, 14, 9);
        else {
            s32 x = 14;
            s32 z = 9;
            Actor_unk13_5(1, 0, 1, 1, x, z);
        }
    }
}

/* overlays/scene/actor/push_puzzle/scene_wrapper.c */

void Scene_CallHelper(void)
{
    Actor_unk115_4();
}

/* overlays/scene/actor/push_puzzle/state_update.c */

void State_SetValues9_3_0(void)
{
    Actor_unk49_3(9, 3, 0);
}

/* overlays/scene/actor/push_puzzle/select_data_by_runtime_selector.c */
#define PrimaryRuntimeSelector Value_0000001e
#define SecondaryRuntimeSelector Value_00000023
#define TertiaryRuntimeSelector Value_00000020

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

/* overlays/scene/actor/push_puzzle/select_data_by_runtime_selector_b.c */
#define PrimaryRuntimeSelector Value_0000001e
#define SecondaryRuntimeSelector Value_00000023
#define TertiaryRuntimeSelector Value_00000020

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectDataByRuntimeSelectorB(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
