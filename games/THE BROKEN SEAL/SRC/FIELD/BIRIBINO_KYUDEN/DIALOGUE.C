#include "KYUDEN.H"

void FieldScene_RunStartledGuard(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_ShowEmote(14, 0x102, 0);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(40);
    Event_SetMessage(MSG_AAAAH);
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
    Event_SetMessage(MSG_FINE_WARRIOR);
    if (GameFlag_IsSet(0x302) != 0) {
        Event_SetMessage(MSG_ITS_VERY_RECKLESS_FOR_SUCH);
    }
    Event_ShowMessage(15, 0);
    GameFlag_Set(0x302);
    Event_End();
}

void SceneDialogue_RunActor16Message1769(void)
{
    Event_Begin();
    Event_SetMessage(MSG_WAS_WATCHING_FROM_HERE_AFTER);
    Event_AskYesNo(16, 0);
    Event_End();
}

void FieldScene_RunActorSeventeenFlaggedDialogue(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x202) != 0) {
        Event_SetMessage(MSG_MAY_CHOOSE_ONLY_ONE_ITEM);
    } else if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
        Event_SetMessage(MSG_REWARD_RECEIVED_WAS_INDEED_GREATEST);
    } else {
        Event_SetMessage(MSG_DO_THINK_THESE_BILIBINS_GREAT);
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
    Event_SetMessage(MSG_SOMETIMES_WE_NEED_CHILDREN_REMIND);
    if (GameFlag_IsSet(0x303) != 0) {
        Event_SetMessage(MSG_LOOKED_VERY_COURAGEOUS_WALKING_TOWARD);
    }
    Event_ShowMessage(15, 0);
    GameFlag_Set(0x303);
    Event_End();
}

void FieldScene_RunActorSeventeenFlagDialogue(void)
{
    Event_Begin();

    if (GameFlag_IsSet(0x202) != 0) {
        Event_SetMessage(MSG_NONETHELESS_IF_YOUR_LUCK_SOUR);
    } else if (GameFlag_IsSet(0x845) == 0) {
        Event_SetMessage(MSG_GOOD_TREASURE_IF_GET_TURNED);
    } else {
        Event_SetMessage(MSG_WE_REALLY_GIVING_OUR_TREASURE);
        if (GameFlag_IsSet(FLAG_REWARD_TAKEN) != 0) {
            Event_SetMessage(MSG_GOT_PRETTY_NICE_REWARD_BUT);
        }
    }

    Event_ShowMessage(17, 0);
    Event_End();
}
