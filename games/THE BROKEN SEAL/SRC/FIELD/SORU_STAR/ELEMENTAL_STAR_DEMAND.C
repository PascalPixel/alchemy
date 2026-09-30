#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
extern u8 MsgSoruLooksLikeTheyveSpottedUs[];
extern u8 MsgSoruWhyDenyDont[];

void Event_SayThenWait();
struct ObjectRuntime;
void BattlePres_RunActionThenWaitIfModeZero();

/* Moves the event's current message on by amount. */
static __inline__ void SkipMessage(s32 amount)
{
    gEventWork->message += amount;
}

/* The demand for the Elemental Stars. Its dialogue starts at MsgSoruLooksLikeTheyveSpottedUs
 * while the actors move into place; a nonzero answer to the prompt skips one
 * reply, and the dialogue then continues from MsgSoruWhyDenyDont. */
void FieldScene_RunElementalStarDemand(void)
{
    u8 *field_85;
    u8 *field_80_38;
    u8 *record12;
    u8 *record8;
    s32 cnt;
    s32 zero;

    Engine_AudioPlayCue(61);
    Engine_ActorSetAnimation(10, 4);
    Event_SetMessage((s32)MsgSoruLooksLikeTheyveSpottedUs);
    Event_SayThenWait(10, 10);
    Engine_ActorSetAnimation(11, 4);
    Event_SayThenWait(11, 30);
    Call3(Engine_ActorShowEmote, 9, 0x102, 60);
    Engine_ActorJump(9, 4, 10);
    Actor_Jump(9, 6, 30);
    Event_SayThenWait(9, 10);
    Engine_ActorRunRepeatedMotion(10, 1);
    Call3(Engine_ActorFaceDirection, 10, 0xb000, 10);
    Event_SayThenWait(10, 20);
    Actor_RunRepeatedMotion(11, 1);
    Call3(Engine_ActorFaceDirection, 11, 0xd000, 20);
    Event_SayThenWait(11, 30);
    Call3(Engine_ActorShowEmote, 9, 0x102, 60);
    Engine_ActorFaceDirection(5, 0, 0);
    Call3(Engine_ActorFaceDirection, 9, 0x7000, 80);
    Call3(Engine_ActorShowEmote, 5, 0x102, 40);
    Event_SayThenWait(5, 20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_ActorSetAnimation(9, 4);
    Event_SayThenWait(9, 10);
    record12 = (u8 *)Engine_ActorGet(12);
    record8 = (u8 *)Engine_ActorGet(8);
    field_80_38 = *(u8 **)(record12 + 80) + 38;
    zero = 0;
    *field_80_38 = zero;
    *(s32 *)(record12 + 24) = 0x1999;
    *(s32 *)(record12 + 28) = 0x1999;
    *(s32 *)(record8 + 24) = 0x1999;
    *(s32 *)(record8 + 28) = 0x1999;
    Call2(Engine_ActorSetChildValue, 12, 0x100);
    Call3(Engine_ActorSetPosition, 12, 0x1d70000, 0x1220000);
    field_85 = record12 + 85;
    *field_85 = zero;
    *(s32 *)(record12 + 12) = 0x280000;
    Engine_EventWait(1);
    Event_SayThenWait(12, 10);
    Call3(Engine_ActorShowEmote, 5, 0x100, 0);
    Call3(Engine_ActorShowEmote, 9, 0x100, 30);
    Call3(Engine_ActorFaceDirection, 5, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 9, 0xb000, 10);
    Actor_FaceDirection(11, 0xd000, 0);
    Call3(Engine_ActorFaceDirection, 10, 0xb000, 0);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x1d70000, -1, 0x1350000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetPosition(8, 0x1d70000, 0x1220000);
    Audio_PlayCue(190);
    Engine_ActorSetSpritePriority(12, 2);
    for (cnt = 0; cnt != 90; cnt++) {
        *(s32 *)(record12 + 12) += -0x1999;
        *(s32 *)(record12 + 24) += 0x28f;
        *(s32 *)(record12 + 28) += 0x28f;
        *(s32 *)(record8 + 24) += 0x28f;
        *(s32 *)(record8 + 28) += 0x28f;
        Engine_EventWait(1);
    }
    *field_85 = 5;
    Engine_EventWait(80);
    for (cnt = 0; cnt != 60; cnt++) {
        *(s32 *)(record12 + 12) += -0x8000;
        Engine_EventWait(1);
    }
    *field_85 = 3;
    Engine_EventWait(30);
    *field_80_38 = 1;
    Engine_ActorSetPosition(8, 0, 0);
    Actor_SetSpritePriority(12, 1);
    {
        u8 *flags = (u8 *)Engine_ActorGet(12) + 35;
        cnt = 1;
        cnt |= *flags;
        *flags = cnt;
    }
    Engine_ActorSetChildValue(12, 0);
    Call3(Engine_ActorSetSpeed, 12, 0x8000, 0x4000);
    Actor_WalkToAndWait(12, 0x1d7, 0x132);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(12, 2);
    Event_SayThenWait(0x400c, 20);
    Engine_ActorFaceEachOther(5, 9, 0);
    Engine_EventWait(20);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_ActorRunRepeatedMotion(9, 2);
    Event_Wait(40);
    Engine_ActorStartRepeatedMotion(10, 1);
    Engine_ActorRunRepeatedMotion(11, 1);
    Event_Wait(20);
    Engine_ActorSetAnimationAndWait(10, 4);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 11, 0x5000, 10);
    Event_SayThenWait(10, 30);
    Engine_ActorSetAttachedEffect(12, 0x102);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(10);
    Event_SayThenWait(11, 30);
    Call3(Engine_ActorFaceDirection, 11, 0xd000, 30);
    Actor_SetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    Event_SayThenWait(11, 30);
    Engine_ActorSetAnimationAndWait(12, 3);
    Event_Wait(20);
    Call3(Engine_ActorFaceDirection, 11, 0x5000, 40);
    Call3(Engine_ActorFaceDirection, 9, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 5, 0x6000, 20);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call4(Engine_CameraMoveTo, 0x1080000, -1, 0x1cc0000, 0);
    Map_Redraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Event_Wait(40);
    Event_SayThenWait(10, 40);
    Engine_ActorStartRepeatedMotion(0, 3);
    Engine_ActorRunRepeatedMotion(1, 3);
    Engine_EventWait(80);
    Engine_EventOpenMessage(11, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        SkipMessage(1);
    }
    BattlePres_RunActionThenWaitIfModeZero(9, 0, 20);
    Event_CloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x1dd0000, -1, 0x14e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(10, 2);
    Actor_FaceDirection(10, 0xb000, 10);
    Engine_EventSetMessage((s32)MsgSoruWhyDenyDont);
    Event_SayThenWait(10, 20);
    Call3(Engine_ActorFaceDirection, 5, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 9, 0x3000, 10);
    Engine_ActorSetAnimation(9, 4);
    Event_SayThenWait(0x5009, 40);
    Engine_ActorRunRepeatedMotion(11, 1);
    Event_SayThenWait(11, 10);
    Engine_ActorStartRepeatedMotion(5, 2);
    Engine_ActorRunRepeatedMotion(9, 2);
}
