#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
/* FAKEMATCH: calls that cast Object_GetById to another return type keep their original register order. */
s32 Object_GetById();

#define ACTOR_FLAGS_OFFSET 90

enum MultiEncounterMessage {
    MSG_WONDER_COULD_HAVE_HAPPENED = 0x1d26,
    MSG_TOLD_WERE_LEAVING_SOON_SET = 0x1d36,
    MSG_IF_WE_DONT_LEAVE_SOON = 0x1d37,
    MSG_SOMEBODY_STOP_THEM = 0x1d6f,
    MSG_THEY_CANT_PLANNING_MUTINY = 0x1d70,
    MSG_DIDNT_DO_ANYTHING = 0x1d8d,
    MSG_NOW_WE_HAVE_PROTECT_SHIP = 0x1e08,
    MSG_HAVE_MAKE_THEM_PROMISE_HELP = 0x1e09,
    MSG_PREPARATIONS_READY = 0x1e39,
    MSG_AYE_CAPTAIN_SEA_MONSTERS = 0x1e41,
    MSG_THANK_ROBIN_DID_GOOD_AGAINST = 0x1ee1,
    MSG_CAN_SEE_LAND = 0x1ee5,
    MSG_ROBIN_DONT_TALK_LIKE_SHOULDNT = 0x1f53,
    MSG_ROBIN_TALKED_PASSENGERS_DIDNT_TOUR = 0x1f55,
    MSG_SEE_YOURE_GOING_GO_FOR = 0x1f5b,
    MSG_HOW_WAS_ROBIN_DID_EXPLORE = 0x1f69
};

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 BuildMotionCountdown(s32, s16);

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The step value differs in the localized scene data. */

/* Offset of a flag byte on an actor record, cleared and set below. */

/* Pointer, held at fixed address 0x03001ebc, to the shared scene work
 * record. The phase/status word lives at offset 0x1c0 of that record. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)&gEventWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_020029d4(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ void Call1_02003a0c(void (*f)(), s32 a0)
{

    f(a0);
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#endif

#if defined(TBS_EDITION_JA)
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#else
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#endif
#if defined(TBS_EDITION_DE)
#endif

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void FieldScene_RunStepThen10(s32 a);
void FieldScene_CallPairWith10(s32 a, s32 b);

/* Sets up actors 1, 2, and 3 from three source records, runs their
 * animations and a wait loop gated on actor 0, then clears a flag byte
 * at +90 on actors 21 and 22 before finishing the scene. */
void FieldScene_RunThreeActorEncounter(void)
{
    u8 *record;
    u8 bits;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 180, 0x28e);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    /* For each of actors 1, 2, and 3: fetch a source record, and if one
     * exists, copy its fields at +8 and +16 into the actor. */
    record = Value1(Object_GetById, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Object_GetById, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Object_GetById, 0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Actor_WalkTo(ACTOR_GERALD, 194, 0x280);
    Actor_WalkTo(ACTOR_IVAN, 198, 0x28e);
    Actor_WalkToAndWait(ACTOR_MIA, 194, 0x2a0);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Value2(FieldScene_CallPairWith10, 3, 0x8000);
    FieldScene_CallPairWith10(22, 0);
    Event_SetMessage(MSG_ROBIN_TALKED_PASSENGERS_DIDNT_TOUR);
    Call1(FieldScene_RunStepThen10, 22);
    Value2(FieldScene_CallPairWith10, 21, 0xd000);
    Event_ShowMessageAndWait(21, 0, 40);
    Actor_ShowEmote(22, 0x100, 20);
    Actor_RunRepeatedMotion(22, 1);
    Event_OpenMessage(22, 0);
    /* Gated on a condition read from actor 0: configure actors 2, 1, and 3,
     * then spin, re-checking actor 0, while a condition on actor 2 holds. */
    if (Event_ChooseYesNo(0, 0) == 1) {
        Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
        FieldScene_RunStepThen10(2);
        Value2(FieldScene_CallPairWith10, 3, 0xa000);
        Actor_SetAnimation(ACTOR_MIA, 3);
        FieldScene_RunStepThen10(3);
        Value2(FieldScene_CallPairWith10, 1, 0x6000);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_OpenMessage(ACTOR_GERALD, 0);
        L_02003dfa:;
        if (Event_ChooseYesNo(0, 0) == 1) {
            Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
            Event_SetMessage(MSG_ROBIN_DONT_TALK_LIKE_SHOULDNT);
            Event_OpenMessage(ACTOR_IVAN, 0);
            goto L_02003dfa;
        }
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(22, 3);
    Event_SetMessage(MSG_SEE_YOURE_GOING_GO_FOR);
    FieldScene_RunStepThen10(22);
    Actor_SetSpeed(22, 0x10000, 0x8000);
    Actor_SetSpeed(21, 0x10000, 0x8000);
    /* Clear the low bit of the flag byte on actor 22. */
    *(u8 *)(Object_GetById(22) + ACTOR_FLAGS_OFFSET) &= 254;
    Actor_WalkToAndWait(22, 162, 0x27a);
    Event_Wait(1);
    bits = 1;
    {
        /* Set the low bit of the flag byte on actor 22. */
        u8 *record = ((u8 *(*)())Object_GetById)(22);
        u8 value = record[ACTOR_FLAGS_OFFSET];

        record[ACTOR_FLAGS_OFFSET] = value | bits;
    }
    /* Clear the low bit of the flag byte on actor 21. */
    *(u8 *)(Value1(Object_GetById, 21) + ACTOR_FLAGS_OFFSET) &= 254;
    Actor_WalkToAndWait(21, 162, 0x2a4);
    Event_Wait(1);
    {
        /* Set the low bit of the flag byte on actor 21. */
        u8 *record = ((u8 *(*)())Object_GetById)(21);

        bits |= record[ACTOR_FLAGS_OFFSET];
        record[ACTOR_FLAGS_OFFSET] = bits;
    }
    Actor_FaceDirection(22, 0x3000, 0);
    Call2(FieldScene_CallPairWith10, 21, 0xd000);
    FieldScene_RunStepThen10(22);
    /* Finish actors 1, 2, and 3 with the same target values used earlier. */
    Actor_WalkTo(ACTOR_GERALD, 180, 0x28e);
    Actor_WalkTo(ACTOR_IVAN, 180, 0x28e);
    Actor_WalkToAndWait(ACTOR_MIA, 180, 0x28e);
    Actor_Destroy(ACTOR_GERALD);
    Actor_Destroy(ACTOR_IVAN);
    Actor_Destroy(ACTOR_MIA);
    GameFlag_Set(0x903);
    Event_End();
}
