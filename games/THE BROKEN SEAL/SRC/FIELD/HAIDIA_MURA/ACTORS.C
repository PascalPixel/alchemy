#include "STAGED_MOTION.H"
extern u8 MsgHaidiaIUsedToPlayHere[];

/* Sets up actors 12, 13, 14 and 20 with shared data and movement/speed
 * parameters, then drives actor 11 through a further sequence of moves. */
void FieldScene_RunCompanionActorSequence(void)
{
    u32 i;
    s32 actor_data;
    s32 shared_data;

    Event_Begin();
    actor_data = Actor_Get(ACTOR_A);
    Actor_SetSpriteFlags(actor_data, 0);
    actor_data = Actor_Get(ACTOR_B);
    Actor_SetSpriteFlags(actor_data, 0);
    actor_data = Actor_Get(ACTOR_C);
    Actor_SetSpriteFlags(actor_data, 0);
    Actor_SetAnimation(ACTOR_A, 0);
    Actor_SetAnimation(ACTOR_B, 0);
    Actor_SetAnimation(ACTOR_C, 0);
    Task_Wait(20);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    shared_data = ((s32)gVillagerAction);
    Actor_EnableActionCallback(ACTOR_A, shared_data);
    Task_Wait(10);
    Engine_ActorEnableActionCallback(ACTOR_B, shared_data);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Task_Wait(20);
    Object_SetActionCallbackAndRefreshById(ACTOR_C, shared_data);
    Actor_ShowEmote(ACTOR_D, 0x100, 40);
    Actor_RunRepeatedMotion(ACTOR_D, 2);
    Actor_FaceDirection(ACTOR_D, 0xd000, 10);
    Event_SetMessage((s32)MsgHaidiaIUsedToPlayHere);
    Event_ShowMessageAndWait(ACTOR_D, 0, 40);
    Actor_FaceActor(ACTOR_D, ACTOR_PARTY_LEADER, 20);
    Event_ShowMessage(ACTOR_D, 0);
    Actor_FaceDirection(ACTOR_D, 0x8000, 10);
    GameFlag_Set(0x305);
    Event_End();
}
