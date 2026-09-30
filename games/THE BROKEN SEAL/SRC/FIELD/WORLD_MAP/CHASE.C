#include "STORY.H"

void FieldScene_RunScene371_02002274(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Object_GetById(10);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Task_Wait(1);
    actor->scale_x = 0x18000;
    actor->scale_y = 0x18000;
    actor->facing = 0x4000;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetPosition(10, 0x15680000, 0x8380000);
    Task_Wait(1);
    Audio_PlayCue(141);
    Actor_SetSpeed(10, 0x19999, 0x6666);
    Actor_SetAnimation(10, 2);
    Actor_MoveToAndWait(10, 0x156d, 0x858);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x15b80000, -1, 0x8580000, 1);
    Actor_MoveToAndWait(10, 0x159e, 0x858);
    Actor_MoveToAndWait(10, 0x15a8, 0x86e);
    Actor_MoveToAndWait(10, 0x15e8, 0x878);
    Actor_SetAnimation(10, 1);
    Audio_PlayCue(0x121);
    Event_Wait(20);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x15d80000, 0x8780000);
    Task_Wait(1);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x15c8, 0x878);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 40);
    Audio_PlayCue(141);
    Actor_SetAnimation(10, 2);
    Actor_MoveToAndWait(10, 0x15f8, 0x878);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_MoveToAndWait(10, 0x15f8, 0x838);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_MoveToAndWait(10, 0x15bd, 0x838);
    Actor_MoveToAndWait(10, 0x15b8, 0x853);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_MoveToAndWait(10, 0x1572, 0x858);
    Actor_MoveToAndWait(10, 0x1568, 0x838);
    Actor_SetPosition(10, 0, 0);
    Audio_PlayCue(0x121);
    Event_Wait(40);
    Camera_MoveTo(0x15d80000, -1, 0x8580000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x15d8, 0x858);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(20);
    Event_End();
}
