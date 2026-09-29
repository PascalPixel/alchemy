/* Draft of RunActorScriptedSequenceA, resource_3bf at 0x02009b24, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines
 * (Engine_ActorEnableActionCallback, Engine_EventCloseScreen,
 * Engine_EventRequestExit).
 * The listing keeps these rows. */
#include "FORTRESS.H"
extern u8 MsgRunpaWho2[];

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
