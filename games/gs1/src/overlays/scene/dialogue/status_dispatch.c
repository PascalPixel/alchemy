#include "types.h"
#include "scene.h"

#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define SCENE_FIELD_1C8 (*(s32 *)(*(u8 **)0x03001ebc + 0x1c8))
#define RECORD_17 17
#define SCENE_WORK (*(u8 **)0x03001ebc)
#define SCENE_PHASE (*(s32 *)(SCENE_WORK + 0x1c0))
#define SCENE_FIELD_1C8 (*(s32 *)(SCENE_WORK + 0x1c8))
#define SCENE_STEP (*(u16 *)(SCENE_WORK + 0x1d8))

extern u8 Value_0000008c;
extern u8 Value_0000008e;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];   /* image offset 0x32bc */
extern u8 Value_0000008d;
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 Value_00002125[];

void gOv11();

/* resource_3b9:0x02001298, 884 bytes: the middle presentation script for
 * actor 16, a sibling of Scene_RunMiddleSequence (0x02001688) with the
 * same opening, scene-phase writes and closing, without the stepping loop. */

/*
 * resource_3b9 owner at 0x02001c48, 12 bytes plus one alignment halfword: a
 * two-call trampoline forwarding the argument to the first callee with a
 * fixed second argument, then making a second call with a fixed constant.
 * The argument is never freshly loaded before the first call, so it is a
 * pass-through parameter.  Neither callee is identified beyond call shape.
 */

/* Scene_BuildActorPresentationSequence - resource_3b9:02001cd4, 1804 bytes.
 *
 * One straight-line field-scene presentation script for actor records 0-3 and
 * 8-11. It resets the battle runtime, places the three lead actors, raises the
 * scene phase word, then walks a long ordered list of spawn, motion, callback
 * and wait calls. Two thirds of the way through it asks the UI work helper
 * whether the player already finished the pending step: the answer selects a
 * short path (one paired call) or a long one (an animation-change wait, a
 * masked-halfword forward and the same paired call), and both paths bump the
 * scene step counter exactly once. Near the end it arms the action callbacks
 * of actors 1, 2 and 3 with one shared 0x0200adac word, then walks actors 11
 * and 0 to their closing positions. It ends by lowering the scene phase word
 * back to 256, storing 40 at field 0x1c8 and running the three closing
 * helpers.
 *
 * Uncertain: the argument roles of the unnamed veneers below, whether the
 * 0x0200adac word passed to the three callback helpers is a function pointer
 * or a work record, and whether the second scene-step bump is a separate
 * source statement or a shared tail the compiler duplicated.
 *
 * Residual: candidate=1804 reference=1804 differing_halfwords=11,
 * topology=equal, wrong_instructions=0, class=scheduling-floor. The candidate
 * emits exactly the reference's instructions; two short runs sit in a
 * different order.
 *
 *   - at owner offset 0x42c the two constant arguments of the Talk_unk23
 *     call are set r0 before r1 where the reference sets r1 before r0;
 *   - the pc-relative load of 0x0200adac that the three action-callback calls
 *     share lands at owner offset 0x5f8, before the third
 *     Motion_SetSpeed call; the reference issues it at 0x604,
 *     after that call and before its first use, which also
 *     swaps the r0/r1 setup of the first of those three calls: the reference
 *     schedule fills the load-use gap with `movs r0, #1`, ours has no gap to
 *     fill.
 *
 * Neither survived respelling. Wrapping or prototyping the Talk_unk23 site
 * leaves it unchanged. Spelling the first action-callback site through the
 * constant-passing wrapper below, or through a shared local, moves the load
 * but does not place it after the call: the wrapper form measures 8 differing
 * halfwords, and is not used here because it buys that number by emitting a
 * second `ldr r1, [pc]` the reference does not contain and dropping an
 * `adds r1, r5, #0` it does (wrong_instructions=2, class=missing-extra-code).
 */

/* Shared scene work record, reached through the pointer at 0x03001ebc. The
 * three field offsets are the ones the adopted sibling
 * games/gs1/src/overlays/scene/script/run_scene_3b9_conditional_scene_setup.c
 * already spells. */

/* Overlay call bindings.
 *
 * The loader rewrites every BL-shaped halfword pair, so the word an overlay
 * image stores for a call site depends on the site as well as on the callee.
 * The legacy Func_<address> alias a disassembler prints is therefore per site,
 * and the 180 call sites in this owner reach only 31 distinct runtime targets.
 * Each declaration below is one runtime target, spelled with an alias that the
 * reference binds to that target and to no other; its comment gives the
 * overlay veneer the call enters and the main-image entry that veneer reaches.
 *
 * The alias a site produces folds in that site's own position: it is
 * RESOURCE_BASE + (owner offset + site + 4) + the stored displacement, while
 * the runtime target the loader reaches is RUNTIME_BASE + that displacement
 * + 2. Two sites therefore share one alias whenever they are as far apart as
 * their two targets. That is the case for the veneer at 0x0200ad0c: it is
 * reached only from 0x02001d28 and 0x02001d64, and each of those sits exactly
 * eight bytes before a site that calls the neighbouring veneer at 0x0200ad04,
 * so both aliases the reference offers for it - Talk_unk14_4 and
 * Talk_unk15_4 - name two targets and neither can bind. Every other target
 * in this owner has at least one alias of its own, and is declared under it
 * exactly once.
 *
 * The one unbindable target is spelled gOv11: a name carrying the
 * runtime address directly, which is the only spelling that reaches an
 * overlay-space address without going through the per-site alias table
 * (a Func_02xxxxxx name is always resolved as a legacy alias). The emitted
 * call word matches the reference at both sites. If this owner is ever given
 * a translation-unit entry, that entry should declare the runtime symbol
 * instead and the name can go back to the ordinary Func_ form.
 *
 * Names come from the established spelling of the same main-image entry in
 * already adopted overlay sources. Unnamed veneers keep their alias.
 */

/* 0x0200abf4 -> 0x0808a018 */

/* 0x0200abec -> 0x0808a010 */

/* 0x0200ac04 -> 0x0808a070 */

/* 0x0200ac54 -> 0x0808a0f0 */

/* 0x0200ac14 -> 0x0808a090 */

/* 0x0200ac1c -> 0x0808a098. Two sites reach this veneer, at 0x020022de and
 * 0x020022e6; each keeps its own unambiguous alias. */

/* 0x0200ac2c -> 0x0808a0b0 */

/* 0x0200ac34 -> 0x0808a0b8 */

/* 0x0200ac3c -> 0x0808a0c0 */

/* 0x0200ac44 -> 0x0808a0c8 */

/* 0x0200ac4c -> 0x0808a0d0 */

/* 0x0200ac5c -> 0x0808a100 */

/* 0x0200ac6c -> 0x0808a110 */

