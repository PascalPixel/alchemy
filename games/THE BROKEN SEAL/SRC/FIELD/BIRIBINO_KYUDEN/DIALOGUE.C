#include "KYUDEN.H"
extern u8 MsgBiribinoAaaah[];
extern u8 MsgBiribinoDoThinkTheseBilibinsGreat[];
extern u8 MsgBiribinoFineWarrior[];
extern u8 MsgBiribinoGoodTreasureIfGetTurned[];
extern u8 MsgBiribinoGotPrettyNiceRewardBut[];
extern u8 MsgBiribinoItsVeryRecklessForSuch[];
extern u8 MsgBiribinoLookedVeryCourageousWalkingToward[];
extern u8 MsgBiribinoMayChooseOnlyOneItem[];
extern u8 MsgBiribinoNonethelessIfYourLuckSour[];
extern u8 MsgBiribinoRewardReceivedWasIndeedGreatest[];
extern u8 MsgBiribinoSometimesWeNeedChildrenRemind[];
extern u8 MsgBiribinoWasWatchingFromHereAfter[];
extern u8 MsgBiribinoWeReallyGivingOurTreasure[];

void FieldScene_RunStartledGuard(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_ShowEmote(14, 0x102, 0);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(40);
    Event_SetMessage((s32)MsgBiribinoAaaah);
    Event_ShowMessageAndWait(14, 0, 20);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Event_ShowMessageAndWait(14, 0, 10);
    Actor_FaceDirection(14, 0xb000, 10);
    Event_End();
}

void FieldScene_RunScene38dSequenceA(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoFineWarrior);
    if (GameFlag_IsSet(0x302) != 0) {
        Event_SetMessage((s32)MsgBiribinoItsVeryRecklessForSuch);
    }
    Event_ShowMessage(15, 0);
    GameFlag_Set(0x302);
    Event_End();
}

void SceneDialogue_RunActor16Message1769(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoWasWatchingFromHereAfter);
    Event_AskYesNo(16, 0);
    Event_End();
}

void FieldScene_RunActorSeventeenFlaggedDialogue(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x202) != 0) {
        Event_SetMessage((s32)MsgBiribinoMayChooseOnlyOneItem);
    } else if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        Event_SetMessage((s32)MsgBiribinoRewardReceivedWasIndeedGreatest);
    } else {
        Event_SetMessage((s32)MsgBiribinoDoThinkTheseBilibinsGreat);
        if (GameFlag_IsSet(0x84d) != 0) {
            gEventWork->message++;
        }
    }

    Event_ShowMessage(17, 0);
    Event_End();
}

void SceneDialogue_RunActor15Flag303Scene(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoSometimesWeNeedChildrenRemind);
    if (GameFlag_IsSet(0x303) != 0) {
        Event_SetMessage((s32)MsgBiribinoLookedVeryCourageousWalkingToward);
    }
    Event_ShowMessage(15, 0);
    GameFlag_Set(0x303);
    Event_End();
}

void FieldScene_RunActorSeventeenFlagDialogue(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x202) != 0) {
        Event_SetMessage((s32)MsgBiribinoNonethelessIfYourLuckSour);
    } else if (GameFlag_IsSet(0x845) == 0) {
        Event_SetMessage((s32)MsgBiribinoGoodTreasureIfGetTurned);
    } else {
        Event_SetMessage((s32)MsgBiribinoWeReallyGivingOurTreasure);
        if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
            Event_SetMessage((s32)MsgBiribinoGotPrettyNiceRewardBut);
        }
    }

    Event_ShowMessage(17, 0);
    Event_End();
}
