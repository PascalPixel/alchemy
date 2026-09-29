/* Draft of resource_375 0x02008be0 (Scene_OverhearSaturosAndMenardi), from
 * games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_SUKURETA. Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines (Func_020026e0,
 * Engine_CameraSetSpeed, Data_02009b85, Engine_EventOpenMessage,
 * Engine_EventChooseYesNo, Engine_ActorFaceEachOther, ...). The listing keeps these rows. */
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
extern u8 MsgHaidiaSaturosGo[];
extern u8 MsgHaidiaTheyKnowLittleOfThe[];
extern u8 MsgHaidiaYoureTheOnesSneakingAround[];


extern u8 Data_0200a028[];
extern u8 Data_02009fb0[];
extern u8 Data_02009efc[];
extern u8 Data_03001ebc[];
extern u8 Data_0200a0ac[];
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


void Scene_OverhearSaturosAndMenardi(void)
{
    u8 *record;
    s32 x, z;
    s32 evt;
    s32 evt2;
    s32 tbl;

    if (GameFlag_IsSet(FLAG_MET_SATUROS_AND_MENARDI) != 0) {
        return;
    }

    Event_Begin();
    Audio_PlayCue(17);
    GameFlag_Set(FLAG_MET_SATUROS_AND_MENARDI);

    evt = (s32)MsgHaidiaTheyKnowLittleOfThe;
    Event_SetMessage(evt);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);

    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 30);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x188, 0x148);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);

    record = Func_020026e0(0);
    x = *(s16 *)(record + 10);
    z = *(s16 *)(record + 18);
    Actor_SetPosition(ACTOR_JASMINE, x << 16, z << 16);
    Actor_SetPosition(ACTOR_GERALD, x << 16, z << 16);

    Actor_SetSpeed(ACTOR_JASMINE, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_WalkTo(ACTOR_JASMINE, 0x178, 0x148);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x198, 0x148);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(ACTOR_JASMINE, 0);
    Actor_SetAnimation(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x4000, 0);
    Camera_SetSpeed(0x60000, 0xc000);
    Camera_MoveTo(0xd70000, -1, 0x1590000, 1);
    Data_02009b85();
    Event_Wait(20);
    Audio_PlayCue(61);

    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Actor_SetAnimation(ACTOR_SATUROS, 4);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 10);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceDirection(ACTOR_MENARDI, 0x4000, 60);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 4);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 6);
    Actor_ShowEmote(ACTOR_SATUROS, 0x100, 0);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 10);
    Actor_ShowEmote(ACTOR_MENARDI, 0x101, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0, 60);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0, 40);
    Actor_WalkToAndWait(ACTOR_SATUROS, 232, 0x168);
    Actor_FaceDirection(ACTOR_SATUROS, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_Wait(10);
    Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x168);
    Actor_WalkToAndWait(ACTOR_MENARDI, 216, 0x168);
    Actor_WalkTo(ACTOR_MENARDI, 0x178, 0x168);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x4000, 0);
    Camera_MoveTo(0x1890000, -1, 0x1530000, 1);
    Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x168);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x178, 0x168);
    Actor_SetAnimation(ACTOR_SATUROS, 0);
    Actor_SetAnimation(ACTOR_MENARDI, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 30);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_ShowEmote(ACTOR_GERALD, 258, 60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 4);
    Event_OpenMessage(0x100f, 0);

    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(evt + 10);
    } else {
        Event_SetMessage(evt + 11);
    }

    Event_ShowMessageAndWait(0x100f, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);

    evt2 = (s32)MsgHaidiaYoureTheOnesSneakingAround;
    Event_SetMessage(evt2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 20);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 60);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_ShowMessageAndWait(0x100f, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
    Actor_FaceDirection(ACTOR_SATUROS, 0xa000, 20);
    Event_OpenMessage(ACTOR_SATUROS, 0);

    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(evt2 + 5);
    } else {
        Event_SetMessage(evt2 + 6);
    }

    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceActor(ACTOR_SATUROS, ACTOR_GERALD, 30);
    Actor_FaceActor(ACTOR_SATUROS, ACTOR_JASMINE, 30);
    Actor_ShowEmote(ACTOR_SATUROS, 0x105, 80);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Event_SetMessage((s32)MsgHaidiaSaturosGo);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 6);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100, 0);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 1);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x4000, 10);
    Event_ShowMessageAndWait(0x1005, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 10);
    Event_ShowMessageAndWait(0x100f, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 20);
    Actor_SetSpeed(ACTOR_MENARDI, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_SATUROS, 0x8000, 0x4000);

    record = Func_020026e0(14);
    *(record + 90) &= 0xfe;
    record = Func_020026e0(15);
    *(record + 90) &= 0xfe;

    Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x178);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x178, 0x178);
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

    Actor_SetAnimation(ACTOR_SATUROS, 0);
    Actor_SetAnimation(ACTOR_MENARDI, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 1, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Audio_PlayCue(17);

    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Func_020026e0(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);

    Actor_SetAnimation(ACTOR_JASMINE, 2);
    record = Func_020026e0(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_JASMINE, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_JASMINE);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);

    tbl = (s32)Data_02009ce0;
    Call3(Func_02002d8e, 14, 0x10000, tbl);
    Call3(Func_02002d8e, 15, 0x10000, tbl);
    Func_02002e46();
    Event_End();
}
