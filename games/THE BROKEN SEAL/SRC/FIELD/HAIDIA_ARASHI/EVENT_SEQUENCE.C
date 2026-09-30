#include "TYPES.H"

void ActorPresentation_SetEightSceneCells();
s32 SceneActor_RunActor22PlacementSequence();
void Engine_TaskWait();
s32 Engine_GameFlagIsSet();
void Engine_WorkSetValuesIfNonNegative();
void Engine_EventBegin();
s32 Engine_ActorGet();
void Engine_MapWaitWorkValuesBelow256();
void Engine_EventWait();
void Engine_GameFlagSet();
void Engine_ActorSetPosition();
void Engine_ActorWalkToAndWait();
void Engine_EventEnd();
void ObjectMotion_SetActionVariant();
void Engine_AudioPlayCue();
void BattleFx_PlayQueuedSound();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void HaidiaArashi_RunEventSequence(void)
{
    s32 rec7;
    s32 record;
    s32 v3;

    if (Value1(Engine_GameFlagIsSet, 0x311) != 0) {
    } else {
        Engine_EventBegin();
        if (Value1(Engine_GameFlagIsSet, 0x831) == 0) {
            rec7 = Engine_ActorGet(12);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
            Engine_AudioPlayCue(141);
            Engine_TaskWait(40);
            Engine_AudioPlayCue(145);
            Call3(Engine_ActorSetPosition, 12, 0x17d0000, 0x3280000);
            *(s32 *)(rec7 + 48) = 0x18000;
            *(s32 *)(rec7 + 52) = 0x18000;
            v3 = (*(s32 *)(rec7 + 12) + 0x1000000);
            *(s32 *)(rec7 + 12) += 0x1000000;
            *(s32 *)(rec7 + 60) = v3;
            *(s32 *)(rec7 + 68) = 0x8000;
            Call3(Engine_ActorWalkToAndWait, 12, 0x122, 0x341);
            ObjectMotion_SetActionVariant(12, 1);
            Call3(Engine_ActorWalkToAndWait, 12, 0x102, 0x354);
            ObjectMotion_SetActionVariant(12, 2);
            Call3(Engine_ActorWalkToAndWait, 12, 224, 0x368);
            Engine_EventWait(40);
            Call1(Engine_AudioPlayCue, 0x121);
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            BattleFx_PlayQueuedSound();
            Call1(Engine_GameFlagSet, 0x831);
        }
        ActorPresentation_SetEightSceneCells();
        Call1(Engine_GameFlagSet, 0x311);
        if (Value1(Engine_GameFlagIsSet, 0x837) != 0) {
            if (Value1(Engine_GameFlagIsSet, 0x841) == 0) {
                if (Value1(Engine_GameFlagIsSet, 0x30c) == 0) {
                    record = Engine_ActorGet(0);
                    if (*(s32 *)(record + 12) > 0x800000) {
                        Value2(SceneActor_RunActor22PlacementSequence, 219, 0x34b);
                        Call3(Engine_ActorWalkToAndWait, 0, 179, 0x33d);
                    } else {
                        Value2(SceneActor_RunActor22PlacementSequence, 214, 0x38c);
                        Call3(Engine_ActorWalkToAndWait, 0, 219, 0x38f);
                    }
                    Call1(Engine_GameFlagSet, 0x30c);
                }
            }
        }
        Engine_EventEnd();
    }
}
