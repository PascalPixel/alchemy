/* Draft of resource_38d 0x020088c0 (FieldScene_RunLongBranchingChoreography),
 * built with games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/KYUDEN.H. It
 * matched the ROM apart from its names. Remaining difference: the ROM loads
 * message 0x1440 from the literal pool where the Kolima offer is refused, as
 * a link-time message value would; the C constant is built with a move and a
 * shift. Its imports still carry their old call-site names: each reaches the
 * import veneer IMPORT.S labels for the same main-image code. The listing
 * keeps these rows. */
#include "KYUDEN.H"

extern u8 LinkedMessage_TooYoungForTheJob;

s32 Func_02002e68();
s32 Func_02002fae();
s32 Func_02002fc2();
s32 Func_02002fe0();
void Func_0200311c();
s32 Func_020031f6();
s32 Func_02003216();
void Func_0200323c();
s32 Func_02003240();
s32 Func_0200330a();
s32 Func_0200331e();
s32 Func_0200333c();
void Func_02003438();
s32 Func_0200348a();
s32 Func_02003496();
s32 Func_020034a4();
void Func_0200353a();
void Func_020035fc();
void Func_02003786();
void Func_020037f0();
s32 Func_020038ba();
void Func_0200392c();
void Func_02003af2();
void Func_02003afc();
void Func_02003b06();
void Func_02003b10();
s32 Func_02003b20();
s32 Func_02003b7e();
s32 Func_02003ba8();
void Func_02003baa_b();
void Func_02003bd4();

