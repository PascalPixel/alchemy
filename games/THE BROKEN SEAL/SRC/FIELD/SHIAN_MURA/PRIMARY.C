#include "SHIAN.H"

u8 *SceneEffect_GetPrimaryData(void)
{
    return gShianMuraEntrances;
}

s32 SceneEffect_GetInitialValue(void)
{
    return 0;
}

u8 *SceneEffect_GetSecondaryData(void)
{
    return gShianMuraExits;
}

s32 SceneEffect_PrepareState(void)
{
    if (GameFlag_IsSet(0x895) != 0)
        gShianMuraPlacements[0xbe] = 0;
    return (s32)gShianMuraPlacements;
}

void SceneEffect_ShowActorSetupMessage(void)
{
    Event_Begin();
    Event_SetMessage(MSG_YOUNG_WARRIORS_VERY_GALLANT_CAME);
    Event_AskYesNo(9, 0);
    Event_End();
}

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 rec7;
    struct FieldActor *actor;
    u8 *record;
    s32 v7;
    s32 v5;
    s32 p5;
    s32 q1;
    s32 t2;
    s32 q2;
    s32 hi;
    s32 lo;

    actor = (struct FieldActor *)Value1(Engine_ActorGet, 20);
    Event_Begin();
    v7 = 0;
    record = Actor_Get(18);
    *(s32 *)((s32)record + 108) = v7;
    if (GameFlag_IsSet(0x200) == 0) {
        record = Value1(Engine_ActorGet, 18);
        if ((*(s32 *)((s32)record + 8) >> 20) > 19) {
            goto L_020006a2;
        }
    }
    record = Actor_Get(18);
    p5 = *(u16 *)((s32)record + 6);
    Actor_FaceActor(18, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessage(MSG_NOW_MUST_GET_WATER_AGAIN);
    if (GameFlag_IsSet(0x200) == 0) {
        bump_step(1);
        Event_ShowMessage(18, 0);
        *(u16 *)((u8 *)Engine_ActorGet(18) + 100) = v7;
        record = Value1(Engine_ActorGet, 18);
        *(u16 *)((s32)record + 6) = p5;
    } else {
        Event_ShowMessage(18, 0);
        Actor_FaceDirection(18, 0x8000, 20);
    }
    record = Actor_Get(18);
    *(s32 *)((s32)record + 108) = (s32)ShianMura_WatchGateTrigger;
    Call0(Engine_EventEnd);
    goto L_02000916;
    L_020006a2:;
    record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if ((*(s32 *)((s32)record + 16) >> 19) > 27) {
        record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
        if ((*(s32 *)((s32)record + 16) >> 19) <= 29) {
            record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            if ((*(s32 *)((s32)record + 8) >> 20) != 26) {
                Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
                Call3((void (*)())Engine_ActorFaceActor, 0, 18, 0);
                Event_Wait(5);
                rec7 = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
                record = Actor_Get(18);
                if (*(s32 *)(rec7 + 8) < *(s32 *)((s32)record + 8)) {
                    *(u8 *)((u8 *)Engine_ActorGet(0) + 90) &= 254;
                    record = Value1(Engine_ActorGet, 18);
                    Actor_WalkTo(ACTOR_PARTY_LEADER, (((*(s32 *)((s32)record + 8) >> 20) << 4) - 8), 232);
                    v7 = 1;
                } else {
                    *(u8 *)((u8 *)Engine_ActorGet(0) + 90) &= 254;
                    record = Value1(Engine_ActorGet, 18);
                    Actor_WalkTo(ACTOR_PARTY_LEADER, (((*(s32 *)((s32)record + 8) >> 20) << 4) + 24), 232);
                }
                Actor_WaitForMove(ACTOR_PARTY_LEADER);
            }
        }
    }
    v5 = 128;
    record = Actor_Get(18);
    *(s32 *)((s32)record + 56) = (v5 << 24);
    record = Value1(Engine_ActorGet, 18);
    *(s32 *)((s32)record + 60) = (v5 << 24);
    record = Actor_Get(18);
    *(s32 *)((s32)record + 64) = (v5 << 24);
    Actor_EnableActionCallback(18, 1);
    Actor_SetAnimation(18, 1);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(10);
    Audio_PlayCue(228);
    actor->scale_x = 0x4ccc;
    actor->scale_y = 0x4ccc;
    record = Value1(Engine_ActorGet, 18);
    q1 = *(s32 *)((s32)record + 8);
    record = Value1(Engine_ActorGet, 18);
    t2 = *(s32 *)((s32)record + 16) >> 20;
    Actor_SetPosition(20, (((q1 >> 20) << 20) + 0x80000), ((t2 << 20) + 0x80000));
    record = Value1(Engine_ActorGet, 18);
    q2 = *(s32 *)((s32)record + 8);
    record = Value1(Engine_ActorGet, 18);
    Map_CopyCellAttributes(16, 16, 1, 1, (q2 >> 20), (*(s32 *)((s32)record + 16) >> 20));
    Actor_SetSpritePriority(20, 2);
    actor->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    do {
        Task_Wait(3);
        hi = actor->scale_y;
        lo = actor->scale_x;
        actor->scale_y = hi + 0x1999;
        lo += 0x1999;
        actor->scale_x = lo;
    } while (lo <= 0xffff);
    Actor_ShowEmote(18, 0x105, 70);
    Actor_FaceActor(18, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Actor_ShowEmote(18, 0x103, 0);
    Actor_StartRepeatedMotion(18, 2);
    Event_Wait(70);
    Event_SetMessage(MSG_DOING_MADE_ME_SPILL_MY);
    Event_ShowMessageAndWait(18, 0, 20);
    BattleFx_PlayQueuedSound();
    record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if ((*(s32 *)((s32)record + 8) >> 20) == 26) {
        record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
        if ((*(s32 *)((s32)record + 16) >> 20) > 13) {
            v7 = 1;
        }
    }
    if (v7 != 0) {
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 10);
        *(u8 *)((u8 *)Engine_ActorGet(0) + 90) &= 254;
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
        Actor_WaitForMove(ACTOR_PARTY_LEADER);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    }
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    record = Value1(Engine_ActorGet, 18);
    if ((*(s32 *)((s32)record + 16) >> 20) != 14) {
        record = Actor_Get(18);
        Actor_WalkToAndWait(18, *(s16 *)((s32)record + 10), 232);
    }
    Actor_WalkToAndWait(18, 0x118, 232);
    GameFlag_Set(0x200);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a |= 1;
    Event_End();
    L_02000916:;
}

void SceneEffect_ActivateNearbyActor(void)
{
    u8 *leader = Actor_Get(ACTOR_PARTY_LEADER);
    if ((*(s32 *)(leader + 16) >> 20) <= 13)
        Actor_SetSpritePriority(20, 1);
}

void FieldScene_RunScene3a0_02000968(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 x;

    Event_Begin();
    *(u8 *)((u8 *)Engine_ActorGet(20) + 35) &= 253;
    v5 = 0;
    *(u8 *)((u8 *)Engine_ActorGet(20) + 85) = v5;
    record = Value1(Engine_ActorGet, 20);
    x = *(s32 *)(record + 8);
    record = Value1(Engine_ActorGet, 20);
    Map_CopyCellAttributes(3, 17, 1, 1, (x >> 20), (*(s32 *)(record + 16) >> 20));
    Call2(Engine_TaskAddCallback, (s32)Actor_UpdatePresentationFlag, 0xc80);
    GameFlag_Set(0x201);
    Actor_SetSpritePriority(20, 2);
    Event_End();
}
