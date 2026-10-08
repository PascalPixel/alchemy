#include "haidia.h"

void HaidiaDou_ToppleActorNine(void)
{

    u32 i;
    struct FieldActor *record;
    s32 zero;

    Engine_EventBegin();
    Engine_EventWait(10);
    Actor_SetMotionSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x1999);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
    Engine_EventWait(15);
    Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 8, 0);
    Engine_EventWait(4);
    Event_PlayCue(0x120);
    Event_PlayCue(239);
    Actor_SetMotionSpeed(9, 0x8000, 0x1999);
    Engine_ActorSetAnimation(9, 2);
    zero = 0;
    Object_GetById(9)->motion_flags = zero;
    record = Actor_Get(9);
    record->bounce = zero;
    Engine_ActorSetDestinationOffset(9, 12, 0);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorWaitForMove(9);
    Event_PlayCue(0x120);
    Event_PlayCue(213);
    Engine_ActorSetAnimation(9, 3);
    Object_GetById(9)->motion_flags = 3;
    Engine_ActorSetDestinationOffset(9, 6, 0);
    SceneActor_WaitActorDescent(Actor_Get(9));
    Engine_ActorSetAnimation(9, 8);
    Engine_ActorSetSpritePriority(9, 3);
    Object_GetById(9)->priority_flags = 2;
    StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
    StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
    GameFlag_Set(0x202);
    Event_PlayCue(240);
    Engine_EventEnd();
}

