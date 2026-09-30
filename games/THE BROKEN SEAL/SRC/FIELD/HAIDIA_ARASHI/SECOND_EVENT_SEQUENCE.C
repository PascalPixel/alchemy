#include "TYPES.H"
#include "CALL.H"

void FieldScene_DrawFiveTileBlocks();
void SceneActor_RunActor22PlacementSequence();
void Engine_TaskWait();
s32 Engine_GameFlagIsSet();
void Engine_WorkSetValuesIfNonNegative();
void Engine_EventBegin();
void Engine_MapWaitWorkValuesBelow256();
void Engine_EventWait();
void Engine_GameFlagSet();
void Engine_ActorSetPosition();
void Engine_ActorWalkToAndWait();
s32 Engine_ActorGet();
void Engine_EventEnd();
void Engine_AudioPlayCue();
void BattleFx_PlayQueuedSound();

void HaidiaArashi_RunSecondEventSequence(void)
{
    s32 rec7;
    s32 record;
    s32 v3;

    if (Value1(Engine_GameFlagIsSet, 0x313) != 0) {
    } else {
        Engine_EventBegin();
        if (Value1(Engine_GameFlagIsSet, 0x833) == 0) {
            rec7 = Engine_ActorGet(14);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
            Engine_AudioPlayCue(141);
            Engine_TaskWait(40);
            Engine_AudioPlayCue(145);
            Call3(Engine_ActorSetPosition, 14, 0x1da0000, 0x47b0000);
            *(s32 *)(rec7 + 48) = 0x10000;
            *(s32 *)(rec7 + 52) = 0x10000;
            v3 = (*(s32 *)(rec7 + 12) + 0x480000);
            *(s32 *)(rec7 + 12) += 0x480000;
            *(s32 *)(rec7 + 60) = v3;
            *(s32 *)(rec7 + 68) = 0x8000;
            Engine_ActorWalkToAndWait(14, 0x1b0, 0x47b);
            Engine_EventWait(40);
            Engine_AudioPlayCue(0x121);
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            BattleFx_PlayQueuedSound();
            Engine_GameFlagSet(0x833);
        }
        FieldScene_DrawFiveTileBlocks();
        Engine_GameFlagSet(0x313);
        if (Engine_GameFlagIsSet(0x837) != 0) {
            if (Engine_GameFlagIsSet(0x841) == 0) {
                if (Engine_GameFlagIsSet(0x30c) == 0) {
                    record = Engine_ActorGet(0);
                    if (*(s32 *)(record + 16) <= 0x479ffff) {
                        SceneActor_RunActor22PlacementSequence(0x19c, 0x460);
                        Call3(Engine_ActorWalkToAndWait, 0, 0x19e, 0x42c);
                    } else {
                        SceneActor_RunActor22PlacementSequence(0x1bd, 0x494);
                        Call3(Engine_ActorWalkToAndWait, 0, 0x1bf, 0x4cb);
                    }
                    Engine_GameFlagSet(0x30c);
                }
            }
        }
        Engine_EventEnd();
    }
}
