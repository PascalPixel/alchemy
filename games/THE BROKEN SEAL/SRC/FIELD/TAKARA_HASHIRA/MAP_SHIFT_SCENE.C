#include "TYPES.H"
#include "CALL.H"

s32 Engine_GameFlagIsSet();
void Engine_GameFlagSet();
void Engine_EventBegin();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventWait();
void Engine_MapCopyCellsTo();
s32 Engine_TaskAddCallback();
s32 Engine_ActorGet();
void Engine_ActorSetAnimation();
void ObjectMotion_WaitForAnimationChange();
void Engine_MapCopyCellAttributes();
void Engine_GameFlagClear();
void Engine_ActorSetChildValue();
void Engine_ActorEnableActionCallback();
void Engine_TaskRemoveCallback();
void Engine_EventEnd();
void SceneEffect_SpawnRandomizedParticle();
void SceneEffect_SpawnRandomEffectEveryEightFrames();

/* Crossbone Isle: the first time (flag 0x203 clear) set flag 0x202, pan the
 * camera and shift the cells at column 73 while a task runs; with flag
 * 0x201, actor 12 plays its part and the cells at (17, 13) are copied. */
void TakaraHashira_RunMapShiftScene(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Engine_GameFlagIsSet(0x203);
    if (rec7 == 0) {
        Engine_GameFlagSet(0x202);
        Engine_EventBegin();
        Engine_CameraSetSpeed(0x9999, 0x1333);
        Engine_CameraMoveTo(0x1380000, -1, 0xb80000, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(20);
        Call6(Engine_MapCopyCellsTo, 73, 10, 60, 10, 1, 2);
        Engine_EventWait(20);
        Engine_TaskAddCallback((s32)SceneEffect_SpawnRandomizedParticle, 0xc80);
        Engine_EventWait(40);
        if (Value1(Engine_GameFlagIsSet, 0x201) != 0) {
            record = Engine_ActorGet(12);
            *(s32 *)(record + 108) = (s32)SceneEffect_SpawnRandomEffectEveryEightFrames;
            Engine_ActorSetAnimation(12, 6);
            ObjectMotion_WaitForAnimationChange(12);
            record = Engine_ActorGet(12);
            *(s32 *)(record + 108) = rec7;
            Call6(Engine_MapCopyCellAttributes, 17, 13, 1, 1, 18, 13);
            Engine_GameFlagClear(0x201);
            Engine_ActorSetChildValue(12, 0);
            Engine_ActorEnableActionCallback(12, 1);
        } else {
            Engine_EventWait(60);
        }
        Engine_TaskRemoveCallback((s32)SceneEffect_SpawnRandomizedParticle);
        Engine_EventWait(20);
        Call6(Engine_MapCopyCellsTo, 72, 10, 60, 10, 1, 2);
        Engine_EventWait(20);
        Engine_EventEnd();
    }
}
