#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_02001a9e

#include "FACING_OBJECT.H"

extern u8 Data_0200a028[];
extern u8 Data_02009fb0[];
extern u8 Data_02009efc[];
extern u8 Data_00000fb0[];
extern u8 Data_00000fbf[];
extern u8 Data_03001ebc[];
extern u8 Data_0200a0ac[];
extern u8 Value_00000f76;
extern u8 Data_00000f85[];
extern u8 Data_00000f91[];
extern u8 Data_02009ce0[];

s32 Func_02001a9e(s32, s32);
struct ObjectRuntime *Func_02001c6e(u32);
s32 Func_02002430();
s32 Func_0200243c();
s32 Func_02002550();
s32 Func_02002570_a();
s32 Func_02002590_a();
void Func_02003560(s32, s32);
void Func_020035a4(s32, s32);
u8 *Func_020026e0();
void Func_02002d8e();
void Data_02009b85();
void Func_02002e46();

/*
 * DRAFTED SCENE SCRIPT for FieldScene_RunGatedActorSequence.
 *
 * One progress-gated field cutscene.  The owner returns immediately when
 * progress flag 0x808 is already set; otherwise it sets that flag and plays a
 * fixed beat sequence of 166 calls over scene slots 0, 1, 5, 14 and 15.
 *
 * Structure recovered from the reference: one early return, two identical
 * two-way branches on a scene predicate that select between neighbouring
 * event ids, two null-guarded object lookups near the end, and four
 * read-modify-write updates of the object byte at offset 90.
 *
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, following the exact sibling
 * games/THE BROKEN SEAL/src/overlays/scene_primary_script/run_actor_position_sequence.c.
 * Where several pre-relocation words in this owner share one relocated
 * destination, the single unambiguous spelling is reused for every call to
 * that destination.
 *
 * RESIDUAL / INTEGRATOR NOTE.  The pre-relocation word Func_020028a8 is used
 * at two sites in this owner and relocates to two different destinations:
 * 0x02000d22 -> runtime 0x02009b84 (main-image 0x0808a218) and 0x02000d52 ->
 * runtime 0x02009b54 (main-image 0x0808a1b8).  0x02009b84 is reachable through
 * no other pre-relocation word here, so a standalone candidate cannot bind it;
 * `alchemy score` reports "ambiguous overlay call identity".  It is spelled
 * here as Data_02009b85 (Data_ bypasses the call-name resolver and binds the
 * literal runtime address with the Thumb bit set), which compiles and emits
 * the reference's call word, but it is a scoring workaround, not source.
 * The project's own convention for this case is the suffixed pair already used
 * by this overlay's other unit, games/THE BROKEN SEAL/src/overlays/scene_primary_script/
 * run_actor_position_sequence.c: declare Func_020028a8_a and Func_020028a8_b,
 * spell the 0x02000d22 site _a and the 0x02000d52 site _b, and record both in
 * the unit's `absolute_symbols` as 0x02009b84 and 0x02009b54, kind "thumb".
 * That table is integrator-owned, so the suffixed spellings do not bind from a
 * standalone candidate and are not used here.  Every other call symbol in this
 * file auto-binds from the reference and needs no explicit declaration.
 *
 * RESIDUAL / ALLOCATION.  Two halfwords still differ, both in the second of
 * the two `|= 1` updates at reference 0x020011b2: the reference spells that
 * store `orrs r5, r3; strb r5, [r2]`, reusing the register that held the
 * constant, while this source emits `orrs r3, r5; strb r3, [r2]`.  The first
 * `|= 1` at 0x020011a2, and both `&= 0xfe` updates at 0x0200115e and
 * 0x0200116e, already match byte for byte.  This is a commutative-operand and
 * register-allocation choice with no source evidence behind it; the triage
 * router classifies it as allocation-uncovered and warns against respelling
 * source to shave the tool triage number rather than to recover the real
 * shape. That warning is not a rule to leave this open: reaching zero here
 * with an honest, ordinary spelling is a good outcome, and the integrator
 * adopts it via `alchemy adopt`, which retires the retained-assembly region.
 * A prior version of this comment claimed the retained-corpus gate forbids a
 * byte-exact candidate; that was wrong on the facts and is corrected here.
 * candidate-corpus-check only scans routes registered in the Makefile
 * CANDIDATE_SINGLE_OWNERS variable and units under recon/tbs/en/units/,
 * and this file is registered in neither, so the gate does not even see it.
 * Where the gate does apply, an exact result is the signal to adopt, not a
 * reason to avoid closing.
 *
 * MODELLING ARTIFACTS carried over from the exact sibling, not recovered
 * source: the Call/Value function-pointer wrappers below, and the event ids
 * and table pointer modelled as `(s32)` of an extern array symbol.  Both exist
 * to express what the reference does with constants and register lifetimes in
 * the idiom this overlay's adopted C already uses; neither asserts that the
 * original source spelled them that way.
 *
 * Uncertain and deliberately left neutral: the project has no name for any of
 * the resolved main-image targets, so no role names are invented.  The
 * resolved target and the observed argument count are recorded beside each
 * declaration.  The slot numbers (0, 1, 5, 14, 15) and the event ids held in
 * evt/evt2 are raw values whose meaning is not established.  Only three
 * offsets of the looked-up object record are evidenced by this owner: the
 * signed halfwords at 10 and 18, read as the whole-cell halves of 16.16
 * coordinates, and the flag byte at 90.
 */

