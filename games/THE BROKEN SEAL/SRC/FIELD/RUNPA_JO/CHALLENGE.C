/* The Lunpa fortress: the other guards' challenges, which also put the party
 * out and set a flag. */
#include "FORTRESS.H"
extern u8 MsgRunpaWho2[];

void RunActor12InteractionSequence(void)
{
    Event_Begin();
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Audio_PlayCue(113);
    Actor_ShowEmote(12, 256, 60);
    {
        s32 t = (s32)MsgRunpaWho2;

        Event_SetMessage(t);
        Event_ShowMessage(12, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 50);
        Event_SetMessage(t + 1);
    }
    Event_ShowMessage(12, 0);
    Event_CloseScreen();
    Event_Wait(60);
    Event_RequestExit(60);
    Event_End();
    GameFlag_Set(548);
}

void RunActors13And21InteractionSequence(void)
{
    u32 i;
    s32 record;
    s32 msg;

    Event_Begin();
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 60);
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    msg = (s32)MsgRunpaWho2;
    Event_SetMessage(msg);
    Event_ShowMessage(13, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 30);
    Event_SetMessage(msg + 1);
    Event_ShowMessage(13, 0);
    Event_CloseScreen();
    Event_Wait(60);
    Event_RequestExit(60);
    Event_End();
    GameFlag_Set(0x225);
}
