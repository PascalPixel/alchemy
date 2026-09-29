/* The periodic particles and the multi-phase actor sequence. */
#include "LOG_ROLLING.H"
extern u8 MsgKorosseoRobin[];
extern u8 MsgKorosseoRobinFellAsleep[];

s32 ColossoLogRollingStage_AdvanceParticleMotion(SceneParticle *particle)
{
    particle->x += particle->velocity_x << 8;
    particle->y += particle->velocity_y << 8;
    particle->scale_x += 0x666;
    particle->scale_y += 0x666;
    particle->velocity_x += 5;
    particle->velocity_y -= 1;
    return 0;
}

void ColossoLogRollingStage_SpawnPeriodicParticle(void)
{
    extern s32 Engine_MathModulo(s32, s32);

    SceneParticle *particle;
    SceneParticle *source;
    s32 x;
    s32 y;
    s32 kind;
    s32 count;

    particle = Engine_ActorGet(0);
    count = gColossoParticleCount + 1;
    kind = 41;
    x = particle->x;
    y = particle->y;
    gColossoParticleCount = count;
    switch (Engine_MathModulo(count, 180)) {
    case 10:
        break;
    case 20:
        kind = 42;
        break;
    case 30:
        kind = 43;
        break;
    default:
        return;
    }
    particle = Engine_ActorGet(kind);
    if (particle == 0) {
        return;
    }
    source = Engine_ActorGet(0);
    if (source != 0) {
        Engine_ActorSetPosition(kind, source->x, source->z);
    }
    Engine_ActorSetSpriteFlags(Engine_ActorGet(kind), 0);
    particle->state = 0;
    particle->scale_x = 0x6666;
    particle->scale_y = 0x6666;
    {
        s32 t = 0x40000;
        particle->x = x + t;
        t += y;
        particle->y = t;
        particle->anchor_y = t;
    }
    particle->velocity_x = 25;
    particle->velocity_y = 128;
    Engine_ActorEnableActionCallback(kind, (s32)gColossoParticleKinds);
}

