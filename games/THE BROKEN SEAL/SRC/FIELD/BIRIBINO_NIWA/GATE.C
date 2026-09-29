#include "NIWA.H"

void FieldScene_RunScene38e_0200045c(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x200) == 0) {
        FieldScene_OpenGate();
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
    Event_Wait(16);
    Event_RequestExit(2);
    Event_End();
}
