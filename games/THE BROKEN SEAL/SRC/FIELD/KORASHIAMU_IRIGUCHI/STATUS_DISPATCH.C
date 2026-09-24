#include "TYPES.H"
#include "FIELD_EVENT.H"

enum StatusDispatchMessage {
    MSG_HOPING_GET_IN_SEE_COLOSSO = 0x2057,
    MSG_COLOSSOS_LUCKY_GUESS_MAKES_BATTLES = 0x205c,
    MSG_WANT_WIN_BIG_PUT_ON = 0x205d,
    MSG_DO_KNOW_LUCKY_GUESS = 0x205e,
    MSG_SILENCE = 0x206d,
    MSG_HAVE_MAKE_THROUGH_COUNTLESS_MATCHES = 0x206e,
    MSG_HAVE_PERFECT_TECHNIQUES_LIKE_THOSE = 0x206f,
    MSG_ROBIN_ONLY_ONE_ENTERING_FINALS = 0x20f8,
    MSG_PREPARE_YOURSELVES_CONTESTANTS_FINALS_WILL = 0x2112,
    MSG_IM_RATED_AS_SECOND_BEST = 0x2114,
    MSG_WAAH_DONT_FRIGHTEN_ME = 0x2116,
    MSG_WHO_HAVE_COME_QUESTION_ME = 0x2118,
    MSG_DEKKA_MUST_WIN_FINALS_DEKKA = 0x211b,
    MSG_AM_NAVAMPA_GONDOWAN_SIXTH_RANKED = 0x211f,
    MSG_IM_BUFORD_SEVENTH_SEED = 0x2122,
    MSG_SEE_YOUVE_MADE_THROUGH_YOUR = 0x2134,
    MSG_COLOSSEUM_ALREADY_FULL_IF_REALLY = 0x2251,
    MSG_WANT_WATCH_FINALS_FROM_GOOD = 0x2254,
    MSG_HOLD_ON_SECOND = 0x2256,
    MSG_WANT_TRY_OUT_LUCKY_GUESS = 0x225a,
    MSG_LOOK_IM_SORRY_BUT_WE = 0x225e
};


#define RuntimeBlock_GetOffset1e0Pointer_1(args...) Func_02003f06(args)
#define RuntimeBlock_GetOffset1e0Pointer_2(args...) Func_02003f06(args)
#define SCENE_PHASE (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define SCENE_FIELD_1C8 (*(s32 *)(*(u8 **)0x03001ebc + 0x1c8))
#define RECORD_17 17
#define Scene_GetRecord_1(args...) Func_02004310_a(args)
#define Scene_GetRecord_2(args...) Func_0200431c(args)
#define Scene_GetRecord_3(a0) Value1(Func_02004326, a0)
#define Scene_GetRecord_4(a0) Value1(Func_020044b0, a0)
#define Scene_GetRecord_5(args...) Func_02004512(args)
#define SCENE_WORK (*(u8 **)0x03001ebc)
#define SCENE_PHASE_02001cd4 (*(s32 *)(SCENE_WORK + 0x1c0))
#define SCENE_FIELD_1C8_02001cd4 (*(s32 *)(SCENE_WORK + 0x1c8))
#define SCENE_STEP (*(u16 *)(SCENE_WORK + 0x1d8))
#define ObjectMotion_MarkActiveAndSetActionCallback Func_02004f1c

extern u8 Value_0000008c;
extern u8 Value_0000008e;
extern u8 Data_0200b094[];
extern u8 Data_0200b274[];
extern u8 Data_0200b034[];
extern u8 Data_0200b2bc[];   /* image offset 0x32bc */
extern u8 Value_0000008d;
extern u8 Data_0200be70[];
extern u8 Data_0200c110[];
extern u8 Data_0200be94[];
extern u8 Data_0200bf60[];
extern u8 Data_0200be64[];
extern u8 Data_0200adac[];
extern u8 Value_00002125[];
s32 Func_0200307a();
void Func_020021ee();
void Func_0200222e();
s32 Func_0200230a();
s32 Func_02003f06();
s32 Func_020034f2();
s32 Func_02004310_a();
s32 Func_0200431c();
s32 Func_02004326();
s32 Func_020044b0();
s32 Func_02004512();
void Func_02004fd0();
void Func_02004fd2();
s32 Func_02005042();
s32 Func_020050ea();
void Func_0200537a();
void Func_02005410();
void Func_02005412();
s32 Func_02005482();
void Func_02005676();
s32 Func_02003f28();
s32 Func_02003f34();
s32 Func_02003f3e();
s32 Func_02003128();
s32 Func_020040e4();
void Func_020047a0(void);
void Func_020047c0(s32 arg0);
void Func_020047c6(s32 arg0);
void Func_020047cc(s32 arg0);
void Func_020047ea(s32 arg0);
void Func_020047f0(s32 arg0);
void Func_020047f6(s32 arg0);
void Func_02004f1c();
void Data_0200ad0c();
void Func_020057c0_b();
void Func_020057c8_a();
void Func_020057d8_a();
void Func_020057e0_a();
void Func_020057e8();

/* resource_3b9:0x02001298, 884 bytes: the middle presentation script for
 * actor 16, a sibling of FieldScene_RunMiddleSequence (0x02001688) with the
 * same opening, scene-phase writes and closing, without the stepping loop. */

/*
 * resource_3b9 owner at 0x02001c48, 12 bytes plus one alignment halfword: a
 * two-call trampoline forwarding the argument to the first callee with a
 * fixed second argument, then making a second call with a fixed constant.
 * The argument is never freshly loaded before the first call, so it is a
 * pass-through parameter.  Neither callee is identified beyond call shape.
 */

