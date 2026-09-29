/* The Lunpa fortress: a guard who catches the party asks who they are, and
 * the party is put out of the fortress. */
#include "FORTRESS.H"
extern u8 MsgRunpaWho2[];

void RunActor9ScriptedSequence(void)
{
    Event_Begin();
    Actor_SetDestinationOffset(9, 0, 0);
    Actor_EnableActionCallback(9, 1);
    Actor_Stop(9);
    Actor_SetAnimation(9, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    {
        u8 *t = (s32)MsgRunpaWho2;

        Event_SetMessage((s32)t);
        Event_ShowMessage(9, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Event_SetMessage((s32)(t + 1));
    }
    Event_ShowMessage(9, 0);
    Event_RequestExit(60);
    Event_CloseScreen();
    Event_End();
}

void RunActorScriptedSequenceA(s32 actor_id)
{
    Event_Begin();
    Event_Begin();
    Actor_ShowEmote(actor_id, 256, 1);
    Actor_SetDestinationOffset(actor_id, 0, 0);
    Actor_EnableActionCallback(actor_id, 1);
    Actor_SetAnimation(actor_id, 0);
    Actor_FaceActor(actor_id, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetDestinationOffset(actor_id, 0, 0);
    Actor_EnableActionCallback(actor_id, 1);
    Actor_Stop(actor_id);
    Actor_SetAnimation(actor_id, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    {
        u8 *t = (s32)MsgRunpaWho2;

        Event_SetMessage((s32)t);
        Event_ShowMessage(actor_id, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, actor_id, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Event_SetMessage((s32)(t + 1));
    }
    Event_ShowMessage(actor_id, 0);
    Event_CloseScreen();
    Event_Wait(60);
    Event_RequestExit(60);
    Event_End();
}