void FieldScene_RunLongBranchingChoreography(void)
{
    s32 record;

    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_TaskWait(1);
    *(u8 *)(Func_02002e68() + 85) = 0;
    Call3(Engine_CameraMoveTo, 0x37e0000, -1, 0x2980000);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(0, 0, 0);
    if (Value1(Engine_GameFlagIsSet, 0x85f) != 0) {
        Call4(Engine_CameraMoveTo, 0x37e0000, -1, 0x2ba0000, 0);
        Call3(Engine_ActorSetPosition, 19, 0x36c0000, 0x27a0000);
        Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
        Call3(Engine_ActorSetPosition, 0, 0x37e0000, 0x31e0000);
    }
    Engine_MapRedraw();
    Engine_TaskWait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 40;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    if (Value1(Engine_GameFlagIsSet, 0x85f) == 0) {
        Engine_EventWait(80);
        Call3(Engine_ActorSetPosition, 19, 0x37e0000, 0x31e0000);
        Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
        Camera_MoveTo(0x37e0000, -1, 0x2ba0000, 1);
        Call3(Engine_ActorSetSpeed, 19, 0xcccc, 0x6666);
        Actor_WalkTo(19, 0x37e, 0x2b8);
        Engine_EventWait(80);
        Call4(Engine_CameraMoveTo, 0x37e0000, -1, 0x2980000, 1);
        Actor_WaitForMove(19);
        Call3(Engine_ActorWalkToAndWait, 19, 0x34a, 0x2b8);
        Actor_WalkToAndWait(19, 0x34a, 0x27c);
        Actor_FaceDirection(18, 0x7000, 20);
        Call3(Engine_ActorWalkToAndWait, 19, 0x36c, 0x27a);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(20);
        Actor_SetAnimationAndWait(18, 3);
        Event_Wait(10);
        Call1(Engine_EventSetMessage, 0x1437);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Actor_RunRepeatedMotion(19, 2);
        Engine_EventShowMessageAndWait(19, 0, 20);
        Engine_ActorRunRepeatedMotion(18, 1);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Engine_ActorSetAnimationAndWait(19, 3);
        Engine_EventWait(40);
        Call3(Engine_ActorShowEmote, 18, 0x105, 60);
        Call2(Engine_EventShowMessage, 0x2012, 0);
        Engine_ActorRunRepeatedMotion(18, 1);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call2(Engine_ActorSetAttachedEffect, 19, 0x102);
        Engine_EventWait(60);
        Call3(Engine_ActorFaceDirection, 19, 0x3000, 10);
        Call3(Engine_ActorFaceDirection, 18, 0x5000, 10);
        Call4(Engine_CameraMoveTo, 0x37e0000, -1, 0x2ba0000, 1);
        Call3(Engine_ActorSetPosition, 0, 0x37e0000, 0x31e0000);
        Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
        Call3(Engine_ActorWalkToAndWait, 0, 0x37e, 0x2d6);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(18, 1);
        Call2(Engine_EventShowMessage, 0x2012, 0);
        Camera_MoveTo(0x37e0000, -1, 0x2980000, 1);
        Call3(Engine_ActorWalkToAndWait, 0, 0x37e, 0x2ac);
        record = Value1(Func_02002fae, 0);
        if (record != 0) {
            Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        record = Value1(Func_02002fc2, 0);
        if (record != 0) {
            Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        if (Value1(Engine_GameFlagIsSet, 3) != 0) {
            record = Value1(Func_02002fe0, 0);
            if (record != 0) {
                Engine_ActorSetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
            }
        }
        Call3(Engine_ActorSetSpeed, 1, 0x9999, 0x4ccc);
        Call3(Engine_ActorSetSpeed, 2, 0x9999, 0x4ccc);
        Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
        Engine_ActorSetAnimation(1, 2);
        Engine_ActorSetAnimation(2, 2);
        Engine_ActorSetAnimation(3, 2);
        Actor_SetDestinationOffset(ACTOR_GERALD, -16, 16);
        Engine_ActorSetDestinationOffset(2, 16, 16);
        if (Value1(Engine_GameFlagIsSet, 3) != 0) {
            Engine_ActorSetDestinationOffset(3, 32, 16);
        }
        Engine_ActorWaitForMove(2);
        Engine_ActorSetAnimation(1, 1);
        Engine_ActorSetAnimation(2, 1);
        Actor_SetAnimation(ACTOR_MIA, 1);
        Engine_EventWait(10);
        Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
        Func_0200311c(18, 2, 20);
        Call3(Engine_ActorFaceDirection, 18, 0x7000, 10);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
        Actor_SetAnimationAndWait(19, 3);
        Call3(Engine_ActorFaceDirection, 18, 0x5000, 40);
        Call3(Engine_ActorFaceDirection, 18, 0x7000, 10);
        Engine_ActorSetAnimationAndWait(18, 4);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call2(Engine_ActorSetAttachedEffect, 19, 0x102);
        Event_Wait(40);
        Call3(Engine_ActorFaceDirection, 18, 0x5000, 20);
        Call3(Engine_ActorShowEmote, 18, 0x105, 40);
        Value2(Engine_EventOpenMessage, 0x2012, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xe000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            goto L_02000f86;
        }
    L_02000cb6:
        Engine_EventSetMessage((s32)&LinkedMessage_TooYoungForTheJob);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
        Engine_ActorSetAnimationAndWait(18, 4);
        Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
        Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
        Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimation(ACTOR_GERALD, 2);
        record = Value1(Func_020031f6, 0);
        if (record != 0) {
            Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorSetAnimation(2, 2);
        record = Value1(Func_02003216, 0);
        if (record != 0) {
            Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        if (Value1(Engine_GameFlagIsSet, 3) != 0) {
            Engine_ActorSetAnimation(3, 2);
            record = Value1(Func_02003240, 0);
            if (record != 0) {
                Engine_ActorSetDestination(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
        }
        Engine_ActorWaitForMove(2);
        Engine_ActorSetPosition(1, 0, 0);
        Engine_ActorSetPosition(2, 0, 0);
        Actor_SetPosition(ACTOR_MIA, 0, 0);
        Call1(Func_0200323c, 0x85f);
        Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x37e, 0x2f0);
        gEventWork->transition_frames = 16;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        goto L_0200177e;
    }
    Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Call3(Engine_ActorWalkTo, 0, 0x37e, 0x2ac);
    Engine_EventWait(80);
    Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
    Call4(Engine_CameraMoveTo, 0x37e0000, -1, 0x2980000, 1);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetAnimation(0, 1);
    record = Value1(Func_0200330a, 0);
    if (record != 0) {
        Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_0200331e, 0);
    if (record != 0) {
        Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    if (Value1(Engine_GameFlagIsSet, 3) != 0) {
        record = Value1(Func_0200333c, 0);
        if (record != 0) {
            Engine_ActorSetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
    }
    Call3(Engine_ActorSetSpeed, 1, 0x9999, 0x4ccc);
    Call3(Engine_ActorSetSpeed, 2, 0x9999, 0x4ccc);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Engine_ActorSetAnimation(1, 2);
    Engine_ActorSetAnimation(2, 2);
    Engine_ActorSetAnimation(3, 2);
    Call3(Engine_ActorSetDestinationOffset, 1, -16, 16);
    Engine_ActorSetDestinationOffset(2, 16, 16);
    if (Value1(Engine_GameFlagIsSet, 3) != 0) {
        Engine_ActorSetDestinationOffset(3, 32, 16);
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetAnimation(1, 1);
    Engine_ActorSetAnimation(2, 1);
    Engine_ActorSetAnimation(3, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
    Call3(Engine_ActorShowEmote, 18, 0x101, 60);
    Event_SetMessage(MSG_YEHVE_HAD_CHANGE_HEART_YEH);
    Value2(Engine_EventOpenMessage, 0x2012, 0);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 1) {
        goto L_02000cb6;
    }
L_02000f86:
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
    Engine_ActorSetAnimation(3, 3);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Call3(Engine_ActorShowEmote, 18, 0x105, 60);
    Call1(Engine_EventSetMessage, 0x1443);
    Call2(Engine_EventShowMessage, 0x2012, 0);
    record = Func_0200348a(20);
    Func_02003438(record, 0);
    record = Func_02003496(20);
    *(s32 *)(record + 24) = 0x8000;
    *(s32 *)(record + 28) = 0x8000;
    record = Value1(Func_020034a4, 18);
    if (record != 0) {
        Engine_ActorSetPosition(20, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Engine_TaskWait(1);
    Func_0200353a(20, 6, 0);
    Call3(Engine_ActorSetSpeed, 20, 0x20000, 0x10000);
    Actor_MoveToAndWait(20, 0x37e, 0x29c);
    Event_Wait(40);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 3, 0x101, 0);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Call3(Engine_ActorShowEmote, 2, 0x101, 60);
    Engine_ActorSetAnimationAndWait(18, 4);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 1, 0x103, 60);
    Call3(Engine_ActorFaceDirection, 1, 0xe000, 10);
    Value2(Engine_EventOpenMessage, 0x4001, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    if (Value2(Engine_EventChooseYesNo, 0, 0) == 1) {
        do {
            Func_020035fc(1, 2);
            Engine_ActorRunRepeatedMotion(2, 2);
            Call1(Engine_EventSetMessage, 0x1447);
            Value2(Engine_EventOpenMessage, 0x4001, 0);
        } while (Value2(Engine_EventChooseYesNo, 0, 0) != 1);
    }
    Engine_ActorSetAnimationAndWait(1, 3);
    Call1(Engine_EventSetMessage, 0x1448);
    Call3(Engine_EventShowMessageAndWait, 0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 10);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Actor_ShowEmote(18, 0x105, 60);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 60);
    Call3(Engine_ActorFaceDirection, 18, 0x3000, 10);
    Call3(Engine_ActorShowEmote, 18, 0x101, 60);
    Call3(Engine_ActorShowEmote, 1, 0x101, 40);
    Engine_ActorFaceDirection(1, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Call3(Engine_EventShowMessageAndWait, 0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x2000, 20);
    Call3(Engine_ActorShowEmote, 2, 0x102, 60);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 10);
    Call3(Engine_EventShowMessageAndWait, 0x4002, 0, 10);
    Engine_ActorRunRepeatedMotion(1, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(1, 3);
    Call3(Engine_EventShowMessageAndWait, 0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 1, 0xe000, 10);
    Func_02003786(1, 1);
    Value2(Engine_EventOpenMessage, 0x4001, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x6000, 0);
    while (Event_ChooseYesNo(0, 0) != 0) {
        Call1(Engine_EventSetMessage, 0x144e);
        Call2(Func_020037f0, 0x4001, 0);
    }
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
    Engine_ActorFaceDirection(1, 0, 10);
    Call3(Engine_ActorFaceDirection, 0, 0x2000, 10);
    Engine_ActorSetAnimationAndWait(1, 3);
    Call3(Engine_ActorShowEmote, 2, 0x105, 60);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 10);
    Engine_ActorSetAnimationAndWait(2, 4);
    Call1(Engine_EventSetMessage, 0x144f);
    Call3(Engine_EventShowMessageAndWait, 0x4002, 0, 20);
    Engine_ActorRunRepeatedMotion(18, 1);
    Call3(Engine_ActorFaceDirection, 18, 0x5000, 10);
    Engine_ActorSetAnimationAndWait(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 20);
    Engine_ActorSetAnimation(3, 3);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(40);
    Actor_ShowEmote(18, 0x105, 80);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
    Call2(Engine_ActorSetAttachedEffect, 19, 0x102);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorRunRepeatedMotion(18, 1);
    Engine_EventWait(40);
    Call3(Engine_ActorFaceDirection, 18, 0x7000, 20);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_EventWait(20);
    Actor_SetAnimationAndWait(18, 4);
    Actor_SetAnimation(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Func_0200392c(20, 6, 0);
    record = Value1(Func_020038ba, 18);
    if (record != 0) {
        Engine_ActorSetDestination(20, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(20);
    Engine_ActorSetPosition(20, 0, 0);
    Engine_EventWait(20);
    Call2(Engine_ActorSetAttachedEffect, 3, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 2, 0x102);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(19, 2);
    Engine_EventShowMessageAndWait(19, 0, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 20);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Call3(Engine_EventShowMessageAndWait, 0x4002, 0, 10);
    Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 18, 0x3000, 10);
    Engine_ActorSetAnimationAndWait(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 1, 0x103, 60);
    Call3(Engine_EventShowMessageAndWait, 0x4001, 0, 10);
    Call3(Engine_ActorFaceDirection, 18, 0x5000, 10);
    Engine_ActorSetAnimation(18, 4);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Engine_ActorSetAnimation(2, 4);
    Call3(Engine_EventShowMessageAndWait, 0x4002, 0, 10);
    Call3(Engine_ActorFaceDirection, 18, 0x3000, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Call3(Engine_EventShowMessageAndWait, 0x2012, 0, 10);
    Call3(Engine_ActorShowEmote, 3, 0x107, 0);
    Call3(Engine_ActorShowEmote, 0, 0x107, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x107, 0);
    Call3(Engine_ActorShowEmote, 2, 0x107, 60);
    Call3(Engine_ActorFaceDirection, 18, 0x7000, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(0x2012, 0, 10);
    Engine_ActorRunRepeatedMotion(19, 2);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
    Engine_ActorSetAnimationAndWait(19, 3);
    Engine_EventWait(20);
    Call3(Func_02003af2, 0, 0x10013, (s32)Kyuden_FacingActions);
    Call3(Func_02003afc, 1, 0x10013, (s32)Kyuden_FacingActions);
    Call3(Func_02003b06, 2, 0x10013, (s32)Kyuden_FacingActions);
    Call3(Func_02003b10, 3, 0x10013, (s32)Kyuden_FacingActions);
    Call3(Engine_ActorSetSpeed, 19, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(19, 0x354, 0x286);
    Call3(Engine_ActorWalkToAndWait, 19, 0x354, 0x29a);
    Call3(Engine_ActorWalkToAndWait, 19, 0x360, 0x2a0);
    Call3(Engine_ActorFaceDirection, 19, 0x1000, 10);
    Event_ShowMessageAndWait(0x4013, 0, 20);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(1);
    Engine_ActorStop(2);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Call3(Engine_ActorShowEmote, 1, 0x105, 0);
    Call3(Engine_ActorShowEmote, 2, 0x105, 60);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Call3(Func_02003baa_b, 19, 0x10000, (s32)Kyuden_FacingActions);
    Engine_ActorSetAnimation(1, 2);
    record = Value1(Func_02003b20, 0);
    if (record != 0) {
        Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorSetAnimation(2, 2);
    record = Value1(Func_02003b7e, 0);
    if (record != 0) {
        Engine_ActorSetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    if (Value1(Engine_GameFlagIsSet, 3) != 0) {
        Engine_ActorSetAnimation(3, 2);
        record = Value1(Func_02003ba8, 0);
        if (record != 0) {
            Engine_ActorSetDestination(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(2, 0, 0);
    Engine_ActorSetPosition(3, 0, 0);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 0, 0x37e, 0x2f0);
    gEventWork->transition_frames = 16;
    Event_CloseScreen();
    Engine_EventWaitForScreen();
    Call1(Func_02003bd4, 0x321);
L_0200177e:
    Event_RequestExit(29);
    Engine_EventEnd();
}
