#include "HAIDIA_BABI.H"

/* The innkeeper's and the villagers' talk about the house. */


void HaidiaBabi_RunInnkeeperTalk(void)
{
    u32 i;
    s32 record;

    record = (s32)Engine_ActorGet(0);
    if ((u32)(*(u16 *)(record + 6) + -0x2000) > 0x9000) {
        Inn_Open(0, 13);
    } else {
        Event_Begin();
        if (GameFlag_IsSet(0x87a) != 0) {
            Actor_RunRepeatedMotion(13, 2);
            Actor_FaceActor(13, ACTOR_PARTY_LEADER, 10);
            if (GameFlag_IsSet(0x300) == 0) {
                Event_SetMessage(MSG_YOU_CAME_BACK);
                Event_ShowMessage(13, 0);
                GameFlag_Set(0x300);
            }
            Event_SetMessage(MSG_HOME_JUST_TO_STAY);
            Event_AskYesNo(13, 0);
            Actor_FaceDirection(13, 0x9000, 10);
        } else {
            if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
                Event_SetMessage(MSG_THE_VISITORS_CAUSED_THE_ERUPTION);
            } else {
                Event_SetMessage(MSG_THE_THREE_TRAVELERS_SEEM_ODD);
            }
            Event_ShowMessage(13, 0);
        }
        Event_End();
    }
}

void SceneDialogue_ShowLine1C13WithActor16Steps(void)
{
    Event_Begin();
    Actor_FaceActor(0x10, ACTOR_PARTY_LEADER, 0xA);
    Event_SetMessage(MSG_YOUVE_GROWN_SO_MUCH);
    Event_ShowMessage(0x10, 0);
    Actor_FaceDirection(0x10, 0xB000, 0xA);
    GameFlag_Set(0x301);
    Event_End();
}

void SceneDialogue_RunActorThirteenDialogue(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DORA_WOULDNT_LET_HIM_STAY);
    Event_ShowMessage(0xD, 0);
    GameFlag_Set(0x81C);
    Event_End();
}

void SceneDialogue_RunActor16LineAndFlag81c(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DORA_WAS_STRUCK_WITH_ILLNESS);
    Event_ShowMessage(0x10, 0);
    GameFlag_Set(0x81C);
    Event_End();
}
