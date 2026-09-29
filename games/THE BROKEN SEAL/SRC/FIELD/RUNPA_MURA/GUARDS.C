#include "VILLAGE.H"
extern u8 MsgRunpaLeftGuardGlimpsedMerchant[];
extern u8 MsgRunpaLeftGuardDismissesThought[];
extern u8 MsgRunpaLeftGuardRecognizesHammet[];
extern u8 MsgRunpaLeftGuardThoughts[];
extern u8 MsgRunpaRightGuardBoasts[];
extern u8 MsgRunpaRightGuardFeelsCreepy[];
extern u8 MsgRunpaRightGuardWondersHow[];

void RightGuard_Talk(void)
{
    if (gGameState.cloaked != 0) {
        Event_SetMessage((s32)MsgRunpaRightGuardFeelsCreepy);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage((s32)MsgRunpaRightGuardWondersHow);
    } else {
        Event_SetMessage((s32)MsgRunpaRightGuardBoasts);
    }
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
}

/* Reading the left guard's mind while Hammet is near makes him look up. */
void LeftGuard_MindRead(void)
{
    s32 seen;

    if (gGameState.cloaked == 0 && GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0
        && GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
        seen = GameFlag_IsSet(FLAG_GATE_GUARD_SAW_HAMMET);
        if (seen == 0) {
            gEventWork->psynergy_request = seen;
            Psynergy_Cancel();
            Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
            Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
            Event_SetMessage((s32)MsgRunpaLeftGuardRecognizesHammet);
            GameFlag_Set(FLAG_GATE_GUARD_SAW_HAMMET);
        } else {
            Event_SetMessage((s32)MsgRunpaLeftGuardGlimpsedMerchant);
        }
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        Event_SetMessage((s32)MsgRunpaLeftGuardDismissesThought);
    } else {
        Event_SetMessage((s32)MsgRunpaLeftGuardThoughts);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}
