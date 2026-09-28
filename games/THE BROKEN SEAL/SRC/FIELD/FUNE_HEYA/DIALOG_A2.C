#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "STAGED_ACTOR.H"

/* Message ids. */
enum {
    LinkedMessage_DontFeelDontTalkMe = 0x1f47,
    LinkedMessage_YouFinallyPickedSomeoneDidnt = 0x1ea0,
    LinkedMessage_IfYouveChosenOarsmanLets = 0x1ea4,
    LinkedMessage_YouveBeenChosenAsOarsman = 0x1ea5,
    LinkedMessage_YouCameAskCaptainSet = 0x1dd1,
    LinkedMessage_YouGoingRow = 0x1e19,
    LinkedMessage_TheresWholeBunchReallyMuscular = 0x1d50,
    LinkedMessage_LooksLikeYouveBeenChosen = 0x1e9e,
    LinkedMessage_HaHaHaChosenOarsman = 0x1e9f,
    LinkedMessage_ThatsWhoYouPickedOarsman = 0x1ea3
};

enum ExtendedChoreographyMessage {
    MSG_WONDER_COULD_HAVE_HAPPENED = 0x1d26,
    MSG_ITS_TOO_LATE_HIRE_MERCENARIES = 0x1d30,
    MSG_BUT_WE_CANT_SEND_SHIP = 0x1d31,
    MSG_LONGER_WE_SIT_HERE_MORE = 0x1d4e,
    MSG_IF_WE_ARENT_GOING_SET = 0x1d56,
    MSG_NOW_WANT_SEE_CAPTAIN_TOO = 0x1d91,
    MSG_YOURE_TRYING_LAUNCH_SHIP = 0x1d93,
    MSG_BAD_LUCK_LOSING_MY_LUCKY = 0x1dcd,
    MSG_IF_SHIP_FROM_TOLBI_HAD = 0x1dd4,
    MSG_ITS_MY_LUCKY_ANCHOR = 0x1ddb,
    MSG_WE_DONT_KNOW_MIGHT_HAPPEN = 0x1e06,
    MSG_THESE_PROUD_WARRIORS_NOT_GOING = 0x1e13,
    MSG_OUR_REPLACEMENT_NEVER_ARRIVED_BUT = 0x1e27,
    MSG_CAST_OFF = 0x1e3b,
    MSG_ROW_THOSE_OARS = 0x1e3c,
    MSG_WERE_OFF = 0x1e3d,
    MSG_IM_TURNING = 0x1e43,
    MSG_HEY_ARE_YOU_OK = 0x1e6e,
    MSG_OHHHH_NOOOO_GOING_MAKE_ME = 0x1e81,
    MSG_HA_HA_HA_ROWING_FEEL = 0x1e84,
    MSG_GIVES_ME_CHILLS_THINK_COULD = 0x1ea1,
    MSG_ROBIN_YOUVE_GOT_GOOD_EYE = 0x1ea2,
    MSG_HO_HO_PERSON_GOING_GET = 0x1ea6,
    MSG_OARSMAN_WAS_INJURED = 0x1eb2,
    MSG_WONDER_WHATS_WRONG_SHIP_SHOULDNT = 0x1ec1,
    MSG_THING_HAS_KAJA_HIS_MEN = 0x1ece,
    MSG_MONSTERS_EVERYWHERE_IM_STUCK_ROWING = 0x1ecf,
    MSG_SHIP_STARTING_LIST_IF_WE = 0x1ed0,
    MSG_HOW_MANY_MONSTERS_OUT_THERE = 0x1ed1,
    MSG_ANOTHER_MONSTER_ISNT_FIRST_CLASS = 0x1ed2,
    MSG_DONT_CARE_TAKES_JUST_HURRY = 0x1edb,
    MSG_IF_THOSE_MONSTERS_COME_BACK = 0x1edc,
    MSG_BOATS_ROCKING_MUCH_IM_CERTAIN = 0x1edd,
    MSG_HAD_IDEA_THERE_WERE_MANY = 0x1ede,
    MSG_WERE_SURROUNDED_BY_MONSTERS_STILL = 0x1edf,
    MSG_HATE_ARGUING = 0x1f48,
    MSG_SORRY_EVERYONE_BUT_WE_NEED = 0x1f78,
    MSG_IM_SPREADING_GOODWILL_WHEREVER_TRAVEL = 0x1f7b,
    MSG_SHIPS_CREW_READY_FOR_ANYTHING = 0x1f7d,
    MSG_SHIPS_CREW_READY_FOR_ANYTHING_2 = 0x1f7f,
    MSG_GOOD_SHIP_HAS_ARRIVED_SAFELY = 0x1f81
};