/* FieldScene_BuildActorPresentationSequence - resource_3b9:02001cd4, 1804 bytes.
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
 *   - at owner offset 0x42c the two constant arguments of the Engine_EventOpenMessage
 *     call are set r0 before r1 where the reference sets r1 before r0;
 *   - the pc-relative load of 0x0200adac that the three action-callback calls
 *     share lands at owner offset 0x5f8, before the third
 *     ObjectMotion_SetSpeedParameters call; the reference issues it at 0x604,
 *     after that call and before its first use, which also
 *     swaps the r0/r1 setup of the first of those three calls: the reference
 *     schedule fills the load-use gap with `movs r0, #1`, ours has no gap to
 *     fill.
 *
 * Neither survived respelling. Wrapping or prototyping the Engine_EventOpenMessage site
 * leaves it unchanged. Spelling the first action-callback site through the
 * constant-passing wrapper below, or through a shared local, moves the load
 * but does not place it after the call: the wrapper form measures 8 differing
 * halfwords, and is not used here because it buys that number by emitting a
 * second `ldr r1, [pc]` the reference does not contain and dropping an
 * `adds r1, r5, #0` it does (wrong_instructions=2, class=missing-extra-code).
 */

/* Shared scene work record, reached through the pointer at 0x03001ebc. The
 * three field offsets are the ones the adopted sibling
 * games/THE BROKEN SEAL/src/overlays/scene_primary_script/run_scene_3b9_conditional_scene_setup.c
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
 * so both aliases the reference offers for it - Func_02004a36 and
 * Func_02004a72 - name two targets and neither can bind. Every other target
 * in this owner has at least one alias of its own, and is declared under it
 * exactly once.
 *
 * The one unbindable target is spelled Data_0200ad0c: a name carrying the
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

/* 0x02009c48, the owner registered as FieldScene_CallPairWith10. Its readable
 * name is bound to Func_02001c48, which is not one of this owner's call words,
 * so the call keeps its alias. Takes one record id. */

/* 0x02009c5c, the owner registered as SceneState_ForwardMaskedHalfwordWith10;
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
 * Data_0200ad0c; if an entry is ever added, declare
 * {"address": "0x0200ad0c", "kind": "thumb"} for it and the call can be
 * spelled with an ordinary Func_ name again. Nothing else in this file
 * depends on that entry existing.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* A second word at 0x1c8 of the shared scene work record, set only on the
 * flag-clear path below. */

/* Record id passed to every Func_02003exx call below. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void FieldScene_RunMiddleSequence(void);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value1_020025f0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3_02002904(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ __attribute__((always_inline)) void bump_step_020006bc(s32 amount)
{
    gEventWork->message += amount;
}

/* A call site spelled through one of these wrappers passes its constants
 * straight into the argument registers; a direct call lets the compiler
 * precompute a costly constant into a pseudo and share it with later uses in
 * the block. Most sites here want the first shape; the three action-callback
 * sites near the end want the second, because the reference reaches all three
 * of them through one shared pointer register. The adopted sibling
 * run_scene_3b9_conditional_scene_setup.c uses the same device for the same
 * reason. */
static __inline__ void Call1_02001cd4(void (*f)(), s32 a0)
{
    f(a0);
}

#if defined(TBS_EDITION_JA)
#define SCENE_STEP_VALUE 0x22c1
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define SCENE_STEP_VALUE 0x211a
#else
#define SCENE_STEP_VALUE 0x2138
#endif

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200adac[];

/*
 * resource_3b9 owner at 0x02000074: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that one pool word at
 * 0x02000078, holding the address 0x0200b2bc -- image offset 0x32bc --
 * which is returned without being dereferenced.
 */
s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000008c) {
        return (s32)Data_0200b094;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)Data_0200b274;
    }
    return (s32)Data_0200b034;
}

s32 Func_02000070(void)
{
    return 0;
}

u8 *SceneData_GetTableB2bc(void)
{
    return Data_0200b2bc;
}

s32 SceneData_SelectDataBySelectorAndSubstate(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000008d) {
        return (s32)Data_0200be70;
    }
    if (v == (s32)&Value_0000008c) {
        if (gGameState.entrance == 12) {
            return (s32)Data_0200c110;
        }
        return (s32)Data_0200be94;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)Data_0200bf60;
    }
    return (s32)Data_0200be64;
}

/* Contiguous unnamed leaf-owner run for resource_3b9. */
void SceneDialogue_RunActor10MessageByFlag962(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x962)) {
        Event_SetMessage(MSG_COLOSSEUM_ALREADY_FULL_IF_REALLY);
        Event_ShowMessage(10, 0);
    } else {
        Event_SetMessage(MSG_HOPING_GET_IN_SEE_COLOSSO);
        Event_AskYesNo(10, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor13MessageByFlag962(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x962)) {
        Actor_ShowEmote(13, 258, 40);
        Event_SetMessage(MSG_WANT_WATCH_FINALS_FROM_GOOD);
        Event_ShowMessage(13, 0);
    } else {
        Event_SetMessage(MSG_COLOSSOS_LUCKY_GUESS_MAKES_BATTLES);
        Event_ShowMessage(13, 0);
    }
    Event_End();
}

void FieldScene_RunScene3b9_02000334(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x962) != 0) {
        Actor_RunRepeatedMotion(14, 2);
        Event_SetMessage(MSG_HOLD_ON_SECOND);
        FieldScene_CallPairWith10(14);
        Actor_FaceEachOther(14, 0, 0);
        Event_Wait(20);
        Event_AskYesNo(14, 0);
        SceneState_ForwardMaskedHalfwordWith10(14, 0);
    } else {
        Event_SetMessage(MSG_WANT_WIN_BIG_PUT_ON);
        Event_ShowMessage(14, 0);
    }
    Event_End();
}

