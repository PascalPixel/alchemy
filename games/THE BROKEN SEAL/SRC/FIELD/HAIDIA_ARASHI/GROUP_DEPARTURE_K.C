#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaCantGetAroundThisRock[];
extern u8 MsgHaidiaNorthLeadsToMtAleph[];
extern u8 MsgHaidiaTheBoulderIsFalling[];

/*
 * The overlay's own work, after its image: whether the boulder has stopped
 * shaking, and the shift that slows the shaking.
 */
s32 HaidiaArashi_ShakeDone;
s32 HaidiaArashi_ShakeShift;

void FieldScene_RunScene372SequenceD(void)
{
    u32 i;
    s32 record;
    struct FieldActor *actor;

    Event_Begin();
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
        Actor_SetPosition(22, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(22, 0x10000, 0x8000);
    Actor_WalkToAndWait(22, 0x119, 0x1fb);
    Actor_FaceEachOther(22, ACTOR_PARTY_LEADER, 0);
    Event_Wait(30);
    Event_SetMessage((s32)MsgHaidiaNorthLeadsToMtAleph);
    Event_ShowMessage(22, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Event_Wait(20);
    Actor_FaceDirection(22, 0x4000, 0);
    Event_ShowMessage(22, 0);
    Actor_SetAnimation(22, 2);
    record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(22);
    Actor_SetPosition(22, 0, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x100, 0x205);
    Event_End();
}

void SceneActor_RunActor22PlacementSequence(s32 x, s32 y)
{
    Thing1 *a;
    s32 w = 0x10000;
    s32 h = 0x8000;
    Thing2 *b;

    a = Actor_Get(ACTOR_PARTY_LEADER);
    if (a != 0) {
        Actor_SetPosition(22, a->unk8, a->unk10);
    }
    Actor_SetSpeed(22, w, h);
    Actor_WalkToAndWait(22, x, y);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
    Event_Wait(20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Event_Wait(40);
    Event_SetMessage((s32)MsgHaidiaCantGetAroundThisRock);
    Event_ShowMessage(22, 0);
    Actor_RunRepeatedMotion(22, 2);
    Event_ShowMessage(22, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(22, 2);
    b = Actor_Get(ACTOR_PARTY_LEADER);
    if (b != 0) {
        Actor_SetDestination(22, b->unkA, b->unk12);
    }
    Actor_WaitForMove(22);
    Actor_SetPosition(22, 0, 0);
}

void Scene_BoulderFalls(void)
{
    u32 i;
    s32 record;
    s32 *phase;
    struct FieldActor *actor;
    s32 steps;
    s32 callback;
    s32 shifted;

    if (GameFlag_IsSet(FLAG_BOULDER_FELL) == 0) {
        Event_Begin();
        Call1(Event_CallWithLastActiveObjectId, (s32)HaidiaArashi_LastObjectCall);
        SceneState_SetValue140Mode0();
        Task_Wait(1);
        Audio_PlayCue(141);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Event_Wait(30);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Audio_PlayCue(145);
        Event_Wait(30);
        actor = Actor_Get(ACTOR_PARTY_LEADER);
        if (actor != NULL) {
            Actor_SetPosition(22, actor->x.fixed, actor->z.fixed);
        }
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
        Actor_SetSpeed(22, 0x20000, 0x10000);
        Value2(Engine_ActorEnableActionCallback, 0, (s32)HaidiaArashi_LeaderScript);
        Call2(Object_SetActionCallbackAndRefreshById, 22, (s32)HaidiaArashi_ActorTwentyTwoScript);
        Object_RefreshSelectorById(0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
        Actor_ShowEmote(22, 0x100, 30);
        Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Audio_PlayCue(145);
        Event_Wait(40);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Audio_PlayCue(145);
        Event_Wait(20);
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAttachedEffect(22, 0x102);
        Event_Wait(40);
        Actor_SetAnimation(32, 5);
        Actor_SetAnimation(33, 5);
        Actor_SetAnimation(30, 8);
        Actor_SetAnimation(29, 8);
        Actor_Get(30)->scale_x = -0x10000;
        ObjectMotion_SetActionVariant(32, 2);
        ObjectMotion_SetActionVariant(33, 2);
        ObjectMotion_SetActionVariant(30, 3);
        ObjectMotion_SetActionVariant(29, 3);
        Event_SetMessage((s32)MsgHaidiaTheBoulderIsFalling);
        Event_ShowMessageAndWait(28, 0, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(22, 0xc000, 20);
        Camera_SetSpeed(0x40000, 0x8000);
        Camera_MoveTo(0x700000, -1, 0x14b0000, 1);
        Camera_WaitForMove();
        for (i = 0; i < 40; i++) {
            SceneActor_SetModeByFrameBit1(Actor_Get(32));
            SceneActor_SetModeByFrameBit1(Actor_Get(33));
            SceneActor_SetModeByFrameBit1(Actor_Get(30));
            SceneActor_SetModeByFrameBit1(Actor_Get(29));
            Task_Wait(1);
        }
        phase = &HaidiaArashi_ShakeShift;
        steps = (s32)FieldScene_RunFourPairedSteps;
        HaidiaArashi_ShakeDone = 0;
        *phase = 0;
        Value2(Scheduler_AddOrUpdateCallback, steps, 0xc80);
        callback = (s32)SceneState_SetValue19ThenCall;
        Call2(Scheduler_AddOrUpdateCallback, callback, 0xc80);
        Event_Wait(40);
        *phase = 1;
        Event_Wait(30);
        Actor_SetPosition(19, 0x720000, 0x1220000);
        record = Actor_Get(19);
        shifted = *(s32 *)(record + 12) + 0x400000;
        *(s32 *)(record + 12) = shifted;
        *(s32 *)(record + 60) = shifted;
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Audio_PlayCue(145);
        Actor_MoveToAndWait(19, 114, 0x14d);
        Actor_SetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Audio_PlayCue(145);
        *phase = 0;
        Actor_SetSpeed(19, 0x6666, 0x3333);
        Actor_MoveToAndWait(19, 114, 0x12c);
        Actor_SetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Audio_PlayCue(145);
        *phase = 2;
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Actor_MoveToAndWait(19, 114, 0x14d);
        Actor_SetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Audio_PlayCue(145);
        *phase = 0;
        Actor_SetSpeed(19, 0x6666, 0x3333);
        Actor_MoveToAndWait(19, 114, 0x12c);
        Actor_SetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Audio_PlayCue(145);
        *phase = 2;
        Actor_SetSpeed(19, 0xcccc, 0x6666);
        Actor_MoveToAndWait(19, 114, 0x14d);
        Actor_SetAnimation(19, 2);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Audio_PlayCue(145);
        *phase = 1;
        Event_Wait(20);
        Actor_SetAttachedEffect(32, 0x102);
        Actor_RunRepeatedMotion(32, 2);
        Event_ShowMessage(31, 0);
        Actor_ShowEmote(33, 0x100, 0);
        Actor_RunRepeatedMotion(33, 2);
        Event_ShowMessageAndWait(28, 0, 40);
        Actor_SetAttachedEffect(30, 0x102);
        Actor_RunRepeatedMotion(30, 2);
        Event_ShowMessage(30, 0);
        HaidiaArashi_ShakeDone = 1;
        Actor_SetAnimation(29, 1);
        Task_Wait(1);
        Actor_SetChildValue(29, 0);
        Actor_ShowEmote(29, 0x105, 20);
        Actor_FaceDirection(29, 0x8000, 40);
        Actor_FaceDirection(29, 0, 20);
        Actor_FaceDirection(29, 0x8000, 20);
        Actor_FaceDirection(29, 0x4000, 40);
        Actor_ShowEmote(29, 0x100, 0);
        Actor_RunRepeatedMotion(29, 2);
        Actor_Jump(29, 4, 40);
        Actor_SetAnimation(29, 9);
        Event_Wait(10);
        Event_ShowMessageAndWait(29, 0, 20);
        Audio_PlayCue(0x121);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Camera_SetSpeed(0x60000, 0xc000);
        Camera_MoveTo(0x540000, -1, 0x2340000, 1);
        Camera_WaitForMove();
        BattleFx_PlayQueuedSound();
        Actor_FaceActor(22, ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Actor_SetAttachedEffect(22, 0x102);
        Event_Wait(30);
        Scheduler_RemoveCallback(steps);
        Scheduler_RemoveCallback(callback);
        Event_ShowMessage(22, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
        Event_Wait(20);
        FieldScene_RunSingleStep();
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimationAndWait(22, 3);
        Event_Wait(20);
        Actor_SetAnimation(22, 2);
        record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
        if (record != 0) {
            Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(22);
        Actor_SetPosition(22, 0, 0);
        Actor_Destroy(31);
        Actor_Destroy(28);
        Actor_Destroy(30);
        Actor_Destroy(29);
        Actor_Destroy(32);
        Actor_Destroy(33);
        GameFlag_Set(FLAG_BOULDER_FELL);
        Event_End();
    }
}

void OverlayObject_SetChildByte5AndMark(u8 *o, s32 n)
{
    if ((*(u8 *)(o + 0x54) & 15) == 1) {
        u8 *c = *(u8 **)(o + 0x50);
        s32 idx = n - 1;
        u8 cnt;
        if (n == 0) {
            idx = HaidiaArashi_FrameModes[(gFrameCount >> 1) & (*(u8 *)(o + 0x54) & 15)];
        }
        cnt = *(u8 *)(c + 0x27);
        if (cnt != 0) {
            u8 **p = (u8 **)(c + 0x28);
            s32 k = cnt;
            do {
                u8 *e = *p++;
                if (e != 0 && *(s32 *)(e + 16) != 0) {
                    *(u8 *)(e + 5) = idx;
                }
                k--;
            } while (k != 0);
        }
        *(u8 *)(c + 0x25) = 1;
    }
}

void ActorPresentation_SetFourActorsModeByBit(void)
{
    if (((gFrameCount >> HaidiaArashi_ShakeShift) & 3) != 0) {
        OverlayObject_SetChildByte5AndMark(Actor_Get(32), 1);
        OverlayObject_SetChildByte5AndMark(Actor_Get(33), 1);
        OverlayObject_SetChildByte5AndMark(Actor_Get(30), 1);
        OverlayObject_SetChildByte5AndMark(Actor_Get(29), 1);
    } else {
        OverlayObject_SetChildByte5AndMark(Actor_Get(32), 8);
        OverlayObject_SetChildByte5AndMark(Actor_Get(33), 8);
        OverlayObject_SetChildByte5AndMark(Actor_Get(30), 8);
        OverlayObject_SetChildByte5AndMark(Actor_Get(29), 8);
    }
}
