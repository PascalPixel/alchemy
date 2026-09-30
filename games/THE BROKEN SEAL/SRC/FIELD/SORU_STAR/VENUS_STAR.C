#include "TYPES.H"
#include "CALL.H"
extern u8 MsgSoruVenusStarBagged[];
extern u16 SoruStar_StarCells[];
extern struct EventWork *gEventWork;

/* The party bags the Venus Star while its chamber changes around them. */

s32 Scene_PresentItem();
void Event_SayThenWait();
void Engine_WorkSetValuesIfNonNegative();
void Engine_MapAnimateCells();
void Engine_EventBegin();
void Engine_EventWait();
void Engine_MapCopyCellAttributes();
void Engine_MapCopyCellsTo();
void Engine_TaskWait();
void Engine_MapRedraw();
void Engine_ColorBufferApplyTarget();
void Engine_ColorBufferInterpolate(s32 frames);
void Engine_CameraSetSpeed();
void Engine_AudioPlayCue();
void Engine_CameraWaitForMove();
void Engine_MapRenderWaitForValues();
void UiWork_PushValueSlotFar();
s32 Engine_MessageShowCentered();
void Engine_CameraMoveTo();
void Engine_GameFlagSet();
void Engine_EventEnd();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();
void Engine_ActorJump();
void Engine_EventOpenScreen();
void Engine_ActorFaceDirection();
void Engine_EventSetMessage();

void Scene_BagVenusStar(void)
{
    u32 i;
    s32 obj;
    s32 mes;

    Engine_EventBegin();
    Engine_AudioPlayCue(141);
    for (i = 0; i != 6; i++) {
        Engine_ColorBufferApplyTarget(0x403a52, 1);
        Engine_ColorBufferInterpolate(8);
        Engine_EventWait(8);
        Engine_ColorBufferApplyTarget(0x10000, 1);
        Engine_ColorBufferInterpolate(8);
        Engine_EventWait(8);
        if (i == 1) {
            Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        }
    }
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_EventWait(30);
    Engine_CameraSetSpeed(0x26666, 0x4ccc);
    Call4(Engine_CameraMoveTo, 0x2980000, -1, 0x1f10000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 96, 29);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 41, 29);
    Call6(Engine_MapCopyCellsTo, 87, 42, 41, 31, 1, 2);
    Engine_EventWait(40);
    Engine_WorkSetValuesIfNonNegative(0, 0, 0);
    Engine_CameraSetSpeed(0x66666, 0xcccc);
    Call4(Engine_CameraMoveTo, 0x1370000, -1, 0x1f10000, 1);
    Engine_CameraWaitForMove();
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 74, 29);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 19, 29);
    Engine_MapCopyCellsTo(87, 42, 19, 31, 1, 2);
    Engine_EventWait(40);
    Engine_WorkSetValuesIfNonNegative(0, 0, 0);
    Call4(Engine_CameraMoveTo, 0x2970000, -1, 0xc00000, 1);
    Engine_CameraWaitForMove();
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 96, 10);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 41, 10);
    Engine_MapCopyCellsTo(87, 42, 41, 12, 1, 2);
    Engine_EventWait(40);
    *(s32 *)((u8 *)gEventWork + 0x1c0) = 0x202;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call4(Engine_CameraMoveTo, 0x2c60000, -1, 0x1da0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x10000, 0x10000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(0x121);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Engine_EventWait(20);
    Engine_MapCopyCellsTo(0, 40, 43, 66, 3, 3);
    Engine_EventWait(20);
    obj = Scene_PresentItem(220, 0x2c80000, 0x100000, 0x1d00000);
    Engine_EventWait(40);
    UiWork_PushValueSlotFar(obj, 1);
    mes = (s32)MsgSoruVenusStarBagged;
    Engine_MessageShowCentered(mes, 1);
    Call3(Engine_ActorFaceDirection, 9, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 5, 0x2000, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call4(Engine_CameraMoveTo, 0x1ce0000, -1, 0x15e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorJump(9, 4, 30);
    Engine_EventSetMessage(mes - 1);
    Event_SayThenWait(9, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x2c60000, -1, 0x1da0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet(0x83c);
    Engine_EventEnd();
}