struct SceneActor {
    u8 pad00[99];
    u8 mode;
};

struct EffectRecord {
    u8 pad00[6];
    u16 angle;
    u8 pad08[83];
    u8 state;
    u8 pad5c[6];
    u8 active;
};
void FuneHeya_TurnActorToOpenSide();
s32 SceneState_ApplyLevelFromFlags();
s32 Object_GetByIdFar();

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}



void FieldScene_RunScene3b1SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x300) != 0) {
        rec7 = Value0(SceneState_ApplyLevelFromFlags);
        FuneHeya_TurnActorToOpenSide();
        Event_SetMessage(MSG_GIVES_ME_CHILLS_THINK_COULD);
        FieldScene_RunStepThen10(12);
        Actor_SetAnimation(rec7, 2);
        record = Value1(Object_GetByIdFar, 0);
        if (record != 0) {
            Actor_SetDestination(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(rec7);
        Actor_SetPosition(rec7, 0, 0);
    } else {
        Actor_RunRepeatedMotion(12, 2);
        Event_Wait(20);
        Event_SetMessage(MSG_OHHHH_NOOOO_GOING_MAKE_ME);
        Event_OpenMessage(12, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            FieldScene_RunStepThen10(12);
            Actor_SetAnimation(12, 2);
            record = Value1(Object_GetByIdFar, 0);
            if (record != 0) {
                Actor_SetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Actor_WaitForMove(12);
            Actor_SetPosition(12, 0, 0);
            GameFlag_Set(0x300);
            if (GameFlag_IsSet(0x92b) != 0) {
                GameFlag_Set(0x994);
                goto L_02000c9a;
            }
            if (GameFlag_IsSet(0x92a) != 0) {
                GameFlag_Set(0x91b);
                goto L_02000c9a;
            }
            if (GameFlag_IsSet(0x929) != 0) {
                GameFlag_Set(0x939);
                goto L_02000c9a;
            }
            GameFlag_Set(0x930);
        } else {
            bump_step(1);
            FieldScene_RunStepThen10(12);
        }
    }
    L_02000c9a:;
    Event_End();
}

void FieldScene_RunScene3b1SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x300) != 0) {
        rec7 = Value0(SceneState_ApplyLevelFromFlags);
        FuneHeya_TurnActorToOpenSide();
        Event_SetMessage(MSG_ROBIN_YOUVE_GOT_GOOD_EYE);
        FieldScene_RunStepThen10(9);
        Actor_SetAnimation(rec7, 2);
        record = Value1(Object_GetByIdFar, 0);
        if (record != 0) {
            Actor_SetDestination(rec7, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ((void (*)())Engine_ActorWaitForMove)(rec7);
        Actor_SetPosition(rec7, 0, 0);
    } else {
        Event_SetMessage(MSG_HA_HA_HA_ROWING_FEEL);
        ((void (*)())Engine_EventShowMessageAndWait)(9, 0, 60);
        Actor_RunRepeatedMotion(9, 1);
        Event_OpenMessage(9, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            FieldScene_RunStepThen10(9);
            Actor_SetAnimation(9, 2);
            record = Value1(Object_GetByIdFar, 0);
            if (record != 0) {
                Actor_SetDestination(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Actor_WaitForMove(9);
            Actor_SetPosition(9, 0, 0);
            GameFlag_Set(0x300);
            if (GameFlag_IsSet(0x92b) != 0) {
                GameFlag_Set(0x991);
                goto L_02000de0;
            }
            if (GameFlag_IsSet(0x92a) != 0) {
                GameFlag_Set(0x918);
                goto L_02000de0;
            }
            if (GameFlag_IsSet(0x929) != 0) {
                GameFlag_Set(0x936);
                goto L_02000de0;
            }
            GameFlag_Set(0x92d);
        } else {
            bump_step(1);
            FieldScene_RunStepThen10(9);
        }
    }
    L_02000de0:;
    Event_End();
}

void SceneDialogue_RunActorThirteenFlag300Branch(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        FuneHeya_TurnActorToOpenSide(o);
        Event_SetMessage(LinkedMessage_ThatsWhoYouPickedOarsman);
        FieldScene_RunStepThen10(13);
        Actor_SetAnimation(o, 2);
        p = Object_GetByIdFar(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(13, 0x1e88, 0x995);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(13, 0x1e88, 0x91c);
    } else if (GameFlag_IsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(13, 0x1e88, 0x93a);
    } else {
        FieldScene_RunPrimarySequence(13, 0x1e88, 0x931);
    }
}

void FieldScene_RunFlag300BranchDialogue(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        FuneHeya_TurnActorToOpenSide(o);
        Event_SetMessage(LinkedMessage_IfYouveChosenOarsmanLets);
        FieldScene_RunStepThen10(14);
        Actor_SetAnimation(o, 2);
        p = Object_GetByIdFar(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(14, 0x1e8b, 0x996);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(14, 0x1e8b, 0x91d);
    } else if (GameFlag_IsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(14, 0x1e8b, 0x93b);
    } else {
        FieldScene_RunPrimarySequence(14, 0x1e8b, 0x932);
    }
}

void FieldScene_RunActor15FlagDialogue(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        FuneHeya_TurnActorToOpenSide(o);
        Event_SetMessage(LinkedMessage_YouveBeenChosenAsOarsman);
        FieldScene_RunStepThen10(15);
        Actor_SetAnimation(o, 2);
        p = Object_GetByIdFar(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(15, 0x1e8e, 0x997);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(15, 0x1e8e, 0x91e);
    } else if (GameFlag_IsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(15, 0x1e8e, 0x93c);
    } else {
        FieldScene_RunPrimarySequence(15, 0x1e8e, 0x933);
    }
}

/*
 * With story flag 0x300 set, opens a scripted sequence, creates a local
 * object, shows message 0x1ea6, configures it as slot 2, moves it onto actor
 * 0's signed halfword coordinates when actor 0 exists, then releases and
 * closes the sequence.  Otherwise flags 0x92b, 0x92a and 0x929 select the
 * setup call's third argument.  The 204-byte owner includes an alignment
 * halfword and its nine pool words.
 */
void FieldScene_RunActor16FlagDialogue(void)
{
    s32 obj;
    u8 *actor;

    if (GameFlag_IsSet(0x300) != 0) {
        obj = SceneState_ApplyLevelFromFlags();
        Event_Begin();
        FuneHeya_TurnActorToOpenSide(obj);
        Event_SetMessage(MSG_HO_HO_PERSON_GOING_GET);
        FieldScene_RunStepThen10(16);
        Actor_SetAnimation(obj, 2);

        actor = Object_GetByIdFar(0);
        if (actor != 0) {
            Actor_SetDestination(obj, *(s16 *)(actor + 10),
                          *(s16 *)(actor + 18));
        }

        Actor_WaitForMove(obj);
        Actor_SetPosition(obj, 0, 0);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x92b) != 0) {
            FieldScene_RunPrimarySequence(16, 0x1e91, 0x998);
        } else if (GameFlag_IsSet(0x92a) != 0) {
            FieldScene_RunPrimarySequence(16, 0x1e91, 0x91f);
        } else if (GameFlag_IsSet(0x929) != 0) {
            FieldScene_RunPrimarySequence(16, 0x1e91, 0x93d);
        } else {
            FieldScene_RunPrimarySequence(16, 0x1e91, 0x934);
        }
    }
}

struct FieldActor *FindActorNearPosition(s32 x, s32 y)
{
    struct EventWork *work;
    struct FieldActor **actor;
    struct FieldActor *current;
    u32 i;
    s32 actor_x;
    s32 actor_y;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;

    work = gEventWork;
    i = 8;
    left = x - 12;
    right = x + 12;
    top = y - 12;
    bottom = y + 12;
    actor = work->placed_actors;
    while (i <= 65) {
        current = *actor++;
        actor_x = current->x.part.pixel;
        actor_y = current->z.part.pixel;
        if (left < actor_x && right > actor_x &&
            top < actor_y && bottom > actor_y)
            return current;
        i++;
    }
    return 0;
}
