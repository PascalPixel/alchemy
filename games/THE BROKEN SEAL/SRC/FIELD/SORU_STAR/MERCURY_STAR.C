#include "STAR.H"
extern u8 MsgSoruPutMercuryStar[];

void Scene_BagMercuryStar(void)
{
    u32 i;
    s32 obj;
    s32 mes;

    Event_Begin();
    Audio_PlayCue(141);
    for (i = 0; i != 6; i++) {
        ColorBuffer_ApplyTarget(0x404a4e, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        if (i == 1) {
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        }
    }
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(30);
    Work_SetValuesIfNonNegative(0, 0, 0);
    Camera_SetSpeed(0x59999, 0xb333);
    Camera_MoveTo(0x1d80000, -1, 0x620000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 84, 4);
    Map_CopyCellAttributes(0, 0, 1, 1, 29, 4);
    Map_CopyCellsTo(87, 42, 29, 6, 1, 2);
    Event_Wait(40);
    Work_SetValuesIfNonNegative(0, 0, 0);
    Camera_MoveTo(0x1570000, -1, 0x1710000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 76, 21);
    Map_CopyCellAttributes(0, 0, 1, 1, 21, 21);
    Map_CopyCellsTo(87, 42, 21, 23, 1, 2);
    Event_Wait(40);
    Work_SetValuesIfNonNegative(0, 0, 0);
    Camera_SetSpeed(0x33333, 0x6666);
    Camera_MoveTo(0x1570000, -1, 0x1f10000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 76, 29);
    Map_CopyCellAttributes(0, 0, 1, 1, 21, 29);
    Map_CopyCellsTo(87, 42, 21, 31, 1, 2);
    Event_Wait(40);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x2c80000, -1, 0x980000, 0);
    Map_Redraw();
    Task_Wait(1);
    Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Event_Wait(20);
    Map_CopyCellsTo(0, 40, 43, 46, 3, 3);
    Event_Wait(20);
    obj = Scene_PresentItem(221, 0x2c80000, 0x100000, 0x900000);
    Event_Wait(40);
    UiWork_PushValueSlotFar(obj, 1);
    mes = (s32)MsgSoruPutMercuryStar;
    Engine_MessageShowCentered(mes, 1);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xe000, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x1ce0000, -1, 0x15e0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_Jump(ACTOR_SUKURETA, 4, 30);
    Event_SetMessage(mes - 2);
    Event_SayThenWait(9, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x2c80000, -1, 0x980000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    GameFlag_Set(FLAG_MERCURY_STAR_BAGGED);
    Event_End();
}
