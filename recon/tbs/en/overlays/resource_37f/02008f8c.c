/* Draft of resource_37f 0x, from
 * games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/SCENARIO_DISPATCH.C.
 * Remaining difference: the ROM loads scene or message numbers from the
 * literal pool, as link-time symbols would; C builds those constants
 * with movs. The Value_/Data_ spellings below are the old address-named
 * forms. The listing keeps these rows. */
#include "SORU.H"

void Scene_SukuretaSuspectsHiddenPassage(void)
{
    u8 *record;

    Event_Begin();
    Event_OpenScreen(); /* main:0808a360 */
    Event_WaitForScreen(); /* main:0808a370 */

    record = Value1(Func_02002b6a, 0);
    if (record != 0)
        Value3(Engine_ActorSetPosition, 8, RECORD_A32(record), RECORD_B32(record));
    record = Value1(Func_02002b7e, 0);
    if (record != 0)
        Value3(Engine_ActorSetPosition, 5, RECORD_A32(record), RECORD_B32(record));
    record = Value1(Func_02002b92, 0);
    if (record != 0)
        Value3(Engine_ActorSetPosition, 1, RECORD_A32(record), RECORD_B32(record));

    Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    Actor_SetAnimation(ACTOR_JASMINE, 2);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    Actor_SetDestinationOffset(ACTOR_GERALD, -16, 0);
    Actor_SetDestinationOffset(ACTOR_JASMINE, 16, 0);
    Value3(Engine_ActorSetDestinationOffset, 8, 0, -16);
    Value1(Engine_ActorWaitForMove, 8);
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
    Value2(Engine_ActorRunRepeatedMotion, 8, 2);
    Value1(Engine_EventWait, 10);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    Value3(Engine_ActorSetDestinationOffset, 8, 0, -16);
    Value1(Engine_ActorWaitForMove, 8);
    Value2(Engine_ActorSetAnimation, 8, 1);
    Value1(Engine_EventWait, 6);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 40);
    Value2(Engine_ActorRunRepeatedMotion, 8, 2);
    Value1(Engine_EventWait, 20);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    Value3(Engine_ActorSetDestinationOffset, 8, 0, -32);
    Value1(Engine_ActorWaitForMove, 8);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);

    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x06310000, -1, 0x00960000, 1);
    Camera_WaitForMove();
    Value1(Engine_EventWait, 10);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0x06550000, -1, 0x00640000, 1);
    Camera_WaitForMove();
    Camera_MoveTo(0x06b60000, -1, 0x00640000, 1);
    Camera_WaitForMove();
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    Value4(Engine_CameraMoveTo, 0x06d80000, -1, 0x00960000, 1);
    Camera_WaitForMove();
    Value1(Engine_EventWait, 40);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x06840000, -1, 0x01000000, 1);
    Camera_WaitForMove();
    Value2(Engine_ActorSetAnimationAndWait, 8, 3);
    Value1(Engine_EventWait, 10);

    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 20);
    Event_SetMessage(MSG_GERALD_WHATS_WRONG_SUKURETA);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 60);
    Actor_StartRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102); /* main:0808a1f0 */
    Value2(Engine_ActorSetAttachedEffect, 5, 0x102); /* main:0808a1f0 */
    Value1(Engine_EventWait, 40);
    Value2(Engine_ActorRunRepeatedMotion, 8, 2);
    Value1(Engine_EventWait, 20);
    Event_ShowMessage(ACTOR_SUKURETA, 0);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Value3(Engine_ActorFaceEachOther, 0, 5, 0);
    Value1(Engine_EventWait, 40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Value2(Engine_ActorRunRepeatedMotion, 5, 1);
    Value1(Engine_EventWait, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 20);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 40);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 20);
    Value2(Engine_ActorRunRepeatedMotion, 8, 1);
    Value1(Engine_EventWait, 10);
    Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102); /* main:0808a1f0 */
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0, 20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 60);
    Value2(Engine_ActorRunRepeatedMotion, 8, 2);
    Value1(Engine_EventWait, 10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x4000, 20);
    Actor_Jump(ACTOR_SUKURETA, 2, 20);

    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 40);
    Actor_FaceDirection(ACTOR_GERALD, 0, 20);
    Event_OpenMessage(ACTOR_GERALD, 0); /* main:0808a178 */
    if (Event_ChooseYesNo(0, 0) == 0) {
        /* Passes the address of Value_00000fe0 in place of a record pointer. */
        Value1(Engine_EventSetMessage, (s32)&Value_00000fe0);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Value3(Engine_EventShowMessageAndWait, 1, 0, 10);
    } else {
        Value1(Engine_EventSetMessage, 0xfe1);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 20);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xa000, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 40);
        Value2(Engine_ActorRunRepeatedMotion, 1, 1);
        Value1(Engine_EventWait, 10);
        Value2(Engine_ActorSetAnimationAndWait, 1, 3);
        Value1(Engine_EventWait, 10);
        Actor_FaceDirection(ACTOR_GERALD, 0, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 30);
        Value2(Engine_ActorRunRepeatedMotion, 1, 1);
        Value1(Engine_EventWait, 10);
        Value3(Engine_EventShowMessageAndWait, 1, 0, 10);
    }

    Actor_SetSpeed(ACTOR_SUKURETA, 0x9999, 0x4ccc);
    Actor_SetAnimation(ACTOR_SUKURETA, 2);
    Value3(Engine_ActorSetDestinationOffset, 8, 0, 48);
    Value1(Engine_ActorWaitForMove, 8);
    Value2(Engine_ActorSetAnimation, 8, 1);
    Value1(Engine_EventWait, 6);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Value3(Engine_ActorFaceDirection, 5, 0xa000, 0);
    Value1(Engine_ActorWaitForMove, 8);
    Value2(Engine_ActorSetAnimation, 8, 1);
    Value1(Engine_EventWait, 20);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_JASMINE, 3);
    Value2(Engine_ActorSetAnimationAndWait, 0, 3);
    Value1(Engine_EventWait, 6);
    Value2(Engine_ActorSetAnimation, 1, 2);

    record = Value1(Func_0200312a, 0);
    if (record != 0)
        Value3(Engine_ActorSetDestination, 1, RECORD_A16(record), RECORD_B16(record));
    Value2(Engine_ActorSetAnimation, 5, 2);
    record = Value1(Func_0200314a, 0);
    if (record != 0)
        Value3(Engine_ActorSetDestination, 5, RECORD_A16(record), RECORD_B16(record));
    Value2(Engine_ActorSetAnimation, 8, 2);
    record = Value1(Func_0200316a, 0);
    if (record != 0)
        Value3(Engine_ActorSetDestination, 8, RECORD_A16(record), RECORD_B16(record));

    Value1(Engine_ActorWaitForMove, 8);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Actor_SetAnimation(ACTOR_SUKURETA, 1);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Value2(Engine_ActorSetAnimation, 5, 1);
    GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
    Value1(Engine_GameFlagClear, FLAG_ARRIVAL_EVENT_PENDING);
    Event_End();
}

