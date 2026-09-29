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

extern s16 Data_02000240[];
extern u8 FuneHeya_SceneTable04[];
extern u8 FuneHeya_SceneTable05[];
extern u8 FuneHeya_SceneTable06[];
extern u8 FuneHeya_SceneTable07[];
extern u8 FuneHeya_SceneTable08[];
extern u8 FuneHeya_SceneTable09[];
extern u8 FuneHeya_SceneTable10[];
extern u8 FuneHeya_SceneTable11[];
extern u8 FuneHeya_SceneTable12[];
extern u8 FuneHeya_SceneTable13[];
extern u8 FuneHeya_SceneTable14[];
void Battle_ResetEffectCounterFar();
void FuneHeya_TurnActorToOpenSide(u8 *obj);
u8 *Object_GetByIdFar(s32 n);

/* Scene-step dispatcher for overlay resource 0x3b1.
 *
 * The owner takes a step selector plus two step parameters and jumps through a
 * 26-entry table into one bounded block of scene setup calls per step. Step 18
 * finishes by re-entering the dispatcher with step 14, which the compiler turns
 * into a jump back to the range check.
 *
 * Uncertain: the roles of the two parameters differ per step (actor slot,
 * count, flag, upper loop bound), so they keep neutral names here. The record
 * fields written at +6 (halfword) and +24 (word) are the same scene-object
 * fields the neighbouring scene sources touch; their meaning is not recovered.
 */

/* Loader-relocated overlay calls: each Func_ symbol names the pre-relocation
 * call word the image holds.
 *
 * Three of those pre-relocation words repeat in this owner while reaching
 * different runtime helpers (0x0200af5a, 0x0200b0e8 and 0x0200b20c each cover
 * two distinct destinations), so one Func_ spelling cannot name both sites.
 * Those six sites are declared by their runtime address instead, which the
 * overlay symbol resolver binds directly. Registering this owner as a
 * translation unit with explicit absolute_symbols would let them go back to
 * suffixed Func_ spellings without changing a byte. */

/* The scene work record pointer; +0x1c0 holds the scene request word. */

/*
 * Actor slot search for resource_3b1.  The 48-byte owner at 0x02005038 has no
 * pool; the halfword at 0x02005066 is alignment before the next owner.
 */

/*
 * Field scene beat for overlay resource_3b1.  Each callee is named for its own
 * call site: every call reaches its target through its own local veneer, even
 * where the same logical callee is used from more than one site.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void FieldScene_CallPairWith10(s32 a, u16 b);

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

/*
 * The 292-byte owner at 0x0200054c covers the dispatcher, a 23-entry jump
 * table, the case bodies, an alignment halfword and the literal pool. Case
 * order and the shared arms reproduce the reference: 23 shares an arm with
 * 4 while 22 does not, and the 15/17/19 arm skips 16, 18 and 20. 2208 is
 * synthesised in the reference and stays decimal; 0x928 and 0x93e are pool
 * loads.
 */
u8 *SceneData_SelectTableBySceneIndexAndFlags(void)
{
    s16 *tbl = Data_02000240;
    s32 scene = tbl[225];

    switch (scene) {
    case 1:
    case 2:
        if (GameFlag_IsSet(2208) != 0) {
            return FuneHeya_SceneTable07;
        }
        if (GameFlag_IsSet(0x928) != 0 && GameFlag_IsSet(0x93e) == 0) {
            return FuneHeya_SceneTable06;
        }
        return FuneHeya_SceneTable05;
    case 4:
    case 23:
        if (GameFlag_IsSet(0x93e) != 0) {
            return FuneHeya_SceneTable14;
        }
        return FuneHeya_SceneTable11;
    case 5:
        if (GameFlag_IsSet(2208) != 0) {
            return FuneHeya_SceneTable09;
        }
        if (GameFlag_IsSet(0x93e) != 0) {
            return FuneHeya_SceneTable10;
        }
        return FuneHeya_SceneTable08;
    case 15:
    case 17:
    case 19:
        return FuneHeya_SceneTable12;
    case 21:
        return FuneHeya_SceneTable13;
    default:
        break;
    }

    return FuneHeya_SceneTable04;
}

void FieldScene_RunScene3b1_02000670(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Battle_ResetEffectCounterFar();
    if (GameFlag_IsSet(0x921) != 0) {
        Event_SetMessage(MSG_IF_SHIP_FROM_TOLBI_HAD);
        Event_ShowMessage(10, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Event_SetMessage(MSG_NOW_WANT_SEE_CAPTAIN_TOO);
            Event_OpenMessage(10, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                FieldScene_RunExtendedActorChoreography();
                goto L_020006ea;
            }
            Actor_StartRepeatedMotion(10, 2);
            Event_ShowMessage(10, 0);
            Actor_FaceDirection(10, 0xd000, 0);
        } else {
            Event_SetMessage(MSG_BUT_WE_CANT_SEND_SHIP);
            Event_ShowMessage(10, 0);
        }
    }
    L_020006ea:;
    Event_End();
}

