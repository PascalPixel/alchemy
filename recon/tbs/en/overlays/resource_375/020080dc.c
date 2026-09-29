/* Draft of resource_375 0x020080dc (Scene_RunActorTwelveDialogue), from
 * games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA. Remaining difference:
 * the ROM loads message 0xf76 from the literal pool and derives the
 * following lines from it, as a link-time message value would; C constants
 * are each built or loaded on their own. The listing keeps these rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * Sukureta's cottage in Vale. Outside it, Saturos and Menardi are overheard
 * plotting to use Sukureta and let the children go; inside, Sukureta and the
 * children decide to see for themselves whether the strangers have been to
 * Mt. Aleph and Sol Sanctum.
 */

enum CottageActor {
    ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 5,
    ACTOR_SATUROS,
    ACTOR_MENARDI
};

enum CottageFlag {
    FLAG_SANCTUM_VISIT_PLANNED = 0x801,
    FLAG_MET_SATUROS_AND_MENARDI = 0x808
};

#define NULL ((void *)0)

#include "FACING_OBJECT.H"

enum CottageMessage {
    MSG_SATUROS_GO = 0xf98,
    MSG_SUKURETA_OH_ROBIN = 0xfa6,
    MSG_SUKURETA_I_WAITED_YEARS_FOR_THE_SANCTUM = 0xfb1,
    MSG_JASMINE_THEY_MIGHT_BE_THIEVES = 0xfb2,
    MSG_SUKURETA_WE_ONLY_CHECK_THE_MOUNTAIN = 0xfbd,
    MSG_SUKURETA_WE_ONLY_CHECK_MT_ALEPH = 0xfbe,
    MSG_JASMINE_OUR_SECRET = 0xfc2,
    MSG_GERALD_YOU_CAN_HANDLE_THE_DANGER = 0xfc6,
    MSG_GERALD_ILL_TAKE_OVER_IF_NERVOUS = 0xfc9,
    MSG_SUKURETA_OUR_BEST_BET = 0xfcc,
    MSG_ILL_CLIMB_THE_FENCE_SOMEDAY = 0x11c4,
    MSG_MEMORIES_OF_THIS_COTTAGE = 0x1c96
};

extern u8 Data_0200a028[];
extern u8 Data_02009fb0[];
extern u8 Data_02009efc[];
extern u8 LinkedMessage_YouCannotEnterMtAleph[];
extern u8 LinkedMessage_FineIfTheyDontSeeUs[];
extern u8 Data_03001ebc[];
extern u8 Data_0200a0ac[];
extern u8 Value_00000f76;
extern u8 LinkedMessage_TheyKnowLittleOfTheSanctum[];
extern u8 LinkedMessage_YoureTheOnesSneakingAround[];
extern u8 Data_02009ce0[];

s32 CalculateFacingAngle(s32, s32);
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
 * DRAFTED SCENE SCRIPT for Scene_OverhearSaturosAndMenardi.
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
 * adopts it via `alchemy adopt`, which retires the not-yet-c region.
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

struct ObjectRuntime;


void Scene_RunActorTwelveDialogue(void)
{
    s32 base;

    Event_Begin();
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Event_SetMessage(MSG_ILL_CLIMB_THE_FENCE_SOMEDAY);
        Event_ShowMessage(12, 0);
    } else {
        base = (s32)&Value_00000f76;
        Event_SetMessage(base);
        Actor_FaceActor(12, ACTOR_PARTY_LEADER, 10);
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
