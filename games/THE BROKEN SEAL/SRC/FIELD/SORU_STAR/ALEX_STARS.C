#include "STAR.H"
#include "CALL.H"
extern u8 MsgSoruDontWantAnything[];
extern u8 MsgSoruDoubtHowFeel[];
extern u8 MsgSoruPermitRelieveElemental[];
extern u8 MsgSoruThankCooperation[];

/* FAKEMATCH: the flag byte's or-assign goes through this helper, which
 * keeps the reference's register for the byte. */
static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

void Scene_AlexTakesStars(void)
{
    u32 i;
    u8 *rec;
    s32 record;
    s32 none;

    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 0);
    record = Actor_Get(ACTOR_ALEX);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetChildValue(ACTOR_ALEX, 15);
    Actor_SetPosition(ACTOR_ALEX, 0x1880000, 0x1c60000);
    SoruStar_RiseActorFourteenSparks();
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 10);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 40);
    Actor_FaceDirection(ACTOR_ALEX, 0x5000, 10);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    Event_Wait(20);
    Event_SetMessage((s32)MsgSoruPermitRelieveElemental);
    Event_ShowMessage(ACTOR_ALEX, 0);
    Actor_SetPosition(ACTOR_SATUROS, 0x1d50000, 0x15c0000);
    Event_Wait(20);
    Event_SayThenWait(0x200a, 10);
    Event_SayThenWait(0x200a, 40);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(40);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x185, 0x1d4);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 60);
    Event_SayThenWait(1, 20);
    UiText_ShowCenteredMessage((s32)MsgSoruPermitRelieveElemental + 4, 1, 10);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    rec = Engine_ActorGet(ACTOR_GERALD);
    rec[90] &= 254;
    /* FAKEMATCH: the zero is parked here, well before its one store, which
     * keeps it in the register the reference holds it in. */
    none = 0;
    Actor_WalkToAndWait(ACTOR_GERALD, 0x178, 0x1d6);
    Event_Wait(30);
    SetFlagBits(&rec[90], 1);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_Wait(10);
    Event_SetMessage((s32)MsgSoruPermitRelieveElemental + 5);
    Event_SayThenWait(14, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Event_SayThenWait(14, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_ALEX, 0xc000, 20);
    Actor_SetChildValue(ACTOR_ALEX, 0x100);
    record = Actor_Get(ACTOR_ALEX);
    Actor_SetSpriteFlags(record, 0);
    rec = Engine_ActorGet(ACTOR_ALEX);
    rec[85] = none;
    Audio_PlayCue(220);
    for (i = 0; i != 30; i++) {
        *(s32 *)(rec + 12) += 0x10000;
        Event_Wait(1);
    }
    rec[85] = 5;
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Event_SayThenWait(1, 10);
    Actor_ShowEmote(ACTOR_ALEX, 0x101, 60);
    Actor_FaceDirection(ACTOR_ALEX, 0x5000, 10);
    Event_SayThenWait(1, 20);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 1);
    Event_SayThenWait(14, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
    Event_SayThenWait(1, 30);
    Actor_ShowEmote(ACTOR_ALEX, 0x105, 80);
    Actor_FaceDirection(ACTOR_ALEX, 0xd000, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 10);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0x1dd0000, -1, 0x14e0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Event_SayThenWait(10, 10);
    Event_OpenMessage(ACTOR_MENARDI, 0);
    Camera_SetSpeed(0x66666, 0xcccc);
    Camera_MoveTo(0x1760000, -1, 0x1d60000, 1);
    Camera_WaitForMove();
    Actor_FaceDirection(ACTOR_ALEX, 0x5000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    if (Event_ChooseYesNo(1, 0) != 0) {
        Event_Wait(10);
        Engine_ActorSetAnimationAndWait(14, 4);
        Event_SetMessage((s32)MsgSoruDontWantAnything);
        Event_OpenMessage(ACTOR_ALEX, 0);
        if (Event_ChooseYesNo(1, 0) == 0) {
            do {
                Event_Wait(20);
                Engine_ActorSetAnimationAndWait(14, 4);
                Event_Wait(10);
                Event_SetMessage((s32)MsgSoruDoubtHowFeel);
                Event_OpenMessage(ACTOR_ALEX, 0);
            } while (Event_ChooseYesNo(1, 0) == 0);
        }
    }
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Event_Wait(20);
    Event_SetMessage((s32)MsgSoruThankCooperation);
    Event_SayThenWait(14, 30);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Event_Wait(10);
    Event_SayThenWait(14, 30);
    rec[85] = 0;
    Actor_SetSpeed(ACTOR_ALEX, 0x26666, 0x13333);
    Call4(Object_SetMoveTarget, (s32)rec, 0x1cc0000, 0, 0x1680000);
    Actor_WaitForMove(ACTOR_ALEX);
    Actor_SetChildValue(ACTOR_ALEX, 0);
    record = Actor_Get(ACTOR_ALEX);
    Actor_SetSpriteFlags(record, 1);
    Event_Wait(30);
    Camera_FollowActor(ACTOR_GERALD, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_Wait(20);
    rec = Engine_ActorGet(ACTOR_GERALD);
    SetFlagBits(&rec[90], 1);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x156, 0x1d6);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x138, 0x1d6);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(30);
    Audio_PlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Actor_SetAnimation(ACTOR_GERALD, 7);
    Actor_MoveToAndWait(ACTOR_GERALD, 0x116, 0x1e0);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_Wait(30);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    {
        s32 slot = Engine_ActorGet(ACTOR_PARTY_LEADER);

        if (slot != 0) {
            Actor_SetDestination(ACTOR_GERALD, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
        }
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    PartyInventory_Discard(220);
    PartyInventory_Discard(221);
    PartyInventory_Discard(223);
}
