#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/status_dispatch.h"

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
 *   - at owner offset 0x42c the two constant arguments of the Talk_Check23
 *     call are set r0 before r1 where the reference sets r1 before r0;
 *   - the pc-relative load of 0x0200adac that the three action-callback calls
 *     share lands at owner offset 0x5f8, before the third
 *     Motion_SetSpeed call; the reference issues it at 0x604,
 *     after that call and before its first use, which also
 *     swaps the r0/r1 setup of the first of those three calls: the reference
 *     schedule fills the load-use gap with `movs r0, #1`, ours has no gap to
 *     fill.
 *
 * Neither survived respelling. Wrapping or prototyping the Talk_Check23 site
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
 * so both aliases the reference offers for it - Talk_Run14 and
 * Talk_Run15 - name two targets and neither can bind. Every other target
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

extern u8 gOv10[];

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

s32 Talk_Run16(void)
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
    Talk_Run17();
    if (Talk_Check24(0x962)) {
        Talk_Do17(0x2251);
        Talk_Apply(10, 0);
    } else {
        Talk_Do18(0x2057);
        Talk_Apply2(10, 0);
    }
    Talk_Run18();
}

void Dialogue_RunActor13MessageByFlag962(void)
{
    Talk_Run19();
    if (Talk_Check25(0x962)) {
        Talk_Place91(13, 258, 40);
        Talk_Do19(0x2254);
        Talk_Apply3(13, 0);
    } else {
        Talk_Do20(0x205c);
        Talk_Apply4(13, 0);
    }
    Talk_Run20();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run21();
    if (Talk_Check(0x962) != 0) {
        Talk_Run22(14, 2);
        Talk_Do(0x2256);
        Talk_Run23(14);
        Talk_Run24(14, 0, 0);
        Talk_Run25(20);
        Talk_Check2(14, 0);
        Talk_Run26(14, 0);
    } else {
        Talk_Do2(0x205d);
        Talk_Run27(14, 0);
    }
    Talk_Run28();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run29();
    if (Talk_Check3(0x962) != 0) {
        if (Talk_Check4(0x3c0) != 0) {
            Talk_Do3(0x225e);
        } else {
            Talk_Do4(0x225a);
            Talk_Check5(16, 0);
            if (Talk_Check6(0, 0) == 0) {
                bump_step(1);
                Talk_Place(16, 0x100, 40);
                Talk_Check7(16, 0);
                if (Talk_Check8(0, 0) == 0) {
                    bump_step(1);
                }
                ((void (*)())Talk_Check26)(40);
                Talk_Run30(16, 0);
                Talk_Do5(0x3c0);
                goto L_02000448;
            }
        }
        Talk_Run31(16, 0);
    } else {
        Talk_Do6(0x205e);
        Talk_Run32(16, 0);
    }
    L_02000448:;
    Talk_Run33();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Talk_Check9(13);
    Talk_Run34();
    Talk_Run35(13);
    Talk_Run36(13, 0, 20);
    Talk_Do7(0x2114);
    Talk_Run37(13);
    Talk_Run38(13, 1);
    Talk_Run39(13, 0);
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
    Talk_Run40(13, 2);
    Talk_Run41();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run42();
    Talk_Run(14, 0x102);
    Talk_Run43(14, 2);
    Talk_Do8(0x2116);
    Talk_Run44(14);
    Talk_Place2(14, 0x102, 40);
    Talk_Run45(14, 0);
    Talk_Run46();
}

