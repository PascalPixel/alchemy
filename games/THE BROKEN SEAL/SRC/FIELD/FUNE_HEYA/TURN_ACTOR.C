#include "TYPES.H"
#include "HEYA.H"
#include "CALL.H"

s32 SceneActor_CheckBucketOffsetPoint();
void FieldScene_CallPairWith10();
s32 Engine_ActorGet();
void Engine_ActorSetSpeed();
void Engine_ActorSetPosition();
void Engine_ActorSetDestinationOffset();
void Engine_ActorSetAnimation();
void ObjectMotion_SetActionVariant();
void Engine_ActorWaitForMove();

extern s32 FuneHeya_TurnSteps[];

void FuneHeya_TurnActorToOpenSide(s32 a0)
{
    s32 p10;
    s32 rec4;
    u8 *record;
    s32 none;
    s32 v8;
    s32 v6;

    rec4 = Engine_ActorGet(0);
    v8 = 1;
    ObjectMotion_SetActionVariant(a0, 2);
    {
        u8 *record = (u8 *)Engine_ActorGet(a0);
        /* FAKEMATCH: the flag byte is read through a volatile access. */
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 1);
    }
    v6 = (((*(u16 *)(rec4 + 6) + 0x4000) & 0xf000) >> 12);
    if (SceneActor_CheckBucketOffsetPoint((((*(u16 *)(rec4 + 6) + 0x4000) & 0xf000) >> 12)) != 0) {
        none = 0;
        v8 = none;
    }
    if (v8 != 0) {
        v6 = (((*(u16 *)(rec4 + 6) + -0x4000) & 0xf000) >> 12);
        if (SceneActor_CheckBucketOffsetPoint((((*(u16 *)(rec4 + 6) + -0x4000) & 0xf000) >> 12)) != 0) {
            none = 0;
            v8 = none;
        }
        if (v8 != 0) {
            v6 = (((*(u16 *)(rec4 + 6) + 0x8000) & 0xf000) >> 12);
        }
    }
    record = Engine_ActorGet(0);
    if ((s32)record != 0) {
        Engine_ActorSetPosition(a0, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
    }
    Call3(Engine_ActorSetSpeed, a0, 0x19999, 0xcccc);
    Engine_ActorSetAnimation(a0, 2);
    {
        s32 w = FuneHeya_TurnSteps[v6];

        Engine_ActorSetDestinationOffset(a0, w >> 16, (w << 16) >> 16);
    }
    Engine_ActorWaitForMove(a0);
    Engine_ActorSetAnimation(a0, 1);
    FieldScene_CallPairWith10(a0, *(u16 *)(rec4 + 6));
    p10 = a0;
}
