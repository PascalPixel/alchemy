#include "TYPES.H"

void SceneActor_SetModeZeroAndValue();
void FieldScene_RunSplitTripleSteps();
void SceneActor_SetPairZeroAndValue();
void SceneEffect_ApplyThreeValuesAndFinish();
void SceneEffect_ApplyPairWithValue141();
void SceneState_SetValue2ThenFinish();
void Engine_MessageShowCentered();
void Engine_EventWait();
void Main_0808a048();
void Engine_EventChooseYesNo();
u8 * Engine_ActorGet();
void Engine_ActorWalkTo();
void Engine_ActorWalkToAndWait();
void Engine_ActorWaitForMove();
void Engine_ActorSetAnimation();
void Engine_ActorJump();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorFaceActor();
void Engine_ActorFaceEachOther();
void Engine_EventSetMessage();
s32 Engine_EventOpenMessage();
void Engine_EventShowMessage();
void Engine_ActorShowEmote();
void Engine_ActorSetAttachedEffect();
void Main_0808a480();

/* FAKEMATCH: Call sites spelled through these wrappers pass their constants straight
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

void Scene_JoinRodSearch(void)
{
    u32 i;
    u8 *record;
    s32 v5;
    s32 v6;
    s32 mes;

    Engine_EventWait(20);
    *(u8 *)(Engine_ActorGet(2) + 91) = 0;
    Engine_ActorJump(2, 4, 0);
    Engine_EventWait(40);
    mes = 0x125f;
    Engine_EventSetMessage(0x125f);
    Engine_EventWait(20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Call3(Engine_ActorShowEmote, 2, 0x101, 0);
    Engine_EventWait(50);
    SceneActor_SetModeZeroAndValue(2, 30);
    Call3(Engine_ActorWalkToAndWait, 2, 0x178, 0x188);
    SceneEffect_ApplyPairWithValue141(2, 0);
    Engine_EventWait(40);
    SceneState_SetValue2ThenFinish();
    v5 = 254;
    Engine_ActorRunRepeatedMotion(0, 1);
    *(u8 *)(Engine_ActorGet(0) + 90) &= v5;
    Call3(Engine_ActorWalkToAndWait, 0, 0x180, 0x1a8);
    v6 = 1;
    Engine_EventWait(1);
    *(u8 *)(Engine_ActorGet(0) + 90) |= v6;
    Engine_EventWait(30);
    Engine_EventShowMessage(2, 0);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(0, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 30);
    Call3(Engine_ActorShowEmote, 2, 0x101, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(2, 1);
    Engine_EventWait(10);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    Engine_EventShowMessage(2, 0);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(10);
    FieldScene_RunSplitTripleSteps(1, 0, 30);
    Engine_ActorRunRepeatedMotion(0, 2);
    Engine_EventWait(10);
    FieldScene_RunSplitTripleSteps(0, 1, 40);
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Engine_EventWait(60);
    Call3(Engine_ActorShowEmote, 2, 0x102, 0);
    Engine_EventWait(60);
    Value2(Engine_EventOpenMessage, 2, 0);
    Engine_ActorFaceActor(0, 2, 0);
    Engine_ActorFaceActor(1, 2, 0);
    Engine_EventChooseYesNo(0, 0);
    Engine_EventWait(30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    Call3(Engine_ActorWalkToAndWait, 2, 0x180, 0x198);
    Engine_EventWait(10);
    Engine_MessageShowCentered((mes + 5), 1);
    Engine_EventSetMessage((mes + 6));
    Engine_ActorFaceEachOther(2, 1, 0);
    FieldScene_RunSplitTripleSteps(0, 1, 20);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(1, 1);
    *(u8 *)(Engine_ActorGet(1) + 90) &= v5;
    Call3(Engine_ActorWalkToAndWait, 1, 0x160, 0x198);
    Engine_EventWait(1);
    {
        u8 *record = Engine_ActorGet(1);
        /* FAKEMATCH: retain this byte read before the flag merge. */
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | v6);
    }
    Engine_EventWait(10);
    Call3(Engine_ActorWalkTo, 2, 0x170, 0x198);
    Engine_EventWait(20);
    Call3(Engine_ActorWalkToAndWait, 0, 0x170, 0x1a8);
    Engine_ActorFaceActor(0, 1, 0);
    Engine_ActorWaitForMove(2);
    SceneEffect_ApplyPairWithValue141(2, 1);
    SceneEffect_ApplyThreeValuesAndFinish(1, 4, 10);
    Engine_EventShowMessage(1, 0);
    SceneState_SetValue2ThenFinish();
    SceneActor_SetPairZeroAndValue(2, 0, 30);
    Engine_EventShowMessage(2, 0);
    Engine_ActorRunRepeatedMotion(0, 1);
    Engine_EventWait(30);
    SceneEffect_ApplyThreeValuesAndFinish(0, 3, 30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    Engine_EventShowMessage(2, 0);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_ActorFaceActor(1, 0, 0);
    Engine_EventShowMessage(1, 0);
    Engine_ActorFaceActor(0, 1, 0);
    SceneEffect_ApplyThreeValuesAndFinish(0, 3, 40);
    Engine_ActorFaceActor(0, 2, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 30);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    Engine_EventShowMessage(2, 0);
    Call3(Engine_ActorShowEmote, 1, 0x103, 0);
    Engine_EventWait(60);
    FieldScene_RunSplitTripleSteps(1, 0, 10);
    Engine_EventShowMessage(1, 0);
    Call3(Engine_ActorShowEmote, 2, 0x100, 0);
    Engine_EventWait(60);
    Engine_EventShowMessage(2, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Engine_EventWait(60);
    Engine_MessageShowCentered((mes + 13), 1);
    Engine_EventSetMessage((mes + 14));
    Engine_ActorFaceEachOther(2, 0, 0);
    Engine_ActorFaceActor(1, 0, 0);
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyPairWithValue141(2, 0);
    Main_0808a480();
    SceneActor_SetModeZeroAndValue(1, 30);
    SceneState_SetValue2ThenFinish();
    Engine_ActorFaceEachOther(2, 1, 0);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    SceneActor_SetModeZeroAndValue(2, 20);
    Engine_ActorFaceEachOther(2, 0, 0);
    SceneEffect_ApplyThreeValuesAndFinish(0, 3, 20);
    Engine_ActorRunRepeatedMotion(1, 1);
    Engine_EventWait(20);
    SceneEffect_ApplyThreeValuesAndFinish(1, 4, 10);
    SceneActor_SetModeZeroAndValue(1, 30);
    Engine_ActorFaceActor(2, 1, 0);
    Call3(Engine_ActorShowEmote, 2, 0x101, 0);
    Engine_EventWait(30);
    SceneActor_SetPairZeroAndValue(0, 1, 10);
    Engine_ActorSetAnimation(0, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 30);
    Engine_ActorFaceActor(0, 2, 0);
    Engine_ActorFaceActor(1, 2, 0);
    Engine_ActorFaceActor(2, 0, 0);
    Call3(Engine_ActorShowEmote, 2, 0x106, 0);
    Engine_EventWait(60);
    SceneActor_SetModeZeroAndValue(2, 10);
    Engine_ActorSetAnimation(0, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    SceneActor_SetModeZeroAndValue(2, 30);
    Call3(Engine_ActorShowEmote, 0, 0x105, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 10);
    SceneActor_SetModeZeroAndValue(2, 30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    Engine_ActorSetAnimation(0, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
    Main_0808a048(2, 1);
    Engine_EventSetMessage(mes + 22);
    Engine_ActorRunRepeatedMotion(2, 1);
    Engine_EventShowMessage(2, 0);
}