void FieldScene_RunMultiPhaseActorSequence(s32 a0)
{
    extern void Object_LinkObjectAndSetCallback();

    extern void Owner_RefreshActiveRatios();
    extern void Graphics_EnableObjLayerAndCallbacks();

    s32 record;
    s32 data_table_addr;
    s32 callback_target;

    Actor_Destroy(39);
    Actor_Destroy(40);
    Owner_RefreshActiveRatios(1);
    Engine_AudioPlayCue(17);
    Event_Begin();
    Actor_SetPosition(8, 0x6080000, 0xc00000);
    if (a0 < 0) {
        Engine_ActorSetAnimation(8, 10);
    } else {
        Actor_SetAnimation(8, 8);
    }
    Value2(Engine_ActorEnableActionCallback, 8, (s32)KorosseoMaruta_Actor8Action);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x5e00000, 0xc00000);
    record = Engine_ActorGet(0);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Value2(Engine_ActorEnableActionCallback, 0, (s32)KorosseoMaruta_LeaderActionA);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 35);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_SetPosition(ACTOR_GERALD, 0x5b80000, 0xb80000);
    Actor_SetPosition(ACTOR_IVAN, 0x5b80000, 0xc80000);
    Actor_SetPosition(ACTOR_MIA, 0x5a80000, 0xc00000);
    record = Value1(Engine_ActorGet, 1);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Value1(Engine_ActorGet, 2);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Value1(Engine_ActorGet, 3);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Task_Wait(1);
    Engine_CameraFollowActor(0, 0);
    gEventWork->start_transition = 0x100;
    ColorBuffer_ApplyTarget(0x10001, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_SetMessage((s32)MsgKorosseoRobin);
    Event_Wait(60);
    data_table_addr = (s32)gColossoMultiPhaseData;
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, data_table_addr);
    record = Engine_ActorGet(0);
    *(s32 *)(record + 24) = 0x10000;
    record = Engine_ActorGet(0);
    *(s32 *)(record + 28) = 0x10000;
    Engine_ActorSetAnimationAndWait(0, 36);
    record = Engine_ActorGet(0);
    *(s32 *)(record + 8) += 0x30000;
    Event_Wait(10);
    record = Engine_ActorGet(0);
    Actor_SetSpriteFlags(record, 0);
    Event_Wait(20);
    Value2(Engine_ActorEnableActionCallback, 0, (s32)KorosseoMaruta_LeaderActionB);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(20);
    Call3(Engine_ActorWalkToAndWait, 1, 0x5e0, 176);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 10);
    Value3(Engine_ActorShowEmote, 1, 0x100, 20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Object_LinkObjectAndSetCallback(1, 2);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x5d0, 176);
    Actor_WalkTo(ACTOR_GERALD, 0x5f0, 184);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x5e0, 176);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(10);
    Object_LinkObjectAndSetCallback(2, 1);
    Event_Wait(30);
    Engine_ActorSetAnimationAndWait(1, 4);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_MIA, 0);
    Object_LinkObjectAndSetCallback(1, 3);
    Object_LinkObjectAndSetCallback(2, 3);
    Actor_WalkToAndWait(ACTOR_MIA, 0x5d0, 184);
    Object_LinkObjectAndSetCallback(2, 0);
    Object_LinkObjectAndSetCallback(1, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Object_LinkObjectAndSetCallback(2, 1);
    Object_LinkObjectAndSetCallback(1, 2);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    ((void (*)())Engine_EventWait)(10);
    Object_LinkObjectAndSetCallback(2, 3);
    Object_LinkObjectAndSetCallback(1, 3);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Value2(Engine_ActorEnableActionCallback, 0, (s32)KorosseoMaruta_LeaderActionC);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Object_LinkObjectAndSetCallback(1, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Object_LinkObjectAndSetCallback(2, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Object_LinkObjectAndSetCallback(3, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, data_table_addr);
    Event_Wait(60);
    callback_target = (s32)ColossoLogRollingStage_SpawnPeriodicParticle;
    gColossoParticleCount = 9;
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
    Event_Wait(5);
    Engine_TaskRemoveCallback(callback_target);
    Event_Wait(55);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
    Event_Wait(20);
    Engine_TaskRemoveCallback(callback_target);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    gColossoParticleCount = 9;
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
    Event_Wait(35);
    Engine_TaskRemoveCallback(callback_target);
    Event_Wait(25);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 60);
    gColossoParticleCount = 9;
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
    Event_Wait(35);
    Engine_TaskRemoveCallback(callback_target);
    Event_Wait(25);
    Call3(Engine_ActorShowEmote, 2, 0x102, 60);
    Object_LinkObjectAndSetCallback(3, 2);
    Object_LinkObjectAndSetCallback(2, 3);
    Event_Wait(60);
    Object_LinkObjectAndSetCallback(3, 0);
    Object_LinkObjectAndSetCallback(2, 0);
    gColossoParticleCount = 9;
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
    Event_Wait(35);
    Engine_TaskRemoveCallback(callback_target);
    Event_Wait(25);
    Actor_ShowEmote(ACTOR_MIA, 0x108, 60);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 3);
    Engine_ActorStartRepeatedMotion(2, 3);
    Engine_ActorRunRepeatedMotion(3, 3);
    Object_LinkObjectAndSetCallback(3, 2);
    Object_LinkObjectAndSetCallback(1, 2);
    gColossoParticleCount = 9;
    Call2(Engine_TaskAddCallback, callback_target, 0xc80);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Event_Wait(60);
    Actor_WalkTo(ACTOR_MIA, 0x5b8, 200);
    Engine_EventWait(5);
    Actor_WalkTo(ACTOR_IVAN, 0x558, 184);
    Engine_EventWait(3);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x5e8, 184);
    Actor_WalkTo(ACTOR_GERALD, 0x558, 184);
    Actor_WaitForMove(ACTOR_MIA);
    Engine_ActorSetAnimation(3, 1);
    Object_LinkObjectAndSetCallback(3, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(ACTOR_MIA, 0x598, 200);
    Actor_WalkTo(ACTOR_MIA, 0x558, 184);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_Wait(30);
    Actor_SetPosition(ACTOR_GERALD, 0x5e80000, 0xb00000);
    Actor_SetPosition(ACTOR_IVAN, 0x5b80000, 0xc00000);
    Actor_SetPosition(ACTOR_MIA, 0x6180000, 0xc80000);
    Graphics_EnableObjLayerAndCallbacks();
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(1);
    Event_SetMessage((s32)MsgKorosseoRobinFellAsleep);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(60);
    Event_End();
}
