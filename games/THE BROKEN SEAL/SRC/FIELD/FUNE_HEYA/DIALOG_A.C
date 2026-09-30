#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "HEYA.H"

#include "STAGED_ACTOR.H"
extern u8 MsgFuneBadLuckLosingMyLucky[];
extern u8 MsgFuneButWeCantSendShip[];
extern u8 MsgFuneHaHaHaChosenOarsman[];
extern u8 MsgFuneIfShipFromTolbiHad[];
extern u8 MsgFuneItsTooLateHireMercenaries[];
extern u8 MsgFuneJokingWantRow[];
extern u8 MsgFuneLongerWeSitHereMore[];
extern u8 MsgFuneLooksLikeYouveBeenChosen[];
extern u8 MsgFuneNoooooNotGoing[];
extern u8 MsgFuneNowWantSeeCaptainToo[];
extern u8 MsgFuneOarsmanWasInjured[];
extern u8 MsgFuneTheresWholeBunchReallyMuscular[];
extern u8 MsgFuneTheseProudWarriorsNotGoing[];
extern u8 MsgFuneWeDontKnowMightHappen[];
extern u8 MsgFuneYouCameAskCaptainSet[];
extern u8 MsgFuneYouGoingRow[];

/* Message ids. */

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

/*
 * Field scene beat for overlay resource_3b1.  Each callee is named for its own
 * call site: every call reaches its target through its own local veneer, even
 * where the same logical callee is used from more than one site.
 */

void FieldScene_CallPairWith10(s32 a, u16 b);

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
        Event_SetMessage((s32)MsgFuneIfShipFromTolbiHad);
        Event_ShowMessage(10, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Event_SetMessage((s32)MsgFuneNowWantSeeCaptainToo);
            Event_OpenMessage(10, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                FieldScene_RunExtendedActorChoreography();
                goto L_020006ea;
            }
            Actor_StartRepeatedMotion(10, 2);
            Event_ShowMessage(10, 0);
            Actor_FaceDirection(10, 0xd000, 0);
        } else {
            Event_SetMessage((s32)MsgFuneButWeCantSendShip);
            Event_ShowMessage(10, 0);
        }
    }
    L_020006ea:;
    Event_End();
}

void SceneDialogue_RunActor12Line(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgFuneYouCameAskCaptainSet);
    Event_AskYesNo(12, 0);
    Event_End();
}

void FieldScene_RunScene3b1_02000728(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x928) != 0) {
        Event_SetMessage((s32)MsgFuneOarsmanWasInjured);
        FieldScene_RunStepThen10(8);
        Actor_FaceDirection(8, 0xd000, 60);
        Actor_SetAnimationAndWait(8, 4);
        FieldScene_RunStepThen10(8);
        Actor_SetAnimationAndWait(8, 3);
    } else if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage((s32)MsgFuneWeDontKnowMightHappen);
        Event_ShowMessage(8, 0);
    } else if (GameFlag_IsSet(0x921) != 0) {
        Event_SetMessage((s32)MsgFuneBadLuckLosingMyLucky);
        Event_ShowMessage(8, 0);
        if (GameFlag_IsSet(0x925) == 0 && GameFlag_IsSet(0x924) != 0) {
            gEventWork->unknown_172 = 1;
        }
    } else {
        Event_SetMessage((s32)MsgFuneItsTooLateHireMercenaries);
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
        Event_SetMessage((s32)MsgFuneTheseProudWarriorsNotGoing);
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
        Event_SetMessage((s32)MsgFuneLongerWeSitHereMore);
        Event_ShowMessage(8, 0);
    }
    L_0200088e:;
    Event_End();
}

void SceneDialogue_ShowLine1E19Or1D50(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage((s32)MsgFuneYouGoingRow);
        Event_AskYesNo(10, 0);
    } else {
        Event_SetMessage((s32)MsgFuneTheresWholeBunchReallyMuscular);
        Event_ShowMessage(10, 0);
    }
    Event_End();
}

/* Scene setup for resource_3b1: installs actors 10 through 17. */

/* The pool word, referenced by address so that it is emitted. */

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
        Event_SetMessage((s32)MsgFuneLooksLikeYouveBeenChosen);
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
        FieldScene_RunPrimarySequence(8, (s32)MsgFuneNoooooNotGoing, 0x990);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(8, (s32)MsgFuneNoooooNotGoing, 0x917);
    } else if (GameFlag_IsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(8, (s32)MsgFuneNoooooNotGoing, 0x935);
    } else {
        FieldScene_RunPrimarySequence(8, (s32)MsgFuneNoooooNotGoing, 0x92c);
    }
}

void SceneDialogue_RunActorTenFlaggedDialogue(void)
{
    if (GameFlag_IsSet(0x300) != 0) {
        u8 *o = SceneState_ApplyLevelFromFlags();
        u8 *p;

        Event_Begin();
        FuneHeya_TurnActorToOpenSide(o);
        Event_SetMessage((s32)MsgFuneHaHaHaChosenOarsman);
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
        FieldScene_RunPrimarySequence(10, (s32)MsgFuneJokingWantRow, 0x992);
    } else if (GameFlag_IsSet(0x92a) != 0) {
        FieldScene_RunPrimarySequence(10, (s32)MsgFuneJokingWantRow, 0x919);
    } else if (GameFlag_IsSet(0x929) != 0) {
        FieldScene_RunPrimarySequence(10, (s32)MsgFuneJokingWantRow, 0x937);
    } else {
        FieldScene_RunPrimarySequence(10, (s32)MsgFuneJokingWantRow, 0x92e);
    }
}