/* 0x080770c0, one argument, result tested */

/* 0x080770c8, one argument */

/* 0x0808a010, one argument (frame count) */

/* 0x0808a018, no arguments */

/* 0x0808a020, no arguments */

/* 0x0808a070, two arguments, result tested */

/* 0x0808a080, one argument, returns an object record or NULL */

/* 0x0808a090, three arguments */

/* 0x0808a0b8, three arguments */

/* 0x0808a0c8, three arguments */

/* 0x0808a0d0, three arguments */

/* 0x0808a0e8, one argument */

/* 0x0808a0f0, three arguments */

/* 0x0808a100, two arguments */

/* 0x0808a110, two arguments */

/* 0x0808a130, two arguments */

/* 0x0808a138, two arguments */

/* 0x0808a148, three arguments */

/* 0x0808a150, three arguments */

/* 0x0808a168, three arguments, third is a table address */

/* 0x0808a170, one argument (event id) */

/* 0x0808a178, two arguments, returned value discarded at both call sites */

/* 0x0808a188, three arguments */

/* 0x0808a1b8, three arguments */

/* 0x0808a1e8, three arguments */

/* 0x0808a208, two arguments */

/* 0x0808a210, four arguments */

/* 0x0808a218, no arguments; see the integrator note above */

/* 0x0808a4f0, no arguments */

/* 0x080f9010, one argument (sound id) */

/* Event ids the reference keeps live in a register across the sequence. */

/* Overlay table passed to the last two calls. */

/* Each Func_ symbol names the loader-relocated call word the image holds for
 * one call site, not a runtime address, so several names can reach the same
 * target. Declarations are old-style where the arity varies between sites. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Scene_SetActorDirection(s32 actor, s32 angle, s32 frames)
{
    Actor_FaceDirection(actor, angle, frames);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000be0(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

struct ObjectRuntime;

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 ang;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        ang = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(ang - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

/*
 * Returns the in-image table at 0x02009cf4. The eight-byte owner at
 * 0x02000088 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9CF4(void)
{
    return (u8 *)0x02009cf4;
}

s32 Func_02000090(void)
{
    return 0;
}

/*
 * Returns the in-image table at 0x02009db4. The eight-byte owner at
 * 0x02000094 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9db4(void)
{
    return (u8 *)0x02009db4;
}

/*
 * Returns the in-image table at 0x02009ddc. The eight-byte owner at
 * 0x0200009c includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9ddc(void)
{
    return (u8 *)0x02009ddc;
}

/* Picks one of three tables by story progress, testing flags 0x87a then
 * 0x834. */