void Dialogue_ShowLine2118WithActor15Steps(void)
{
    Talk_Run47();
    Talk_Run48(0x2118);
    Talk_Run49(15);
    Talk_Run50(15, 0, 20);
    Talk_Run51(15);
    Talk_Run52(15, 3);
    Talk_Run53(15, 0);
    Talk_Run54(15);
    Talk_Run55(15, 20480);
    Talk_Run56();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run57();
    Talk_Run58(16, 2);
    Talk_Do9(0x211b);
    Talk_Run59(16, 0, 20);
    if (Talk_Check10(0x3c1) != 0) {
        Talk_Run60(20);
    } else {
        Talk_Run61(17, 0);
        Talk_Run62(17, 1);
        Talk_Run63(17);
        Talk_Run64(17, 0, 20);
        Talk_Run65(17, 4);
        Talk_Run66(17);
        Talk_Place3(17, 0x105, 40);
        Talk_Run67(17);
        Talk_Run2(17, 0x5000);
        Talk_Do10(0x3c1);
    }
    Talk_Run68();
}

void Scene_RunActorSeventeenDialogueSteps(void)
{
    extern u8 gWork[];

    Talk_Run69();
    Talk_Run70(17, 0, 20);
    Talk_Run71(0x211f);
    Talk_Run72(17);
    Talk_Run73(0, 3);
    Talk_Run74(17, 3);
    Talk_Run75(17);
    Talk_Run76(17, 1);
    Talk_Run77(17);
    Talk_Run78(17, 20480);
    Talk_Run79();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run80();
    Talk_Run81(18, 0, 20);
    Talk_Do11(0x2122);
    Talk_Run82(18);
    Talk_Place4(18, 0xd000, 20);
    Talk_Place5(18, 0xb000, 20);
    Talk_Place6(18, 0x8000, 40);
    Talk_Run83(18, 0, 20);
    Talk_Run84(18);
    Talk_Run85(18, 3);
    Talk_Run86(18);
    Talk_Check11(18, 0x5000);
    Talk_Run87();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    Talk_Run88();
    Talk_Check27(8, 0, 20);
    Talk_Run89((s32)&Value_00002125);
    Talk_Check28(8, 0);
    if (Talk_Check29(0, 0) != 0)
        bump_step(1);
    Talk_Run90(8, 0);
    Talk_Run91();
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

    Talk_Run92(0x235);
    Talk_Run93(564);
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
        Talk_Run94(RECORD_17);
        Motion_ArmCb_1(RECORD_17, 0x3000, 20);
    } else {
        Motion_SetVarCb_1(RECORD_17, 2);
        SceneWork_SetStepValue_2(0x206d);
        BattleEv_RunWait_1(RECORD_17, 0);
        /* Byte at +85 of the record returned by RuntimeBlock_GetOffset1e0Pointer_1(); written
         * with the (already known zero) flag value here. */
        *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_2() + 85) = flag_8a4;
        Talk_Run95(1);
        Motion_SetSpeedLim_1(0x66666, 0xcccc);
        Motion_CamBounds_1(0x21c0000, -1, 0xd00000, 1);
        Object_CommitPositionThenWaitIfModeZero_1();
        SCENE_PHASE = 0x200;
        SCENE_FIELD_1C8 = 32;
        Talk_Run96();
        Motion_SetSpeed_1();
        if (GameFlag_IsSet_2(0x8a3) != 0) {
            Talk_Run97(70);
        } else {
            Talk_Run98(7);
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

    Talk_Run99();
    Talk_Run3(-1, -1, -1, 0);
    Talk_Run100(247);
    Talk_Run101(8, 2);
    Talk_Run102(9, 2);
    Talk_Run103(10, 2);
    Talk_Run104(11, 2);
    Talk_Run105(12, 2);
    Talk_Run106(13, 2);
    Talk_Run107(14, 0);
    Talk_Run108(15, 0);
    Talk_Run109(16, 0);
    Talk_Run110(17, 0);
    Talk_Run111(18, 0);
    rec2 = Talk_Check30(21);
    Talk_Run112(rec2, 0);
    rec2 = Talk_Check31(19);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    rec2 = Talk_Check12(20);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    Talk_Run113(1);
    Talk_Run114(0, 0, 0);
    Talk_Run115(1);
    work = *(u8 *volatile *)gWork;
    phase = 0x1c0;
    *(volatile s32 *)(work + phase) = 0x200;
    field = 0x1c8;
    *(volatile s32 *)(work + field) = 32;
    Talk_Run116();
    Talk_Run117();
    Talk_Run118(40);
    Talk_Run119(16, 1);
    Talk_Run120(20);
    Talk_Place7(16, 0xcccc, 0x6666);
    Talk_Run121(16, 164, 0x388);
    Talk_Run122(20);
    Talk_Run123(16, 9);
    Talk_Run124(40);
    Talk_Run125(16, 10);
    Talk_Run126(60);
    Talk_Run127(16, 1);
    Talk_Run128(20);
    Talk_Place8(16, 164, 0x398);
    Talk_Place9(16, 185, 0x398);
    Talk_Place10(16, 0xc000, 20);
    Talk_Place11(16, 185, 0x394);
    Talk_Run129(16, 11);
    Talk_Run130(40);
    Talk_Run131(16, 1);
    Talk_Run132(60);
    Talk_Run133(16, 3);
    Talk_Run134(40);
    Talk_Run135(16, 0x200af88);
    Talk_Run136(80);
    Talk_Run4(16, 0x102);
    Talk_Run137(60);
    Talk_Place12(14, 0xd000, 0);
    Talk_Place13(15, 0x5000, 0);
    Talk_Run138(17, 0, 0);
    Talk_Place14(18, 0x8000, 20);
    Talk_Run5(14, 0x102);
    Talk_Run6(15, 0x102);
    Talk_Run7(17, 0x102);
    Talk_Run8(18, 0x102);
    Talk_Run139(60);
    Talk_Place15(14, 0x3000, 0);
    Talk_Place16(15, 0x3000, 0);
    Talk_Place17(17, 0x3000, 0);
    shift = 0x3000;
    Talk_Check13(18, shift);
    Talk_Run140(16);
    rec2 = Talk_Check32(16);
    {
        s32 shown = 0xd000;
        *(volatile u16 *)(rec2 + 6) = shown;
    }
    *(volatile s32 *)(rec2 + 24) = 0x10000;
    *(volatile s32 *)(rec2 + 28) = 0x10000;
    Talk_Run141(20);
    Talk_Run142(16, 0);
    Talk_Run143(40);
    Talk_Run144(19, 5);
    Talk_Run145(20, 5);
    Talk_Run146(60);
    Talk_Run147(16, shift, 20);
    Talk_Run148(16, 8);
    Talk_Run149(20);
    Talk_Run150(14, 4);
    Talk_Run151(15, 4);
    Talk_Run152(17, 4);
    Talk_Run153(18, 4);
    Talk_Run154(40);
    Talk_Run155(16, 4);
    Talk_Run156(10);
    Talk_Place18(16, 0x20000, 0x10000);
    Talk_Place19(16, 162, 0x394);
    Talk_Run157(16, 162, 0x37a);
    Talk_Run158(19, 1);
    Talk_Run159(20, 1);
    Talk_Place20(16, 184, 0x35f);
    Talk_Place21(16, 184, 0x31c);
    Talk_Run160(16, 0, 0);
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(work + phase) = 0x201;
    *(volatile s32 *)(work + field) = 16;
    Talk_Run161();
    Talk_Run162();
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(work + phase) = 0x100;
    Talk_Run163(69);
    Talk_Run164();
}

