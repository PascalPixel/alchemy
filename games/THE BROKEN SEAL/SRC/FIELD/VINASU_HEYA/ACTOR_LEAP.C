#include "TYPES.H"
#include "CALL.H"

s32 OverlayObject_SpawnWithMode14();
void OverlayObject_WaitUntilIdle();
void Engine_ActorSetSpriteFlags();
void Engine_EventBegin();
s32 Engine_ActorGet();
s32 Engine_ActorGet();
void Engine_EventWait();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_MapCopyCellAttributes();
void Engine_EventWait();
void Engine_ActorMoveToAndWait();
void Engine_ActorSetAnimation();
void Engine_EventEnd();
void Engine_ActorSetAnimation();
void Engine_ActorRunRepeatedMotion();
void Object_SetActionById();
void Engine_CameraMoveTo();
void Engine_ActorSetAttachedEffect();
void Engine_ActorSetChildValue();
void Engine_EventRequestExit();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();

void Scene_RunActorLeapSequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 zero;

    zero = 0;
    rec7 = Engine_ActorGet(0);
    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    record = Engine_ActorGet(0);
    Engine_ActorSetSpriteFlags(record, 0);
    {
        s32 shown = 0x4000;
    
        *(u16 *)(rec7 + 6) = shown;
    }
    Call3(Engine_ActorSetSpeed, 0, 0x30000, 0x18000);
    Engine_ActorMoveToAndWait(0, *(s16 *)(rec7 + 10), 0x228);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(0, 22);
    Engine_EventWait(30);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Engine_ActorRunRepeatedMotion(0, 2);
    Engine_EventWait(20);
    {
        s32 shown = 0xc000;
    
        *(u16 *)(rec7 + 6) = shown;
    }
    Engine_ActorSetAnimation(0, 5);
    Object_SetActionById(0, 24);
    Engine_EventWait(40);
    *(s32 *)(rec7 + 72) = 0x9999;
    {
        s32 z = *(s32 *)(rec7 + 16) + 0x480000;

        *(s32 *)(rec7 + 68) = zero;
        OverlayObject_SpawnWithMode14(*(s32 *)(rec7 + 8), 0, z, 223);
    }
    Engine_MapCopyCellAttributes(34, 35, 5, 1, 34, 34);
    OverlayObject_WaitUntilIdle(0);
    Engine_ActorSetChildValue(0, 15);
    Engine_EventRequestExit(20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}
