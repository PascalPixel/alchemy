#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TEMPLE.H"

void FieldScene_SpawnRandomizedParticle(void)
{
    struct Params params;
    u8 *record;
    s32 draw;
    s32 offset;

    record = Actor_Get(ACTOR_PARTY_LEADER);

    params.field1 = 7;
    draw = (u32)(Random_Next() * 7) >> 16;
    if ((draw & 7) == 0)
        params.field1 = 5;

    params.field2 = 0xb333;
    params.field3 = 0xcccc;

    offset = ((u32)(Random_Next() * 8) >> 16) * 13107;

    Effect_Spawn(*(s32 *)(record + 8) + ((8 - (gFrameCount & 15)) << 16),
                  *(s32 *)(record + 12) + (192 << 13),
                  *(s32 *)(record + 16),
                  0,
                  -offset,
                  0,
                  144 << 12,
                  (u8 *)&params);

    if ((gFrameCount & 1) != 0)
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    else
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 1);
}

void FieldScene_ApplyOffset0Neg32(void)
{
    FieldScene_RunOpeningAuxiliarySequence(0, -32);
}

void FieldScene_ApplyOffset0Pos32(void)
{
    FieldScene_RunOpeningAuxiliarySequence(0, 32);
}

void FieldScene_ApplyOffsetNeg32_0(void)
{
    FieldScene_RunOpeningAuxiliarySequence(-32, 0);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
    Value3(Engine_ActorSetDestinationOffset, 0, a0, a1);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 7);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 6);
    Event_End();
}

void FieldScene_RunScene39eSequenceA(void)
{
    u32 i;
    s32 record;
    s32 base5_200a5b9;

    Event_Begin();
    base5_200a5b9 = (s32)FieldScene_SpawnRandomizedParticle;
    Call2(Engine_ScheduleCallbackFar, base5_200a5b9, 0xc80);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
    gEventWork->transition_frames = 60;
    Event_CloseScreen();
    Audio_PlayCue(154);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(record, 0);
    Scheduler_RemoveCallbackFar(base5_200a5b9);
    Event_WaitForScreen();
    Event_RequestExit(3);
    Event_End();
}

void FieldScene_PlaySound123AndEnable(void)
{
    Audio_PlayCue(123);
    Event_RequestExit(1);
}

void FieldScene_RunScene39e_02002778(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Audio_PlayCue(188);
    Map_AnimateCells((const u16 *)ShianJiin_CellStepsA, 77, 8);
    *(u8 *)(((s32)Engine_ActorGet(0)) + 85) = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    Event_Wait(16);
    Event_RequestExit(2);
    Event_End();
}
