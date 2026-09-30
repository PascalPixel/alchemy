#include "TASK.H"
#include "CALL.H"

void Object_RefreshSelectorById(s32 actor);
void BattleFx_SetWeightedResult(s32 value, s32 mode);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void Event_SetPair1d4(s32 scene, s32 entrance);

/* The wall stage's start: the party walks up to the guide, the two face each
 * other and bow, then walk on to the wall; the result the approach returned
 * weights the round, and a retreat returns to the wall's entrance four. */
void KorosseoKabe_RunStageStart(void)
{
    s32 rec8;
    s32 v5;
    s32 base;
    s32 base3_2000240;

    base = 2;
    SceneState_EmptyHook();
    Engine_EventBegin();
    rec8 = ((s32 (*)())FieldScene_RunFlag211ApproachScene)(77, 89);
    SceneState_WaitUntilStatusNine();
    v5 = 9;
    do {
        Object_RefreshSelectorById(8);
        v5 = (v5 - 1);
    } while (v5 >= 0);
    Call3((void (*)())Engine_ActorSetSpeed, 8, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorWalkTo, 8, 88, 0x100);
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorWalkToAndWait, 0, 120, 0x100);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorFaceEachOther(0, 8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x20000, 0x10000);
    Call3((void (*)())Engine_ActorSetSpeed, 8, 0x20000, 0x10000);
    Call3((void (*)())Engine_ActorWalkTo, 0, 112, 0x100);
    ((void (*)())Engine_ActorWalkToAndWait)(8, 96, 0x100);
    Engine_ActorSetAnimation(0, 16);
    Engine_ActorSetAnimation(8, 9);
    Engine_EventWait(10);
    /* FAKEMATCH: base is set at the top of the function, so 2 - rec8 + 1
     * is not folded into 3 - rec8. */
    ((s32 (*)())BattleFx_SetWeightedResult)(72, base - rec8 + 1);
    base3_2000240 = (s32)&gGameState;
    /* FAKEMATCH: the do/while keeps the stage flag store ahead of the
     * pool load that follows it. */
    do {
        *(u8 *)((base3_2000240 + 0x22b)) = 3;
    } while (0);
    ((s32 (*)())Party_SetFields1ceAnd1d0)((s32)&SceneId_KorosseoKabe, 4);
    Event_SetPair1d4((s32)&SceneId_KorosseoKabe, 5);
    ((void (*)())Engine_GameFlagSet)(0x11a);
}