void FieldScene_RunScene3b9_0200039c(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x962) != 0) {
        if (GameFlag_IsSet(0x3c0) != 0) {
            Event_SetMessage(MSG_LOOK_IM_SORRY_BUT_WE);
        } else {
            Event_SetMessage(MSG_WANT_TRY_OUT_LUCKY_GUESS);
            Event_OpenMessage(16, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                bump_step(1);
                Actor_ShowEmote(16, 0x100, 40);
                Event_OpenMessage(16, 0);
                if (Event_ChooseYesNo(0, 0) == 0) {
                    bump_step(1);
                }
                ((void (*)())Engine_EventWait)(40);
                Event_ShowMessage(16, 0);
                GameFlag_Set(0x3c0);
                goto L_02000448;
            }
        }
        Event_ShowMessage(16, 0);
    } else {
        Event_SetMessage(MSG_DO_KNOW_LUCKY_GUESS);
        Event_AskYesNo(16, 0);
    }
    L_02000448:;
    Event_End();
}

void FieldScene_RunScene3b9_02000468(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_0200307a, 13);
    Event_Begin();
    Actor_Stop(13);
    Actor_FaceEachOther(13, 0, 20);
    Event_SetMessage(MSG_IM_RATED_AS_SECOND_BEST);
    FieldScene_CallPairWith10(13);
    Actor_RunRepeatedMotion(13, 1);
    Event_ShowMessage(13, 0);
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
    Actor_EnableActionCallback(13, 2);
    Event_End();
}

void FieldScene_RunScene3b9_020004c8(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetAttachedEffect(14, 0x102);
    Actor_RunRepeatedMotion(14, 2);
    Event_SetMessage(MSG_WAAH_DONT_FRIGHTEN_ME);
    FieldScene_CallPairWith10(14);
    Actor_ShowEmote(14, 0x102, 40);
    Event_ShowMessage(14, 0);
    Event_End();
}

void SceneDialogue_ShowLine2118WithActor15Steps(void)
{
    Event_Begin();
    Event_SetMessage(MSG_WHO_HAVE_COME_QUESTION_ME);
    FieldScene_CallPairWith10(15);
    Actor_FaceEachOther(15, 0, 20);
    FieldScene_CallPairWith10(15);
    Actor_SetAnimationAndWait(15, 3);
    Actor_SetAnimation(15, 0);
    FieldScene_CallPairWith10(15);
    SceneState_ForwardMaskedHalfwordWith10(15, 20480);
    Event_End();
}

void FieldScene_RunScene3b9_0200055c(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_RunRepeatedMotion(16, 2);
    Event_SetMessage(MSG_DEKKA_MUST_WIN_FINALS_DEKKA);
    Event_ShowMessageAndWait(16, 0, 20);
    if (GameFlag_IsSet(0x3c1) != 0) {
        Event_Wait(20);
    } else {
        Func_020021ee(17, 0);
        Actor_RunRepeatedMotion(17, 1);
        FieldScene_CallPairWith10(17);
        Actor_FaceEachOther(17, 0, 20);
        Actor_SetAnimation(17, 4);
        FieldScene_CallPairWith10(17);
        Actor_ShowEmote(17, 0x105, 40);
        FieldScene_CallPairWith10(17);
        Call2(Func_0200222e, 17, 0x5000);
        GameFlag_Set(0x3c1);
    }
    Event_End();
}

void FieldScene_RunActorSeventeenDialogueSteps(void)
{
    Event_Begin();
    Actor_FaceEachOther(17, 0, 20);
    Event_SetMessage(MSG_AM_NAVAMPA_GONDOWAN_SIXTH_RANKED);
    FieldScene_CallPairWith10(17);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(17, 3);
    FieldScene_CallPairWith10(17);
    Actor_RunRepeatedMotion(17, 1);
    FieldScene_CallPairWith10(17);
    SceneState_ForwardMaskedHalfwordWith10(17, 20480);
    Event_End();
}

void FieldScene_RunScene3b9_02000648(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_FaceEachOther(18, 0, 20);
    Event_SetMessage(MSG_IM_BUFORD_SEVENTH_SEED);
    FieldScene_CallPairWith10(18);
    Actor_FaceDirection(18, 0xd000, 20);
    Actor_FaceDirection(18, 0xb000, 20);
    Actor_FaceDirection(18, 0x8000, 40);
    Actor_FaceEachOther(18, 0, 20);
    FieldScene_CallPairWith10(18);
    Actor_SetAnimationAndWait(18, 3);
    FieldScene_CallPairWith10(18);
    Value2(Func_0200230a, 18, 0x5000);
    Event_End();
}

void FieldScene_RunScene3b9_020006bc(void)
{
    Event_Begin();
    Actor_FaceEachOther(8, 0, 20);
    Event_SetMessage((s32)&Value_00002125);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) != 0)
        bump_step_020006bc(1);
    Event_ShowMessage(8, 0);
    Event_End();
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
void SceneState_ApplyFlags565And564(void)
{
    GameFlag_Clear(0x235);
    GameFlag_Set(564);
}

/* Reads flag record 0x8a4; when set, runs one short setup on record 17.
 * When clear, runs a longer setup on record 17 plus scene phase/field
 * updates, then checks flag record 0x8a3 to pick a final call. Either path
 * ends with Engine_EventEnd(). */
void FieldScene_RunConditionalSceneSetup(void)
{
    u32 i;
    s32 flag_8a4;
    s32 record;

    Event_Begin();
    flag_8a4 = GameFlag_IsSet(0x8a4);
    if (flag_8a4 != 0) {
        Actor_FaceEachOther(RECORD_17, 0, 40);
        Event_SetMessage(MSG_HAVE_PERFECT_TECHNIQUES_LIKE_THOSE);
        FieldScene_CallPairWith10(RECORD_17);
        Actor_FaceDirection(RECORD_17, 0x3000, 20);
    } else {
        Actor_StartRepeatedMotion(RECORD_17, 2);
        Event_SetMessage(MSG_SILENCE);
        Event_ShowMessage(RECORD_17, 0);
        /* Byte at +85 of the record returned by RuntimeBlock_GetOffset1e0Pointer_1(); written
         * with the (already known zero) flag value here. */
        *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_2() + 85) = flag_8a4;
        Task_Wait(1);
        Camera_SetSpeed(0x66666, 0xcccc);
        Camera_MoveTo(0x21c0000, -1, 0xd00000, 1);
        Camera_WaitForMove();
        SCENE_PHASE = 0x200;
        SCENE_FIELD_1C8 = 32;
        Event_CloseScreen();
        Event_WaitForScreen();
        if (GameFlag_IsSet(0x8a3) != 0) {
            Event_RequestExit(70);
        } else {
            Event_RequestExit(7);
        }
    }
    Event_End();
}

