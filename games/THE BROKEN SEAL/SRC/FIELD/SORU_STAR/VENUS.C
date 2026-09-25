#include "TYPES.H"
extern u8 LinkedMessage_VenusStarBagged;

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

/* FAKEMATCH: call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Scene_BagVenusStar(void)
{
    u32 i;
    s32 obj;
    s32 mes;

    Engine_EventBegin();
    Engine_AudioPlayCue(141);
    for (i = 0; i != 6; i++) {
        Call2(Engine_ColorBufferApplyTarget, 0x403a52, 1);
        Engine_ColorBufferInterpolate(8);
        Engine_EventWait(8);
        Call2(Engine_ColorBufferApplyTarget, 0x10000, 1);
        Engine_ColorBufferInterpolate(8);
        Engine_EventWait(8);
        if (i == 1) {
            Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
        }
    }
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_EventWait(30);
    Call2(Engine_CameraSetSpeed, 0x26666, 0x4ccc);
    Call4(Engine_CameraMoveTo, 0x2980000, -1, 0x1f10000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(0x200d088, 96, 29);
    Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 41, 29);
    Call6(Engine_MapCopyCellsTo, 87, 42, 41, 31, 1, 2);
    Engine_EventWait(40);
    Call3(Engine_WorkSetValuesIfNonNegative, 0, 0, 0);
    Call2(Engine_CameraSetSpeed, 0x66666, 0xcccc);
    Call4(Engine_CameraMoveTo, 0x1370000, -1, 0x1f10000, 1);
    Engine_CameraWaitForMove();
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(0x200d088, 74, 29);
    Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 19, 29);
    Call6(Engine_MapCopyCellsTo, 87, 42, 19, 31, 1, 2);
    Engine_EventWait(40);
    Call3(Engine_WorkSetValuesIfNonNegative, 0, 0, 0);
    Call4(Engine_CameraMoveTo, 0x2970000, -1, 0xc00000, 1);
    Engine_CameraWaitForMove();
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(0x200d088, 96, 10);
    Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 41, 10);
    Call6(Engine_MapCopyCellsTo, 87, 42, 41, 12, 1, 2);
    Engine_EventWait(40);
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x202;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call4(Engine_CameraMoveTo, 0x2c60000, -1, 0x1da0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x10000, 0x10000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Call1(Engine_AudioPlayCue, 0x121);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Engine_EventWait(20);
    Call6(Engine_MapCopyCellsTo, 0, 40, 43, 66, 3, 3);
    Engine_EventWait(20);
    obj = Value4(Scene_PresentItem, 220, 0x2c80000, 0x100000, 0x1d00000);
    Engine_EventWait(40);
    UiWork_PushValueSlotFar(obj, 1);
    mes = (s32)&LinkedMessage_VenusStarBagged;
    Value2(Engine_MessageShowCentered, mes, 1);
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
    Call4(Engine_CameraMoveTo, 0x2c60000, -1, 0x1da0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Call1(Engine_GameFlagSet, 0x83c);
    Engine_EventEnd();
}
