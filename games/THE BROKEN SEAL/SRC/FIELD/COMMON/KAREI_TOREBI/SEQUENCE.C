#include "KAREI.H"

extern u8 MsgKareiIncredibleOcean[];
extern u8 MsgKareiReturnedTicketCost[];

void SceneState_SetRuntimeWord448To521AndSend303(void)
{
    u8 *state = Data_03001ebc;
    s32 *slot = (s32 *)(state + 0x1C0);

    *slot = 0x209;
    GameFlag_Clear(0x12F);
}

void FieldScene_PlaceSlots14And15(void)
{
    s32 pos14;
    s32 pos15;

    pos14 = ((s32 *)Engine_ActorGet(14))[2] >> 20;   /* [r0,#8], asrs #20 */
    pos15 = ((s32 *)Engine_ActorGet(15))[2] >> 20;

    Map_CopyCellAttributes(5, 12, 5, 1, 5, 11);
    Map_CopyCellAttributes(1, 0, 1, 1, pos15, 11);
    Map_CopyCellAttributes(1, 0, 1, 1, pos14, 11);

    SceneActor_SetMode3AndFlagBit1(14);
    SceneActor_SetMode3AndFlagBit1(15);
}

void SceneActor_SetMode3AndFlagBit1(s32 no)
{
    u8 *p = ((u8 *)Engine_ActorGet(no));
    u8 *flag;

    Actor_SetSpriteFlags(((u8 *)Engine_ActorGet(no)), 0);
    Actor_SetSpritePriority(no, 3);
    flag = p + 85;
    *flag = 0;
    p += 35;
    {
        u8 bit = 2;

        *p = bit | *p;
    }
}

void FieldScene_RunScene3aeSequenceA(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Battle_ResetEffectCounter();
    Actor_SetPosition(8, 0x1480000, 0x5900000);
    /* Set the flag byte at +91 of record 8. */
    *(u8 *)(((s32)Engine_ActorGet(8)) + 91) = 1;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Call4(Motion_LaunchFromFocusedObject, 1, -16, 0, 0x8000);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Event_Wait(20);
    Event_SetMessage((s32)MsgKareiIncredibleOcean);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_GERALD, 232, 0x590);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Camera_MoveTo(0xb80000, -1, 0x5a00000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_Jump(ACTOR_GERALD, 6, 15);
    Actor_Jump(ACTOR_GERALD, 6, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(20);
    Camera_MoveTo(0x1080000, -1, 0x5a80000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_ShowEmote(8, 0x100, 50);
    Actor_SetSpeed(8, 0x13333, 0x9999);
    Actor_WalkToAndWait(8, 0x108, 0x590);
    Actor_FaceDirection(8, 0x8000, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(10);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 40);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Event_Wait(50);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 0);
    Event_Wait(30);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x108, 0x5b8);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(8, 0x4000, 0);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(30);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    /* If a record is returned, pass its s16 fields at +10 and +18 back in as
     * arguments. */
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    /* Same routine as above, called directly instead of through the Value
     * wrapper. */
    ((void (*)())Engine_EventWait)(20);
    /* Clear the flag byte at +91 of record 8. */
    *(u8 *)(((s32)Engine_ActorGet(8)) + 91) = 0;
    Value2(Engine_ActorEnableActionCallback, 8, 2);
    record = Value1(Engine_ActorGet, 8);
    /* Store the integer part of the 16.16 fixed-point fields at +8 and +16
     * into the halfwords at +100 and +102. */
    {
        s32 shown = *(s32 *)(record + 8) / 0x10000;

        *(u16 *)(record + 100) = shown;
    }
    {
        s32 shown = *(s32 *)(record + 16) / 0x10000;

        *(u16 *)(record + 102) = shown;
    }
    Event_End();
}