void FieldScene_BuildActorPresentationGroup(void)
{
    extern u8 Data_03001ebc[];

    struct FieldActor *rec2;
    u8 *work;
    s32 shift;
    s32 phase;
    s32 field;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Audio_PlayCue(247);
    Actor_SetAnimation(8, 2);
    Actor_SetAnimation(9, 2);
    Actor_SetAnimation(10, 2);
    Actor_SetAnimation(11, 2);
    Actor_SetAnimation(12, 2);
    Actor_SetAnimation(13, 2);
    Actor_SetAnimation(14, 0);
    Actor_SetAnimation(15, 0);
    Actor_SetAnimation(16, 0);
    Actor_SetAnimation(17, 0);
    Actor_SetAnimation(18, 0);
    rec2 = Func_02003f28(21);
    Actor_SetSpriteFlags(rec2, 0);
    rec2 = Func_02003f34(19);
    rec2->scale_x = -0x10000;
    rec2 = Value1(Func_02003f3e, 20);
    rec2->scale_x = -0x10000;
    Task_Wait(1);
    Actor_SetPosition(0, 0, 0);
    Task_Wait(1);
    work = *(u8 **)Data_03001ebc;
    phase = 0x1c0;
    *(s32 *)(work + phase) = 0x200;
    field = 0x1c8;
    *(s32 *)(work + field) = 32;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(16, 1);
    Event_Wait(20);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkToAndWait(16, 164, 0x388);
    Event_Wait(20);
    Actor_SetAnimation(16, 9);
    Event_Wait(40);
    Actor_SetAnimation(16, 10);
    Event_Wait(60);
    Actor_SetAnimation(16, 1);
    Event_Wait(20);
    Actor_WalkToAndWait(16, 164, 0x398);
    Actor_WalkToAndWait(16, 185, 0x398);
    Actor_FaceDirection(16, 0xc000, 20);
    Actor_WalkToAndWait(16, 185, 0x394);
    Actor_SetAnimation(16, 11);
    Event_Wait(40);
    Actor_RunRepeatedMotion(16, 1);
    Event_Wait(60);
    Actor_RunRepeatedMotion(16, 3);
    Event_Wait(40);
    Actor_EnableActionCallback(16, 0x200af88);
    Event_Wait(80);
    Actor_SetAttachedEffect(16, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_FaceDirection(15, 0x5000, 0);
    Actor_FaceDirection(17, 0, 0);
    Actor_FaceDirection(18, 0x8000, 20);
    Actor_SetAttachedEffect(14, 0x102);
    Actor_SetAttachedEffect(15, 0x102);
    Actor_SetAttachedEffect(17, 0x102);
    Actor_SetAttachedEffect(18, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(14, 0x3000, 0);
    Actor_FaceDirection(15, 0x3000, 0);
    Actor_FaceDirection(17, 0x3000, 0);
    shift = 0x3000;
    Value2(Func_02003128, 18, shift);
    Actor_Stop(16);
    rec2 = Func_020040e4(16);
    rec2->facing = 0xd000;
    rec2->scale_x = 0x10000;
    rec2->scale_y = 0x10000;
    Event_Wait(20);
    Actor_SetAnimation(16, 0);
    Event_Wait(40);
    Actor_SetAnimation(19, 5);
    Actor_SetAnimation(20, 5);
    Event_Wait(60);
    Actor_FaceDirection(16, shift, 20);
    Actor_SetAnimation(16, 8);
    Event_Wait(20);
    Actor_SetAnimation(14, 4);
    Actor_SetAnimation(15, 4);
    Actor_SetAnimation(17, 4);
    Actor_SetAnimationAndWait(18, 4);
    Event_Wait(40);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(10);
    Actor_SetSpeed(16, 0x20000, 0x10000);
    Actor_WalkToAndWait(16, 162, 0x394);
    Actor_WalkToAndWait(16, 162, 0x37a);
    Actor_SetAnimation(19, 1);
    Actor_SetAnimation(20, 1);
    Actor_WalkToAndWait(16, 184, 0x35f);
    Actor_WalkToAndWait(16, 184, 0x31c);
    Actor_SetPosition(16, 0, 0);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + phase) = 0x201;
    *(s32 *)(work + field) = 16;
    Event_CloseScreen();
    Event_WaitForScreen();
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + phase) = 0x100;
    Event_RequestExit(69);
    Event_End();
}

void SceneState_SetRuntimeWord448To513(void)
{
    extern u8 *Data_03001ebc;

    Event_Begin();
    *(s32 *)((u8 *)Data_03001ebc + 448) = 513;

    Event_OpenScreen();
    Event_WaitForScreen();

    Event_Wait(20);
    SceneState_ForwardMaskedHalfwordWith10(17, 160 << 7);
    Event_SetMessage(MSG_HAVE_MAKE_THROUGH_COUNTLESS_MATCHES);

    if (GameFlag_IsSet(0x8a4) != 0) {
        *(u16 *)((u8 *)Data_03001ebc + 472) =
            (u16)(*(u16 *)((u8 *)Data_03001ebc + 472) + 1);
    }

    FieldScene_CallPairWith10(17);
    SceneState_ForwardMaskedHalfwordWith10(17, 192 << 6);
    GameFlag_Set(0x8a3);

    Event_End();
}

/* Sets up actors 8-20 (position, pose, or movement/sprite flags), advances
 * two actor records' +24 fields, advances the shared scene phase, then runs
 * a long chain of per-actor moves, pose changes, and waits. */