void SceneDialogue_RunActor12Line(void)
{
    Event_Begin();
    Event_SetMessage(LinkedMessage_YouCameAskCaptainSet);
    Event_AskYesNo(12, 0);
    Event_End();
}

void FieldScene_RunScene3b1_02000728(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x928) != 0) {
        Event_SetMessage(MSG_OARSMAN_WAS_INJURED);
        FieldScene_RunStepThen10(8);
        Actor_FaceDirection(8, 0xd000, 60);
        Actor_SetAnimationAndWait(8, 4);
        FieldScene_RunStepThen10(8);
        Actor_SetAnimationAndWait(8, 3);
    } else if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage(MSG_WE_DONT_KNOW_MIGHT_HAPPEN);
        Event_ShowMessage(8, 0);
    } else if (GameFlag_IsSet(0x921) != 0) {
        Event_SetMessage(MSG_BAD_LUCK_LOSING_MY_LUCKY);
        Event_ShowMessage(8, 0);
        if (GameFlag_IsSet(0x925) == 0 && GameFlag_IsSet(0x924) != 0) {
            gEventWork->unknown_172 = 1;
        }
    } else {
        Event_SetMessage(MSG_ITS_TOO_LATE_HIRE_MERCENARIES);
        Event_ShowMessage(8, 0);
    }
    Event_End();
}

void FieldScene_RunScene3b1_020007f8(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x925) != 0) {
        Actor_StartRepeatedMotion(8, 2);
        Event_SetMessage(MSG_THESE_PROUD_WARRIORS_NOT_GOING);
        FieldScene_RunStepThen10(8);
        Actor_FaceActor(8, ACTOR_PARTY_LEADER, 10);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(40);
            FieldScene_RunStepThen10(8);
            Value2(FieldScene_CallPairWith10, 8, 0x3000);
            Event_ShowMessage(8, 0);
            goto L_0200088e;
        }
        bump_step(2);
        Event_ShowMessage(8, 0);
        Actor_FaceDirection(8, 0x3000, 0);
    } else {
        Event_SetMessage(MSG_LONGER_WE_SIT_HERE_MORE);
        Event_ShowMessage(8, 0);
    }
    L_0200088e:;
    Event_End();
}

void SceneDialogue_ShowLine1E19Or1D50(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage(LinkedMessage_YouGoingRow);
        Event_AskYesNo(10, 0);
    } else {
        Event_SetMessage(LinkedMessage_TheresWholeBunchReallyMuscular);
        Event_ShowMessage(10, 0);
    }
    Event_End();
}

/*
 * Flag-branched scene setup for overlay resource_3b1. Each callee name
 * refers to that call site's own call word rather than to a shared runtime
 * address.
 */

/*
 * Actors 24 and 25 setup for overlay resource_3b1. Each callee name refers
 * to its own call word rather than to a shared runtime address.
 */

/* Scene setup for resource_3b1: installs actors 10 through 17. */

/*
 * Set up actors 24 and 25 -- resource_3b1. A flat setter sequence with no
 * branches; the owner includes its one literal pool word.
 */

/* The pool word, referenced by address so that it is emitted. */

/*
 * The aliases name the call words encoded in the overlay image, and the
 * declarations are old-style because the call sites vary in arity.
 */

/*
 * Actors 24 and 25 setup for overlay resource_3b1. Each callee slot uses
 * its own local veneer, so the names are per call site and not the shared
 * main-image symbol.
 */
void SceneState_RunFlagBranchedActor8Setup(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *obj = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        FuneHeya_TurnActorToOpenSide(obj);
        Event_SetMessage(LinkedMessage_LooksLikeYouveBeenChosen);
        FieldScene_RunStepThen10(8);
        Actor_SetAnimation(obj, 2);
        p = Object_GetByIdFar(0);
        if (p != 0) {
            Actor_SetDestination(obj, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(obj);
        Actor_SetPosition(obj, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(8, 0x1e78, 0x990);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(8, 0x1e78, 0x917);
    } else if (GameFlag_IsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(8, 0x1e78, 0x935);
    } else {
        FieldScene_RunPrimarySequence(8, 0x1e78, 0x92c);
    }
}

void SceneDialogue_RunActorTenFlaggedDialogue(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        FuneHeya_TurnActorToOpenSide(o);
        Event_SetMessage(LinkedMessage_HaHaHaChosenOarsman);
        FieldScene_RunStepThen10(10);
        Actor_SetAnimation(o, 2);
        p = Object_GetByIdFar(0);
        if (p != 0) {
            Actor_SetDestination(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Actor_WaitForMove(o);
        Actor_SetPosition(o, 0, 0);
        Event_End();
    } else if (GameFlag_IsSet(0x92b) != 0) {
        FieldScene_RunPrimarySequence(10, 0x1e7b, 0x992);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(10, 0x1e7b, 0x919);
    } else if (GameFlag_IsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(10, 0x1e7b, 0x937);
    } else {
        FieldScene_RunPrimarySequence(10, 0x1e7b, 0x92e);
    }
}