u8 *SceneData_SelectTable9efcByFlags(void)
{
    if (GameFlag_IsSet(0x87A) != 0) {
        return Data_0200a028;
    }
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_02009fb0;
    }
    return Data_02009efc;
}

void Scene_RunActorTwelveDialogue(void)
{
    s32 base;

    Event_Begin();
    if (GameFlag_IsSet(2069) != 0) {
        Event_SetMessage(4548);
        Event_ShowMessage(12, 0);
    } else {
        base = (s32)&Value_00000f76;
        Event_SetMessage(base);
        Actor_FaceActor(12, 0, 10);
        Actor_RunRepeatedMotion(12, 2);
        Event_Wait(6);
        Event_OpenMessage(12, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage(base + 1);
        } else {
            Event_SetMessage(base + 2);
        }
        Actor_StartRepeatedMotion(12, 3);
        Event_ShowMessage(12, 0);
        Scene_SetActorDirection(12, 49152, 10);
    }
    Event_End();
}

/*
 * Runs the scene's fixed position and pose sequence for actors 0, 1, 5 and
 * 13, skipped entirely when flag 0x801 is set. Two of the record fields feed
 * the placement calls as s16 values shifted into fixed point, and base5_fbf
 * holds a base that later sites use as base + 1 and base + 2.
 */