/* 0x0200ac74 -> 0x0808a130 */

/* 0x0200ac7c -> 0x0808a138 */

/* 0x0200ac94 -> 0x0808a170 */

/* 0x0200acbc -> 0x0808a1b8 */

/* 0x0200acc4 -> 0x0808a1e8 */

/* 0x0200acdc -> 0x0808a208 */

/* 0x0200ace4 -> 0x0808a210 */

/* 0x02009c48, the owner registered as Scene_CallPairWith10. Its readable
 * name is bound to Scene_CallPairWith10, which is not one of this owner's call words,
 * so the call keeps its alias. Takes one record id. */

/* 0x02009c5c, the owner registered as State_ForwardMaskedHalfwordWith10;
 * same reason. Takes a record id and the masked halfword. */

/* Unnamed veneers: no adopted source spells these main-image entries, so the
 * alias stands and the comment records the entry and the observed arity. */

/* 0x0200ab8c -> 0x080000c0, (arg0) */

/* 0x0200ac9c -> 0x0808a178, (id, arg1) */

/* 0x0200acfc -> 0x0808a248, (arg0) */

/* 0x0200ad04 -> 0x0808a330, (arg0, arg1) */

/* 0x0200ad0c -> 0x0808a338, (arg0, arg1). Named by runtime address rather
 * than by a legacy alias; see the binding note above. */

/* 0x0200ad14 -> 0x0808a348, (arg0) */

/* 0x0200ad1c -> 0x0808a360, () */

/* 0x0200ad24 -> 0x0808a368, () */

/* 0x0200ad2c -> 0x0808a370, () */

/* Integration note. This owner has no translation-unit entry, so it scores
 * standalone with `alchemy score <this file> --owner resource_3b9:02001cd4`.
 * The only symbol that cannot come from the reference alias table is
 * gOv11; if an entry is ever added, declare
 * {"address": "0x0200ad0c", "kind": "thumb"} for it and the call can be
 * spelled with an ordinary Func_ name again. Nothing else in this file
 * depends on that entry existing.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* A second word at 0x1c8 of the shared scene work record, set only on the
 * flag-clear path below. */

/* Record id passed to every status call below. */

static __inline__ __attribute__((always_inline)) void bump_step(s32 amount)
{
    extern u8 gWork[];

    u8 *scene = *(u8 **)gWork;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}

/* A call site spelled through one of these wrappers passes its constants
 * straight into the argument registers; a direct call lets the compiler
 * precompute a costly constant into a pseudo and share it with later uses in
 * the block. Most sites here want the first shape; the three action-callback
 * sites near the end want the second, because the reference reaches all three
 * of them through one shared pointer register. The adopted sibling
 * run_scene_3b9_conditional_scene_setup.c uses the same device for the same
 * reason. */

#if defined(GS1_EDITION_JA)
#define SCENE_STEP_VALUE 0x22c1
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define SCENE_STEP_VALUE 0x211a
#else
#define SCENE_STEP_VALUE 0x2138
#endif


/*
 * resource_3b9 owner at 0x02000074: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that one pool word at
 * 0x02000078, holding the address 0x0200b2bc -- image offset 0x32bc --
 * which is returned without being dereferenced.
 */
s32 SceneData_SelectTableBySceneId(void)
{
    extern s16 gCell[];

    s16 v = gCell[224];

    if (v == (s32)&Value_0000008c) {
        return (s32)gOv;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)gOv2;
    }
    return (s32)gOv3;
}

s32 get_secondary_default_result(void)
{
    return 0;
}

u8 *SceneData_GetTableB2bc(void)
{
    extern s16 gCell[];

    return gOv4;
}

s32 SceneData_SelectDataBySelectorAndSubstate(void)
{
    extern s16 gCell[];

    s16 v = gCell[224];

    if (v == (s32)&Value_0000008d) {
        return (s32)gOv5;
    }
    if (v == (s32)&Value_0000008c) {
        if (gCell[225] == 12) {
            return (s32)gOv6;
        }
        return (s32)gOv7;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)gOv8;
    }
    return (s32)gOv9;
}

/* Contiguous unnamed leaf-owner run for resource_3b9. */
void Dialogue_RunActor10MessageByFlag962(void)
{
    Talk_unk17_4();
    if (Talk_unk24(0x962)) {
        Talk_unk17_2(0x2251);
        Talk_Apply(10, 0);
    } else {
        Talk_unk18_2(0x2057);
        Talk_Apply2(10, 0);
    }
    Talk_unk18_4();
}

void Dialogue_RunActor13MessageByFlag962(void)
{
    Talk_unk19_4();
    if (Talk_unk25(0x962)) {
        Talk_unk91_2(13, 258, 40);
        Talk_unk19_2(0x2254);
        Talk_Apply3(13, 0);
    } else {
        Talk_unk20_2(0x205c);
        Talk_Apply4(13, 0);
    }
    Talk_unk20_4();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk21_4();
    if (Talk_Check(0x962) != 0) {
        Talk_unk22_4(14, 2);
        Talk_Do(0x2256);
        Talk_unk23_4(14);
        Talk_unk24_4(14, 0, 0);
        Talk_unk25_4(20);
        Talk_unk2(14, 0);
        Talk_unk26_4(14, 0);
    } else {
        Talk_unk2_2(0x205d);
        Talk_unk27_4(14, 0);
    }
    Talk_unk28_4();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk29_4();
    if (Talk_unk3(0x962) != 0) {
        if (Talk_unk4(0x3c0) != 0) {
            Talk_unk3_2(0x225e);
        } else {
            Talk_unk4_2(0x225a);
            Talk_unk5(16, 0);
            if (Talk_unk6(0, 0) == 0) {
                bump_step(1);
                Talk_Place(16, 0x100, 40);
                Talk_unk7(16, 0);
                if (Talk_unk8(0, 0) == 0) {
                    bump_step(1);
                }
                ((void (*)())Talk_unk26)(40);
                Talk_unk30_4(16, 0);
                Talk_unk5_2(0x3c0);
                goto L_02000448;
            }
        }
        Talk_unk31_4(16, 0);
    } else {
        Talk_unk6_2(0x205e);
        Talk_unk32_4(16, 0);
    }
    L_02000448:;
    Talk_unk33_4();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Talk_unk9(13);
    Talk_unk34_4();
    Talk_unk35_4(13);
    Talk_unk36_4(13, 0, 20);
    Talk_unk7_2(0x2114);
    Talk_unk37_4(13);
    Talk_unk38_4(13, 1);
    Talk_unk39_4(13, 0);
    {
        u16 *target = (u16 *)(rec7 + 100);
        s32 shown = 0x2d0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)(rec7 + 102);
        s32 shown = 112;

        *target = shown;
    }
    Talk_unk40_4(13, 2);
    Talk_unk41_4();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk42_4();
    Talk_Run(14, 0x102);
    Talk_unk43_4(14, 2);
    Talk_unk8_2(0x2116);
    Talk_unk44_4(14);
    Talk_unk2_3(14, 0x102, 40);
    Talk_unk45_4(14, 0);
    Talk_unk46_4();
}