void FieldScene_RunMiddleSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 counter;
    struct FieldActor *rec;
    struct FieldActor *rec2;
    u8 *work;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Audio_PlayCue(247);
    Actor_SetAnimation(8, 2);
    Call2((void (*)())Engine_ActorSetAnimation, 9, 2);
    Actor_SetAnimation(10, 2);
    Actor_SetAnimation(11, 2);
    Actor_SetAnimation(12, 2);
    Actor_SetAnimation(13, 2);
    Actor_SetAnimation(14, 0);
    Actor_SetAnimation(15, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetAnimation(17, 0);
    Actor_SetAnimation(18, 0);
    rec2 = Scene_GetRecord_1(21);
    Actor_SetSpriteFlags(rec2, 0);
    rec2 = Scene_GetRecord_2(19);
    rec2->scale_x = -0x10000;
    rec2 = Scene_GetRecord_3(20);
    rec2->scale_x = -0x10000;
    Task_Wait(1);
    Actor_SetPosition(0, 0, 0);
    Task_Wait(1);
    /* Advance the scene phase/status word at +0x1c0 and its companion word
     * at +0x1c8 of the shared scene work record. */
    work = *(u8 **)Data_03001ebc;
    *(s32 *)((work + 0x1c0)) = 0x200;
    *(s32 *)((work + 0x1c8)) = 32;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(17, 1);
    Event_Wait(20);
    Actor_SetSpeed(17, 0xcccc, 0x6666);
    Actor_WalkToAndWait(17, 164, 0x388);
    Event_Wait(20);
    Actor_SetAnimation(17, 9);
    Event_Wait(40);
    Actor_SetAnimation(17, 10);
    Event_Wait(60);
    Actor_SetAnimation(17, 1);
    Event_Wait(20);
    Actor_WalkToAndWait(17, 164, 0x398);
    Actor_WalkToAndWait(17, 185, 0x398);
    Actor_FaceDirection(17, 0xc000, 20);
    Actor_WalkToAndWait(17, 185, 0x394);
    Actor_SetAnimation(17, 11);
    Event_Wait(40);
    Actor_RunRepeatedMotion(17, 1);
    Event_Wait(60);
    Actor_RunRepeatedMotion(17, 3);
    Event_Wait(40);
    Actor_EnableActionCallback(17, 0x200af88);
    Event_Wait(80);
    Actor_SetAttachedEffect(17, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_FaceDirection(15, 0x5000, 0);
    Actor_FaceDirection(18, 0x8000, 20);
    Actor_SetAttachedEffect(14, 0x102);
    Actor_SetAttachedEffect(15, 0x102);
    Actor_SetAttachedEffect(17, 0x102);
    Actor_SetAttachedEffect(18, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(14, 0x3000, 0);
    Actor_FaceDirection(15, 0x3000, 0);
    Value2(Func_020034f2, 18, 0x3000);
    Actor_SetAttachedEffect(17, 0x101);
    /* Clear the motion flags, then step the height up and back down
     * 20 times, waiting between each step. */
    rec = Scene_GetRecord_4(21);
    rec->motion_flags = 0;
    for (counter = 0; counter < 20; counter++) {
        rec->y.fixed += 0x9999;
        Task_Wait(4);
        rec->y.fixed += -0x4ccc;
        Task_Wait(4);
    }
    Actor_SetAnimation(19, 6);
    Actor_SetAnimation(20, 6);
    Event_Wait(60);
    Actor_SetAttachedEffect(17, 0x100);
    Actor_Stop(17);
    Actor_SetAnimation(17, 1);
    rec2 = Scene_GetRecord_5(17);
    rec2->facing = 0xd000;
    rec->motion_flags = 3;
    rec->scale_x = 0x10000;
    rec->scale_y = 0x10000;
    Event_Wait(10);
    Audio_PlayCue(107);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(10);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_SetSpeed(17, 0x19999, 0xcccc);
    Actor_WalkToAndWait(17, 208, 0x3a0);
    Audio_PlayCue(92);
    Actor_FaceDirection(17, 0x3000, 20);
    Actor_SetAnimation(17, 9);
    Event_Wait(20);
    Actor_SetAnimation(17, 10);
    Event_Wait(40);
    Actor_SetAnimation(17, 9);
    Event_Wait(20);
    Actor_SetAnimation(17, 10);
    Event_Wait(80);
    /* Advance the scene phase/status word at +0x1c0 and its companion word
     * at +0x1c8 of the shared scene work record. */
    work = *(u8 **)Data_03001ebc;
    *(s32 *)((work + 0x1c0)) = 0x201;
    *(s32 *)((work + 0x1c8)) = 16;
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x8a4);
    Event_RequestExit(69);
    Event_End();
}

s32 SceneState_DispatchByStatus448(void)
{
    extern u8 Data_02000240[];
    s32 off = 448;
    s16 status = *(s16 *)(Data_02000240 + off);

    if (status == (s32)&Value_0000008c) {
        FieldScene_DispatchBySelector();
    } else if (status == (s32)&Value_0000008e) {
        SceneState_ApplyRectsByFlatla384And962();
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
 * Engine_ActorSetAnimation and Engine_GameFlagSet share an encoded word but reach different
 * veneers, so the two declarations must stay distinct; the suffix marks the
 * second call word, not a second entry point. */

/*
 * The case order is load-bearing and it is not the selector order: the arms
 * are laid out in the order the reference places their bodies, and the jump
 * table stores their absolute addresses. Each callee name is keyed to the
 * address of the instruction that calls it, so an arm moved out of place
 * emits the wrong call word.
 */
void FieldScene_DispatchBySelector(void)
{
    extern u8 Data_02000240[];

    s32 no;

    Task_Wait(1);
    {
        s32 off = 450;

        no = *(s16 *)(Data_02000240 + off);
    }
    switch (no) {
    case 5:
        Actor_SetAnimation(8, 2);
        Actor_SetAnimation(9, 2);
        break;
    case 69:
        Actor_SetAnimation(8, 2);
        Actor_SetAnimation(9, 2);
        if (GameFlag_IsSet(0x109) != 0)
            break;
        SceneState_SetRuntimeWord448To513();
        break;
    case 7:
        FieldScene_BuildActorPresentationGroup();
        break;
    case 70:
        FieldScene_RunMiddleSequence();
        break;
    case 64:
        FieldScene_BuildActorPresentationSequence();
        Func_020047a0();
        break;
    case 65:
        FieldScene_RunScene3b9_020023e0();
        break;
    case 66:
        FieldScene_RunScene3b9_020025f0();
        break;
    case 12:
        GameFlag_Set(324);
        ActorPresentation_SetActorsTwelveToEighteen();
        if (GameFlag_IsSet(0x109) != 0)
            break;
        FieldScene_RunScene3b9_020024d8();
        break;
    case 21:
        Func_020047c0(1);
        Func_020047c6(2);
        Func_020047cc(3);
        GameFlag_Set(0x90e);
        FieldScene_RunScene3b9_02002668();
        break;
    case 67:
        FieldScene_RunScene3b9_02002820();
        break;
    case 68:
        FieldScene_RunScene3b9_02002904();
        break;
    case 31:
        Func_020047ea(1);
        Func_020047f0(2);
        Func_020047f6(3);
        GameFlag_Set(0x90f);
        FieldScene_RunScene3b9_02002964();
        break;
    default:
        break;
    }
}

void FieldScene_CallPairWith10(s32 no)
{
    Event_ShowMessage(no, 0);
    Event_Wait(10);
}

void SceneState_ForwardMaskedHalfwordWith10(s32 arg0, s32 arg1)
{
    Actor_FaceDirection(arg0, (u16)arg1, 10);
}

/* resource_3b9 owner at 0x02001c6c, 100 bytes. Two alternative layouts. */
void SceneState_ApplyRectsByFlatla384And962(void)
{
    s32 pair;
    s32 a, b;

    if (GameFlag_IsSet(2384) != 0) {
        pair = 2;
        Map_CopyCellsTo(64, 0, 48, 5, pair, pair);
        a = 16;
        b = 8;
        Map_CopyCellAttributes(14, 8, 2, 1, a, b);
    } else {
        Actor_SetChildValue(16, 2);
        if (GameFlag_IsSet(0x962) != 0) {
            a = 14;
            b = 11;
            Map_CopyCellAttributes(30, 22, 1, 2, a, b);
        }
    }
}

void FieldScene_BuildActorPresentationSequence(void)
{
    s32 flag;

    Event_Begin();
    Actor_SetPosition(1, 0x3180000, 0x880000);
    Actor_SetPosition(2, 0x3380000, 0x880000);
    Actor_SetPosition(3, 0x3280000, 0x980000);
    Task_Wait(1);
    Camera_MoveTo(-1, -1, -1, 0);
    Data_0200ad0c(0, 0);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
    SCENE_PHASE_02001cd4 = 515;
    SCENE_FIELD_1C8_02001cd4 = 1;
    Event_OpenScreen();
    Event_WaitForScreen();
    Data_0200ad0c(0, 0);
    ColorBuffer_ApplyTarget(0x10002, 0);
    ColorBuffer_Interpolate(40);
    Event_Wait(80);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(40);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(40);
    Event_Wait(80);
    Actor_ShowEmote(2, 0x100, 0);
    Actor_RunRepeatedMotion(2, 1);
    Event_Wait(20);
    Event_SetMessage(MSG_ROBIN_ONLY_ONE_ENTERING_FINALS);
    FieldScene_CallPairWith10(2);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_CallPairWith10(8);
    Actor_ShowEmote(3, 0x102, 40);
    FieldScene_CallPairWith10(3);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0, 40);
    Actor_FaceDirection(8, 0x5000, 0);
    SceneState_ForwardMaskedHalfwordWith10(9, 0x3000);
    Actor_ShowEmote(9, 0x105, 20);
    FieldScene_CallPairWith10(9);
    Actor_ShowEmote(1, 0x103, 20);
    Actor_StartRepeatedMotion(1, 2);
    FieldScene_CallPairWith10(1);
    Actor_ShowEmote(10, 0x102, 40);
    FieldScene_CallPairWith10(10);
    Actor_SetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    SceneState_ForwardMaskedHalfwordWith10(2, 0xa000);
    Actor_SetAnimationAndWait(2, 4);
    FieldScene_CallPairWith10(2);
    Actor_RunRepeatedMotion(3, 1);
    FieldScene_CallPairWith10(3);
    Actor_ShowEmote(1, 0x103, 20);
    SceneState_ForwardMaskedHalfwordWith10(1, 0);
    Actor_StartRepeatedMotion(1, 2);
    FieldScene_CallPairWith10(1);
    SceneState_ForwardMaskedHalfwordWith10(0, 0x6000);
    Actor_ShowEmote(0, 0x102, 60);
    Actor_RunRepeatedMotion(2, 1);
    SceneState_ForwardMaskedHalfwordWith10(2, 0x8000);
    FieldScene_CallPairWith10(2);
    Actor_SetAnimation(3, 4);
    Event_Wait(20);
    FieldScene_CallPairWith10(3);
    Actor_RunRepeatedMotion(1, 1);
    SceneState_ForwardMaskedHalfwordWith10(1, 0x2000);
    FieldScene_CallPairWith10(1);
    Actor_ShowEmote(2, 0x105, 60);
    Actor_FaceDirection(2, 0x2000, 0);
    Actor_FaceDirection(1, 0, 0);
    Actor_FaceDirection(3, 0xe000, 20);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Actor_ShowEmote(3, 0x102, 80);
    Actor_ShowEmote(2, 0x106, 0);
    Actor_RunRepeatedMotion(2, 1);
    SceneState_ForwardMaskedHalfwordWith10(2, 0xc000);
    FieldScene_CallPairWith10(2);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 20);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    Actor_SetAnimationAndWait(8, 3);
    FieldScene_CallPairWith10(8);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(3, 0x100, 40);
    Actor_RunRepeatedMotion(9, 1);
    FieldScene_CallPairWith10(9);
    Actor_FaceDirection(0, 0x6000, 0);
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0x6000, 0);
    Actor_FaceDirection(3, 0xe000, 40);
    Actor_RunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    SceneState_ForwardMaskedHalfwordWith10(3, 0xc000);
    Actor_SetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(0, 0x6000, 0);
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0x6000, 0);
    Actor_FaceDirection(3, 0xe000, 20);
    Actor_ShowEmote(1, 0x105, 0);
    Actor_ShowEmote(2, 0x105, 0);
    Actor_ShowEmote(3, 0x105, 80);
    Actor_ShowEmote(2, 0x106, 0);
    Actor_RunRepeatedMotion(2, 1);
    SceneState_ForwardMaskedHalfwordWith10(2, 0xe000);
    FieldScene_CallPairWith10(2);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(1, 0, 20);
    Actor_ShowEmote(11, 0x101, 60);
    Event_OpenMessage(2, 0);
    Actor_FaceDirection(0, 0x2000, 0);
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0xa000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    flag = 0;
    if (Event_ChooseYesNo(0, 0) == 1) {
        FieldScene_CallPairWith10(2);
        flag = 1;
    } else {
        SCENE_STEP++;
        Actor_SetAnimationAndWait(2, 3);
        SceneState_ForwardMaskedHalfwordWith10(2, 0xc000);
        FieldScene_CallPairWith10(2);
    }
    if (flag != 0) {
        SCENE_STEP++;
    }
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0, 0);
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(11, 0xb000, 20);
    Actor_ShowEmote(8, 0x105, 0);
    Actor_ShowEmote(9, 0x105, 0);
    Actor_ShowEmote(10, 0x105, 0);
    Actor_ShowEmote(11, 0x105, 60);
    Actor_RunRepeatedMotion(8, 1);
    SceneState_ForwardMaskedHalfwordWith10(8, 0x3000);
    FieldScene_CallPairWith10(8);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 20);
    Actor_RunRepeatedMotion(9, 1);
    SceneState_ForwardMaskedHalfwordWith10(9, 0x3000);
    FieldScene_CallPairWith10(9);
    Actor_RunRepeatedMotion(10, 1);
    SceneState_ForwardMaskedHalfwordWith10(10, 0x5000);
    FieldScene_CallPairWith10(10);
    SceneState_ForwardMaskedHalfwordWith10(11, 0x8000);
    Actor_SetAnimationAndWait(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0xa000, 20);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(3, 3);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Actor_SetSpeed(3, 0x10000, 0x8000);
    Actor_EnableActionCallback(1, (s32)Data_0200adac);
    Actor_EnableActionCallback(2, (s32)Data_0200adac);
    ObjectMotion_MarkActiveAndSetActionCallback(3, (s32)Data_0200adac);
    Event_Wait(20);
    SceneState_ForwardMaskedHalfwordWith10(0, 0);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(11, 3);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_SetAnimation(11, 2);
    Actor_MoveToAndWait(11, 830, 152);
    Actor_MoveToAndWait(11, 808, 164);
    Actor_SetDestination(11, 808, 312);
    Event_Wait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
    Actor_WalkToAndWait(0, 808, 164);
    Actor_WalkTo(0, 808, 312);
    Event_Wait(60);
    SCENE_PHASE_02001cd4 = 256;
    SCENE_FIELD_1C8_02001cd4 = 40;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(64);
}

