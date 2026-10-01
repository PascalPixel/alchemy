#include "HAIDIA.H"

void FieldScene_RunScene3a6_020014ac(void)
{

    u32 i;
    s32 record;
    s32 zero;

    Engine_EventBegin();
    Battle_WaitMode0(10);
    Actor_SetMotionSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x1999);
    Object_SetModeById(ACTOR_PARTY_LEADER, 8);
    Battle_WaitMode0(15);
    ObjectMotion_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, 8, 0);
    Battle_WaitMode0(4);
    Audio_PlayCue(0x120);
    Audio_PlayCue(239);
    Actor_SetMotionSpeed(9, 0x8000, 0x1999);
    Object_SetModeById(9, 2);
    zero = 0;
    *(u8 *)((s32)Object_GetById(9) + 85) = zero;
    record = Actor_Get(9);
    *(s32 *)(record + 68) = zero;
    ObjectMotion_OffsetPositionAndResetMotion(9, 12, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    ObjectMotion_CommitCurrentPositionAndActivate(9);
    Audio_PlayCue(0x120);
    Audio_PlayCue(213);
    Object_SetModeById(9, 3);
    *(u8 *)((s32)Object_GetById(9) + 85) = 3;
    ObjectMotion_OffsetPositionAndResetMotion(9, 6, 0);
    SceneActor_WaitActorDescent((u8 *)Actor_Get(9));
    Object_SetModeById(9, 8);
    Engine_ActorSetSpritePriority(9, 3);
    *(u8 *)((s32)Object_GetById(9) + 35) = 2;
    StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
    StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
    GameFlag_Set(0x202);
    Audio_PlayCue(240);
    Engine_EventEnd();
}

