#include "VILLAGE.H"

void RightGuard_Talk(void)
{
    if (gGameState.cloaked != 0) {
        Event_SetMessage(MSG_RIGHT_GUARD_FEELS_CREEPY);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage(MSG_RIGHT_GUARD_WONDERS_HOW);
    } else {
        Event_SetMessage(MSG_RIGHT_GUARD_BOASTS);
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
            Event_SetMessage(MSG_LEFT_GUARD_RECOGNIZES_HAMMET
                             + RECOGNITION_SEEN_THAT_MAN);
            GameFlag_Set(FLAG_GATE_GUARD_SAW_HAMMET);
        } else {
            Event_SetMessage(MSG_LEFT_GUARD_GLIMPSED_MERCHANT);
        }
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        Event_SetMessage(MSG_LEFT_GUARD_RECOGNIZES_HAMMET + RECOGNITION_IMPOSSIBLE);
    } else {
        Event_SetMessage(MSG_LEFT_GUARD_THOUGHTS);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}