void FieldScene_RunScene3b9_020023e0(void)
{
    u32 i;
    struct FieldActor *record;

    if (GameFlag_IsSet(5) != 0) {
        GameFlag_Set(0x16d);
        Func_02004fd2(5);
        Func_02004fd0(3);
    }
    Event_Begin();
    Actor_SetPosition(11, 0x2c80000, 0x24c0000);
    Task_Wait(1);
    Camera_FollowActor(11, 1);
    Actor_SetSpeed(11, 0x19999, 0xcccc);
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    record = Func_02005042(11);
    record->facing = 0;
    Event_OpenScreen();
    Actor_SetAnimation(0, 2);
    Actor_SetAnimation(11, 2);
    Actor_SetDestination(0, 0x30c, 0x24c);
    Actor_MoveToAndWait(11, 0x32c, 0x24c);
    Actor_SetDestination(0, 0x370, 0x24c);
    Actor_MoveToAndWait(11, 0x390, 0x24c);
    Actor_SetDestination(0, 0x3d4, 0x24c);
    Actor_SetDestination(11, 0x3f4, 0x24c);
    Event_CloseScreen();
    Event_WaitForScreen();
    if (GameFlag_IsSet(0x90f) != 0) {
        Event_RequestExit(31);
    } else {
        Event_RequestExit(65);
    }
}

