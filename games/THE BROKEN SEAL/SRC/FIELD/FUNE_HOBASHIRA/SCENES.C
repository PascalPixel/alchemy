#include "FUNE.H"
#include "CALL.H"
extern u8 MsgFuneShipsCourseClear[];

void FieldScene_RunScene3b0_0200040c(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    *(u8 *)(Battle_GetWorkObject1e0() + 85) = 0;
    Call3(Engine_CameraMoveTo, 0xa40000, 0x400000, 0x1410000);
    Map_Redraw();
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    FieldScene_RunScene3b0_020004b0();
    Event_End();
}

void FieldScene_RunScene3b0_02000468(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xa40000, 0x1410000);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = (u8 *)Engine_ActorGet(0);
    Actor_SetSpriteFlags(record, 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    FieldScene_RunScene3b0_020004b0();
    Event_End();
}

void FieldScene_RunScene3b0_020004b0(void)
{
    gEventWork->start_transition = 0x202;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkToAndWait(8, 164, 0x141);
    Actor_FaceDirection(8, 0xd000, 40);
    Actor_FaceDirection(8, 0xb000, 40);
    Actor_FaceDirection(8, 0xd000, 40);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_WalkToAndWait(8, 164, 0x14e);
    Actor_Jump(8, 4, 40);
    Actor_StartRepeatedMotion(8, 2);
    Event_SetMessage((s32)MsgFuneShipsCourseClear);
    Event_ShowMessageAndWait(8, 0, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(10);
}

/* Runs the record-8/record-9 pair through two near-identical setup-then-move
 * cycles (position waypoints, a movement flag reset, then animation/sound
 * calls), followed by a shorter closing cycle for record 8 alone. */