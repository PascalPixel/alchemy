#include "SUKURETA.H"
extern u8 MsgHaidiaFineIfTheyDontSee[];
extern u8 MsgHaidiaGeraldIllTakeOverIf[];
extern u8 MsgHaidiaGeraldYouCanHandleThe[];
extern u8 MsgHaidiaHearAwfulGrowls[];
extern u8 MsgHaidiaIllClimbTheFenceSomeday[];
extern u8 MsgHaidiaJasmineOurSecret[];
extern u8 MsgHaidiaJasmineTheyMightBeThieves[];
extern u8 MsgHaidiaSukuretaIWaitedYearsFor[];
extern u8 MsgHaidiaSukuretaOhRobin[];
extern u8 MsgHaidiaSukuretaWeOnlyCheckMt[];
extern u8 MsgHaidiaSukuretaWeOnlyCheckThe[];
extern u8 MsgHaidiaYouCannotEnterMtAleph[];

void Scene_RunActorTwelveDialogue(void)
{
    s32 base;

    Event_Begin();
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Event_SetMessage((s32)MsgHaidiaIllClimbTheFenceSomeday);
        Event_ShowMessage(12, 0);
    } else {
        base = (s32)MsgHaidiaHearAwfulGrowls;
        Event_SetMessage(base);
        Actor_FaceActor(12, ACTOR_PARTY_LEADER, 10);
        Actor_RunRepeatedMotion(12, 2);
        Event_Wait(6);
        Event_OpenMessage(12, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage(base + 1);
        } else {
            Event_SetMessage(base + 2);
        }
        Actor_StartRepeatedMotion(12, 3);
        Event_ShowMessage(12, 0);
        Scene_SetActorDirection(12, 49152, 10);
    }
    Event_End();
}