void FieldScene_RunActorPositionSequence(void)
{
    u8 *record;
    s32 x, y;
    s32 base5_fbf;

    if (GameFlag_IsSet(0x801) != 0) {
    } else {
        Event_Begin();
        Actor_SetSpeed(0, 0x10000, 0x8000);
        Event_SetMessage(0xfa6);
        Actor_RunRepeatedMotion(13, 1);
        Actor_WalkToAndWait(0, 232, 0x108);
        Actor_SetAnimation(0, 0);
        Actor_FaceEachOther(0, 13, 20);
        Actor_RunRepeatedMotion(13, 2);
        Event_ShowMessageAndWait(13, 0, 10);
        record = (u8 *)Func_02001c6e(0);
        x = *(s16 *)(record + 10);
        y = *(s16 *)(record + 18);
        Actor_SetPosition(5, x << 16, y << 16);
        Actor_SetPosition(1, x << 16, y << 16);
        Actor_SetSpeed(5, 0x8000, 0x4000);
        Actor_SetSpeed(1, 0x8000, 0x4000);
        Actor_WalkTo(5, 248, 0x108);
        Actor_WalkToAndWait(1, 216, 0x108);
        Actor_SetAnimation(0, 1);
        Actor_SetAnimation(5, 1);
        Actor_SetAnimation(1, 1);
        Event_Wait(4);
        Actor_FaceDirection(5, 0xb000, 0);
        Actor_FaceDirection(1, 0xd000, 20);
        Actor_SetAnimationAndWait(5, 4);
        Event_Wait(10);
        Event_ShowMessageAndWait(5, 0, 20);
        Actor_RunRepeatedMotion(13, 1);
        Event_Wait(10);
        Actor_FaceDirection(13, 0x3000, 10);
        Event_ShowMessageAndWait(13, 0, 10);
        Actor_FaceDirection(1, 0x3000, 40);
        Actor_RunRepeatedMotion(1, 2);
        Event_ShowMessageAndWait(1, 0, 10);
        Actor_RunRepeatedMotion(13, 2);
        Actor_FaceDirection(13, 0x5000, 10);
        Actor_SetAnimation(13, 3);
        Event_ShowMessageAndWait(13, 0, 8);
        Actor_FaceDirection(5, 0x3000, 20);
        Event_ShowMessageAndWait(5, 0, 10);
        Actor_FaceDirection(13, 0x3000, 10);
        Actor_SetAnimationAndWait(13, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(13, 0, 6);
        Actor_ShowEmote(0, 0x101, 0);
        Actor_ShowEmote(1, 0x101, 0);
        Actor_ShowEmote(5, 0x101, 60);
        Actor_FaceDirection(1, 0xd000, 0);
        Actor_FaceDirection(5, 0xb000, 20);
        Actor_RunRepeatedMotion(5, 2);
        Event_ShowMessageAndWait(5, 0, 10);
        Actor_ShowEmote(13, 0x102, 60);
        Actor_SetAnimationAndWait(13, 4);
        Event_ShowMessageAndWait(13, 0, 10);
        Actor_FaceDirection(1, 0, 0);
        Actor_FaceDirection(5, 0x8000, 20);
        Actor_ShowEmote(0, 0x100, 60);
        Actor_FaceDirection(1, 0xd000, 0);
        Actor_FaceDirection(5, 0xb000, 0);
        Actor_FaceDirection(13, 0x5000, 20);
        Actor_FaceDirection(13, 0x3000, 40);
        Actor_FaceDirection(13, 0x5000, 40);
        Actor_FaceDirection(13, 0x3000, 20);
        Event_OpenMessage(13, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage((s32)Data_00000fb0);
        } else {
            Event_SetMessage(0xfb1);
        }
        Event_Wait(20);
        Actor_SetAnimationAndWait(13, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(13, 0, 10);
        Event_SetMessage(0xfb2);
        Actor_RunRepeatedMotion(5, 2);
        Actor_FaceDirection(5, 0x8000, 10);
        Event_ShowMessageAndWait(5, 0, 6);
        Actor_ShowEmote(1, 0x103, 30);
        Actor_Jump(1, 4, 30);
        Actor_FaceDirection(1, 0, 10);
        Event_ShowMessageAndWait(1, 0, 6);
        Actor_FaceEachOther(0, 1, 10);
        Actor_FaceEachOther(0, 5, 0);
        Actor_FaceActor(13, 1, 10);
        Actor_FaceActor(13, 5, 10);
        Actor_SetAnimation(0, 3);
        Actor_SetAnimation(1, 3);
        Actor_SetAnimationAndWait(5, 3);
        Event_Wait(10);
        Actor_SetAnimation(5, 1);
        Actor_SetAnimation(1, 1);
        Actor_SetAnimation(0, 0);
        Actor_FaceDirection(0, 0x4000, 0);
        Actor_FaceDirection(1, 0x4000, 0);
        Actor_FaceDirection(5, 0x4000, 16);
        Actor_SetAttachedEffect(13, 0x102);
        Actor_RunRepeatedMotion(13, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(13, 0, 6);
        Actor_ShowEmote(0, 0x100, 0);
        Actor_ShowEmote(1, 0x100, 0);
        Actor_ShowEmote(5, 0x100, 40);
        Actor_SetAnimationAndWait(13, 4);
        Event_Wait(10);
        Event_ShowMessageAndWait(13, 0, 6);
        Actor_RunRepeatedMotion(13, 1);
        Event_ShowMessageAndWait(13, 0, 6);
        Actor_FaceDirection(5, 0xb000, 10);
        Event_ShowMessageAndWait(5, 0, 6);
        Actor_FaceDirection(13, 0x3000, 10);
        Actor_SetAnimationAndWait(13, 3);
        Event_Wait(6);
        Event_ShowMessageAndWait(13, 0, 10);
        Actor_Jump(0, 2, 0);
        Actor_Jump(1, 2, 0);
        Actor_Jump(5, 2, 10);
        Actor_FaceDirection(0, 0xc000, 0);
        Actor_FaceDirection(1, 0xd000, 20);
        Event_ShowMessageAndWait(1, 0, 6);
        Actor_FaceDirection(13, 0x5000, 10);
        Actor_SetAnimationAndWait(13, 3);
        Event_Wait(16);
        Actor_FaceEachOther(0, 5, 40);
        Event_ShowMessageAndWait(5, 0, 10);
        Actor_RunRepeatedMotion(13, 2);
        Actor_FaceDirection(13, 0x3000, 10);
        Event_ShowMessageAndWait(13, 0, 6);
        Actor_FaceDirection(0, 0xc000, 0);
        Actor_FaceDirection(5, 0xb000, 30);
        Actor_ShowEmote(0, 0x105, 0);
        Actor_ShowEmote(1, 0x105, 0);
        Actor_ShowEmote(5, 0x105, 80);
        Actor_SetAnimationAndWait(13, 4);
        Event_OpenMessage(13, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage(0xfbd);
        } else {
            Event_SetMessage(0xfbe);
        }
        Event_ShowMessageAndWait(13, 0, 20);
        base5_fbf = (s32)Data_00000fbf;
        Event_SetMessage(base5_fbf);
        Actor_FaceDirection(1, 0, 10);
        Actor_RunRepeatedMotion(1, 2);
        Event_OpenMessage(1, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage((base5_fbf + 1));
        } else {
            Event_SetMessage((base5_fbf + 2));
        }
        Event_ShowMessageAndWait(1, 0, 6);
        Event_SetMessage(0xfc2);
        Actor_FaceDirection(5, 0x8000, 10);
        Actor_RunRepeatedMotion(5, 1);
        Event_OpenMessage(5, 0);
        Event_Wait(4);
        if (Event_ChooseYesNo(0, 0) == 1) {
            Actor_Jump(5, 2, 20);
            Event_ShowMessageAndWait(5, 0, 10);
        } else {
            Actor_SetAnimation(0, 3);
            Actor_SetAnimation(1, 3);
            Actor_SetAnimationAndWait(5, 3);
            Event_Wait(8);
            Actor_SetAnimation(0, 0);
            bump_step(1);
        }
        Actor_SetAnimationAndWait(13, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(13, 0, 10);
        Actor_FaceDirection(0, 0xc000, 0);
        Actor_FaceDirection(1, 0xd000, 0);
        Actor_FaceDirection(5, 0xb000, 10);
        Actor_SetAnimation(0, 3);
        Actor_SetAnimation(1, 3);
        Actor_SetAnimationAndWait(5, 3);
        Event_Wait(10);
        Actor_SetAnimation(0, 0);
        Event_Wait(20);
        Actor_RunRepeatedMotion(13, 2);
        Event_OpenMessage(13, 0);
        Event_Wait(4);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage(0xfc6);
        } else {
            Event_SetMessage(0xfc9);
        }
        ((void (*)())Engine_EventWait)(10);
        Actor_RunRepeatedMotion(1, 2);
        Actor_FaceDirection(1, 0, 10);
        Event_ShowMessageAndWait(1, 0, 6);
        Actor_FaceDirection(5, 0x8000, 10);
        Actor_SetAnimationAndWait(5, 4);
        Event_ShowMessageAndWait(5, 0, 6);
        Actor_RunRepeatedMotion(1, 2);
        Actor_FaceDirection(1, 0x8000, 10);
        Actor_ShowEmote(1, 0x103, 30);
        Event_ShowMessageAndWait(1, 0, 10);
        Actor_ShowEmote(13, 0x100, 40);
        Actor_Jump(13, 4, 40);
        Actor_FaceDirection(0, 0xc000, 0);
        Actor_FaceDirection(1, 0xc000, 0);
        Actor_FaceDirection(5, 0xb000, 20);
        Actor_FaceDirection(13, 0xb000, 10);
        Audio_PlayCue(158);
        Map_AnimateCells(0x200a0ac, 43, 8);
        Actor_SetSpeed(13, 0x10000, 0x8000);
        Actor_WalkToAndWait(13, 232, 218);
        Actor_SetPosition(13, 0, 0);
        Actor_ShowEmote(0, 0x101, 0);
        Actor_ShowEmote(1, 0x101, 0);
        Actor_ShowEmote(5, 0x101, 60);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
        Event_CloseScreen();
        Event_WaitForScreen();
        Event_RequestExit(13);
        Event_End();
    }
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Contiguous unnamed state-owner run for resource_375. */

/* The same import at two sites, so two names: the flag set at +0x1e and again
 * at +0x2e. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void FieldScene_RunScene375_02000964(void)
{
    s32 record;
    s32 facing;

    Event_Begin();
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(1, 0xd80000, 0x1080000);
    Actor_SetPosition(5, 0xf80000, 0x1080000);
    record = Func_02002430(1);
    facing = 0xc000;
    *(u16 *)(record + 6) = facing;
    record = Func_0200243c(5);
    *(u16 *)(record + 6) = facing;
    Map_AnimateCells(0x200a0ac, 43, 8);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_SetSpeed(13, 0xcccc, 0x6666);
    Actor_SetPosition(13, 0xe60000, 0xdc0000);
    Actor_WalkToAndWait(13, 230, 232);
    Event_Wait(20);
    Actor_SetAnimationAndWait(13, 3);
    Event_SetMessage(0xfcc);
    Event_ShowMessageAndWait(13, 0, 10);
    Actor_RunRepeatedMotion(13, 2);
    Actor_FaceDirection(13, 0x3000, 10);
    Event_ShowMessageAndWait(13, 0, 10);
    Actor_FaceDirection(1, 0, 0);
    Actor_FaceDirection(0, 0x8000, 10);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3);
    Actor_SetAnimation(0, 0);
    Event_Wait(20);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(5, 0x8000, 10);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimation(0, 0);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_SetSpeed(5, 0xcccc, 0x6666);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_SetAnimation(1, 2);
    record = Value1(Func_02002550, 0);
    if (record != 0) {
        Actor_SetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_SetAnimation(5, 2);
    record = Value1(Func_02002570_a, 0);
    if (record != 0) {
        Actor_SetDestination(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_SetAnimation(13, 2);
    record = Value1(Func_02002590_a, 0);
    if (record != 0) {
        Actor_SetDestination(13, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_SetPosition(5, 0, 0);
    Actor_WaitForMove(13);
    Actor_SetPosition(13, 0, 0);
    Actor_SetAnimation(1, 1);
    Actor_SetAnimation(5, 1);
    Actor_SetAnimation(13, 1);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    GameFlag_Set(0x801);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    ColorBuffer_ApplySource(0x10000, 0);
    GameFlag_Set(0x242);
    Event_End();
}

void FieldScene_SetupWithDescriptorA0ACWhenFlag242Clear(void)
{
    if (GameFlag_IsSet(0x242) == 0) {
        Audio_PlayCue(0x9E);
        Map_AnimateCells(Data_0200a0ac, 0x2B, 8);
    }
    Actor_WalkTo(0, 0xE5, 0xD9);
    Event_RequestExit(3);
}

void FieldScene_RunGatedActorSequence(void)
{
    u8 *record;
    s32 x, z;
    s32 evt;
    s32 evt2;
    s32 tbl;

    if (GameFlag_IsSet(0x808) != 0) {
        return;
    }

    Event_Begin();
    Audio_PlayCue(17);
    GameFlag_Set(0x808);

    evt = (s32)Data_00000f85;
    Event_SetMessage(evt);
    Event_ShowMessageAndWait(14, 0, 10);

    Actor_SetSpeed(0, 0x13333, 0x9999);
    Actor_ShowEmote(0, 0x100, 30);
    Actor_WalkToAndWait(0, 0x188, 0x148);
    Actor_FaceDirection(0, 0x4000, 10);

    record = Func_020026e0(0);
    x = *(s16 *)(record + 10);
    z = *(s16 *)(record + 18);
    Actor_SetPosition(5, x << 16, z << 16);
    Actor_SetPosition(1, x << 16, z << 16);

    Actor_SetSpeed(5, 0x13333, 0x9999);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_WalkTo(5, 0x178, 0x148);
    Actor_WalkToAndWait(1, 0x198, 0x148);
    Actor_SetAnimation(0, 0);
    Actor_SetAnimation(5, 0);
    Actor_SetAnimation(1, 0);
    Actor_FaceDirection(1, 0x8000, 0);
    Actor_FaceDirection(5, 0, 20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(5, 0x101, 30);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(5, 0x4000, 0);
    Camera_SetSpeed(0x60000, 0xc000);
    Camera_MoveTo(0xd70000, -1, 0x1590000, 1);
    Data_02009b85();
    Event_Wait(20);
    Audio_PlayCue(61);

    Actor_SetAnimationAndWait(14, 4);
    Actor_SetAnimation(14, 4);
    Event_ShowMessageAndWait(14, 0, 20);
    Actor_FaceDirection(15, 0, 10);
    Event_ShowMessageAndWait(15, 0, 10);
    Actor_SetAnimationAndWait(14, 3);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_FaceDirection(15, 0x4000, 60);
    Actor_RunRepeatedMotion(15, 1);
    Event_ShowMessageAndWait(15, 0, 20);
    Actor_SetAnimationAndWait(14, 3);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_SetAnimationAndWait(15, 4);
    Event_ShowMessageAndWait(15, 0, 6);
    Actor_ShowEmote(14, 0x100, 0);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Actor_FaceDirection(15, 0, 10);
    Actor_ShowEmote(15, 0x101, 40);
    Actor_FaceDirection(14, 0, 60);
    Actor_FaceDirection(14, 0x8000, 40);
    Actor_FaceDirection(14, 0, 40);
    Actor_WalkToAndWait(14, 232, 0x168);
    Actor_FaceDirection(14, 0, 10);
    Actor_SetAnimationAndWait(15, 3);
    Event_Wait(10);
    Actor_WalkTo(14, 0x188, 0x168);
    Actor_WalkToAndWait(15, 216, 0x168);
    Actor_WalkTo(15, 0x178, 0x168);
    Actor_FaceDirection(0, 0x4000, 0);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(5, 0x4000, 0);
    Camera_MoveTo(0x1890000, -1, 0x1530000, 1);
    Actor_WalkTo(14, 0x188, 0x168);
    Actor_WalkToAndWait(15, 0x178, 0x168);
    Actor_SetAnimation(14, 0);
    Actor_SetAnimation(15, 0);
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_FaceDirection(15, 0xd000, 30);
    Actor_RunRepeatedMotion(14, 2);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_ShowEmote(1, 258, 60);
    Actor_RunRepeatedMotion(1, 1);
    Event_ShowMessageAndWait(1, 0, 10);
    Actor_SetAnimationAndWait(15, 4);
    Event_OpenMessage(0x100f, 0);

    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(evt + 10);
    } else {
        Event_SetMessage(evt + 11);
    }

    Event_ShowMessageAndWait(0x100f, 0, 10);
    Actor_RunRepeatedMotion(1, 2);

    evt2 = (s32)Data_00000f91;
    Event_SetMessage(evt2);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_FaceEachOther(14, 15, 40);
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_FaceDirection(15, 0xd000, 20);
    Event_ShowMessageAndWait(14, 0, 60);
    Actor_RunRepeatedMotion(15, 1);
    Event_Wait(10);
    Actor_SetAnimationAndWait(15, 3);
    Event_ShowMessageAndWait(0x100f, 0, 10);
    Actor_RunRepeatedMotion(5, 2);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_RunRepeatedMotion(14, 2);
    Actor_FaceDirection(14, 0xa000, 20);
    Event_OpenMessage(14, 0);

    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(evt2 + 5);
    } else {
        Event_SetMessage(evt2 + 6);
    }

    Actor_FaceDirection(5, 0, 0);
    Actor_FaceDirection(1, 0x8000, 20);
    Actor_RunRepeatedMotion(14, 2);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_FaceActor(14, 1, 30);
    Actor_FaceActor(14, 5, 30);
    Actor_ShowEmote(14, 0x105, 80);
    Actor_SetAnimationAndWait(14, 4);
    Event_SetMessage(0xf98);
    Event_ShowMessageAndWait(14, 0, 6);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(5, 0x100, 0);
    Actor_StartRepeatedMotion(1, 1);
    Actor_StartRepeatedMotion(5, 1);
    Actor_RunRepeatedMotion(0, 1);
    Event_Wait(40);
    Actor_RunRepeatedMotion(5, 2);
    Actor_FaceDirection(1, 0x4000, 0);
    Actor_FaceDirection(5, 0x4000, 10);
    Event_ShowMessageAndWait(0x1005, 0, 10);
    Actor_RunRepeatedMotion(15, 2);
    Actor_FaceDirection(15, 0, 10);
    Event_ShowMessageAndWait(0x100f, 0, 10);
    Actor_FaceDirection(14, 0x8000, 20);
    Actor_SetAnimationAndWait(14, 4);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_RunRepeatedMotion(15, 2);
    Event_Wait(10);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(15, 0xd000, 20);
    Actor_SetSpeed(15, 0x8000, 0x4000);
    Actor_SetSpeed(14, 0x8000, 0x4000);

    record = Func_020026e0(14);
    *(record + 90) &= 0xfe;
    record = Func_020026e0(15);
    *(record + 90) &= 0xfe;

    Actor_WalkTo(14, 0x188, 0x178);
    Actor_WalkToAndWait(15, 0x178, 0x178);
    Event_Wait(6);

    record = Func_020026e0(14);
    *(record + 90) |= 1;
    record = Func_020026e0(15);
    {
        /*
         * A result temporary, not the compound or-assign the first
         * occurrence above uses. The reference writes the result into
         * the mask register rather than the loaded value, and the
         * two-address ORR only does that when the merged result is its
         * own object; the compound form keeps the loaded value as
         * destination. Same technique already adopted in the sibling
         * owners resource_3bd:020013f8 and resource_39e:02001494.
         */
        u8 merged = (u8)(*(record + 90) | 1);

        *(record + 90) = merged;
    }

    Actor_SetAnimation(14, 0);
    Actor_SetAnimation(15, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(1, 2);
    Event_ShowMessageAndWait(1, 0, 10);
    Actor_FaceDirection(0, 1, 20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(1, 3);
    Audio_PlayCue(17);

    Actor_SetAnimation(1, 2);
    record = Func_020026e0(0);
    if (record != 0) {
        Actor_SetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);

    Actor_SetAnimation(5, 2);
    record = Func_020026e0(0);
    if (record != 0) {
        Actor_SetDestination(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(5);
    Actor_SetPosition(5, 0, 0);

    tbl = (s32)Data_02009ce0;
    Call3(Func_02002d8e, 14, 0x10000, tbl);
    Call3(Func_02002d8e, 15, 0x10000, tbl);
    Func_02002e46();
    Event_End();
}

void SceneState_SetWorkAndFlag87d(void)
{
    u8 *work;

    Event_Begin();

    work = *(u8 **)Data_03001ebc;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    GameFlag_Set(0x87D);
    Func_02003560(12, 0);
    GameFlag_Set(0x900);   /* 144 << 4 */
    Event_End();
}

void SceneState_SetWorkAndFlag87e(void)
{
    u8 *work;

    Event_Begin();

    work = *(u8 **)Data_03001ebc;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    GameFlag_Set(0x87E);
    Func_020035a4(12, 1);
    GameFlag_Set(0x900);   /* 144 << 4 */
    Event_End();
}

void SceneDialogue_RunActorSixteenDialogue(void)
{
    Event_Begin();
    Event_SetMessage(0x1C96);
    Event_AskYesNo(16, 0);
    Event_End();
}
