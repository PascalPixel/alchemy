#include "STORY.H"
extern u8 MsgWorldMapNowUseOnShip[];
extern u8 MsgWorldMapRobinWhereGoingSaidUse[];
extern u8 MsgWorldMapWreckageShipScuttledOffCoast[];

/* The actor that speaks for the world map's triggers; the scene entry and the
 * Black Orb scene set it. */
s32 gWorldMapTriggerActor;

void FieldScene_RunScene371_0200281c(void)
{
    Event_Begin();
    Actor_FaceActor(55, ACTOR_PARTY_LEADER, 0);
    Event_SetMessage((s32)MsgWorldMapNowUseOnShip);
    Engine_EventShowMessage(gWorldMapTriggerActor, 0);
    Actor_FaceDirection(55, 0x3000, 0);
    Event_End();
}

void FieldScene_RunScene371_02002858(void)
{
    Event_Begin();
    Battle_SetObjectFlag5bWhenMode3();
    Event_SetMessage((s32)MsgWorldMapRobinWhereGoingSaidUse);
    Engine_EventShowMessage(gWorldMapTriggerActor, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1778, 0xd48);
    Event_End();
}

/* Runs dialogue 0x264c and publishes the story result when flag 0x234 is
 * set. */
void StoryScene_ShowRewardDialogue(void)
{

    Event_Begin();
    Battle_SetObjectFlag5bWhenMode3();
    Message_ShowCentered((s32)MsgWorldMapWreckageShipScuttledOffCoast, 1);
    if (GameFlag_IsSet(0x234) != 0) {
        ((struct StoryDialogueWork *)gEventWork)->story_result = 1;
    }
    Battle_ClearObjectFlag5bWhenMode3();
    Event_End();
}
