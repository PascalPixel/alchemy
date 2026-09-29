#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgFuneHaveMakeThemPromiseHelp[];
extern u8 MsgFuneIfWeDontLeaveSoon[];
extern u8 MsgFuneNowWeHaveProtectShip[];
extern u8 MsgFuneSomebodyStopThem[];
extern u8 MsgFuneTheyCantPlanningMutiny[];
extern u8 MsgFuneToldWereLeavingSoonSet[];
extern u8 FuneKanpan_RandomActorActions[];


union Slot {
    s32 w;
    s16 h[2];
};


s32 BuildMotionCountdown(s32, s16);
s32 Object_GetById();

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

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Engine_EventBegin)();
    if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage((s32)MsgFuneNowWeHaveProtectShip);
        Event_ShowMessage(21, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Actor_RunRepeatedMotion(21, 2);
            Event_SetMessage((s32)MsgFuneSomebodyStopThem);
            Event_ShowMessage(21, 0);
            rec7 = Value1(Object_GetById, 21);
            record = Random_Next();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(u16 *)(rec7 + 100) = shown;
            Engine_ActorEnableActionCallback(21, (u32)FuneKanpan_RandomActorActions);
        } else {
            Actor_ShowEmote(21, 0x103, 0);
            Actor_StartRepeatedMotion(21, 3);
            Event_SetMessage((s32)MsgFuneToldWereLeavingSoonSet);
            Event_ShowMessage(21, 0);
        }
    }
    Event_End();
}

void FieldScene_RunScene3afSequenceA(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Engine_EventBegin)();
    if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage((s32)MsgFuneHaveMakeThemPromiseHelp);
        Event_ShowMessage(24, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Actor_RunRepeatedMotion(24, 2);
            Event_SetMessage((s32)MsgFuneTheyCantPlanningMutiny);
            Event_ShowMessage(24, 0);
            rec7 = Value1(Object_GetById, 24);
            record = Random_Next();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(u16 *)(rec7 + 100) = shown;
            Engine_ActorEnableActionCallback(24, (u32)FuneKanpan_RandomActorActions);
        } else {
            Actor_ShowEmote(24, 0x103, 0);
            Actor_StartRepeatedMotion(24, 3);
            Event_SetMessage((s32)MsgFuneIfWeDontLeaveSoon);
            Event_ShowMessage(24, 0);
        }
    }
    Event_End();
}
