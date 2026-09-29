#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void Resource3ba_NoOpCallback(void);
s32 FieldScene_RunFlag211ApproachScene();
void SceneState_WaitUntilWord1000IsNine(void);
void Object_RefreshSelectorById(s32 actor);
s32 BattleFx_SetWeightedResult();
s32 Party_SetFields1ceAnd1d0();
void Event_SetPair1d4(s32 scene, s32 entrance);

/* FAKEMATCH: call sites spelled through these wrappers pass their constants
 * straight into the argument registers; a direct call precomputes a costly
 * constant into a pseudo that the compiler then shares with later uses in
 * the block. A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The river stage's start: after the approach scene and once the stage is
   ready, actor 8 and the leader walk to the start line and face each
   other. The approach's outcome goes to the weighted result, the river's
   scene is stored with entrance 4 and 5 in the two scene pairs, the stage
   byte at 0x22b becomes 3 and flag 0x11a is set. */
void KorosseoKawa_RunStageStart(void)
{
    s32 approach;
    s32 n;
    s32 base;
    u8 *state;

    base = 0;
    Resource3ba_NoOpCallback();
    Engine_EventBegin();
    approach = Value2(FieldScene_RunFlag211ApproachScene, 120, 127);
    SceneState_WaitUntilWord1000IsNine();
    n = 9;
    do {
        Object_RefreshSelectorById(8);
        n = n - 1;
    } while (n >= 0);
    Call3(Engine_ActorSetSpeed, 8, 0x10000, 0x8000);
    Call3(Engine_ActorWalkTo, 8, 0x528, 192);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 0, 0x508, 192);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorFaceEachOther(0, 8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 0, 0x20000, 0x10000);
    Call3(Engine_ActorSetSpeed, 8, 0x20000, 0x10000);
    Call3(Engine_ActorWalkTo, 0, 0x510, 192);
    Call3(Engine_ActorWalkToAndWait, 8, 0x520, 192);
    Engine_ActorSetAnimation(0, 16);
    Engine_ActorSetAnimation(8, 9);
    Engine_EventWait(10);
    /* FAKEMATCH: base is 0 from the top of the function, so 0 - approach
     * + 1 is not folded into 1 - approach. */
    Value2(BattleFx_SetWeightedResult, 72, base - approach + 1);
    state = (u8 *)&gGameState;
    /* FAKEMATCH: the do-while keeps the stage flag store ahead of the
     * scene's pool load. */
    do {
        state[0x22b] = 3;
    } while (0);
    Value2(Party_SetFields1ceAnd1d0, (s32)&SceneId_KorosseoKawa, 4);
    Event_SetPair1d4((s32)&SceneId_KorosseoKawa, 5);
    Call1(Engine_GameFlagSet, 0x11a);
}