void Scene_PlanSanctumVisit(void)
{
    u8 *record;
    s32 x, y;
    s32 sneak;

    if (GameFlag_IsSet(FLAG_SANCTUM_VISIT_PLANNED) == 0) {
        Event_Begin();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        Event_SetMessage((s32)MsgHaidiaSukuretaOhRobin);
        Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 0x108);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_SUKURETA, 20);
        Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        record = (u8 *)Engine_ActorGet(0);
        x = *(s16 *)(record + 10);
        y = *(s16 *)(record + 18);
        Actor_SetPosition(ACTOR_JASMINE, x << 16, y << 16);
        Actor_SetPosition(ACTOR_GERALD, x << 16, y << 16);
        Actor_SetSpeed(ACTOR_JASMINE, 0x8000, 0x4000);
        Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
        Actor_WalkTo(ACTOR_JASMINE, 248, 0x108);
        Actor_WalkToAndWait(ACTOR_GERALD, 216, 0x108);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
        Actor_SetAnimation(ACTOR_JASMINE, 1);
        Actor_SetAnimation(ACTOR_GERALD, 1);
        Event_Wait(4);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
        Actor_SetAnimationAndWait(ACTOR_JASMINE, 4);
        Event_Wait(10);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
        Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
        Event_Wait(10);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        Actor_FaceDirection(ACTOR_GERALD, 0x3000, 40);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 10);
        Actor_SetAnimation(ACTOR_SUKURETA, 3);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 8);
        Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 20);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
        Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
        Actor_ShowEmote(ACTOR_JASMINE, 0x101, 60);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 20);
        Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 60);
        Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        Actor_FaceDirection(ACTOR_GERALD, 0, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 20);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 60);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 0);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 20);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 40);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 40);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 20);
        Event_OpenMessage(ACTOR_SUKURETA, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage((s32)MsgHaidiaYouCannotEnterMtAleph);
        } else {
            Event_SetMessage((s32)MsgHaidiaSukuretaIWaitedYearsFor);
        }
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        Event_SetMessage((s32)MsgHaidiaJasmineTheyMightBeThieves);
        Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 10);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 6);
        Actor_ShowEmote(ACTOR_GERALD, 0x103, 30);
        Actor_Jump(ACTOR_GERALD, 4, 30);
        Actor_FaceDirection(ACTOR_GERALD, 0, 10);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 6);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 10);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JASMINE, 0);
        Actor_FaceActor(ACTOR_SUKURETA, ACTOR_GERALD, 10);
        Actor_FaceActor(ACTOR_SUKURETA, ACTOR_JASMINE, 10);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
        Event_Wait(10);
        Actor_SetAnimation(ACTOR_JASMINE, 1);
        Actor_SetAnimation(ACTOR_GERALD, 1);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0x4000, 16);
        Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
        Actor_RunRepeatedMotion(ACTOR_SUKURETA, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
        Actor_ShowEmote(ACTOR_JASMINE, 0x100, 40);
        Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
        Event_Wait(10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
        Actor_RunRepeatedMotion(ACTOR_SUKURETA, 1);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 10);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 6);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
        Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
        Event_Wait(6);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
        Actor_Jump(ACTOR_GERALD, 2, 0);
        Actor_Jump(ACTOR_JASMINE, 2, 10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 6);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 10);
        Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
        Event_Wait(16);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JASMINE, 40);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x105, 0);
        Actor_ShowEmote(ACTOR_JASMINE, 0x105, 80);
        Actor_SetAnimationAndWait(ACTOR_SUKURETA, 4);
        Event_OpenMessage(ACTOR_SUKURETA, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage((s32)MsgHaidiaSukuretaWeOnlyCheckThe);
        } else {
            Event_SetMessage((s32)MsgHaidiaSukuretaWeOnlyCheckMt);
        }
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 20);
        sneak = (s32)MsgHaidiaFineIfTheyDontSee;
        Event_SetMessage(sneak);
        Actor_FaceDirection(ACTOR_GERALD, 0, 10);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_OpenMessage(ACTOR_GERALD, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage((sneak + 1));
        } else {
            Event_SetMessage((sneak + 2));
        }
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 6);
        Event_SetMessage((s32)MsgHaidiaJasmineOurSecret);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 10);
        Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
        Event_OpenMessage(ACTOR_JASMINE, 0);
        Event_Wait(4);
        if (Event_ChooseYesNo(0, 0) == 1) {
            Actor_Jump(ACTOR_JASMINE, 2, 20);
            Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        } else {
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
            Actor_SetAnimation(ACTOR_GERALD, 3);
            Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
            Event_Wait(8);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
            bump_step(1);
        }
        Actor_SetAnimationAndWait(ACTOR_SUKURETA, 3);
        Event_Wait(10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 10);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimationAndWait(ACTOR_JASMINE, 3);
        Event_Wait(10);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
        Event_OpenMessage(ACTOR_SUKURETA, 0);
        Event_Wait(4);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage((s32)MsgHaidiaGeraldYouCanHandleThe);
        } else {
            Event_SetMessage((s32)MsgHaidiaGeraldIllTakeOverIf);
        }
        Engine_EventWait(10);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Actor_FaceDirection(ACTOR_GERALD, 0, 10);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 6);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 10);
        Actor_SetAnimationAndWait(ACTOR_JASMINE, 4);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 6);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Actor_FaceDirection(ACTOR_GERALD, 0x8000, 10);
        Actor_ShowEmote(ACTOR_GERALD, 0x103, 30);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Actor_ShowEmote(ACTOR_SUKURETA, 0x100, 40);
        Actor_Jump(ACTOR_SUKURETA, 4, 40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 20);
        Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 10);
        Audio_PlayCue(158);
        Map_AnimateCells(Sukureta_GateCells, 43, 8);
        Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
        Actor_WalkToAndWait(ACTOR_SUKURETA, 232, 218);
        Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
        Actor_ShowEmote(ACTOR_JASMINE, 0x101, 60);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
        Event_CloseScreen();
        Event_WaitForScreen();
        Event_RequestExit(13);
        Event_End();
    }
}
