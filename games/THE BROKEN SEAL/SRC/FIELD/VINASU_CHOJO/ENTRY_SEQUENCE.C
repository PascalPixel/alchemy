#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgVinasuRobin[];
extern u8 MsgVinasuRobinHandedOverShamansRod[];


extern const s32 SceneAction_EntryGroup[];
extern const s32 SceneAction_EntryPair[];
void BattleFx_SetWeightedResult();
void Event_SetPair1d4(u16 first, u16 second);
void Party_SetFields1ceAnd1d0(u16 first, u16 second);
void Object_SetActionCallbackAndRefreshById();
s32 PartyInventory_Remove();
s32 PartyInventory_FindOwner();
void FieldScene_ForwardValue81fc();
s32 Scheduler_RemoveCallback();
void VinasuChojo_ShowMessage();
void VinasuChojo_FaceActor();
void FieldScene_RunStep6(void);

/* FAKEMATCH: call sites spelled through these wrappers pass their constants
   straight into the argument registers, and a value-returning call sets r0
   last of its arguments; a direct call builds the constants first. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The party reaches the summit: Robin speaks, the others gather, and the
   summit's scene is recorded in two scene and entrance pairs of the game
   state, with entrances 2 and 9. */
void Scene_RunActorEntrySequence(void)
{
    s32 itemOwner;
    u8 *object;
    s32 hidden;
    s32 disableMask;
    s32 advanceStep;
    s32 facing;
    s32 finalMask;
    s32 actor20Key;
    s32 frame;
    s32 effectCallback;
    s32 enableMask;
    s32 actor20LateKey;
    s32 message;
    const s32 *groupActions;
    const s32 *pairActions;
    s32 sharedData;

    Event_Begin();
    hidden = 0;
    Engine_EventGetViewCenter()->motion_flags = hidden;
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x14c0000, 0x200000, 0xb40000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x154, 184);
    VinasuChojo_FaceActor(0, 0x8000);
    Actor_RunRepeatedMotion(21, 1);
    Event_SetMessage((s32)MsgVinasuRobin);
    Call1(VinasuChojo_ShowMessage, 0x9015);
    Engine_EventGetViewCenter()->motion_flags = hidden;
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x1300000, 0x200000, 0xb40000, 1);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x16666, 0xb333);
    object = Engine_ActorGet(0);
    if (object != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    object = Engine_ActorGet(0);
    if (object != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    object = Engine_ActorGet(0);
    if (object != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    Actor_WalkTo(ACTOR_GERALD, 0x148, 168);
    Actor_WalkTo(ACTOR_IVAN, 0x154, 196);
    Actor_WalkToAndWait(ACTOR_MIA, 0x146, 204);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Engine_ActorFaceDirection(1, 0x8000, 0);
    Engine_ActorFaceDirection(2, 0x8000, 0);
    VinasuChojo_FaceActor(3, 0x8000);
    Engine_ActorFaceDirection(20, 0, 0);
    Engine_ActorFaceDirection(19, 0, 40);
    Actor_RunRepeatedMotion(20, 2);
    VinasuChojo_ShowMessage(20);
    VinasuChojo_FaceActor(19, 0x8000);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x103, 20);
    VinasuChojo_ShowMessage(21);
    Actor_StartRepeatedMotion(21, 2);
    Event_ShowMessageAndWait(21, 0, 20);
    Call3(Engine_ActorFaceDirection, 21, 0xd000, 40);
    VinasuChojo_ShowMessage(21);
    Actor_SetAnimationAndWait(21, 4);
    VinasuChojo_FaceActor(21, 0);
    Event_ShowMessageAndWait(21, 0, 20);
    actor20Key = 0x2014;
    Actor_RunRepeatedMotion(20, 1);
    VinasuChojo_FaceActor(20, 0x8000);
    VinasuChojo_ShowMessage(actor20Key);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    VinasuChojo_ShowMessage(3);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    VinasuChojo_ShowMessage(2);
    VinasuChojo_FaceActor(19, 0);
    VinasuChojo_ShowMessage(19);
    Actor_ShowEmote(20, 0x106, 40);
    Engine_ActorFaceDirection(20, 0, 20);
    VinasuChojo_ShowMessage(20);
    Call3(Engine_ActorFaceDirection, 0, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 0);
    Engine_ActorFaceDirection(21, 0x8000, 0);
    VinasuChojo_FaceActor(19, 0x3000);
    Actor_ShowEmote(19, 0x102, 40);
    VinasuChojo_ShowMessage(19);
    VinasuChojo_FaceActor(20, 0xb000);
    VinasuChojo_ShowMessage(actor20Key);
    Engine_ActorFaceDirection(20, 0x8000, 20);
    VinasuChojo_ShowMessage(actor20Key);
    Engine_ActorFaceDirection(19, 0x8000, 0);
    Engine_ActorFaceDirection(0, 0x8000, 0);
    Engine_ActorFaceDirection(1, 0x8000, 0);
    Engine_ActorFaceDirection(2, 0x8000, 0);
    Engine_ActorFaceDirection(3, 0x8000, 0);
    Engine_ActorFaceDirection(21, 0, 20);
    Actor_ShowEmote(6, 0x101, 40);
    VinasuChojo_ShowMessage(6);
    Actor_ShowEmote(20, 0x103, 20);
    VinasuChojo_ShowMessage(actor20Key);
    Actor_RunRepeatedMotion(6, 2);
    Event_Wait(20);
    Actor_SetAnimation(6, 3);
    VinasuChojo_ShowMessage(6);
    Actor_SetAnimationAndWait(20, 3);
    VinasuChojo_ShowMessage(actor20Key);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_WalkToAndWait(6, 0x104, 186);
    Engine_ActorFaceDirection(21, 0x3000, 0);
    Actor_WalkToAndWait(6, 0x114, 192);
    object = Engine_ActorGet(19);
    {
        s32 shown = 0x5000;

        *(u16 *)((s32)object + 6) = shown;
    }
    Task_Wait(1);
    Actor_StartRepeatedMotion(19, 2);
    VinasuChojo_ShowMessage(0x2013);
    Actor_Jump(6, 2, 20);
    Actor_SetSpeed(6, 0x26666, 0x13333);
    Actor_WalkToAndWait(6, 0x104, 186);
    Engine_ActorFaceDirection(21, 0x8000, 0);
    Actor_WalkToAndWait(6, 248, 172);
    Engine_ActorFaceDirection(19, 0x8000, 0);
    Engine_ActorFaceDirection(21, 0x8000, 0);
    Engine_ActorFaceDirection(6, 0, 20);
    Actor_SetAnimationAndWait(6, 3);
    Event_Wait(40);
    SceneState_ApplyPair140And0();
    Task_Wait(1);
    frame = 0;
    do {
        object = Engine_ActorGet(6);
        SceneEffect_UpdateObjectByFrameParity((s32)object);
        frame = (frame + 1);
        Task_Wait(1);
    } while ((u32)frame <= 39);
    effectCallback = (s32)FieldScene_RunStep6;
    Value2(Engine_TaskAddCallback, effectCallback, 0xc80);
    Event_Wait(80);
    Call3(Engine_ActorFaceDirection, 0, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 20);
    Engine_ActorFaceDirection(21, 0, 40);
    Call3(Engine_ActorFaceDirection, 0, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
    VinasuChojo_FaceActor(21, 0x8000);
    Actor_ShowEmote(20, 0x101, 40);
    Event_ShowMessageAndWait(0x2014, 0, 20);
    Actor_ShowEmote(6, 0x105, 80);
    Actor_StartRepeatedMotion(19, 2);
    VinasuChojo_ShowMessage(0x2013);
    Scheduler_RemoveCallback(effectCallback);
    Task_Wait(1);
    Actor_SetChildValue(6, 0);
    Task_Wait(10);
    FieldScene_ForwardValue81fc();
    Actor_Jump(6, 2, 40);
    VinasuChojo_ShowMessage(6);
    Actor_ShowEmote(20, 0x103, 20);
    VinasuChojo_ShowMessage(0x2014);
    disableMask = 254;
    Actor_RunRepeatedMotion(6, 2);
    *(u8 *)((u8 *)Engine_ActorGet(6) + 90) &= disableMask;
    Actor_WalkToAndWait(6, 250, 176);
    enableMask = 1;
    Event_Wait(1);
    {
        u8 *actor = Engine_ActorGet(6);
        s32 flags = actor[90];
        flags |= enableMask;
        actor[90] = flags;
    }
    Actor_ShowEmote(21, 0x103, 20);
    Engine_ActorFaceDirection(21, 0, 20);
    VinasuChojo_ShowMessage(21);
    Actor_SetAnimationAndWait(19, 4);
    VinasuChojo_ShowMessage(0x2013);
    Actor_RunRepeatedMotion(6, 2);
    Event_Wait(40);
    Actor_SetSpeed(6, 0x9999, 0x4ccc);
    *(u8 *)((u8 *)Engine_ActorGet(6) + 90) &= disableMask;
    Actor_WalkToAndWait(6, 248, 172);
    Event_Wait(1);
    {
        u8 *object = Engine_ActorGet(6);
        enableMask |= object[90];
        object[90] = enableMask;
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(6, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(19, 3);
    VinasuChojo_ShowMessage(0x2013);
    Actor_SetAnimationAndWait(6, 3);
    VinasuChojo_ShowMessage(6);
    Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 20, 0xb000, 20);
    Actor_ShowEmote(19, 0x105, 0);
    Actor_ShowEmote(20, 0x105, 60);
    Actor_SetAnimation(20, 4);
    VinasuChojo_ShowMessage(0x2014);
    Actor_ShowEmote(19, 0x101, 40);
    VinasuChojo_ShowMessage(19);
    Actor_ShowEmote(20, 0x105, 100);
    Actor_RunRepeatedMotion(20, 1);
    Event_Wait(20);
    VinasuChojo_FaceActor(20, 0);
    VinasuChojo_FaceActor(19, 0);
    Event_OpenMessage(20, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 0);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(20, 3);
        advanceStep = 1;
    } else {
        Event_Wait(20);
        Actor_SetAnimationAndWait(20, 4);
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
        advanceStep = 0;
    }
    VinasuChojo_ShowMessage(20);
    if (advanceStep != 0) {
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
    }
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 20);
    Actor_ShowEmote(20, 0x108, 40);
    Event_OpenMessage(20, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 0);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        advanceStep = 1;
    } else {
        Event_Wait(20);
        Actor_SetAnimation(ACTOR_IVAN, 4);
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
        advanceStep = 0;
    }
    VinasuChojo_ShowMessage(2);
    if (advanceStep != 0) {
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
    }
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    VinasuChojo_FaceActor(1, 0x4000);
    VinasuChojo_ShowMessage(1);
    Actor_RunRepeatedMotion(19, 1);
    VinasuChojo_ShowMessage(19);
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    VinasuChojo_FaceActor(3, 0x8000);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Actor_SetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(20, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    VinasuChojo_FaceActor(19, 0x3000);
    Actor_SetAnimation(19, 3);
    VinasuChojo_ShowMessage(19);
    Call3(Engine_ActorFaceDirection, 20, 0xb000, 20);
    Actor_SetAnimationAndWait(20, 3);
    actor20LateKey = 0x2014;
    Event_Wait(40);
    VinasuChojo_FaceActor(20, 0x8000);
    VinasuChojo_ShowMessage(actor20LateKey);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(21, 0, 40);
    VinasuChojo_ShowMessage(actor20LateKey);
    Actor_ShowEmote(21, 0x103, 60);
    VinasuChojo_FaceActor(19, 0x8000);
    Actor_RunRepeatedMotion(19, 1);
    Call1(VinasuChojo_ShowMessage, 0x2013);
    Actor_ShowEmote(21, 0x105, 60);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(20);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_WalkToAndWait(21, 0x120, 192);
    Engine_ActorFaceDirection(19, 0, 0);
    Engine_ActorFaceDirection(20, 0, 0);
    Actor_WalkToAndWait(21, 0x136, 192);
    Actor_WalkToAndWait(21, 0x148, 186);
    Event_Wait(20);
    Actor_RunRepeatedMotion(21, 2);
    /* The message is a link-time name, loaded from the literal pool after
       the two preceding calls, as the game loads it. */
    message = (s32)MsgVinasuRobinHandedOverShamansRod;
    Message_ShowCentered(message, 1);
    Actor_WalkToAndWait(21, 0x136, 192);
    Engine_ActorFaceDirection(19, 0x8000, 0);
    Engine_ActorFaceDirection(20, 0x8000, 0);
    Actor_WalkToAndWait(21, 0x120, 192);
    Actor_WalkToAndWait(21, 0x106, 176);
    Engine_ActorFaceDirection(21, 0, 40);
    Event_SetMessage((message + 1));
    VinasuChojo_ShowMessage(21);
    Actor_SetAnimationAndWait(20, 3);
    VinasuChojo_ShowMessage(actor20LateKey);
    Actor_SetAnimationAndWait(21, 3);
    VinasuChojo_FaceActor(20, 0);
    VinasuChojo_FaceActor(21, 0x8000);
    Actor_SetAnimationAndWait(21, 3);
    Actor_SetAnimationAndWait(6, 3);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_WalkToAndWait(6, 0x104, 186);
    Engine_ActorFaceDirection(21, 0x3000, 0);
    Actor_WalkToAndWait(6, 0x114, 192);
    Audio_PlayCue(19);
    facing = 160;
    object = Engine_ActorGet(19);
    *(u16 *)((s32)object + 6) = (facing << 7);
    Task_Wait(1);
    Actor_RunRepeatedMotion(19, 1);
    VinasuChojo_ShowMessage(19);
    Actor_RunRepeatedMotion(6, 2);
    Engine_ActorFaceDirection(21, 0, 0);
    Engine_ActorFaceDirection(20, (facing << 7), 0);
    Call3(Engine_ActorFaceDirection, 6, 0xd000, 20);
    Actor_Jump(ACTOR_MIA, 2, 20);
    VinasuChojo_FaceActor(3, 0xa000);
    VinasuChojo_ShowMessage(3);
    Engine_ActorFaceDirection(21, 0, 0);
    Engine_ActorFaceDirection(6, 0, 0);
    Engine_ActorFaceDirection(19, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 20, 0xb000, 80);
    Engine_ActorFaceDirection(19, 0, 0);
    Engine_ActorFaceDirection(20, 0, 40);
    Audio_PlayCue(29);
    VinasuChojo_ShowMessage(20);
    Engine_ActorFaceDirection(21, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 6, 0xb000, 20);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    VinasuChojo_ShowMessage(2);
    VinasuChojo_FaceActor(19, 0x3000);
    Actor_SetAnimationAndWait(19, 3);
    VinasuChojo_ShowMessage(19);
    Engine_ActorFaceDirection(21, 0, 0);
    VinasuChojo_FaceActor(6, 0xd000);
    Actor_SetAnimation(ACTOR_MIA, 3);
    VinasuChojo_ShowMessage(3);
    Actor_ShowEmote(20, 0x100, 20);
    Engine_ActorFaceDirection(20, 0x3000, 20);
    VinasuChojo_ShowMessage(20);
    Actor_ShowEmote(6, 0x102, 0);
    Actor_ShowEmote(21, 0x102, 60);
    Engine_ActorFaceDirection(6, 0, 0);
    VinasuChojo_FaceActor(21, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 40);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    VinasuChojo_ShowMessage(1);
    VinasuChojo_FaceActor(19, 0);
    Actor_SetAnimationAndWait(19, 4);
    VinasuChojo_ShowMessage(19);
    VinasuChojo_FaceActor(20, 0);
    Actor_SetAnimation(20, 4);
    VinasuChojo_ShowMessage(20);
    Engine_ActorFaceDirection(6, 0xd000, 0);
    Actor_ShowEmote(6, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    VinasuChojo_FaceActor(19, 0x3000);
    Actor_SetAnimation(19, 4);
    VinasuChojo_ShowMessage(19);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    VinasuChojo_FaceActor(19, 0);
    Actor_SetAnimationAndWait(20, 3);
    VinasuChojo_ShowMessage(20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Engine_ActorFaceDirection(0, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 40);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    VinasuChojo_FaceActor(3, 0xa000);
    VinasuChojo_ShowMessage(3);
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    VinasuChojo_ShowMessage(1);
    Engine_ActorFaceDirection(6, 0, 0);
    Engine_ActorFaceDirection(0, 0x8000, 0);
    Engine_ActorFaceDirection(2, 0x8000, 40);
    Actor_ShowEmote(20, 0x103, 40);
    Actor_StartRepeatedMotion(20, 2);
    VinasuChojo_ShowMessage(20);
    Actor_RunRepeatedMotion(19, 1);
    VinasuChojo_ShowMessage(19);
    itemOwner = PartyInventory_FindOwner(65);
    GameFlag_Set(itemOwner + 0x345);
    finalMask = 254;
    PartyInventory_Remove(65);
    *(u8 *)((u8 *)Engine_ActorGet(0) + 90) &= finalMask;
    *(u8 *)((u8 *)Engine_ActorGet(1) + 90) &= finalMask;
    *(u8 *)((u8 *)Engine_ActorGet(2) + 90) &= finalMask;
    *(u8 *)((u8 *)Engine_ActorGet(3) + 90) &= finalMask;
    *(u8 *)((u8 *)Engine_ActorGet(19) + 90) &= finalMask;
    *(u8 *)((u8 *)Engine_ActorGet(20) + 90) &= finalMask;
    *(u8 *)((u8 *)Engine_ActorGet(21) + 90) &= finalMask;
    {
        u8 *actor = Engine_ActorGet(6);
        actor += 90;
        groupActions = SceneAction_EntryGroup;
        finalMask &= *actor;
        *actor = finalMask;
    }
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, groupActions);
    Actor_EnableActionCallback(ACTOR_GERALD, groupActions);
    Actor_EnableActionCallback(ACTOR_IVAN, groupActions);
    Actor_EnableActionCallback(ACTOR_MIA, groupActions);
    pairActions = SceneAction_EntryPair;
    Actor_EnableActionCallback(19, pairActions);
    Actor_EnableActionCallback(20, pairActions);
    Actor_EnableActionCallback(21, groupActions);
    Object_SetActionCallbackAndRefreshById(6, groupActions);
    /* These tables are shared by the final actor-action assignments. */
    /* FAKEMATCH: an empty do-while around these statements; it only changes instruction scheduling. */
    do {
        sharedData = (s32)&gGameState;
        *(u8 *)((sharedData + 0x22b)) = 3;
    } while (0);
    {
        s32 actor = (s32)&SceneId_VinasuChojo;

        /*
         * Through Call2 rather than called directly: the wrapper's parameter
         * pseudos fix the order the two argument registers are materialised
         * in, and direct calls here emit them the other way round.
         */
        Call2(Party_SetFields1ceAnd1d0, actor, 2);
        Call2(Event_SetPair1d4, actor, 9);
    }
    BattleFx_SetWeightedResult(98, 1);
    GameFlag_Set(0x350);
}
