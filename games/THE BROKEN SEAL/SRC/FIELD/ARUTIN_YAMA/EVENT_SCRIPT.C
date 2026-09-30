#include "YAMA.H"
#include "CALL.H"
extern u8 MsgArutinGuardianStatuesWereCreatedLong[];
extern u8 MsgArutinWeDidRobinWeBeat[];

/*
 * Bit 1 of the runtime status word at 0x03001e40 selects mode 7 or mode 6
 * for record 8. That bit's meaning is unverified: other callbacks here mask
 * different bits of the same word. Both branches reach the same veneer, and
 * the declarations carry no parameter list because the arguments are set up
 * in registers at the call site. The owner spans 44 bytes -- the body, one
 * alignment halfword and one literal pool word.
 */
void SceneActor_SetActor8ModeByCounterBit(void)
{
    extern u32 Data_03001e40;

    if (((Data_03001e40 >> 1) & 1) != 0) {
        Actor_SetChildValue(8, 7);
    } else {
        Actor_SetChildValue(8, 6);
    }
}

/* Record returned by Engine_ActorGet/26/3a: a pair of s32 fields at +8 and
 * +16 that get forwarded straight into the matching setup call. */
void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 addr_0200affd;
    s32 addr_0200c0e4;
    s32 addr_0200c12c;

    Event_Begin();
    Actor_SetPosition(8, 0x1480000, 0x580000);
    Actor_SetPosition(9, 0x1480000, 0x580000);
    Actor_SetAnimation(8, 0);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 40;
    Event_OpenScreen();
    Event_WaitForScreen(); /* main:0808a370 */
    Event_Wait(20);
    record = Engine_ActorGet(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Engine_ActorGet(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Engine_ActorGet(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_IVAN, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_MIA, 0x9999, 0x4ccc);
    Engine_ActorEnableActionCallback(1, ArutinYama_GeraldScript);
    Engine_ActorEnableActionCallback(2, ArutinYama_IvanScript);
    Object_SetActionCallbackAndRefreshById(3, ArutinYama_MiaScript);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Event_SetMessage((s32)MsgArutinWeDidRobinWeBeat);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Event_OpenMessage(ACTOR_IVAN, 0); /* main:0808a178 */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    } else {
        Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
        bump_step(1);
    }
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_SetMessage((s32)MsgArutinGuardianStatuesWereCreatedLong);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 40);
    Audio_PlayCue(190);
    Actor_SetChildValue(8, 7);
    Event_Wait(10);
    Audio_PlayCue(0x121);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 40);
    Audio_PlayCue(103);
    addr_0200affd = (s32)SceneActor_SetActor8ModeByCounterBit;
    Call2(Engine_TaskAddCallback, addr_0200affd, 0xc80); /* main:080000d0 */
    addr_0200c0e4 = (s32)ArutinYama_CelebrateScript;
    Actor_EnableActionCallback(9, addr_0200c0e4);
    Object_SetActionCallbackAndRefreshById(8, addr_0200c0e4);
    Engine_TaskRemoveCallback(addr_0200affd); /* main:080000d8 */
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 40);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    addr_0200c12c = (s32)ArutinYama_PartyScript;
    Actor_EnableActionCallback(ACTOR_GERALD, addr_0200c12c);
    Engine_ActorEnableActionCallback(2, addr_0200c12c);
    Object_SetActionCallbackAndRefreshById(3, addr_0200c12c);
    Event_Wait(20);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x204;
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 16;
    GameFlag_Set(0x909);
    Event_End();
}