void FieldScene_RunScene3aeSequenceB(void)
{
    u32 i;
    u8 *record;
    s32 none;
    s32 v5;
    s32 v6;

    GameFlag_Set(0x8ab);
    Event_Begin();
    Battle_ResetEffectCounter();
    Event_SetMessage((s32)MsgKareiReturnedTicketCost);
    record = ((u8 *)Engine_ActorGet(11));
    none = 0;
    record[35] = none;
    *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(s32 *)((s32)record + 80) + 21) |= 12;
    Camera_MoveTo(0xe80000, -1, 0x1300000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 0x110);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_ShowEmote(13, 0x102, 50);
    Event_ShowMessage(13, 0);
    Event_Wait(10);
    Actor_ShowEmote(10, 0x107, 50);
    Actor_FaceDirection(10, 0, 0);
    Actor_Jump(10, 4, 13);
    Actor_Jump(10, 4, 30);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(11, 2);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(13, 4);
    Event_Wait(20);
    Call2((void (*)())Engine_EventShowMessage, 13, 0);
    Event_Wait(10);
    Actor_ShowEmote(10, 0x103, 55);
    Actor_SetSpeed(10, 0x20000, 0x10000);
    Actor_WalkByAndWait(10, 16, 0);
    Actor_Jump(10, 7, 0);
    v5 = 254;
    Actor_WalkByAndWait(10, 24, 0);
    *(u8 *)(((s32)Engine_ActorGet(10)) + 90) &= v5;
    Actor_WalkBy(10, -16, 0);
    Audio_PlayCue(153);
    Actor_SetSpeed(13, 0x26666, 0x13333);
    Actor_WalkByAndWait(13, 16, 0);
    Event_Wait(10);
    v6 = 1;
    Actor_SetAnimation(10, 1);
    *(u8 *)(((s32)Engine_ActorGet(10)) + 90) |= v6;
    Actor_SetAttachedEffect(13, 0x102);
    Actor_StartRepeatedMotion(13, 2);
    Audio_PlayCue(155);
    Call1((void (*)())Engine_EventWait, 10);
    Audio_PlayCue(155);
    Event_Wait(10);
    Audio_PlayCue(155);
    Event_Wait(10);
    Event_Wait(20);
    Actor_SetSpeed(13, 0x6666, 0x3333);
    Actor_Jump(13, 6, 0);
    Audio_PlayCue(159);
    Actor_WalkByAndWait(13, -8, 0);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_ShowEmote(13, 0x102, 70);
    Actor_SetSpeed(16, 0x10000, 0x8000);
    Actor_WalkByAndWait(16, -8, 0);
    Actor_FaceDirection(16, 0x5000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(20);
    Event_ShowMessage(16, 0);
    Event_Wait(10);
    Actor_FaceDirection(10, 0xe000, 0);
    Event_Wait(35);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_FaceDirection(16, 0x2000, 0);
    Event_Wait(55);
    Actor_FaceDirection(16, 0x5000, 0);
    Event_Wait(30);
    Event_ShowMessage(16, 0);
    Event_Wait(10);
    Actor_FaceDirection(11, 0xe000, 0);
    Event_Wait(20);
    Actor_ShowEmote(11, 0x102, 50);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(13, 2);
    Event_Wait(20);
    Event_ShowMessage(13, 0);
    Event_Wait(10);
    Actor_FaceDirection(13, 0xa000, 0);
    Event_Wait(60);
    Actor_FaceDirection(13, 0x8000, 0);
    Event_Wait(30);
    Event_ShowMessage(13, 0);
    Event_Wait(10);
    Actor_FaceDirection(10, 0, 0);
    Actor_FaceDirection(11, 0, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Actor_SetSpeed(10, 0x13333, 0x9999);
    Actor_WalkByAndWait(10, 8, 0);
    Actor_SetSpeed(16, 0x20000, 0x10000);
    Actor_WalkByAndWait(16, -8, 16);
    Actor_FaceDirection(16, 0x8000, 0);
    Event_ShowMessage(16, 0);
    Event_Wait(10);
    Actor_ShowEmote(10, 0x102, 50);
    *(u8 *)(((s32)Engine_ActorGet(10)) + 90) &= v5;
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkByAndWait(10, -8, 0);
    *(u8 *)(((s32)Engine_ActorGet(10)) + 90) |= v6;
    Event_Wait(20);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(20);
    Event_ShowMessage(16, 0);
    Event_Wait(10);
    *(u8 *)(((s32)Engine_ActorGet(10)) + 90) &= v5;
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkByAndWait(10, -16, 0);
    *(u8 *)(((s32)Engine_ActorGet(10)) + 90) |= v6;
    Actor_FaceDirection(10, 0, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(10, 4);
    Event_Wait(20);
    Actor_SetSpeed(10, 0x1cccc, 0xe666);
    Actor_WalkByAndWait(10, 8, 0);
    Actor_Jump(10, 6, 0);
    Actor_WalkByAndWait(10, 24, 0);
    Audio_PlayCue(133);
    Actor_Jump(16, 6, 0);
    Actor_EnableActionCallback(16, 0x20096e4);
    *(u8 *)(((s32)Engine_ActorGet(10)) + 90) &= v5;
    Actor_Jump(10, 6, 0);
    Actor_WalkByAndWait(10, -12, 4);
    record = Value1(Engine_ActorGet, 10);
    record[89] = none;
    record[35] = 2;
    *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(s32 *)((s32)record + 80) + 38) = none;
    {
        s32 target = *(s32 *)((s32)record + 80);
        s32 shown = 0xc000;

        *(u16 *)(target + 30) = shown;
    }
    Actor_WalkByAndWait(10, -12, 4);
    Actor_FaceDirection(10, 0x4000, 0);
    {
        u8 *flags = ((u8 *)Engine_ActorGet(10)) + 90;
        u8 value = *flags | v6;

        *flags = value;
    }
    Audio_PlayCue(159);
    Event_Wait(20);
    Actor_ShowEmote(11, 0x102, 50);
    Actor_SetSpeed(11, 0x18000, 0xc000);
    Actor_WalkByAndWait(11, 24, 0);
    Actor_FaceDirection(11, 0xc000, 0);
    Event_Wait(10);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(20);
    Event_ShowMessage(16, 0);
    Event_Wait(20);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkByAndWait(16, -8, 0);
    Event_Wait(20);
    Event_ShowMessage(16, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_Wait(10);
    Actor_FaceDirection(16, 0, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(10);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(20);
    Event_Wait(10);
    Actor_SetSpeed(16, 0x10000, 0x8000);
    Actor_WalkByAndWait(16, 24, -24);
    Actor_WalkByAndWait(16, 8, 0);
    Actor_FaceDirection(16, 0xe000, 0);
    Event_Wait(20);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkByAndWait(13, 0, -8);
    Event_Wait(10);
    Event_End();
}
