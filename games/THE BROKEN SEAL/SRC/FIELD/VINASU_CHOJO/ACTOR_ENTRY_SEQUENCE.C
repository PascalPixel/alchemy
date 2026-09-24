#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum ActorEntrySequenceMessage {
    MSG_ROBIN = 0x278e
};


extern u8 LinkedMessage_RobinHandedOverShamansRod[];

extern u8 Value_000000bb[];

/* Complete scene owner 020012c8..020020db. The object effect loop runs
 * forty frames; flag-dependent branches preserve the shared step increments.
 * Calls bind to original-site-resolved loader runtime addresses. */
extern u8 Data_02000240[];
extern const s32 SceneAction_EntryGroup[];
extern const s32 SceneAction_EntryPair[];
void Func_02002350(void);
u8 *Func_02005e4c();
void Func_020008a8();
void Func_02000894();
u8 *Func_02005d4c();
void Func_02002334();
void Func_020020dc();
s32 Func_02005c04();
s32 Func_02005c0c();
void Func_02002344();
s32 Func_02005cfc();
s32 Func_02005d04();
void Func_02005d7c();
void Func_02005e6c(u16 first, u16 second);
void Engine_Import0808a268(u16 first, u16 second);
void Func_02005e64();

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
    *(u8 *)(Func_02005e4c() + 85) = hidden;
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x14c0000, 0x200000, 0xb40000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x154, 184);
    Func_020008a8(0, 0x8000);
    Actor_RunRepeatedMotion(21, 1);
    Event_SetMessage(MSG_ROBIN);
    Call1(Func_02000894, 0x9015);
    *(u8 *)(Func_02005e4c() + 85) = hidden;
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x1300000, 0x200000, 0xb40000, 1);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x16666, 0xb333);
    object = Func_02005d4c(0);
    if (object != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    object = Func_02005d4c(0);
    if (object != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    object = Func_02005d4c(0);
    if (object != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    Actor_WalkTo(ACTOR_GERALD, 0x148, 168);
    Actor_WalkTo(ACTOR_IVAN, 0x154, 196);
    Actor_WalkToAndWait(ACTOR_MIA, 0x146, 204);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Engine_ObjectMotionArmCallback(1, 0x8000, 0);
    Engine_ObjectMotionArmCallback(2, 0x8000, 0);
    Func_020008a8(3, 0x8000);
    Engine_ObjectMotionArmCallback(20, 0, 0);
    Engine_ObjectMotionArmCallback(19, 0, 40);
    Actor_RunRepeatedMotion(20, 2);
    Func_02000894(20);
    Func_020008a8(19, 0x8000);
    Func_02000894(0x2013);
    Actor_ShowEmote(21, 0x103, 20);
    Func_02000894(21);
    Actor_StartRepeatedMotion(21, 2);
    Event_ShowMessageAndWait(21, 0, 20);
    Call3(Engine_ObjectMotionArmCallback, 21, 0xd000, 40);
    Func_02000894(21);
    Actor_SetAnimationAndWait(21, 4);
    Func_020008a8(21, 0);
    Event_ShowMessageAndWait(21, 0, 20);
    actor20Key = 0x2014;
    Actor_RunRepeatedMotion(20, 1);
    Func_020008a8(20, 0x8000);
    Func_02000894(actor20Key);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Func_02000894(3);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Func_02000894(2);
    Func_020008a8(19, 0);
    Func_02000894(19);
    Actor_ShowEmote(20, 0x106, 40);
    Engine_ObjectMotionArmCallback(20, 0, 20);
    Func_02000894(20);
    Call3(Engine_ObjectMotionArmCallback, 0, 0xa000, 0);
    Call3(Engine_ObjectMotionArmCallback, 1, 0x2000, 0);
    Call3(Engine_ObjectMotionArmCallback, 2, 0x6000, 0);
    Call3(Engine_ObjectMotionArmCallback, 3, 0xe000, 0);
    Engine_ObjectMotionArmCallback(21, 0x8000, 0);
    Func_020008a8(19, 0x3000);
    Actor_ShowEmote(19, 0x102, 40);
    Func_02000894(19);
    Func_020008a8(20, 0xb000);
    Func_02000894(actor20Key);
    Engine_ObjectMotionArmCallback(20, 0x8000, 20);
    Func_02000894(actor20Key);
    Engine_ObjectMotionArmCallback(19, 0x8000, 0);
    Engine_ObjectMotionArmCallback(0, 0x8000, 0);
    Engine_ObjectMotionArmCallback(1, 0x8000, 0);
    Engine_ObjectMotionArmCallback(2, 0x8000, 0);
    Engine_ObjectMotionArmCallback(3, 0x8000, 0);
    Engine_ObjectMotionArmCallback(21, 0, 20);
    Actor_ShowEmote(6, 0x101, 40);
    Func_02000894(6);
    Actor_ShowEmote(20, 0x103, 20);
    Func_02000894(actor20Key);
    Actor_RunRepeatedMotion(6, 2);
    Event_Wait(20);
    Actor_SetAnimation(6, 3);
    Func_02000894(6);
    Actor_SetAnimationAndWait(20, 3);
    Func_02000894(actor20Key);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_WalkToAndWait(6, 0x104, 186);
    Engine_ObjectMotionArmCallback(21, 0x3000, 0);
    Actor_WalkToAndWait(6, 0x114, 192);
    object = Func_02005d4c(19);
    {
        s32 shown = 0x5000;

        *(u16 *)((s32)object + 6) = shown;
    }
    Task_Wait(1);
    Actor_StartRepeatedMotion(19, 2);
    Func_02000894(0x2013);
    Actor_Jump(6, 2, 20);
    Actor_SetSpeed(6, 0x26666, 0x13333);
    Actor_WalkToAndWait(6, 0x104, 186);
    Engine_ObjectMotionArmCallback(21, 0x8000, 0);
    Actor_WalkToAndWait(6, 248, 172);
    Engine_ObjectMotionArmCallback(19, 0x8000, 0);
    Engine_ObjectMotionArmCallback(21, 0x8000, 0);
    Engine_ObjectMotionArmCallback(6, 0, 20);
    Actor_SetAnimationAndWait(6, 3);
    Event_Wait(40);
    Func_02002334();
    Task_Wait(1);
    frame = 0;
    do {
        object = Func_02005d4c(6);
        Func_020020dc((s32)object);
        frame = (frame + 1);
        Task_Wait(1);
    } while ((u32)frame <= 39);
    effectCallback = (s32)Func_02002350;
    Value2(Func_02005c04, effectCallback, 0xc80);
    Event_Wait(80);
    Call3(Engine_ObjectMotionArmCallback, 0, 0xa000, 0);
    Call3(Engine_ObjectMotionArmCallback, 1, 0x2000, 0);
    Call3(Engine_ObjectMotionArmCallback, 2, 0x6000, 0);
    Call3(Engine_ObjectMotionArmCallback, 3, 0xe000, 20);
    Engine_ObjectMotionArmCallback(21, 0, 40);
    Call3(Engine_ObjectMotionArmCallback, 0, 0x8000, 0);
    Call3(Engine_ObjectMotionArmCallback, 1, 0x8000, 0);
    Call3(Engine_ObjectMotionArmCallback, 2, 0x8000, 0);
    Call3(Engine_ObjectMotionArmCallback, 3, 0x8000, 0);
    Func_020008a8(21, 0x8000);
    Actor_ShowEmote(20, 0x101, 40);
    Event_ShowMessageAndWait(0x2014, 0, 20);
    Actor_ShowEmote(6, 0x105, 80);
    Actor_StartRepeatedMotion(19, 2);
    Func_02000894(0x2013);
    Func_02005c0c(effectCallback);
    Task_Wait(1);
    Actor_SetChildValue(6, 0);
    Task_Wait(10);
    Func_02002344();
    Actor_Jump(6, 2, 40);
    Func_02000894(6);
    Actor_ShowEmote(20, 0x103, 20);
    Func_02000894(0x2014);
    disableMask = 254;
    Actor_RunRepeatedMotion(6, 2);
    *(u8 *)(Func_02005d4c(6) + 90) &= disableMask;
    Actor_WalkToAndWait(6, 250, 176);
    enableMask = 1;
    Event_Wait(1);
    {
        u8 *actor = Func_02005d4c(6);
        s32 flags = actor[90];
        flags |= enableMask;
        actor[90] = flags;
    }
    Actor_ShowEmote(21, 0x103, 20);
    Engine_ObjectMotionArmCallback(21, 0, 20);
    Func_02000894(21);
    Actor_SetAnimationAndWait(19, 4);
    Func_02000894(0x2013);
    Actor_RunRepeatedMotion(6, 2);
    Event_Wait(40);
    Actor_SetSpeed(6, 0x9999, 0x4ccc);
    *(u8 *)(Func_02005d4c(6) + 90) &= disableMask;
    Actor_WalkToAndWait(6, 248, 172);
    Event_Wait(1);
    {
        u8 *object = Func_02005d4c(6);
        enableMask |= object[90];
        object[90] = enableMask;
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(6, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(19, 3);
    Func_02000894(0x2013);
    Actor_SetAnimationAndWait(6, 3);
    Func_02000894(6);
    Call3(Engine_ObjectMotionArmCallback, 19, 0x3000, 0);
    Call3(Engine_ObjectMotionArmCallback, 20, 0xb000, 20);
    Actor_ShowEmote(19, 0x105, 0);
    Actor_ShowEmote(20, 0x105, 60);
    Actor_SetAnimation(20, 4);
    Func_02000894(0x2014);
    Actor_ShowEmote(19, 0x101, 40);
    Func_02000894(19);
    Actor_ShowEmote(20, 0x105, 100);
    Actor_RunRepeatedMotion(20, 1);
    Event_Wait(20);
    Func_020008a8(20, 0);
    Func_020008a8(19, 0);
    Event_OpenMessage(20, 0);
    Call3(Engine_ObjectMotionArmCallback, 1, 0x2000, 0);
    Call3(Engine_ObjectMotionArmCallback, 2, 0xc000, 0);
    Call3(Engine_ObjectMotionArmCallback, 3, 0xe000, 0);
    if (Value2(Engine_UiWorkWaitThenFinalizeCapacity, 0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(20, 3);
        advanceStep = 1;
    } else {
        Event_Wait(20);
        Actor_SetAnimationAndWait(20, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        advanceStep = 0;
    }
    Func_02000894(20);
    if (advanceStep != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Call3(Engine_ObjectMotionArmCallback, 1, 0x8000, 0);
    Call3(Engine_ObjectMotionArmCallback, 2, 0x8000, 0);
    Call3(Engine_ObjectMotionArmCallback, 3, 0x8000, 20);
    Actor_ShowEmote(20, 0x108, 40);
    Event_OpenMessage(20, 0);
    Call3(Engine_ObjectMotionArmCallback, 1, 0x2000, 0);
    Call3(Engine_ObjectMotionArmCallback, 2, 0xc000, 0);
    Call3(Engine_ObjectMotionArmCallback, 3, 0xe000, 0);
    if (Value2(Engine_UiWorkWaitThenFinalizeCapacity, 0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        advanceStep = 1;
    } else {
        Event_Wait(20);
        Actor_SetAnimation(ACTOR_IVAN, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        advanceStep = 0;
    }
    Func_02000894(2);
    if (advanceStep != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Func_020008a8(1, 0x4000);
    Func_02000894(1);
    Actor_RunRepeatedMotion(19, 1);
    Func_02000894(19);
    Call3(Engine_ObjectMotionArmCallback, 1, 0x8000, 0);
    Call3(Engine_ObjectMotionArmCallback, 2, 0x8000, 0);
    Func_020008a8(3, 0x8000);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Actor_SetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(20, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Func_020008a8(19, 0x3000);
    Actor_SetAnimation(19, 3);
    Func_02000894(19);
    Call3(Engine_ObjectMotionArmCallback, 20, 0xb000, 20);
    Actor_SetAnimationAndWait(20, 3);
    actor20LateKey = 0x2014;
    Event_Wait(40);
    Func_020008a8(20, 0x8000);
    Func_02000894(actor20LateKey);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(21, 0, 40);
    Func_02000894(actor20LateKey);
    Actor_ShowEmote(21, 0x103, 60);
    Func_020008a8(19, 0x8000);
    Actor_RunRepeatedMotion(19, 1);
    Call1(Func_02000894, 0x2013);
    Actor_ShowEmote(21, 0x105, 60);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(20);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_WalkToAndWait(21, 0x120, 192);
    Engine_ObjectMotionArmCallback(19, 0, 0);
    Engine_ObjectMotionArmCallback(20, 0, 0);
    Actor_WalkToAndWait(21, 0x136, 192);
    Actor_WalkToAndWait(21, 0x148, 186);
    Event_Wait(20);
    Actor_RunRepeatedMotion(21, 2);
    /*
     * Spelled as the address of a Value_ symbol rather than the integer
     * 0x27ba. A CONST_INT that fits an immediate is materialised with mov,
     * and one that does not is still free to be hoisted and shared; a
     * SYMBOL_REF has to come from the literal pool and stays where it is
     * written. The reference loads this constant from the pool after the two
     * preceding calls, which the integer spelling does not reproduce.
     */
    message = (s32)LinkedMessage_RobinHandedOverShamansRod;
    Message_ShowCentered(message, 1);
    Actor_WalkToAndWait(21, 0x136, 192);
    Engine_ObjectMotionArmCallback(19, 0x8000, 0);
    Engine_ObjectMotionArmCallback(20, 0x8000, 0);
    Actor_WalkToAndWait(21, 0x120, 192);
    Actor_WalkToAndWait(21, 0x106, 176);
    Engine_ObjectMotionArmCallback(21, 0, 40);
    Event_SetMessage((message + 1));
    Func_02000894(21);
    Actor_SetAnimationAndWait(20, 3);
    Func_02000894(actor20LateKey);
    Actor_SetAnimationAndWait(21, 3);
    Func_020008a8(20, 0);
    Func_020008a8(21, 0x8000);
    Actor_SetAnimationAndWait(21, 3);
    Actor_SetAnimationAndWait(6, 3);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_WalkToAndWait(6, 0x104, 186);
    Engine_ObjectMotionArmCallback(21, 0x3000, 0);
    Actor_WalkToAndWait(6, 0x114, 192);
    Audio_PlayCue(19);
    facing = 160;
    object = Func_02005d4c(19);
    *(u16 *)((s32)object + 6) = (facing << 7);
    Task_Wait(1);
    Actor_RunRepeatedMotion(19, 1);
    Func_02000894(19);
    Actor_RunRepeatedMotion(6, 2);
    Engine_ObjectMotionArmCallback(21, 0, 0);
    Engine_ObjectMotionArmCallback(20, (facing << 7), 0);
    Call3(Engine_ObjectMotionArmCallback, 6, 0xd000, 20);
    Actor_Jump(ACTOR_MIA, 2, 20);
    Func_020008a8(3, 0xa000);
    Func_02000894(3);
    Engine_ObjectMotionArmCallback(21, 0, 0);
    Engine_ObjectMotionArmCallback(6, 0, 0);
    Engine_ObjectMotionArmCallback(19, 0x3000, 0);
    Call3(Engine_ObjectMotionArmCallback, 20, 0xb000, 80);
    Engine_ObjectMotionArmCallback(19, 0, 0);
    Engine_ObjectMotionArmCallback(20, 0, 40);
    Audio_PlayCue(29);
    Func_02000894(20);
    Engine_ObjectMotionArmCallback(21, 0x3000, 0);
    Call3(Engine_ObjectMotionArmCallback, 6, 0xb000, 20);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Func_02000894(2);
    Func_020008a8(19, 0x3000);
    Actor_SetAnimationAndWait(19, 3);
    Func_02000894(19);
    Engine_ObjectMotionArmCallback(21, 0, 0);
    Func_020008a8(6, 0xd000);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Func_02000894(3);
    Actor_ShowEmote(20, 0x100, 20);
    Engine_ObjectMotionArmCallback(20, 0x3000, 20);
    Func_02000894(20);
    Actor_ShowEmote(6, 0x102, 0);
    Actor_ShowEmote(21, 0x102, 60);
    Engine_ObjectMotionArmCallback(6, 0, 0);
    Func_020008a8(21, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 40);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Func_02000894(1);
    Func_020008a8(19, 0);
    Actor_SetAnimationAndWait(19, 4);
    Func_02000894(19);
    Func_020008a8(20, 0);
    Actor_SetAnimation(20, 4);
    Func_02000894(20);
    Engine_ObjectMotionArmCallback(6, 0xd000, 0);
    Actor_ShowEmote(6, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Func_020008a8(19, 0x3000);
    Actor_SetAnimation(19, 4);
    Func_02000894(19);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Func_020008a8(19, 0);
    Actor_SetAnimationAndWait(20, 3);
    Func_02000894(20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Engine_ObjectMotionArmCallback(0, 0xa000, 0);
    Call3(Engine_ObjectMotionArmCallback, 1, 0x2000, 0);
    Call3(Engine_ObjectMotionArmCallback, 2, 0x6000, 0);
    Call3(Engine_ObjectMotionArmCallback, 3, 0xe000, 40);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Func_020008a8(3, 0xa000);
    Func_02000894(3);
    Call3(Engine_ObjectMotionArmCallback, 1, 0x8000, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Func_02000894(1);
    Engine_ObjectMotionArmCallback(6, 0, 0);
    Engine_ObjectMotionArmCallback(0, 0x8000, 0);
    Engine_ObjectMotionArmCallback(2, 0x8000, 40);
    Actor_ShowEmote(20, 0x103, 40);
    Actor_StartRepeatedMotion(20, 2);
    Func_02000894(20);
    Actor_RunRepeatedMotion(19, 1);
    Func_02000894(19);
    itemOwner = Func_02005cfc(65);
    GameFlag_Set(itemOwner + 0x345);
    finalMask = 254;
    Func_02005d04(65);
    *(u8 *)(Func_02005d4c(0) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(1) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(2) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(3) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(19) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(20) + 90) &= finalMask;
    *(u8 *)(Func_02005d4c(21) + 90) &= finalMask;
    {
        u8 *actor = Func_02005d4c(6);
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
    Func_02005d7c(6, groupActions);
    /* These tables are shared by the final actor-action assignments. */
    /* FAKEMATCH: an empty do-while around these statements; it only changes instruction scheduling. */
    do {
        sharedData = (s32)Data_02000240;
        *(u8 *)((sharedData + 0x22b)) = 3;
    } while (0);
    {
        s32 actor = (s32)Value_000000bb;

        /*
         * Through Call2 rather than called directly: the wrapper's parameter
         * pseudos fix the order the two argument registers are materialised
         * in, and direct calls here emit them the other way round.
         */
        Call2(Func_02005e6c, actor, 2);
        Call2(Engine_Import0808a268, actor, 9);
    }
    Func_02005e64(98, 1);
    GameFlag_Set(0x350);
}
