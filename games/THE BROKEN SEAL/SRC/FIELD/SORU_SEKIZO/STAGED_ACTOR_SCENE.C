#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
extern u8 MsgSoruHmphWellTold[];
extern u8 MsgSoruHonestlyDoubtUnderstand[];
extern u8 MsgSoruTryFindSolution[];
extern u8 MsgSoruWait[];

/* Staged scene for actors 8, 5, 1 and 0. The shared work pointer is fetched
 * again at the tail after the intervening calls. Runtime veneer bindings
 * belong to this module's translation-unit declaration. */

void Event_SayThenWait();
void ObjectMotion_ResetAndSetPositionInMode2();
s32 Inventory_PromptAndSetObjectMode();
s32 UiText_OpenMessageAtObject();

void FieldScene_RunStagedActorScene(void)
{
    u8 *work;
    s32 record;

    Engine_EventBegin();
    work = (u8 *)gEventWork;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 32;
    Event_OpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorSetPosition(8, 0x2400000, 0x1280000);
    Engine_EventWait(1);
    Engine_EventSetMessage((s32)MsgSoruWait);
    Event_SayThenWait(8, 6);
    Camera_SetSpeed(0xcccc, 0x1999);
    Call4(Engine_CameraMoveTo, 0x23e0000, -1, 0xb40000, 1);
    Call3(Engine_ActorSetSpeed, 8, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 8, 0x240, 216);
    Engine_EventWait(20);
    Engine_ActorJump(5, 2, 0);
    Engine_EventWait(30);
    Event_SayThenWait(5, 6);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(6);
    Engine_ActorFaceDirection(8, 0x9000, 0);
    Engine_EventWait(10);
    Engine_CameraSetSpeed(0x59999, 0xb333);
    Camera_MoveTo(0x11f0000, -1, 0xb00000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(60);
    Call4(Engine_CameraMoveTo, 0x23e0000, -1, 0xb40000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(10);
    Actor_FaceDirection(8, 0xc000, 0);
    Event_Wait(10);
    Engine_ActorJump(8, 6, 0);
    Call3(Engine_ActorSetSpeed, 8, 0x30000, 0x20000);
    Call3(Engine_ActorWalkToAndWait, 8, 0x240, 184);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_SayThenWait(8, 6);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(5, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 3);
    Event_SayThenWait(8, 6);
    Engine_ActorJump(0, 2, 0);
    Engine_ActorJump(1, 2, 0);
    Engine_ActorJump(5, 2, 0);
    Event_Wait(30);
    Engine_ActorRunRepeatedMotion(1, 2);
    Event_SayThenWait(1, 6);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_SayThenWait(8, 6);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JASMINE, 0);
    Engine_EventWait(40);
    Engine_ActorFaceActor(8, 0, 0);
    Actor_FaceActor(8, ACTOR_JASMINE, 0);
    Engine_EventWait(40);
    Actor_Jump(8, 6, 0);
    Call3(Engine_ActorWalkToAndWait, 8, 0x240, 216);
    Call3(Engine_ActorFaceDirection, 8, 0x8000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 8, 0x13333, 0x9999);
    ObjectMotion_ResetAndSetPositionInMode2(8, 0x1b0, 200);
    Engine_EventWait(20);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Call4(Engine_CameraMoveTo, 0x1200000, -1, 0xab0000, 1);
    Engine_CameraWaitForMove();
    Event_Wait(80);
    Engine_ActorSetAnimation(8, 1);
    Call4(Engine_CameraMoveTo, 0x23e0000, -1, 0xb40000, 1);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(8, 0, 0);
    Engine_EventWait(30);
    Call3(Engine_ActorWalkToAndWait, 8, 0x240, 216);
    Call3(Engine_ActorFaceDirection, 8, 0xc000, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 5, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Engine_CameraMoveTo(0x23e0000, -1, 0xab0000, 1);
    Engine_ActorJump(8, 6, 0);
    Call3(Engine_ActorSetSpeed, 8, 0x30000, 0x20000);
    Engine_ActorWalkToAndWait(8, 0x240, 184);
    Engine_EventWait(80);
    Event_SayThenWait(8, 6);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_SayThenWait(8, 20);
    Engine_ActorShowEmote(5, 0x102, 0);
    Engine_EventWait(40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_SayThenWait(5, 6);
    Actor_SetAnimationAndWait(8, 3);
    Engine_ActorFaceDirection(8, 0x8000, 0);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(8, 0xc000, 0);
    Event_Wait(30);
    Event_SayThenWait(8, 6);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_SayThenWait(8, 6);
    Engine_ActorFaceEachOther(1, 0, 0);
    Engine_EventWait(40);
    Actor_FaceEachOther(ACTOR_JASMINE, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(40);
    UiText_OpenMessageAtObject(8, 0);
    Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 5, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x4000, 0);
    if (Inventory_PromptAndSetObjectMode(0, 0) == 0) {
        Engine_EventSetMessage((s32)MsgSoruHonestlyDoubtUnderstand);
    } else {
        Engine_EventSetMessage((s32)MsgSoruHmphWellTold);
    }
    Event_SayThenWait(8, 6);
    Engine_EventSetMessage((s32)MsgSoruTryFindSolution);
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_SayThenWait(8, 6);
    Engine_ActorShowEmote(1, 0x102, 0);
    Engine_EventWait(60);
    Event_SayThenWait(1, 6);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_SayThenWait(8, 6);
    Engine_ActorFaceDirection(8, 0x4000, 0);
    Engine_EventWait(20);
    Engine_ActorJump(8, 6, 0);
    Call3(Engine_ActorWalkToAndWait, 8, 0x240, 216);
    Engine_EventWait(40);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Call4(Engine_CameraMoveTo, 0x23e0000, -1, 0xbf0000, 1);
    Call3(Engine_ActorSetSpeed, 8, 0x10000, 0x8000);
    Engine_ActorWalkToAndWait(8, 0x240, 232);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    Event_SayThenWait(8, 6);
    Engine_ActorFaceDirection(8, 0xc000, 0);
    Engine_EventWait(30);
    Event_SayThenWait(8, 6);
    Engine_ActorSetAnimationAndWait(8, 3);
    Call4(Engine_CameraMoveTo, 0x2400000, -1, 0xd70000, 1);
    Call3(Engine_ActorWalkToAndWait, 8, 0x23e, 0x143);
    Engine_ActorSetPosition(8, 0, 0);
    Camera_SetSpeed(0x39999, 0x7333);
    Engine_CameraMoveTo(0x2400000, -1, 0x880000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorFaceDirection(5, 0, 0);
    Engine_EventWait(10);
    Event_SayThenWait(5, 6);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimationAndWait(5, 3);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 5, 0x10000, 0x8000);
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    record = Engine_ActorGet(0);
    if (record != 0) {
        Engine_ActorSetDestination(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(5);
    Engine_ActorSetPosition(5, 0, 0);
    Engine_ActorSetAnimation(1, 2);
    record = Engine_ActorGet(0);
    if (record != 0) {
        Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(1, 0, 0);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    work = (u8 *)gEventWork;
    *(s32 *)(work + 0x1c0) = 0x204;
    *(s32 *)(work + 0x1c8) = 16;
    Engine_EventEnd();
}
