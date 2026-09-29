/* Stages the two moving actors around actor 25, sends actors 23, 24, 25 and
 * the leader off on their own actions, then advances the area's scene state
 * and records this house as the scene the party returns to. */
/* FAKEMATCH: HAIDIA.H's inline call and value wrappers keep the game's
 * argument order at the calls that use them. */
#include "HAIDIA.H"

/* The four actors' closing actions, where the overlay's data lies. */
extern u8 HaidiaIe_Actor23Actions[];
extern u8 HaidiaIe_Actor24Actions[];
extern u8 HaidiaIe_Actor25Actions[];
extern u8 HaidiaIe_LeaderActions[];

void FieldScene_RunGroupChoreography(void)
{
    extern u8 Data_0200ac00[];

    s32 record;
    s32 walk_speed;
    s32 turn_speed;
    s32 approach_speed;

    Engine_ActorSetChildValue(25, 15);
    record = Engine_ActorGet(25);
    Engine_ActorSetSpriteFlags(record, 0);
    Call3(Engine_ActorSetPosition, 25, 0, 0x14b0000);
    Engine_TaskWait(1);
    Call2(Engine_EventShowMessage, 0x1019, 0);
    Call3(Engine_ActorShowEmote, 23, 0x100, 0);
    Call3(Engine_ActorShowEmote, 24, 0x100, 40);
    Engine_ActorSetPosition(25, 0, 0);
    Call3(Engine_ActorFaceDirection, 23, 0x5000, 0);
    walk_speed = 0x5000;
    SceneActor_SetPairZeroAndValue(24, walk_speed, 40);
    Call2(Engine_CameraSetSpeed, 0x18000, 0x3000);
    Call4(Engine_CameraMoveTo, 0x590000, 0xb00000, 0x1390000, 1);
    BattleFx_CommitObjectPositionAndWait();
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 23, 0xe000, 0);
    Value3(SceneActor_SetPairZeroAndValue, 24, 0x7000, 40);
    Camera_SetSpeed(0xcccc, 0x1999);
    Call4(Engine_CameraMoveTo, 0x640000, 0x900000, 0x14d0000, 1);
    Call3(Engine_ActorSetSpeed, 23, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 24, 0x10000, 0x8000);
    Call3(Engine_ActorWalkTo, 23, 105, 0x149);
    Engine_EventWait(10);
    Actor_WalkTo(24, 124, 0x149);
    Engine_ActorWaitForMove(23);
    Actor_SetAnimation(23, 1);
    Engine_ActorFaceDirection(23, walk_speed, 0);
    Actor_WaitForMove(24);
    Engine_ActorSetAnimation(24, 1);
    Value3(Engine_ActorFaceDirection, 24, walk_speed, 0);
    Engine_ActorSetChildValue(25, 0);
    record = Engine_ActorGet(25);
    Engine_ActorSetSpriteFlags(record, 1);
    Call3(Engine_ActorSetPosition, 25, 0, 0x14b0000);
    Call3(Engine_ActorSetSpeed, 25, 0x13333, 0x9999);
    Actor_WalkToAndWait(25, 37, 0x153);
    Event_Wait(20);
    Engine_ActorSetAnimationAndWait(23, 3);
    Value2(Engine_EventOpenMessage, 23, 0);
    Call3(Engine_ActorShowEmote, 25, 0x101, 0);
    approach_speed = 0xd000;
    Value3(SceneActor_SetPairZeroAndValue, 0, approach_speed, 10);
    Engine_EventChooseYesNo(0, 0);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(23, 0, 0);
    turn_speed = 0x8000;
    Value3(SceneActor_SetPairZeroAndValue, 24, turn_speed, 20);
    Engine_ActorRunRepeatedMotion(25, 2);
    Call2(Event_SayThenWait, 0x1019, 10);
    Call3(Object_SetTargetAndCallback, 0, 0x10019, (s32)Data_0200ac00);
    Call3(Engine_ActorWalkToAndWait, 25, 93, 0x169);
    Value3(SceneActor_SetPairZeroAndValue, 25, approach_speed, 40);
    Event_SayThenWait(25, 20);
    Engine_ActorStop(0);
    Engine_ActorFaceDirection(23, 0, 0);
    Value3(SceneActor_SetPairZeroAndValue, 24, turn_speed, 15);
    Engine_ActorSetAnimation(23, 3);
    Engine_ActorSetAnimationAndWait(24, 3);
    Engine_ActorFaceDirection(23, walk_speed, 0);
    Value3(SceneActor_SetPairZeroAndValue, 24, walk_speed, 30);
    Engine_ActorSetAnimationAndWait(24, 4);
    Call2(Engine_EventShowMessage, 0x2018, 0);
    SceneActor_SetPairZeroAndValue(0, approach_speed, 30);
    Value3(SceneActor_SetPairZeroAndValue, 0, 0x4000, 40);
    Engine_ActorRunRepeatedMotion(23, 2);
    Engine_ActorSetAnimationAndWait(23, 3);
    Event_SayThenWait(23, 20);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 25, 0x102);
    Engine_EventWait(80);
    Call2(Engine_ActorEnableActionCallback, 24, (s32)HaidiaIe_Actor24Actions);
    Engine_EventWait(6);
    Call2(Engine_ActorEnableActionCallback, 23, (s32)HaidiaIe_Actor23Actions);
    Engine_EventWait(20);
    Call2(Engine_ActorEnableActionCallback, 0, (s32)HaidiaIe_LeaderActions);
    Engine_EventWait(6);
    Value2(Object_SetActionCallbackAndRefreshById, 25, (s32)HaidiaIe_Actor25Actions);
    /* FAKEMATCH: the do/while loads the linked scene after the store. */
    do {
        gGameState.unknown_1f8[0x22b - 0x1f8] = 2;
    } while (0);
    {
        const void *message = &SceneId_HaidiaIe;

        Party_SetFields1ceAnd1d0(message, 19);
        Event_SetPair1d4(message, 19);
    }
    BattleFx_SetWeightedResult(12, 4);
    Call1(Engine_GameFlagSet, 0x11a);
}