void Dialogue_ShowLine2118WithActor15Steps(void)
{
    Talk_unk47_4();
    Talk_unk48_4(0x2118);
    Talk_unk49_4(15);
    Talk_unk50_4(15, 0, 20);
    Talk_unk51_4(15);
    Talk_unk52_4(15, 3);
    Talk_unk53_4(15, 0);
    Talk_unk54_4(15);
    Talk_unk55_4(15, 20480);
    Talk_unk56_4();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk57_4();
    Talk_unk58_4(16, 2);
    Talk_unk9_2(0x211b);
    Talk_unk59_4(16, 0, 20);
    if (Talk_unk10(0x3c1) != 0) {
        Talk_unk60_4(20);
    } else {
        Talk_unk61_4(17, 0);
        Talk_unk62_4(17, 1);
        Talk_unk63_4(17);
        Talk_unk64_4(17, 0, 20);
        Talk_unk65_4(17, 4);
        Talk_unk66_4(17);
        Talk_unk3_3(17, 0x105, 40);
        Talk_unk67_4(17);
        Talk_unk2_4(17, 0x5000);
        Talk_unk10_2(0x3c1);
    }
    Talk_unk68_4();
}

void Scene_RunActorSeventeenDialogueSteps(void)
{
    extern u8 gWork[];

    Talk_unk69_4();
    Talk_unk70_4(17, 0, 20);
    Talk_unk71_4(0x211f);
    Talk_unk72_4(17);
    Talk_unk73_4(0, 3);
    Talk_unk74_4(17, 3);
    Talk_unk75_4(17);
    Talk_unk76_4(17, 1);
    Talk_unk77_3(17);
    Talk_unk78_3(17, 20480);
    Talk_unk79_3();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk80_3();
    Talk_unk81_3(18, 0, 20);
    Talk_unk11_2(0x2122);
    Talk_unk82_3(18);
    Talk_unk4_3(18, 0xd000, 20);
    Talk_unk5_3(18, 0xb000, 20);
    Talk_unk6_3(18, 0x8000, 40);
    Talk_unk83_3(18, 0, 20);
    Talk_unk84_3(18);
    Talk_unk85_3(18, 3);
    Talk_unk86_3(18);
    Talk_unk11(18, 0x5000);
    Talk_unk87_3();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    Talk_unk88_3();
    Talk_unk27(8, 0, 20);
    Talk_unk89_3((s32)&Value_00002125);
    Talk_unk28(8, 0);
    if (Talk_unk29(0, 0) != 0)
        bump_step(1);
    Talk_unk90_3(8, 0);
    Talk_unk91_3();
}

/*
 * Story-flag hand-off in overlay resource_3b9: clear one flag, set its
 * immediate neighbour.  It is installed into a script record rather than
 * called, and each Func_ name spells a relocated call word.
 */

/*
 * resource_3b9 owner at 0x02001c5c, 12 bytes with no literal pool: a
 * single-call trampoline forwarding `arg0' unchanged, `arg1' masked to its
 * low 16 bits, and a fixed third argument.  `arg0' is never freshly loaded
 * before the call, so it is a pass-through parameter.  The callee is not
 * identified beyond call shape.
 */

/*
 * The two flag ids are adjacent but spelled differently, and the spellings
 * are load-bearing.  565 is 0x235 and comes from the owner's single pool
 * word; 564 is built as movs #141 / lsls #2, so it must stay a plain decimal
 * value rather than another pool constant.  The 24-byte owner covers that
 * pool word and the alignment halfword after it.  What the pair gates is not
 * established.
 */
void State_ApplyFlags565And564(void)
{
    extern u8 gCell[];
    extern u8 *gWork;

    Talk_unk92_3(0x235);
    Talk_unk93_3(564);
}

/* Reads flag record 0x8a4; when set, runs one short setup on record 17.
 * When clear, runs a longer setup on record 17 plus scene phase/field
 * updates, then checks flag record 0x8a3 to pick a final call. Either path
 * ends with Battle_SchedShoulder_1(). */
void Scene_RunConditionalSceneSetup(void)
{
    extern u8 gWork[];

    u32 i;
    s32 flag_8a4;
    s32 record;

    Battle_Reset_1();
    flag_8a4 = GameFlag_IsSet_1(0x8a4);
    if (flag_8a4 != 0) {
        Object_LinkPair_1(RECORD_17, 0, 40);
        SceneWork_SetStepValue_1(0x206f);
        Talk_unk94_3(RECORD_17);
        Motion_ArmCb_1(RECORD_17, 0x3000, 20);
    } else {
        Motion_SetVarCb_1(RECORD_17, 2);
        SceneWork_SetStepValue_2(0x206d);
        BattleEv_RunWait_1(RECORD_17, 0);
        /* Byte at +85 of the record returned by RuntimeBlock_GetOffset1e0Pointer_1(); written
         * with the (already known zero) flag value here. */
        *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_2() + 85) = flag_8a4;
        Talk_unk95_3(1);
        Motion_SetSpeedLim_1(0x66666, 0xcccc);
        Motion_CamBounds_1(0x21c0000, -1, 0xd00000, 1);
        Object_CommitPositionThenWaitIfModeZero_1();
        SCENE_PHASE = 0x200;
        SCENE_FIELD_1C8 = 32;
        Talk_unk96_3();
        Motion_SetSpeed_1();
        if (GameFlag_IsSet_2(0x8a3) != 0) {
            Talk_unk97_3(70);
        } else {
            Talk_unk98_3(7);
        }
    }
    Battle_SchedShoulder_1();
}