void FieldScene_RunScene3b9_020024d8(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_020050ea, 13);
    Event_Begin();
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(8, 2);
    Actor_Stop(13);
    Task_Wait(1);
    Actor_FaceDirection(0, 0xe000, 0);
    Actor_SetAnimation(13, 1);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(13, 0, 0);
    Actor_FaceDirection(14, 0x8000, 0);
    Actor_FaceDirection(15, 0xd000, 0);
    Actor_FaceDirection(16, 0x8000, 0);
    Actor_FaceDirection(17, 0xb000, 0);
    Actor_FaceDirection(18, 0xb000, 0);
    Event_SetMessage(MSG_PREPARE_YOURSELVES_CONTESTANTS_FINALS_WILL);
    FieldScene_CallPairWith10(8);
    Actor_SetAnimationAndWait(0, 3);
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
    Actor_EnableActionCallback(13, 2);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(15, 0x5000, 0);
    Actor_FaceDirection(16, 0, 0);
    Actor_FaceDirection(17, 0x5000, 0);
    Actor_FaceDirection(18, 0x5000, 0);
    Event_End();
}

void FieldScene_RunScene3b9_020025f0(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    Event_OpenScreen();
    Actor_SetAnimation(0, 2);
    Actor_MoveToAndWait(0, 0x30c, 0x1ac);
    Actor_MoveToAndWait(0, 0x370, 0x1ac);
    Actor_SetDestination(0, 0x3d4, 0x1ac);
    Event_CloseScreen();
    Event_WaitForScreen();
    if (GameFlag_IsSet(0x90f) != 0) {
        Event_RequestExit(32);
    } else {
        Event_RequestExit(12);
    }
}

