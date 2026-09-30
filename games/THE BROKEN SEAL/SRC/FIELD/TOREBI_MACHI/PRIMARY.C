#include "MACHI.H"
#include "CALL.H"
extern u8 MsgTorebiSeenAnyoneWho[];

void FieldScene_RunPrimarySequence(void)
{
    s32 base;

    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Call3(Engine_ActorSetSpeed, 29, 0x10000, 0x8000);
    Engine_ActorSetSpeed(30, 0x10000, 0x8000);
    base = (s32)MsgTorebiSeenAnyoneWho;
    Engine_EventSetMessage(base);
    Call3(Engine_ActorSetPosition, 29, 0x480000, 0xd00000);
    Call3(Engine_ActorSetPosition, 30, 0x380000, 0xd00000);
    Engine_ActorSetChildValue(32, 15);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(32), 0);
    Call3(Engine_ActorSetPosition, 32, 0x5f0000, 0x280000);
    Engine_ActorWalkTo(29, 72, 248);
    Engine_ActorWalkTo(30, 56, 248);
    Call3(Engine_ActorWalkToAndWait, 0, 64, 0x108);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Engine_ActorWaitForMove(29);
    Engine_ActorSetAnimation(29, 1);
    Engine_ActorSetAnimation(30, 1);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorFaceActor(29, 0, 0);
    Engine_ActorFaceActor(30, 0, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(29, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 30, 0x102);
    Engine_ActorStartRepeatedMotion(29, 2);
    Actor_RunRepeatedMotion(30, 2);
    Engine_EventWait(20);
    Event_OpenMessage(29, 0);
    Engine_EventWait(25);
    UiWindow_CreateWithSideObject(52, 0, 12, 7);
    UiText_OpenMessageWindow((base + 3), 11, 12, 2);
    SCENE_OBJECT_ID = 32;
    if (Engine_EventChooseYesNo(0, 0) == 0) { /* object_id 0, force 0 */
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(30, 2);
        Engine_EventWait(30);
        Engine_ActorFaceDirection(30, 0, 0);
        Event_Wait(30);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(29, 3);
        Event_Wait(20);
        Engine_ActorFaceDirection(29, 0, 0);
        Engine_EventWait(30);
        Engine_EventShowMessage(29, 0);
        Engine_EventWait(20);
        Call3(Engine_ActorFaceDirection, 29, 0x4000, 0);
        Engine_ActorFaceDirection(30, 0x4000, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimation(29, 3);
        Engine_ActorSetAnimationAndWait(30, 3);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 29, 0x1cccc, 0xe666);
        Call3(Engine_ActorSetSpeed, 30, 0x1cccc, 0xe666);
        Actor_WalkTo(29, 232, 248);
        Engine_EventWait(2);
        Actor_WalkTo(30, 232, 248);
        Engine_ActorWaitForMove(29);
        Engine_ActorWalkTo(29, 248, 248);
        Engine_ActorWalkToAndWait(30, 248, 248);
    } else {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(30, 2);
        Event_Wait(30);
        Engine_ActorFaceDirection(30, 0, 0);
        Event_Wait(30);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(29, 4);
        Engine_EventWait(20);
        Engine_ActorFaceDirection(29, 0, 0);
        Engine_EventWait(30);
        gEventWork->message += 1;
        Engine_EventShowMessage(29, 0);
        Engine_EventWait(20);
        Call3(Engine_ActorFaceDirection, 29, 0x4000, 0);
        Call3(Engine_ActorFaceDirection, 30, 0x4000, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimation(29, 3);
        Engine_ActorSetAnimationAndWait(30, 3);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 29, 0x19999, 0xcccc);
        Call3(Engine_ActorSetSpeed, 30, 0x19999, 0xcccc);
        Engine_ActorWalkTo(29, 72, 184);
        Engine_ActorWalkToAndWait(30, 56, 184);
    }
    Engine_ActorSetPosition(29, 0, 0);
    Engine_ActorSetPosition(30, 0, 0);
    Engine_ActorSetPosition(32, 0, 0);
    Engine_GameFlagSet(0x8c0);
    Engine_EventEnd();
}
