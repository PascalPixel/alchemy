#include "ENTRANCE.H"

void MakyuriIriguchi_RunDoorScene(void)
{
    s32 record;

    if (GameFlag_IsSet(0x250) == 0) {
        GameFlag_Set(0x250);
        Event_Begin();
        record = Actor_Get(12);
        *(s32 *)(record + 24) = -0x10000;
        record = Object_GetById(13);
        *(s32 *)(record + 24) = -0x10000;
        record = Actor_Get(14);
        *(s32 *)(record + 24) = -0x10000;
        Actor_SetPosition(ACTOR_MIA, 0x880000, 0x900000);
        Actor_FaceDirection(ACTOR_MIA, 0x4000, 10);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(60);
        Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
        Actor_SetAnimationAndWait(ACTOR_MIA, 3);
        Event_Wait(30);
        Actor_WalkTo(ACTOR_MIA, 136, 72);
        Event_Wait(40);
        Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Actor_WaitForMove(ACTOR_MIA);
        Actor_SetPosition(ACTOR_MIA, 0, 0);
        GameFlag_Set(0x872);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
        Event_End();
    }
}