void FieldScene_RunScene3b9_02002668(void)
{
    u32 i;
    s32 record;
    s32 base5_200adac;

    Event_Begin();
    Actor_SetPosition(1, 0x3180000, 0x880000);
    Actor_SetPosition(2, 0x3380000, 0x880000);
    Actor_SetPosition(3, 0x3280000, 0x980000);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimation(8, 3);
    Event_SetMessage(MSG_SEE_YOUVE_MADE_THROUGH_YOUR);
    FieldScene_CallPairWith10(8);
    Actor_RunRepeatedMotion(9, 1);
    FieldScene_CallPairWith10(9);
    Actor_RunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10);
    Actor_RunRepeatedMotion(11, 1);
    Actor_SetAnimation(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0xa000, 20);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Actor_SetSpeed(3, 0x10000, 0x8000);
    base5_200adac = (s32)Data_0200adac;
    Actor_EnableActionCallback(1, base5_200adac);
    Value2(Engine_ActorEnableActionCallback, 2, base5_200adac);
    Func_0200537a(3, base5_200adac);
    Event_Wait(20);
    SceneState_ForwardMaskedHalfwordWith10(0, 0);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(11, 3);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_SetAnimation(11, 2);
    Actor_MoveToAndWait(11, 0x33e, 152);
    Actor_MoveToAndWait(11, 0x328, 164);
    Actor_SetDestination(11, 0x328, 0x138);
    Event_Wait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
    Actor_WalkToAndWait(0, 0x328, 164);
    Actor_WalkTo(0, 0x328, 0x138);
    Event_Wait(60);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(67);
}

void FieldScene_RunScene3b9_02002820(void)
{
    u32 i;
    struct FieldActor *record;

    if (GameFlag_IsSet(5) != 0) {
        GameFlag_Set(0x16d);
        Func_02005412(5);
        Func_02005410(3);
    }
    Event_Begin();
    Actor_SetPosition(11, 0x3640000, 0x24c0000);
    Task_Wait(1);
    Camera_FollowActor(11, 1);
    Actor_SetSpeed(11, 0x19999, 0xcccc);
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    record = Func_02005482(11);
    record->facing = 0x8000;
    Event_OpenScreen();
    Actor_SetAnimation(0, 2);
    Actor_SetAnimation(11, 2);
    Actor_SetDestination(0, 0x320, 0x24c);
    Actor_MoveToAndWait(11, 0x300, 0x24c);
    Actor_SetDestination(0, 0x2bc, 0x24c);
    Actor_MoveToAndWait(11, 0x29c, 0x24c);
    Actor_SetDestination(0, 0x258, 0x24c);
    Actor_SetDestination(11, 0x238, 0x24c);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(21);
}

void FieldScene_RunScene3b9_02002904(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    Event_OpenScreen();
    Actor_SetAnimation(0, 2);
    Actor_MoveToAndWait(0, 0x320, 0x1ac);
    Actor_MoveToAndWait(0, 0x2bc, 0x1ac);
    Actor_SetDestination(0, 0x258, 0x1ac);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(22);
}

void FieldScene_RunScene3b9_02002964(void)
{
    u32 i;
    s32 record;
    s32 base5_200adac;

    Event_Begin();
    Actor_SetPosition(1, 0x3180000, 0x880000);
    Actor_SetPosition(2, 0x3380000, 0x880000);
    Actor_SetPosition(3, 0x3280000, 0x980000);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimation(8, 3);
    Event_SetMessage(SCENE_STEP_VALUE);
    FieldScene_CallPairWith10(8);
    Actor_RunRepeatedMotion(9, 1);
    FieldScene_CallPairWith10(9);
    Actor_RunRepeatedMotion(10, 1);
    FieldScene_CallPairWith10(10);
    Actor_RunRepeatedMotion(11, 1);
    Actor_SetAnimation(11, 3);
    FieldScene_CallPairWith10(11);
    Actor_FaceDirection(1, 0xe000, 0);
    Actor_FaceDirection(2, 0xa000, 20);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Actor_SetSpeed(3, 0x10000, 0x8000);
    base5_200adac = (s32)Data_0200adac;
    Actor_EnableActionCallback(1, base5_200adac);
    Value2(Engine_ActorEnableActionCallback, 2, base5_200adac);
    Func_02005676(3, base5_200adac);
    Event_Wait(20);
    SceneState_ForwardMaskedHalfwordWith10(0, 0);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(11, 3);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_SetAnimation(11, 2);
    Actor_MoveToAndWait(11, 0x33e, 152);
    Actor_MoveToAndWait(11, 0x328, 164);
    Actor_SetDestination(11, 0x328, 0x138);
    Event_Wait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3280000, -1, 0x1380000, 1);
    Actor_WalkToAndWait(0, 0x328, 164);
    Actor_WalkTo(0, 0x328, 0x138);
    Event_Wait(60);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(64);
}

void ActorPresentation_SetActorsTwelveToEighteen(void)
{
    Actor_SetChildValue(12, 3);
    Actor_SetChildValue(13, 0);
    Actor_SetChildValue(14, 4);
    Actor_SetChildValue(15, 1);
    Actor_SetChildValue(16, 5);
    Actor_SetChildValue(17, 2);
    Actor_SetChildValue(18, 6);
    Func_020057c0_b(13, 10);
    Func_020057c8_a(14, 20);
    Actor_SetAnimation(15, 0);
    Func_020057d8_a(16, 40);
    Func_020057e0_a(17, 50);
    Func_020057e8(18, 60);
}