void Scene_BuildActorPresentationGroup(void)
{
    extern u8 gWork[];

    s32 rec2;
    u8 *work;
    s32 shift;
    s32 phase;
    s32 field;

    Talk_unk99_3();
    Talk_unk3_4(-1, -1, -1, 0);
    Talk_unk100_3(247);
    Talk_unk101_3(8, 2);
    Talk_unk102_3(9, 2);
    Talk_unk103_3(10, 2);
    Talk_unk104_3(11, 2);
    Talk_unk105_3(12, 2);
    Talk_unk106_3(13, 2);
    Talk_unk107_3(14, 0);
    Talk_unk108_3(15, 0);
    Talk_unk109_3(16, 0);
    Talk_unk110_3(17, 0);
    Talk_unk111_3(18, 0);
    rec2 = Talk_unk30(21);
    Talk_unk112_3(rec2, 0);
    rec2 = Talk_unk31(19);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    rec2 = Talk_unk12(20);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    Talk_unk113_3(1);
    Talk_unk114_3(0, 0, 0);
    Talk_unk115_3(1);
    work = *(u8 *volatile *)gWork;
    phase = 0x1c0;
    *(volatile s32 *)(work + phase) = 0x200;
    field = 0x1c8;
    *(volatile s32 *)(work + field) = 32;
    Talk_unk116_3();
    Talk_unk117_3();
    Talk_unk118_3(40);
    Talk_unk119_3(16, 1);
    Talk_unk120_3(20);
    Talk_unk7_3(16, 0xcccc, 0x6666);
    Talk_unk121_3(16, 164, 0x388);
    Talk_unk122_3(20);
    Talk_unk123_3(16, 9);
    Talk_unk124_3(40);
    Talk_unk125_3(16, 10);
    Talk_unk126_3(60);
    Talk_unk127_3(16, 1);
    Talk_unk128_3(20);
    Talk_unk8_3(16, 164, 0x398);
    Talk_unk9_3(16, 185, 0x398);
    Talk_unk10_3(16, 0xc000, 20);
    Talk_unk11_3(16, 185, 0x394);
    Talk_unk129_3(16, 11);
    Talk_unk130_3(40);
    Talk_unk131_3(16, 1);
    Talk_unk132_3(60);
    Talk_unk133_3(16, 3);
    Talk_unk134_3(40);
    Talk_unk135_3(16, 0x200af88);
    Talk_unk136_3(80);
    Talk_unk4_4(16, 0x102);
    Talk_unk137_3(60);
    Talk_unk12_3(14, 0xd000, 0);
    Talk_unk13_3(15, 0x5000, 0);
    Talk_unk138_3(17, 0, 0);
    Talk_unk14_3(18, 0x8000, 20);
    Talk_unk5_4(14, 0x102);
    Talk_unk6_4(15, 0x102);
    Talk_unk7_4(17, 0x102);
    Talk_unk8_4(18, 0x102);
    Talk_unk139_3(60);
    Talk_unk15_3(14, 0x3000, 0);
    Talk_unk16_3(15, 0x3000, 0);
    Talk_unk17_3(17, 0x3000, 0);
    shift = 0x3000;
    Talk_unk13(18, shift);
    Talk_unk140_3(16);
    rec2 = Talk_unk32(16);
    {
        s32 shown = 0xd000;
        *(volatile u16 *)(rec2 + 6) = shown;
    }
    *(volatile s32 *)(rec2 + 24) = 0x10000;
    *(volatile s32 *)(rec2 + 28) = 0x10000;
    Talk_unk141_3(20);
    Talk_unk142_3(16, 0);
    Talk_unk143_3(40);
    Talk_unk144_3(19, 5);
    Talk_unk145_3(20, 5);
    Talk_unk146_3(60);
    Talk_unk147_3(16, shift, 20);
    Talk_unk148_3(16, 8);
    Talk_unk149_3(20);
    Talk_unk150_3(14, 4);
    Talk_unk151_2(15, 4);
    Talk_unk152_2(17, 4);
    Talk_unk153_2(18, 4);
    Talk_unk154_2(40);
    Talk_unk155_2(16, 4);
    Talk_unk156_2(10);
    Talk_unk18_3(16, 0x20000, 0x10000);
    Talk_unk19_3(16, 162, 0x394);
    Talk_unk157_2(16, 162, 0x37a);
    Talk_unk158_2(19, 1);
    Talk_unk159_2(20, 1);
    Talk_unk20_3(16, 184, 0x35f);
    Talk_unk21_3(16, 184, 0x31c);
    Talk_unk160_2(16, 0, 0);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(work + phase) = 0x201;
    *(volatile s32 *)(work + field) = 16;
    Talk_unk161_2();
    Talk_unk162_2();
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(work + phase) = 0x100;
    Talk_unk163_2(69);
    Talk_unk164_2();
}

void State_SetRuntimeWord448To513(void)
{
    extern u8 gCell[];
    extern u8 *gWork;

    Talk_unk165_2();
    *(s32 *)((u8 *)gWork + 448) = 513;

    Talk_unk166_2();
    Talk_unk167_2();

    Talk_unk168_2(20);
    Talk_unk169_2(17, 160 << 7);
    Talk_unk170_2(0x206e);

    if (Talk_unk33(0x8a4) != 0) {
        *(u16 *)((u8 *)gWork + 472) =
            (u16)(*(u16 *)((u8 *)gWork + 472) + 1);
    }

    Talk_unk171_2(17);
    Talk_unk172_2(17, 192 << 6);
    Talk_unk173_2(0x8a3);

    Talk_unk174_2();
}

/* Sets up actors 8-20 (position, pose, or movement/sprite flags), advances
 * two actor records' +24 fields, advances the shared scene phase, then runs
 * a long chain of per-actor moves, pose changes, and waits. */
