#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void SceneActor_SetModeZeroAndValue();
void SceneEffect_ApplyThreeValuesAndFinish();
void FieldScene_RunSplitTripleSteps();
void SceneActor_SetPairZeroAndValue();
void Object_RefreshSelectorById();
s32 BattleFx_PlayCueAndStartEmitterOnTarget();
void Party_RemoveOwnerRestored();

extern u8 MsgKuupuappuIvanGotShamansRod[];
extern u8 MsgKuupuappuWaitDontWantTakeYour[];
extern u8 MsgKuupuappuYouRobinRightWontForget[];

extern u8 KuupuappuHeya_VaultScriptA[];
extern u8 KuupuappuHeya_VaultScriptB[];
extern u8 KuupuappuHeya_VaultScriptC[];
extern u8 KuupuappuHeya_VaultScriptD[];
extern u8 KuupuappuHeya_VaultScriptE[];

void SceneActor_SetModeZeroAndValue(s32 a, s32 b);

void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c);

/* FAKEMATCH: calls spelled through these value wrappers keep the
 * reference's argument and reload order. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void bump_halfword(s32 off, s32 amount)
{
    u8 *work = *(u8 **)&gEventWork;
    u16 *slot = (u16 *)((s32)work + off);
    s32 next = *slot + amount;

    *slot = next;
}

s32 OverlayObject_GetObjectTwoByte118(void);

s32 OverlayObject_RunObjectTwoWhenFlagged(void);

/* The room's closing scene: Ivan is given the Shaman's Rod and the party
 * takes its leave. */
