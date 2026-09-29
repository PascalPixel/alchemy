/* Draft of RunActor9ScriptedSequence, resource_3bf at 0x02009ab0, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines
 * (Engine_ActorEnableActionCallback, Engine_EventRequestExit,
 * Engine_EventCloseScreen).
 * The listing keeps these rows. */
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