void State_SetRuntimeWord448To513(void)
{
    extern u8 gCell[];
    extern u8 *gWork;

    Talk_Run165();
    *(s32 *)((u8 *)gWork + 448) = 513;

    Talk_Run166();
    Talk_Run167();

    Talk_Run168(20);
    Talk_Run169(17, 160 << 7);
    Talk_Run170(0x206e);

    if (Talk_Check33(0x8a4) != 0) {
        *(u16 *)((u8 *)gWork + 472) =
            (u16)(*(u16 *)((u8 *)gWork + 472) + 1);
    }

    Talk_Run171(17);
    Talk_Run172(17, 192 << 6);
    Talk_Run173(0x8a3);

    Talk_Run174();
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
    Talk_Run9(9, 2);
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
    Talk_Run175(rec2, 0);
    rec2 = Scene_GetRecord_2(19);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    rec2 = Scene_GetRecord_3(20);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    Talk_Run176(1);
    Motion_SetHPosTerrain_2(0, 0, 0);
    Talk_Run177(1);
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
    Talk_Check14(18, 0x3000);
    Battle_WaitMode0_19(17, 0x101);
    /* Clear the flag byte at +85, then step the +12 field back and forth
     * 20 times, waiting between each step. */
    rec = Scene_GetRecord_4(21);
    rec[85] = 0;
    for (counter = 0; counter < 20; counter++) {
        *(volatile s32 *)(rec + 12) += 0x9999;
        Talk_Run178(4);
        *(volatile s32 *)(rec + 12) += -0x4ccc;
        Talk_Run179(4);
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
    Talk_Run180(0x10000, 0x10000, 0x10000);
    Battle_WaitMode0_23(10);
    Audio_PlayCue_3(0x121);
    Talk_Place22(-1, -1, 0xe666);
    Talk_Run181();
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
    Talk_Run182();
    Motion_SetSpeed_4();
    GameFlag_Set_1(0x8a4);
    Talk_Run183(69);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

s32 State_DispatchByStatus448(void)
{
    extern u8 gCell[];
    extern u8 *gWork;

    s32 off = 448;
    s16 status = *(s16 *)(gCell + off);

    if (status == (s32)&Value_0000008c) {
        Talk_Run184();
    } else if (status == (s32)&Value_0000008e) {
        Talk_Run185();
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
 * Talk_Apply5 and Talk_Do21 share an encoded word but reach different
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

    Talk_Do22(1);
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
        if (Talk_Check34(0x109) != 0)
            break;
        Talk_Run186();
        break;
    case 7:
        Talk_Run187();
        break;
    case 70:
        Talk_Run188();
        break;
    case 64:
        Talk_Run189();
        Talk_Run190();
        break;
    case 65:
        Talk_Run191();
        break;
    case 66:
        Talk_Run192();
        break;
    case 12:
        Talk_Do23(324);
        Talk_Run193();
        if (Talk_Check35(0x109) != 0)
            break;
        Talk_Run194();
        break;
    case 21:
        Talk_Do24(1);
        Talk_Do25(2);
        Talk_Do26(3);
        Talk_Do27(0x90e);
        Talk_Run195();
        break;
    case 67:
        Talk_Run196();
        break;
    case 68:
        Talk_Run197();
        break;
    case 31:
        Talk_Do28(1);
        Talk_Do29(2);
        Talk_Do30(3);
        Talk_Do21(0x90f);
        Talk_Run198();
        break;
    default:
        break;
    }
}

void Scene_CallPairWith10(s32 no)
{
    Talk_Apply9(no, 0);
    Talk_Do31(10);
}

void State_ForwardMaskedHalfwordWith10(s32 arg0, s32 arg1)
{
    extern u8 gCell[];
    extern u8 *gWork;

    Talk_Place92(arg0, (u16)arg1, 10);
}

/* resource_3b9 owner at 0x02001c6c, 100 bytes. Two alternative layouts. */
void State_ApplyRectsByFlags2384And962(void)
{
    s32 pair;
    s32 a, b;

    if (Talk_Check36(2384) != 0) {
        pair = 2;
        Talk_Run199(64, 0, 48, 5, pair, pair);
        a = 16;
        b = 8;
        Talk_Run200(14, 8, 2, 1, a, b);
    } else {
        Talk_Run201(16, 2);
        if (Talk_Check37(0x962) != 0) {
            a = 14;
            b = 11;
            Talk_Run202(30, 22, 1, 2, a, b);
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
    Talk_Run203(1);
    Motion_CamBounds(-1, -1, -1, 0);
    gOv11(0, 0);
    Talk_Run204(0, 0);
    Talk_Run205(1);
    Talk_Run203(1);
    SCENE_PHASE = 515;
    SCENE_FIELD_1C8 = 1;
    Talk_Run206();
    Talk_Run207();
    gOv11(0, 0);
    Talk_Run204(0x10002, 0);
    Talk_Run205(40);
    Battle_WaitMode0(80);
    Motion_SetVarCbObj(8, 1);
    Battle_WaitMode0(20);
    Motion_SetVarCbObj(2, 2);
    Battle_WaitMode0(40);
    Talk_Run208(8, 0x3000);
    Motion_SetVarCbObj(8, 2);
    Battle_WaitMode0(20);
    Talk_Run204(0x10000, 0);
    Talk_Run205(40);
    Battle_WaitMode0(80);
    BattleFx_SpawnLinked(2, 0x100, 0);
    Motion_SetVarCbObj(2, 1);
    Battle_WaitMode0(20);
    SceneWork_SetStepValue(0x20f8);
    Talk_Run209(2);
    Motion_CallWaitAnim(8, 3);
    Talk_Run209(8);
    BattleFx_SpawnLinked(3, 0x102, 40);
    Talk_Run209(3);
    Motion_ArmCb(8, 0x8000, 0);
    Motion_ArmCb(9, 0, 40);
    Motion_ArmCb(8, 0x5000, 0);
    Talk_Run208(9, 0x3000);
    BattleFx_SpawnLinked(9, 0x105, 20);
    Talk_Run209(9);
    BattleFx_SpawnLinked(1, 0x103, 20);
    Motion_SetVarCb(1, 2);
    Talk_Run209(1);
    BattleFx_SpawnLinked(10, 0x102, 40);
    Talk_Run209(10);
    Motion_CallWaitAnim(11, 3);
    Talk_Run209(11);
    Talk_Run208(2, 0xa000);
    Motion_CallWaitAnim(2, 4);
    Talk_Run209(2);
    Motion_SetVarCbObj(3, 1);
    Talk_Run209(3);
    BattleFx_SpawnLinked(1, 0x103, 20);
    Talk_Run208(1, 0);
    Motion_SetVarCb(1, 2);
    Talk_Run209(1);
    Talk_Run208(0, 0x6000);
    BattleFx_SpawnLinked(0, 0x102, 60);
    Motion_SetVarCbObj(2, 1);
    Talk_Run208(2, 0x8000);
    Talk_Run209(2);
    Object_SetModeById(3, 4);
    Battle_WaitMode0(20);
    Talk_Run209(3);
    Motion_SetVarCbObj(1, 1);
    Talk_Run208(1, 0x2000);
    Talk_Run209(1);
    BattleFx_SpawnLinked(2, 0x105, 60);
    Motion_ArmCb(2, 0x2000, 0);
    Motion_ArmCb(1, 0, 0);
    Motion_ArmCb(3, 0xe000, 20);
    BattleFx_SpawnLinked(0, 0x102, 0);
    BattleFx_SpawnLinked(1, 0x102, 0);
    BattleFx_SpawnLinked(3, 0x102, 80);
    BattleFx_SpawnLinked(2, 0x106, 0);
    Motion_SetVarCbObj(2, 1);
    Talk_Run208(2, 0xc000);
    Talk_Run209(2);
    Motion_ArmCb(0, 0xc000, 0);
    Motion_ArmCb(1, 0xc000, 0);
    Motion_ArmCb(3, 0xc000, 20);
    Talk_Run208(8, 0x3000);
    Motion_CallWaitAnim(8, 3);
    Talk_Run209(8);
    BattleFx_SpawnLinked(0, 0x100, 0);
    BattleFx_SpawnLinked(1, 0x100, 0);
    BattleFx_SpawnLinked(3, 0x100, 40);
    Motion_SetVarCbObj(9, 1);
    Talk_Run209(9);
    Motion_ArmCb(0, 0x6000, 0);
    Motion_ArmCb(1, 0xe000, 0);
    Motion_ArmCb(2, 0x6000, 0);
    Motion_ArmCb(3, 0xe000, 40);
    Motion_SetVarCbObj(10, 1);
    Talk_Run209(10);
    Motion_ArmCb(0, 0xc000, 0);
    Motion_ArmCb(1, 0xc000, 0);
    Motion_ArmCb(2, 0xc000, 0);
    Talk_Run208(3, 0xc000);
    Motion_CallWaitAnim(11, 3);
    Talk_Run209(11);
    Motion_ArmCb(0, 0x6000, 0);
    Motion_ArmCb(1, 0xe000, 0);
    Motion_ArmCb(2, 0x6000, 0);
    Motion_ArmCb(3, 0xe000, 20);
    BattleFx_SpawnLinked(1, 0x105, 0);
    BattleFx_SpawnLinked(2, 0x105, 0);
    BattleFx_SpawnLinked(3, 0x105, 80);
    BattleFx_SpawnLinked(2, 0x106, 0);
    Motion_SetVarCbObj(2, 1);
    Talk_Run208(2, 0xe000);
    Talk_Run209(2);
    Motion_ArmCb(0, 0, 0);
    Motion_ArmCb(1, 0, 20);
    BattleFx_SpawnLinked(11, 0x101, 60);
    Talk_Check15(2, 0);
    Motion_ArmCb(0, 0x2000, 0);
    Motion_ArmCb(1, 0xe000, 0);
    Motion_ArmCb(2, 0xa000, 0);
    Motion_ArmCb(3, 0xc000, 0);
    flag = 0;
    if (UiWork_WaitThenFinalizeCapacity(0, 0) == 1) {
        Talk_Run209(2);
        flag = 1;
    } else {
        SCENE_STEP++;
        Motion_CallWaitAnim(2, 3);
        Talk_Run208(2, 0xc000);
        Talk_Run209(2);
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
    Talk_Run208(8, 0x3000);
    Talk_Run209(8);
    Motion_ArmCb(0, 0xc000, 0);
    Motion_ArmCb(1, 0xc000, 0);
    Motion_ArmCb(2, 0xc000, 0);
    Motion_ArmCb(3, 0xc000, 20);
    Motion_SetVarCbObj(9, 1);
    Talk_Run208(9, 0x3000);
    Talk_Run209(9);
    Motion_SetVarCbObj(10, 1);
    Talk_Run208(10, 0x5000);
    Talk_Run209(10);
    Talk_Run208(11, 0x8000);
    Motion_CallWaitAnim(11, 3);
    Talk_Run209(11);
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
    Talk_Run208(0, 0);
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
    Talk_Run210();
    Talk_Run207();
    Talk_Run211(64);
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Talk_Check16(5) != 0) {
        Talk_Do12(0x16d);
        Talk_Run212(5);
        Talk_Run213(3);
    }
    Talk_Run214();
    Talk_Place23(11, 0x2c80000, 0x24c0000);
    Talk_Run215(1);
    Talk_Run216(11, 1);
    Talk_Place24(11, 0x19999, 0xcccc);
    Talk_Place25(0, 0x19999, 0xcccc);
    record = Talk_Check38(11);
    {
        s32 shown = 0;

        *(volatile u16 *)(record + 6) = shown;
    }
    Talk_Run217();
    Talk_Run218(0, 2);
    Talk_Run219(11, 2);
    Talk_Place26(0, 0x30c, 0x24c);
    Talk_Place27(11, 0x32c, 0x24c);
    Talk_Place28(0, 0x370, 0x24c);
    Talk_Place29(11, 0x390, 0x24c);
    Talk_Place30(0, 0x3d4, 0x24c);
    Talk_Place31(11, 0x3f4, 0x24c);
    Talk_Run220();
    Talk_Run221();
    if (Talk_Check17(0x90f) != 0) {
        Talk_Run222(31);
    } else {
        Talk_Run223(65);
    }
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Talk_Check18(13);
    Talk_Run224();
    Talk_Run225();
    Talk_Run226();
    Talk_Run227(40);
    Talk_Run228(8, 2);
    Talk_Run229(13);
    Talk_Run230(1);
    Talk_Place32(0, 0xe000, 0);
    Talk_Run231(13, 1);
    Talk_Place33(12, 0xd000, 0);
    Talk_Run232(13, 0, 0);
    Talk_Place34(14, 0x8000, 0);
    Talk_Place35(15, 0xd000, 0);
    Talk_Place36(16, 0x8000, 0);
    Talk_Place37(17, 0xb000, 0);
    Talk_Place38(18, 0xb000, 0);
    Talk_Do13(0x2112);
    Talk_Run233(8);
    Talk_Run234(0, 3);
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
    Talk_Run235(13, 2);
    Talk_Place39(12, 0x3000, 0);
    Talk_Place40(14, 0xb000, 0);
    Talk_Place41(15, 0x5000, 0);
    Talk_Run236(16, 0, 0);
    Talk_Place42(17, 0x5000, 0);
    Talk_Place43(18, 0x5000, 0);
    Talk_Run237();
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run238();
    Talk_Place44(0, 0x19999, 0xcccc);
    Talk_Run239();
    Talk_Run240(0, 2);
    Talk_Place45(0, 0x30c, 0x1ac);
    Talk_Place46(0, 0x370, 0x1ac);
    Talk_Place47(0, 0x3d4, 0x1ac);
    Talk_Run241();
    Talk_Run242();
    if (Talk_Check19(0x90f) != 0) {
        Talk_Run243(32);
    } else {
        Talk_Run244(12);
    }
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 base5_200adac;

    Talk_Run245();
    Talk_Place48(1, 0x3180000, 0x880000);
    Talk_Place49(2, 0x3380000, 0x880000);
    Talk_Place50(3, 0x3280000, 0x980000);
    Talk_Run246();
    Talk_Run247();
    Talk_Run248(40);
    Talk_Run249(8, 1);
    Talk_Run250(8, 3);
    Talk_Do14(0x2134);
    Talk_Run251(8);
    Talk_Run252(9, 1);
    Talk_Run253(9);
    Talk_Run254(10, 1);
    Talk_Run255(10);
    Talk_Run256(11, 1);
    Talk_Run257(11, 3);
    Talk_Run258(11);
    Talk_Place51(1, 0xe000, 0);
    Talk_Place52(2, 0xa000, 20);
    Talk_Place53(1, 0x10000, 0x8000);
    Talk_Place54(2, 0x10000, 0x8000);
    Talk_Place55(3, 0x10000, 0x8000);
    base5_200adac = (s32)gOv10;
    Talk_Run259(1, base5_200adac);
    Talk_Check20(2, base5_200adac);
    Talk_Run260(3, base5_200adac);
    Talk_Run261(20);
    Talk_Run262(0, 0);
    Talk_Run263(0, 3);
    Talk_Run264(11, 3);
    Talk_Place56(11, 0x10000, 0x8000);
    Talk_Place57(0, 0x10000, 0x8000);
    Talk_Run265(11, 2);
    Talk_Place58(11, 0x33e, 152);
    Talk_Place59(11, 0x328, 164);
    Talk_Place60(11, 0x328, 0x138);
    Talk_Run266(20);
    Talk_Run10(0x6666, 0xccc);
    Talk_Run11(0x3280000, -1, 0x1380000, 1);
    Talk_Place61(0, 0x328, 164);
    Talk_Place62(0, 0x328, 0x138);
    Talk_Run267(60);
    Talk_Run268();
    Talk_Run269();
    Talk_Run270(67);
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Talk_Check21(5) != 0) {
        Talk_Do15(0x16d);
        Talk_Run271(5);
        Talk_Run272(3);
    }
    Talk_Run273();
    Talk_Place63(11, 0x3640000, 0x24c0000);
    Talk_Run274(1);
    Talk_Run275(11, 1);
    Talk_Place64(11, 0x19999, 0xcccc);
    Talk_Place65(0, 0x19999, 0xcccc);
    record = Talk_Check39(11);
    {
        s32 shown = 0x8000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Talk_Run276();
    Talk_Run277(0, 2);
    Talk_Run278(11, 2);
    Talk_Place66(0, 0x320, 0x24c);
    Talk_Place67(11, 0x300, 0x24c);
    Talk_Place68(0, 0x2bc, 0x24c);
    Talk_Place69(11, 0x29c, 0x24c);
    Talk_Place70(0, 0x258, 0x24c);
    Talk_Place71(11, 0x238, 0x24c);
    Talk_Run279();
    Talk_Run280();
    Talk_Run281(21);
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run282();
    Talk_Place72(0, 0x19999, 0xcccc);
    Talk_Run283();
    Talk_Run284(0, 2);
    Talk_Place73(0, 0x320, 0x1ac);
    Talk_Place74(0, 0x2bc, 0x1ac);
    Talk_Place75(0, 0x258, 0x1ac);
    Talk_Run285();
    Talk_Run286();
    Talk_Run287(22);
}

void Scene_RunScene3b9(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 base5_200adac;

    Talk_Run288();
    Talk_Place76(1, 0x3180000, 0x880000);
    Talk_Place77(2, 0x3380000, 0x880000);
    Talk_Place78(3, 0x3280000, 0x980000);
    Talk_Run289();
    Talk_Run290();
    Talk_Run291(40);
    Talk_Run292(8, 1);
    Talk_Run293(8, 3);
    Talk_Do16(SCENE_STEP_VALUE);
    Talk_Run294(8);
    Talk_Run295(9, 1);
    Talk_Run296(9);
    Talk_Run297(10, 1);
    Talk_Run298(10);
    Talk_Run299(11, 1);
    Talk_Run300(11, 3);
    Talk_Run301(11);
    Talk_Place79(1, 0xe000, 0);
    Talk_Place80(2, 0xa000, 20);
    Talk_Place81(1, 0x10000, 0x8000);
    Talk_Place82(2, 0x10000, 0x8000);
    Talk_Place83(3, 0x10000, 0x8000);
    base5_200adac = (s32)gOv10;
    Talk_Run302(1, base5_200adac);
    Talk_Check22(2, base5_200adac);
    Talk_Run303(3, base5_200adac);
    Talk_Run304(20);
    Talk_Run305(0, 0);
    Talk_Run306(0, 3);
    Talk_Run307(11, 3);
    Talk_Place84(11, 0x10000, 0x8000);
    Talk_Place85(0, 0x10000, 0x8000);
    Talk_Run308(11, 2);
    Talk_Place86(11, 0x33e, 152);
    Talk_Place87(11, 0x328, 164);
    Talk_Place88(11, 0x328, 0x138);
    Talk_Run309(20);
    Talk_Run12(0x6666, 0xccc);
    Talk_Run13(0x3280000, -1, 0x1380000, 1);
    Talk_Place89(0, 0x328, 164);
    Talk_Place90(0, 0x328, 0x138);
    Talk_Run310(60);
    Talk_Run311();
    Talk_Run312();
    Talk_Run313(64);
}

void ActorDraw_SetActorsTwelveToEighteen(void)
{
    Talk_Run314(12, 3);
    Talk_Run315(13, 0);
    Talk_Run316(14, 4);
    Talk_Run317(15, 1);
    Talk_Run318(16, 5);
    Talk_Run319(17, 2);
    Talk_Run320(18, 6);
    Talk_Run321(13, 10);
    Talk_Run322(14, 20);
    Talk_Run323(15, 0);
    Talk_Run324(16, 40);
    Talk_Run325(17, 50);
    Talk_Run326(18, 60);
}
