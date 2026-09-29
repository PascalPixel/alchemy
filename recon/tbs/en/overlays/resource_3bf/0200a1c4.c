/* Draft of FieldScene_RunScene3bf_020021c4, resource_3bf at 0x0200a1c4, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines
 * (Engine_ActorEnableActionCallback, Engine_EventCloseScreen,
 * Engine_EventRequestExit).
 * The listing keeps these rows. */
#include "FORTRESS.H"
extern u8 MsgRunpaWho2[];

void FieldScene_RunScene3bf_020021c4(void)
{
    u32 i;
    s32 record;
    s32 base5_240d;

    Event_Begin();
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 60);
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    base5_240d = (s32)MsgRunpaWho2;
    Event_SetMessage(base5_240d);
    Event_ShowMessage(13, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 30);
    Event_SetMessage((base5_240d + 1));
    Event_ShowMessage(13, 0);
    Event_CloseScreen();
    Event_Wait(60);
    Event_RequestExit(60);
    Event_End();
    GameFlag_Set(0x225);
}
