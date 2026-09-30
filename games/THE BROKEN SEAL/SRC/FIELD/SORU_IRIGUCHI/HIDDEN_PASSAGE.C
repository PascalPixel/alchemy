/* Sukureta suspects a hidden passage at the sanctum entrance, and the party
 * decides whether to split up and search. */
#include "SORU.H"
#include "CALL.H"
extern u8 MsgSoruCantSeriouslyWant[];
extern u8 MsgSoruDangerousSplitStay[];
extern u8 MsgSoruWrongSukureta[];

void Scene_SukuretaSuspectsHiddenPassage(void)
{
    u8 *record;

    Event_Begin();
    Event_OpenScreen();
    Event_WaitForScreen();

    record = (u8 *)((s32 (*)())Engine_ActorGet)(0);
    if (record != 0)
        Engine_ActorSetPosition(8, RECORD_A32(record), RECORD_B32(record));
    record = (u8 *)((s32 (*)())Engine_ActorGet)(0);
    if (record != 0)
        Engine_ActorSetPosition(5, RECORD_A32(record), RECORD_B32(record));
    record = (u8 *)((s32 (*)())Engine_ActorGet)(0);
    if (record != 0)
        Engine_ActorSetPosition(1, RECORD_A32(record), RECORD_B32(record));

    Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    Actor_SetDestinationOffset(ACTOR_GERALD, -16, 0);
    Actor_SetDestinationOffset(ACTOR_JASMINE, 16, 0);
    Value3(Engine_ActorSetDestinationOffset, 8, 0, -16);
    Engine_ActorWaitForMove(8);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(ACTOR_GERALD, 0);
    Actor_SetAnimation(ACTOR_JASMINE, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 30);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 30);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 30);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(10);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    Engine_ActorSetDestinationOffset(8, 0, -16);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_EventWait(6);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    Engine_ActorSetDestinationOffset(8, 0, -32);
    Engine_ActorWaitForMove(8);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);

    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x06310000, -1, 0x00960000, 1);
    Camera_WaitForMove();
    Engine_EventWait(10);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0x06550000, -1, 0x00640000, 1);
    Camera_WaitForMove();
    Camera_MoveTo(0x06b60000, -1, 0x00640000, 1);
    Camera_WaitForMove();
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    Value4(Engine_CameraMoveTo, 0x06d80000, -1, 0x00960000, 1);
    Camera_WaitForMove();
    Engine_EventWait(40);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x06840000, -1, 0x01000000, 1);
    Camera_WaitForMove();
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(10);

    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 20);
    Event_SetMessage((s32)MsgSoruWrongSukureta);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 60);
    Actor_StartRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Value2(Engine_ActorSetAttachedEffect, 5, 0x102);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_SUKURETA, 0);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Engine_ActorFaceEachOther(0, 5, 0);
    Engine_EventWait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(5, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 20);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 20);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(10);
    Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 20);
    Actor_Jump(ACTOR_SUKURETA, 2, 20);

    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 40);
    Actor_FaceDirection(ACTOR_GERALD, 0, 20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Engine_EventSetMessage((s32)MsgSoruDangerousSplitStay);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Engine_EventShowMessageAndWait(1, 0, 10);
    } else {
        Engine_EventSetMessage((s32)MsgSoruCantSeriouslyWant);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 20);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 40);
        Engine_ActorRunRepeatedMotion(1, 1);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(1, 3);
        Engine_EventWait(10);
        Actor_FaceDirection(ACTOR_GERALD, 0, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 30);
        Engine_ActorRunRepeatedMotion(1, 1);
        Engine_EventWait(10);
        Engine_EventShowMessageAndWait(1, 0, 10);
    }

    Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    Engine_ActorSetDestinationOffset(8, 0, 48);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_EventWait(6);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Engine_ActorFaceDirection(5, 0xa000, 0);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetAnimation(8, 1);
    Engine_EventWait(20);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_JASMINE, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(1, 2);

    record = (u8 *)((s32 (*)())Engine_ActorGet)(0);
    if (record != 0)
        Engine_ActorSetDestination(1, RECORD_A16(record), RECORD_B16(record));
    Engine_ActorSetAnimation(5, 2);
    record = (u8 *)((s32 (*)())Engine_ActorGet)(0);
    if (record != 0)
        Engine_ActorSetDestination(5, RECORD_A16(record), RECORD_B16(record));
    Engine_ActorSetAnimation(8, 2);
    record = (u8 *)((s32 (*)())Engine_ActorGet)(0);
    if (record != 0)
        Engine_ActorSetDestination(8, RECORD_A16(record), RECORD_B16(record));

    Engine_ActorWaitForMove(8);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Engine_ActorSetAnimation(5, 1);
    GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
    Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
    Event_End();
}

