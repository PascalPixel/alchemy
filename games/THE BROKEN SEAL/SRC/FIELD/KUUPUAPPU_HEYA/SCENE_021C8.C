#include "TYPES.H"
#include "CALL.H"
extern u8 MsgKuupuappuLearn[];
extern u8 MsgKuupuappuLeave[];
extern struct EventWork *gEventWork;

void Engine_AudioPlaySceneCue();
void Engine_ActorSetSpritePriority();
u8 * Object_GetById();
void Map_SetWorkFourValues();
void Engine_ActorSetSpeed();
void Engine_ActorSetPosition();
void Engine_ActorFaceEachOther();
void FieldScene_RunSplitTripleSteps();
void Engine_CameraFollowActor();
s32 Engine_CameraWaitForMove();
void Engine_MapRedraw();
void Engine_EventWait();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void SceneActor_SetModeZeroAndValue();
void SceneEffect_ApplyThreeValuesAndFinish();
void SceneActor_SetPairZeroAndValue();
void Engine_ActorFaceActor();
void Engine_EventShowMessage();
s32 Engine_ActorShowEmote();
s32 Engine_EventOpenMessage();
s32 Engine_UiWorkWaitThenFinalizeCapacity();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorWalkTo();
void Engine_ActorWalkToAndWait();

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

void KuupuappuHeya_RunScene021C8(void)
{
    u32 i;
    u8 *record;
    s32 v5;

    Engine_AudioPlaySceneCue();
    Engine_ActorSetSpritePriority(0, 1);
    {
        u8 *record = Object_GetById(0);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 1);
    }
    Map_SetWorkFourValues(0x200000, 0x2400000, 0x1900000, 0x3a80000);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 2, 0xcccc, 0x6666);
    Call3(Engine_ActorSetPosition, 0, 0xf80000, 0x2d80000);
    Call3(Engine_ActorSetPosition, 2, 0x1080000, 0x2e80000);
    Engine_ActorSetPosition(1, 0xe80000, 0x2e80000);
    record = Object_GetById(0);
    ((struct Flags9 *)(*(u8 **)(record + 80)))->mode = 1;
    record = Object_GetById(1);
    ((struct Flags9 *)(*(u8 **)(record + 80)))->mode = 1;
    record = Object_GetById(2);
    ((struct Flags9 *)(*(u8 **)(record + 80)))->mode = 1;
    Engine_ActorFaceEachOther(0, 2, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 30);
    Call3(Engine_ActorSetPosition, 24, 0x680000, 0x2b80000);
    Call3(Engine_ActorSetPosition, 25, 0x780000, 0x2b80000);
    Engine_ActorFaceEachOther(24, 25, 0);
    Engine_CameraFollowActor(0, 0);
    ((void (*)())Engine_CameraWaitForMove)();
    Engine_MapRedraw();
    Engine_EventWait(30);
    {
        u8 *work = *(u8 **)&gEventWork;

        *(s32 *)(work + 0x1c8) = 24;
        *(s32 *)(work + 0x1c0) = 0x201;
    }
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(1, 1);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgKuupuappuLearn);
    SceneActor_SetModeZeroAndValue(1, 20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 50);
    Engine_ActorFaceActor(0, 2, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
    Engine_EventShowMessage(2, 0);
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(2, 30);
    Engine_ActorRunRepeatedMotion(1, 1);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(0, 1, 20);
    Engine_EventOpenMessage(1, 0);
    v5 = 0;
    if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) != 0) {
        Engine_EventWait(20);
        Engine_ActorStartRepeatedMotion(2, 2);
        SceneEffect_ApplyThreeValuesAndFinish(1, 4, 30);
        Engine_EventOpenMessage(1, 0);
        v5 = 0;
        if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) != 0) {
            Engine_EventWait(20);
            Engine_ActorShowEmote(2, 0x102, 0);
            Engine_EventWait(60);
            Engine_ActorRunRepeatedMotion(2, 2);
            SceneActor_SetPairZeroAndValue(0, 2, 20);
            Engine_EventOpenMessage(2, 0);
            v5 = 0;
            if (Engine_UiWorkWaitThenFinalizeCapacity(2, 0) != 0) {
                Engine_EventWait(20);
                Engine_ActorShowEmote(2, 0x105, 0);
                Engine_EventWait(60);
                SceneActor_SetModeZeroAndValue(2, 20);
                FieldScene_RunSplitTripleSteps(1, 2, 10);
                Engine_ActorRunRepeatedMotion(1, 1);
                Engine_EventWait(10);
                SceneActor_SetModeZeroAndValue(1, 10);
                FieldScene_RunSplitTripleSteps(2, 1, 20);
                Engine_ActorShowEmote(2, 0x101, 0);
                Engine_EventWait(60);
                SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
                SceneActor_SetModeZeroAndValue(1, 10);
                SceneActor_SetPairZeroAndValue(1, 0, 20);
                SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
                v5 = 1;
                SceneActor_SetModeZeroAndValue(1, 20);
            }
        }
    }
    if (v5 == 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuLeave);
        SceneActor_SetModeZeroAndValue(1, 20);
        SceneActor_SetPairZeroAndValue(1, 0, 20);
        SceneActor_SetModeZeroAndValue(1, 20);
    }
    Engine_ActorShowEmote(0, 0x105, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(1, 1);
    SceneActor_SetModeZeroAndValue(1, 10);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 10);
    Engine_ActorFaceEachOther(1, 2, 0);
    SceneActor_SetPairZeroAndValue(0, 2, 10);
    Engine_EventShowMessage(1, 0);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    Call3(Engine_ActorWalkTo, 2, 248, 0x2d8);
    Call3(Engine_ActorWalkToAndWait, 1, 248, 0x2d8);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(2, 0, 0);
}