void Scene_RunMiddleSequence(void)
{
    extern u8 gWork[];

    u32 counter;
    u8 *rec;
    s32 rec2;
    u8 *work;

    BattleRuntime_Reset_1();
    ObjectMotion_PlaceWithinCameraBounds_1(-1, -1, -1, 0);
    Audio_PlayCue_1(247);
    Object_SetModeById_1(8, 2);
    Talk_unk9_4(9, 2);
    Object_SetModeById_2(10, 2);
    Object_SetModeById_3(11, 2);
    Object_SetModeById_4(12, 2);
    Object_SetModeById_5(13, 2);
    Object_SetModeById_6(14, 0);
    Object_SetModeById_7(15, 0);
    Motion_SetHPosTerrain_1(16, 0, 0);
    Object_SetModeById_8(17, 0);
    Object_SetModeById_9(18, 0);
    rec2 = Scene_GetRecord_1(21);
    Talk_unk175_2(rec2, 0);
    rec2 = Scene_GetRecord_2(19);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    rec2 = Scene_GetRecord_3(20);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    Talk_unk176_2(1);
    Motion_SetHPosTerrain_2(0, 0, 0);
    Talk_unk177_2(1);
    /* Advance the scene phase/status word at +0x1c0 and its companion word
     * at +0x1c8 of the shared scene work record. */
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)((work + 0x1c0)) = 0x200;
    *(volatile s32 *)((work + 0x1c8)) = 32;
    Battle_WaitMode0_1();
    ObjectMotion_SetSpeedParameters_1();
    Battle_WaitMode0_2(40);
    Motion_SetVarCbObj_1(17, 1);
    Battle_WaitMode0_3(20);
    Motion_SetSpeed_2(17, 0xcccc, 0x6666);
    Motion_SetPosReset_1(17, 164, 0x388);
    Battle_WaitMode0_4(20);
    Object_SetModeById_10(17, 9);
    Battle_WaitMode0_5(40);
    Object_SetModeById_11(17, 10);
    Battle_WaitMode0_6(60);
    Object_SetModeById_12(17, 1);
    Battle_WaitMode0_7(20);
    Motion_SetPosReset_2(17, 164, 0x398);
    Motion_SetPosReset_3(17, 185, 0x398);
    ObjectMotion_ArmCallback_1(17, 0xc000, 20);
    Motion_SetPosReset_4(17, 185, 0x394);
    Object_SetModeById_13(17, 11);
    Battle_WaitMode0_8(40);
    Motion_SetVarCbObj_2(17, 1);
    Battle_WaitMode0_9(60);
    Motion_SetVarCbObj_3(17, 3);
    Battle_WaitMode0_10(40);
    Motion_EnableActCb_1(17, 0x200af88);
    Battle_WaitMode0_11(80);
    Battle_WaitMode0_12(17, 0x102);
    Battle_WaitMode0_13(60);
    Motion_ArmCb_2(14, 0xd000, 0);
    Motion_ArmCb_3(15, 0x5000, 0);
    Motion_ArmCb_4(18, 0x8000, 20);
    Battle_WaitMode0_14(14, 0x102);
    Battle_WaitMode0_15(15, 0x102);
    Battle_WaitMode0_16(17, 0x102);
    Battle_WaitMode0_17(18, 0x102);
    Battle_WaitMode0_18(60);
    Motion_ArmCb_5(14, 0x3000, 0);
    Motion_ArmCb_6(15, 0x3000, 0);
    Talk_unk14(18, 0x3000);
    Battle_WaitMode0_19(17, 0x101);
    /* Clear the flag byte at +85, then step the +12 field back and forth
     * 20 times, waiting between each step. */
    rec = Scene_GetRecord_4(21);
    rec[85] = 0;
    for (counter = 0; counter < 20; counter++) {
        *(volatile s32 *)(rec + 12) += 0x9999;
        Talk_unk178_2(4);
        *(volatile s32 *)(rec + 12) += -0x4ccc;
        Talk_unk179_2(4);
    }
    Object_SetModeById_14(19, 6);
    Object_SetModeById_15(20, 6);
    Battle_WaitMode0_20(60);
    Battle_WaitMode0_21(17, 0x100);
    Motion_EnableReset_1(17);
    Object_SetModeById_16(17, 1);
    rec2 = Scene_GetRecord_5(17);
    {
        /* Set the u16 field at +6. */
        s32 shown = 0xd000;

        *(volatile u16 *)(rec2 + 6) = shown;
    }
    rec[85] = 3;
    *(volatile s32 *)(rec + 24) = 0x10000;
    *(volatile s32 *)(rec + 28) = 0x10000;
    Battle_WaitMode0_22(10);
    Audio_PlayCue_2(107);
    Talk_unk180_2(0x10000, 0x10000, 0x10000);
    Battle_WaitMode0_23(10);
    Audio_PlayCue_3(0x121);
    Talk_unk22_3(-1, -1, 0xe666);
    Talk_unk181_2();
    Motion_SetSpeed_3(17, 0x19999, 0xcccc);
    Motion_SetPosReset_5(17, 208, 0x3a0);
    Audio_PlayCue_4(92);
    Motion_ArmCb_7(17, 0x3000, 20);
    Object_SetModeById_17(17, 9);
    Battle_WaitMode0_24(20);
    Object_SetModeById_18(17, 10);
    Battle_WaitMode0_25(40);
    Object_SetModeById_19(17, 9);
    Battle_WaitMode0_26(20);
    Object_SetModeById_20(17, 10);
    Battle_WaitMode0_27(80);
    /* Advance the scene phase/status word at +0x1c0 and its companion word
     * at +0x1c8 of the shared scene work record. */
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)((work + 0x1c0)) = 0x201;
    *(volatile s32 *)((work + 0x1c8)) = 16;
    Talk_unk182_2();
    Motion_SetSpeed_4();
    GameFlag_Set_1(0x8a4);
    Talk_unk183_2(69);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

s32 State_DispatchByStatus448(void)
{
    extern u8 gCell[];
    extern u8 *gWork;

    s32 off = 448;
    s16 status = *(s16 *)(gCell + off);

    if (status == (s32)&Value_0000008c) {
        Talk_unk184_2();
    } else if (status == (s32)&Value_0000008e) {
        Talk_unk185_2();
    }
    return 0;
}

/*
 * The message/event dispatcher for resource_3b9: read the selector from the
 * shared table, subtract 5, and dispatch through a 66-entry jump table. Only
 * twelve selectors act; the rest fall through. The 508-byte owner includes the
 * alignment halfword and the five-word literal pool after the return.
 */

/* A Func_ name here is a loader-relocated call word, not a runtime address.
 * Talk_Apply5 and Talk_unk21_2 share an encoded word but reach different
 * veneers, so the two declarations must stay distinct; the suffix marks the
 * second call word, not a second entry point. */

/*
 * The case order is load-bearing and it is not the selector order: the arms
 * are laid out in the order the reference places their bodies, and the jump
 * table stores their absolute addresses. Each callee name is keyed to the
 * address of the instruction that calls it, so an arm moved out of place
 * emits the wrong call word.
 */
void Scene_DispatchBySelector(void)
{
    extern u8 gCell[];

    s32 no;

    Talk_unk22_2(1);
    {
        s32 off = 450;

        no = *(s16 *)(gCell + off);
    }
    switch (no) {
    case 5:
        Talk_Apply6(8, 2);
        Talk_Apply7(9, 2);
        break;
    case 69:
        Talk_Apply5(8, 2);
        Talk_Apply8(9, 2);
        if (Talk_unk34(0x109) != 0)
            break;
        Talk_unk186_2();
        break;
    case 7:
        Talk_unk187_2();
        break;
    case 70:
        Talk_unk188_2();
        break;
    case 64:
        Talk_unk189_2();
        Talk_unk190_2();
        break;
    case 65:
        Talk_unk191_2();
        break;
    case 66:
        Talk_unk192_2();
        break;
    case 12:
        Talk_unk23_2(324);
        Talk_unk193_2();
        if (Talk_unk35(0x109) != 0)
            break;
        Talk_unk194_2();
        break;
    case 21:
        Talk_unk24_2(1);
        Talk_unk25_2(2);
        Talk_unk26_2(3);
        Talk_unk27_2(0x90e);
        Talk_unk195_2();
        break;
    case 67:
        Talk_unk196_2();
        break;
    case 68:
        Talk_unk197_2();
        break;
    case 31:
        Talk_unk28_2(1);
        Talk_unk29_2(2);
        Talk_unk30_2(3);
        Talk_unk21_2(0x90f);
        Talk_unk198_2();
        break;
    default:
        break;
    }
}