void RunDialoguePromptScene(void)
{
    u32 i;
    s32 off1c8;
    s32 off1d8;
    u8 *rec8;
    u8 *record;
    u8 *work;
    s32 aftermath;
    s32 crowd;
    s32 late;
    s32 rod;
    u8 *p7;

    p7 = (u8 *)gEventWork;
    GameFlag_Set(0x855);
    Event_Begin();
    {
        u8 *record = ((u8 * (*)())Engine_ActorGet)(12);
        /* FAKEMATCH: a result temporary, not a compound or-assign: the
         * reference merges the byte into the mask's register, which the
         * two-address ORR does only when the result is its own object. */
        u8 merged = (u8)(record[35] | 1);

        record[35] = merged;
    }
    Actor_MoveToAndWait(15, 0x368, 0x1a9);
    Actor_MoveToAndWait(16, 0x368, 0x199);
    Actor_MoveToAndWait(17, 0x368, 0x179);
    Actor_SetPosition(11, 0x3080000, 0x1880000);
    Actor_SetPosition(10, 0x3180000, 0x1880000);
    Actor_SetPosition(12, 0x3280000, 0x1880000);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Actor_SetAnimation(12, 5);
    Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    record = ((u8 * (*)())Engine_ActorGet)(10);
    Actor_SetSpriteFlags((s32)record, 1);
    record = ((u8 * (*)())Engine_ActorGet)(11);
    Actor_SetSpriteFlags((s32)record, 1);
    record = ((u8 * (*)())Engine_ActorGet)(12);
    Actor_SetSpriteFlags((s32)record, 1);
    Actor_SetPosition(13, 0x3000000, 0x1980000);
    Actor_SetPosition(14, 0x3000000, 0x1a80000);
    Actor_MoveToAndWait(9, 0x310, 0x1a8);
    Actor_SetPosition(8, 0x3280000, 0x1980000);
    Actor_FaceActor(13, 9, 0);
    Actor_FaceActor(8, 9, 0);
    Actor_FaceActor(14, 10, 0);
    Actor_FaceActor(9, 10, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3180000, 0x1b80000);
    Actor_SetPosition(ACTOR_GERALD, 0x3280000, 0x1b80000);
    Actor_SetPosition(ACTOR_IVAN, 0x3080000, 0x1b80000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 10, 0);
    Actor_FaceActor(ACTOR_GERALD, 10, 0);
    Actor_FaceActor(ACTOR_IVAN, 10, 0);
    work = (u8 *)gEventWork;
    off1c8 = 0x1c8;
    *(s32 *)(work + off1c8) = 30;
    *(s32 *)(work + 0x1c0) = 0x201;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(10, 2);
    aftermath = (s32)MsgKuupuappuYouRobinRightWontForget;
    Event_SetMessage(aftermath);
    SceneActor_SetModeZeroAndValue(10, 20);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_SetAnimation(13, 3);
    SceneEffect_ApplyThreeValuesAndFinish(8, 3, 20);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Event_Wait(60);
    Actor_SetSpeed(13, 0xcccc, 0x6666);
    Actor_WalkTo(13, 0x2ea, 0x198);
    Actor_FaceDirection(9, 0xb000, 0);
    Actor_FaceDirection(14, 0xb000, 0);
    crowd = (s32)KuupuappuHeya_VaultScriptB;
    Actor_EnableActionCallback(11, crowd);
    Event_Wait(20);
    Actor_EnableActionCallback(10, crowd);
    Event_Wait(15);
    Actor_EnableActionCallback(12, crowd);
    Event_Wait(35);
    Actor_EnableActionCallback(8, KuupuappuHeya_VaultScriptA);
    Event_Wait(20);
    Actor_WaitForMove(13);
    Actor_SetPosition(13, 0, 0);
    Event_Wait(40);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Actor_WalkToAndWait(9, 0x310, 0x198);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_WalkToAndWait(14, 0x300, 0x198);
    FieldScene_RunSplitTripleSteps(14, 0, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    SceneEffect_ApplyThreeValuesAndFinish(14, 3, 20);
    Actor_FaceDirection(14, 0x2000, 10);
    SceneActor_SetModeZeroAndValue(14, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 50);
    SceneActor_SetPairZeroAndValue(0, 2, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    FieldScene_RunSplitTripleSteps(2, 9, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 50);
    Actor_FaceEachOther(9, 14, 0);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_EnableActionCallback(14, KuupuappuHeya_VaultScriptC);
    Event_Wait(50);
    Value2((s32 (*)())Engine_ActorEnableActionCallback, 9, (s32)KuupuappuHeya_VaultScriptD);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x318, off1c8);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x318, 0x198);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x328, off1c8);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Event_Wait(100);
    FieldScene_RunSplitTripleSteps(14, 9, 60);
    FieldScene_RunSplitTripleSteps(9, 14, 40);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 40);
    Actor_FaceDirection(9, 0, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 2);
    Audio_PlayCue(124);
    Actor_SetAnimation(15, 4);
    Actor_SetPosition(18, 0x3680000, 0x1a80000);
    Actor_SetSpritePriority(18, 1);
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(18, 0, -8);
    Actor_WaitForMove(18);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(60);
    Message_ShowCentered((aftermath + 5), 1);
    Actor_SetAnimation(15, 2);
    Actor_SetPosition(18, 0, 0);
    {
        u8 *work0 = *(u8 **)&gEventWork;
        u16 *slot0;
        s32 next0;

        off1d8 = 0x1d8;
        slot0 = (u16 *)((s32)work0 + off1d8);
        next0 = *slot0 + 1;
        *slot0 = next0;
    }
    Actor_RunRepeatedMotion(14, 1);
    SceneActor_SetModeZeroAndValue(14, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 40);
    FieldScene_RunSplitTripleSteps(9, 14, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    FieldScene_RunSplitTripleSteps(1, 14, 40);
    Actor_FaceDirection(14, 0, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(14, 2);
    Audio_PlayCue(124);
    Actor_SetAnimation(16, 4);
    {
        u8 *rec;
        s32 t = 0;
        u16 zero_sym = (u16)(t + t);

        rec = (u8 *)((s32 (*)())Engine_ActorGet)(19);
        rec[85] = zero_sym;
    }
    Actor_SetSpritePriority(19, 1);
    Actor_SetPosition(19, 0x3680000, 0x1980000);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(19, 0, -8);
    Actor_WaitForMove(19);
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(60);
    Message_ShowCentered((aftermath + 8), 1);
    Actor_SetAnimation(16, 2);
    Actor_SetPosition(19, 0, 0);
    bump_halfword(off1d8, 1);
    Actor_ShowEmote(9, 0x102, 0);
    Event_Wait(60);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(14, 3, 50);
    FieldScene_RunSplitTripleSteps(9, 0, 20);
    SceneActor_SetModeZeroAndValue(9, 30);
    Actor_FaceDirection(14, 0xd000, 0);
    Event_Wait(30);
    Actor_ShowEmote(14, 0x100, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(14, 0x358, 0x178);
    Event_Wait(20);
    FieldScene_RunSplitTripleSteps(14, 9, 20);
    SceneActor_SetModeZeroAndValue(14, 20);
    Actor_FaceActor(9, 14, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Event_Wait(60);
    FieldScene_RunSplitTripleSteps(2, 14, 30);
    FieldScene_RunSplitTripleSteps(9, 2, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(14, 0x5000, 0);
    Event_Wait(30);
    FieldScene_RunSplitTripleSteps(2, 9, 20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 40);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    FieldScene_RunSplitTripleSteps(2, 9, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 0);
    Event_Wait(60);
    SceneActor_SetModeZeroAndValue(2, 40);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 4);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 3);
    Event_Wait(30);
    SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 30);
    Actor_SetSpeed(ACTOR_IVAN, 0x18000, 0xc000);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x320, 0x198);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    FieldScene_RunSplitTripleSteps(1, 9, 30);
    SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(60);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 40);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(9, 0x348, 0x1a8);
    SceneActor_SetPairZeroAndValue(9, 0, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    SceneEffect_ApplyThreeValuesAndFinish(0, 3, 20);
    Actor_FaceDirection(9, 0x5000, 0);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(1, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 30);
    SceneActor_SetPairZeroAndValue(9, 14, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_WalkToAndWait(14, 0x358, 0x198);
    late = (s32)KuupuappuHeya_VaultScriptE;
    Actor_EnableActionCallback(9, late);
    Actor_EnableActionCallback(14, late);
    Actor_WalkTo(ACTOR_GERALD, 0x318, off1c8);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkTo(ACTOR_IVAN, 0x308, 0x1b0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, 0xd000, 0);
    Object_RefreshSelectorById(9);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    rec8 = Value1(((s32 (*)())Engine_ActorGet), 14);
    {
        u8 *target = rec8 + 91;
        s32 shown = 1;

        *target = shown;
    }
    *(s32 *)((s32)rec8 + 56) = -0x80000000;
    *(s32 *)((s32)rec8 + 60) = -0x80000000;
    *(s32 *)((s32)rec8 + 64) = -0x80000000;
    Actor_ShowEmote(9, 0x100, 0);
    Actor_SetAnimation(14, 1);
    Event_Wait(50);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x328, 0x1b8);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 0);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(9, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    FieldScene_RunSplitTripleSteps(1, 9, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    Actor_WalkTo(9, 0x2e8, 0x198);
    Actor_WalkTo(14, 0x2e8, 0x198);
    Actor_WaitForMove(9);
    Actor_WaitForMove(14);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x318, 0x198);
    SceneActor_SetPairZeroAndValue(0, 1, 30);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Event_Wait(50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(1, 40);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
    SceneActor_SetPairZeroAndValue(0, 1, 40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 4);
    SceneEffect_ApplyThreeValuesAndFinish(1, 4, 30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(1, 40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
    SceneActor_SetModeZeroAndValue(1, 30);
    Event_OpenMessage(ACTOR_IVAN, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Event_Wait(20);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_Wait(20);
        SceneActor_SetModeZeroAndValue(1, 20);
        SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
        SceneActor_SetModeZeroAndValue(2, 20);
    }
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x320, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x358, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x358, 0x178);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    rec8 = Value3(BattleFx_PlayCueAndStartEmitterOnTarget, 2, 17, 65);
    Event_Wait(60);
    rod = (s32)MsgKuupuappuIvanGotShamansRod;
    Message_ShowCentered(rod, 1);
    Engine_ObjectDispatchRelease((s32)rec8);
    Actor_SetAnimation(17, 2);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x358, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x320, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x308, 0x1a8);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    Event_SetMessage((rod + 1));
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    {
        u16 *slot = (u16 *)((s32)p7 + 0x1d8);
        s32 saved = *(s16 *)slot;

        if (Value0(OverlayObject_GetObjectTwoByte118)!= 0) {
            Event_SetMessage((s32)MsgKuupuappuWaitDontWantTakeYour);
            Event_ShowMessage(ACTOR_IVAN, 0);
            OverlayObject_RunObjectTwoWhenFlagged();
        }
        Party_RemoveOwnerRestored(2);
        *slot = saved;
    }
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 50);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x308, 0x198);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x2e8, 0x198);
    Event_Wait(40);
    SceneActor_SetPairZeroAndValue(0, 1, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(1, 20);
    SceneEffect_ApplyThreeValuesAndFinish(0, 3, 20);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Value1(((s32 (*)())Engine_ActorGet), 0);
    if ((s32)record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Event_Wait(30);
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    *(s32 *)((u8 *)gEventWork + 0x1c0) = 0x209;
    Event_End();
}

void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c);