void Scene_CallPairWith10(s32 no)
{
    Talk_Apply9(no, 0);
    Talk_unk31_2(10);
}

void State_ForwardMaskedHalfwordWith10(s32 arg0, s32 arg1)
{
    extern u8 gCell[];
    extern u8 *gWork;

    Talk_unk92_2(arg0, (u16)arg1, 10);
}

/* resource_3b9 owner at 0x02001c6c, 100 bytes. Two alternative layouts. */
void State_ApplyRectsByFlags2384And962(void)
{
    s32 pair;
    s32 a, b;

    if (Talk_unk36(2384) != 0) {
        pair = 2;
        Talk_unk199_2(64, 0, 48, 5, pair, pair);
        a = 16;
        b = 8;
        Talk_unk200_2(14, 8, 2, 1, a, b);
    } else {
        Talk_unk201_2(16, 2);
        if (Talk_unk37(0x962) != 0) {
            a = 14;
            b = 11;
            Talk_unk202_2(30, 22, 1, 2, a, b);
        }
    }
}

void Scene_BuildActorPresentationSequence(void)
{
    s32 flag;

    Battle_Reset();
    Motion_SetHPosTerrain(1, 0x3180000, 0x880000);
    Motion_SetHPosTerrain(2, 0x3380000, 0x880000);
    Motion_SetHPosTerrain(3, 0x3280000, 0x980000);
    Talk_unk203_2(1);
    Motion_CamBounds(-1, -1, -1, 0);
    gOv11(0, 0);
    Talk_unk204_2(0, 0);
    Talk_unk205_2(1);
    Talk_unk203_2(1);
    SCENE_PHASE = 515;
    SCENE_FIELD_1C8 = 1;
    Talk_unk206_2();
    Talk_unk207_2();
    gOv11(0, 0);
    Talk_unk204_2(0x10002, 0);
    Talk_unk205_2(40);
    Battle_WaitMode0(80);
    Motion_SetVarCbObj(8, 1);
    Battle_WaitMode0(20);
    Motion_SetVarCbObj(2, 2);
    Battle_WaitMode0(40);
    Talk_unk208_2(8, 0x3000);
    Motion_SetVarCbObj(8, 2);
    Battle_WaitMode0(20);
    Talk_unk204_2(0x10000, 0);
    Talk_unk205_2(40);
    Battle_WaitMode0(80);
    BattleFx_SpawnLinked(2, 0x100, 0);
    Motion_SetVarCbObj(2, 1);
    Battle_WaitMode0(20);
    SceneWork_SetStepValue(0x20f8);
    Talk_unk209_2(2);
    Motion_CallWaitAnim(8, 3);
    Talk_unk209_2(8);
    BattleFx_SpawnLinked(3, 0x102, 40);
    Talk_unk209_2(3);
    Motion_ArmCb(8, 0x8000, 0);
    Motion_ArmCb(9, 0, 40);
    Motion_ArmCb(8, 0x5000, 0);
    Talk_unk208_2(9, 0x3000);
    BattleFx_SpawnLinked(9, 0x105, 20);
    Talk_unk209_2(9);
    BattleFx_SpawnLinked(1, 0x103, 20);
    Motion_SetVarCb(1, 2);
    Talk_unk209_2(1);
    BattleFx_SpawnLinked(10, 0x102, 40);
    Talk_unk209_2(10);
    Motion_CallWaitAnim(11, 3);
    Talk_unk209_2(11);
    Talk_unk208_2(2, 0xa000);
    Motion_CallWaitAnim(2, 4);
    Talk_unk209_2(2);
    Motion_SetVarCbObj(3, 1);
    Talk_unk209_2(3);
    BattleFx_SpawnLinked(1, 0x103, 20);
    Talk_unk208_2(1, 0);
    Motion_SetVarCb(1, 2);
    Talk_unk209_2(1);
    Talk_unk208_2(0, 0x6000);
    BattleFx_SpawnLinked(0, 0x102, 60);
    Motion_SetVarCbObj(2, 1);
    Talk_unk208_2(2, 0x8000);
    Talk_unk209_2(2);
    Object_SetModeById(3, 4);
    Battle_WaitMode0(20);
    Talk_unk209_2(3);
    Motion_SetVarCbObj(1, 1);
    Talk_unk208_2(1, 0x2000);
    Talk_unk209_2(1);
    BattleFx_SpawnLinked(2, 0x105, 60);
    Motion_ArmCb(2, 0x2000, 0);
    Motion_ArmCb(1, 0, 0);
    Motion_ArmCb(3, 0xe000, 20);
    BattleFx_SpawnLinked(0, 0x102, 0);
    BattleFx_SpawnLinked(1, 0x102, 0);
    BattleFx_SpawnLinked(3, 0x102, 80);
    BattleFx_SpawnLinked(2, 0x106, 0);
    Motion_SetVarCbObj(2, 1);
    Talk_unk208_2(2, 0xc000);
    Talk_unk209_2(2);
    Motion_ArmCb(0, 0xc000, 0);
    Motion_ArmCb(1, 0xc000, 0);
    Motion_ArmCb(3, 0xc000, 20);
    Talk_unk208_2(8, 0x3000);
    Motion_CallWaitAnim(8, 3);
    Talk_unk209_2(8);
    BattleFx_SpawnLinked(0, 0x100, 0);
    BattleFx_SpawnLinked(1, 0x100, 0);
    BattleFx_SpawnLinked(3, 0x100, 40);
    Motion_SetVarCbObj(9, 1);
    Talk_unk209_2(9);
    Motion_ArmCb(0, 0x6000, 0);
    Motion_ArmCb(1, 0xe000, 0);
    Motion_ArmCb(2, 0x6000, 0);
    Motion_ArmCb(3, 0xe000, 40);
    Motion_SetVarCbObj(10, 1);
    Talk_unk209_2(10);
    Motion_ArmCb(0, 0xc000, 0);
    Motion_ArmCb(1, 0xc000, 0);
    Motion_ArmCb(2, 0xc000, 0);
    Talk_unk208_2(3, 0xc000);
    Motion_CallWaitAnim(11, 3);
    Talk_unk209_2(11);
    Motion_ArmCb(0, 0x6000, 0);
    Motion_ArmCb(1, 0xe000, 0);
    Motion_ArmCb(2, 0x6000, 0);
    Motion_ArmCb(3, 0xe000, 20);
    BattleFx_SpawnLinked(1, 0x105, 0);
    BattleFx_SpawnLinked(2, 0x105, 0);
    BattleFx_SpawnLinked(3, 0x105, 80);
    BattleFx_SpawnLinked(2, 0x106, 0);
    Motion_SetVarCbObj(2, 1);
    Talk_unk208_2(2, 0xe000);
    Talk_unk209_2(2);
    Motion_ArmCb(0, 0, 0);
    Motion_ArmCb(1, 0, 20);
    BattleFx_SpawnLinked(11, 0x101, 60);
    Talk_unk15(2, 0);
    Motion_ArmCb(0, 0x2000, 0);
    Motion_ArmCb(1, 0xe000, 0);
    Motion_ArmCb(2, 0xa000, 0);
    Motion_ArmCb(3, 0xc000, 0);
    flag = 0;
    if (UiWork_WaitThenFinalizeCapacity(0, 0) == 1) {
        Talk_unk209_2(2);
        flag = 1;
    } else {
        SCENE_STEP++;
        Motion_CallWaitAnim(2, 3);
        Talk_unk208_2(2, 0xc000);
        Talk_unk209_2(2);
    }
    if (flag != 0) {
        SCENE_STEP++;
    }
    Motion_ArmCb(8, 0x8000, 0);
    Motion_ArmCb(9, 0, 0);
    Motion_ArmCb(10, 0x3000, 0);
    Motion_ArmCb(11, 0xb000, 20);
    BattleFx_SpawnLinked(8, 0x105, 0);
    BattleFx_SpawnLinked(9, 0x105, 0);
    BattleFx_SpawnLinked(10, 0x105, 0);
    BattleFx_SpawnLinked(11, 0x105, 60);
    Motion_SetVarCbObj(8, 1);
    Talk_unk208_2(8, 0x3000);
    Talk_unk209_2(8);
    Motion_ArmCb(0, 0xc000, 0);
    Motion_ArmCb(1, 0xc000, 0);
    Motion_ArmCb(2, 0xc000, 0);
    Motion_ArmCb(3, 0xc000, 20);
    Motion_SetVarCbObj(9, 1);
    Talk_unk208_2(9, 0x3000);
    Talk_unk209_2(9);
    Motion_SetVarCbObj(10, 1);
    Talk_unk208_2(10, 0x5000);
    Talk_unk209_2(10);
    Talk_unk208_2(11, 0x8000);
    Motion_CallWaitAnim(11, 3);
    Talk_unk209_2(11);
    Motion_ArmCb(1, 0xe000, 0);
    Motion_ArmCb(2, 0xa000, 20);
    Object_SetModeById(1, 3);
    Object_SetModeById(2, 3);
    Motion_CallWaitAnim(3, 3);
    Motion_SetSpeed(1, 0x10000, 0x8000);
    Motion_SetSpeed(2, 0x10000, 0x8000);
    Motion_SetSpeed(3, 0x10000, 0x8000);
    ObjectMotion_EnableActionAndSetCallback_1(1, (s32)gOv10);
    Motion_EnableActCb_2(2, (s32)gOv10);
    Motion_MarkActiveAndSetActionCallback(3, (s32)gOv10);
    Battle_WaitMode0(20);
    Talk_unk208_2(0, 0);
    Motion_CallWaitAnim(0, 3);
    Motion_CallWaitAnim(11, 3);
    Motion_SetSpeed(11, 0x10000, 0x8000);
    Motion_SetSpeed(0, 0x10000, 0x8000);
    Object_SetModeById(11, 2);
    Motion_SetPositionAndCommit(11, 830, 152);
    Motion_SetPositionAndCommit(11, 808, 164);
    Motion_ResetAndSetPosition(11, 808, 312);
    Battle_WaitMode0(20);
    Motion_SetSpeedLim(0x6666, 0xccc);
    Motion_CamBounds(0x3280000, -1, 0x1380000, 1);
    Motion_SetPosReset(0, 808, 164);
    Motion_ResetPosMode2(0, 808, 312);
    Battle_WaitMode0(60);
    SCENE_PHASE = 256;
    SCENE_FIELD_1C8 = 40;
    Talk_unk210_2();
    Talk_unk207_2();
    Talk_unk211_2(64);
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Talk_unk16(5) != 0) {
        Talk_unk12_2(0x16d);
        Talk_unk212_2(5);
        Talk_unk213_2(3);
    }
    Talk_unk214_2();
    Talk_unk23_3(11, 0x2c80000, 0x24c0000);
    Talk_unk215_2(1);
    Talk_unk216_2(11, 1);
    Talk_unk24_3(11, 0x19999, 0xcccc);
    Talk_unk25_3(0, 0x19999, 0xcccc);
    record = Talk_unk38(11);
    {
        s32 shown = 0;

        *(volatile u16 *)(record + 6) = shown;
    }
    Talk_unk217_2();
    Talk_unk218_2(0, 2);
    Talk_unk219_2(11, 2);
    Talk_unk26_3(0, 0x30c, 0x24c);
    Talk_unk27_3(11, 0x32c, 0x24c);
    Talk_unk28_3(0, 0x370, 0x24c);
    Talk_unk29_3(11, 0x390, 0x24c);
    Talk_unk30_3(0, 0x3d4, 0x24c);
    Talk_unk31_3(11, 0x3f4, 0x24c);
    Talk_unk220_2();
    Talk_unk221_2();
    if (Talk_unk17(0x90f) != 0) {
        Talk_unk222_2(31);
    } else {
        Talk_unk223_2(65);
    }
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Talk_unk18(13);
    Talk_unk224_2();
    Talk_unk225_2();
    Talk_unk226_2();
    Talk_unk227_2(40);
    Talk_unk228_2(8, 2);
    Talk_unk229_2(13);
    Talk_unk230_2(1);
    Talk_unk32_3(0, 0xe000, 0);
    Talk_unk231_2(13, 1);
    Talk_unk33_3(12, 0xd000, 0);
    Talk_unk232_2(13, 0, 0);
    Talk_unk34_3(14, 0x8000, 0);
    Talk_unk35_3(15, 0xd000, 0);
    Talk_unk36_3(16, 0x8000, 0);
    Talk_unk37_3(17, 0xb000, 0);
    Talk_unk38_3(18, 0xb000, 0);
    Talk_unk13_2(0x2112);
    Talk_unk233_2(8);
    Talk_unk234_2(0, 3);
    {
        u16 *target = (u16 *)(rec7 + 100);
        s32 shown = 0x2d0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)(rec7 + 102);
        s32 shown = 112;

        *target = shown;
    }
    Talk_unk235_2(13, 2);
    Talk_unk39_3(12, 0x3000, 0);
    Talk_unk40_3(14, 0xb000, 0);
    Talk_unk41_3(15, 0x5000, 0);
    Talk_unk236_2(16, 0, 0);
    Talk_unk42_3(17, 0x5000, 0);
    Talk_unk43_3(18, 0x5000, 0);
    Talk_unk237_2();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk238_2();
    Talk_unk44_3(0, 0x19999, 0xcccc);
    Talk_unk239_2();
    Talk_unk240_2(0, 2);
    Talk_unk45_3(0, 0x30c, 0x1ac);
    Talk_unk46_3(0, 0x370, 0x1ac);
    Talk_unk47_3(0, 0x3d4, 0x1ac);
    Talk_unk241_2();
    Talk_unk242_2();
    if (Talk_unk19(0x90f) != 0) {
        Talk_unk243_2(32);
    } else {
        Talk_unk244_2(12);
    }
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 base5_200adac;

    Talk_unk245_2();
    Talk_unk48_3(1, 0x3180000, 0x880000);
    Talk_unk49_3(2, 0x3380000, 0x880000);
    Talk_unk50_3(3, 0x3280000, 0x980000);
    Talk_unk246_2();
    Talk_unk247_2();
    Talk_unk248_2(40);
    Talk_unk249_2(8, 1);
    Talk_unk250_2(8, 3);
    Talk_unk14_2(0x2134);
    Talk_unk251_2(8);
    Talk_unk252_2(9, 1);
    Talk_unk253_2(9);
    Talk_unk254_2(10, 1);
    Talk_unk255_2(10);
    Talk_unk256_2(11, 1);
    Talk_unk257_2(11, 3);
    Talk_unk258_2(11);
    Talk_unk51_3(1, 0xe000, 0);
    Talk_unk52_3(2, 0xa000, 20);
    Talk_unk53_3(1, 0x10000, 0x8000);
    Talk_unk54_3(2, 0x10000, 0x8000);
    Talk_unk55_3(3, 0x10000, 0x8000);
    base5_200adac = (s32)gOv10;
    Talk_unk259_2(1, base5_200adac);
    Talk_unk20(2, base5_200adac);
    Talk_unk260_2(3, base5_200adac);
    Talk_unk261_2(20);
    Talk_unk262_2(0, 0);
    Talk_unk263_2(0, 3);
    Talk_unk264_2(11, 3);
    Talk_unk56_3(11, 0x10000, 0x8000);
    Talk_unk57_3(0, 0x10000, 0x8000);
    Talk_unk265_2(11, 2);
    Talk_unk58_3(11, 0x33e, 152);
    Talk_unk59_3(11, 0x328, 164);
    Talk_unk60_3(11, 0x328, 0x138);
    Talk_unk266_2(20);
    Talk_unk10_4(0x6666, 0xccc);
    Talk_unk11_4(0x3280000, -1, 0x1380000, 1);
    Talk_unk61_3(0, 0x328, 164);
    Talk_unk62_3(0, 0x328, 0x138);
    Talk_unk267_2(60);
    Talk_unk268_2();
    Talk_unk269_2();
    Talk_unk270_2(67);
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Talk_unk21(5) != 0) {
        Talk_unk15_2(0x16d);
        Talk_unk271_2(5);
        Talk_unk272_2(3);
    }
    Talk_unk273_2();
    Talk_unk63_3(11, 0x3640000, 0x24c0000);
    Talk_unk274_2(1);
    Talk_unk275_2(11, 1);
    Talk_unk64_3(11, 0x19999, 0xcccc);
    Talk_unk65_3(0, 0x19999, 0xcccc);
    record = Talk_unk39(11);
    {
        s32 shown = 0x8000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Talk_unk276_2();
    Talk_unk277_2(0, 2);
    Talk_unk278_2(11, 2);
    Talk_unk66_3(0, 0x320, 0x24c);
    Talk_unk67_3(11, 0x300, 0x24c);
    Talk_unk68_3(0, 0x2bc, 0x24c);
    Talk_unk69_3(11, 0x29c, 0x24c);
    Talk_unk70_3(0, 0x258, 0x24c);
    Talk_unk71_3(11, 0x238, 0x24c);
    Talk_unk279_2();
    Talk_unk280();
    Talk_unk281(21);
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk282();
    Talk_unk72_3(0, 0x19999, 0xcccc);
    Talk_unk283();
    Talk_unk284(0, 2);
    Talk_unk73_3(0, 0x320, 0x1ac);
    Talk_unk74_3(0, 0x2bc, 0x1ac);
    Talk_unk75_3(0, 0x258, 0x1ac);
    Talk_unk285();
    Talk_unk286();
    Talk_unk287(22);
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 base5_200adac;

    Talk_unk288();
    Talk_unk76_3(1, 0x3180000, 0x880000);
    Talk_unk77_2(2, 0x3380000, 0x880000);
    Talk_unk78_2(3, 0x3280000, 0x980000);
    Talk_unk289();
    Talk_unk290();
    Talk_unk291(40);
    Talk_unk292(8, 1);
    Talk_unk293(8, 3);
    Talk_unk16_2(SCENE_STEP_VALUE);
    Talk_unk294(8);
    Talk_unk295(9, 1);
    Talk_unk296(9);
    Talk_unk297(10, 1);
    Talk_unk298(10);
    Talk_unk299(11, 1);
    Talk_unk300(11, 3);
    Talk_unk301(11);
    Talk_unk79_2(1, 0xe000, 0);
    Talk_unk80_2(2, 0xa000, 20);
    Talk_unk81_2(1, 0x10000, 0x8000);
    Talk_unk82_2(2, 0x10000, 0x8000);
    Talk_unk83_2(3, 0x10000, 0x8000);
    base5_200adac = (s32)gOv10;
    Talk_unk302(1, base5_200adac);
    Talk_unk22(2, base5_200adac);
    Talk_unk303(3, base5_200adac);
    Talk_unk304(20);
    Talk_unk305(0, 0);
    Talk_unk306(0, 3);
    Talk_unk307(11, 3);
    Talk_unk84_2(11, 0x10000, 0x8000);
    Talk_unk85_2(0, 0x10000, 0x8000);
    Talk_unk308(11, 2);
    Talk_unk86_2(11, 0x33e, 152);
    Talk_unk87_2(11, 0x328, 164);
    Talk_unk88_2(11, 0x328, 0x138);
    Talk_unk309(20);
    Talk_unk12_4(0x6666, 0xccc);
    Talk_unk13_4(0x3280000, -1, 0x1380000, 1);
    Talk_unk89_2(0, 0x328, 164);
    Talk_unk90_2(0, 0x328, 0x138);
    Talk_unk310(60);
    Talk_unk311();
    Talk_unk312();
    Talk_unk313(64);
}

void ActorDraw_SetActorsTwelveToEighteen(void)
{
    Talk_unk314(12, 3);
    Talk_unk315(13, 0);
    Talk_unk316(14, 4);
    Talk_unk317(15, 1);
    Talk_unk318(16, 5);
    Talk_unk319(17, 2);
    Talk_unk320(18, 6);
    Talk_unk321(13, 10);
    Talk_unk322(14, 20);
    Talk_unk323(15, 0);
    Talk_unk324(16, 40);
    Talk_unk325(17, 50);
    Talk_unk326(18, 60);
}
