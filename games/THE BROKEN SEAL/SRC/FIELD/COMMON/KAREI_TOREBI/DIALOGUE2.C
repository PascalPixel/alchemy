#include "KAREI.H"

extern u8 MsgKareiAfterHandingOverTicket[];
extern u8 MsgKareiBoardWaitSetSail[];
extern u8 MsgKareiHandInTicketPlank[];
extern u8 MsgKareiHaveTicketsGiveHim[];
extern u8 MsgKareiHearScaryTrip[];
extern u8 MsgKareiLookingForTickets[];
extern u8 MsgKareiSilkRoadBlocked[];
extern u8 MsgKareiThanksMessageParents[];
extern u8 MsgKareiTouristsLookUpset[];

void FieldScene_RunActorThirteenFlagDialogue(void)
{
    Engine_EventBegin();

    if (GameFlag_IsSet(0x8A7) != 0) {
        Event_SetMessage((s32)MsgKareiBoardWaitSetSail);
        Engine_EventOpenMessage(13, 0);
    } else if (GameFlag_IsSet(0x8A5) != 0) {
        Event_SetMessage((s32)MsgKareiHaveTicketsGiveHim);
        Event_ShowMessage(13, 0);
    } else {
        Event_SetMessage((s32)MsgKareiAfterHandingOverTicket);
        Event_ShowMessage(13, 0);
    }

    Event_End();
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_2000240;
    s32 six00;

    six00 = 0x258;
    Event_Begin();
    if (GameFlag_IsSet(0x8a5) != 0) {
        Event_SetMessage((s32)MsgKareiHandInTicketPlank);
        Event_ShowMessage(8, 0);
    } else {
        Event_SetMessage((s32)MsgKareiLookingForTickets);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            Event_ShowMessageAndWait(8, 0, 10);
        } else {
            bump_step(1);
            UiWork_PushValueSlot(six00, 5);
            Event_OpenMessage(8, 0);
            rec7 = UiWindow_Create(19, 8, 11, 4, 2);
            UiText_DrawCharacterAtOffset(0xc8a, rec7, 0, 0);
            base6_2000240 = (s32)Data_02000240;
            UiText_DrawNumberInWindow(*(s32 *)(base6_2000240 + 16), 6, rec7, 24, 8);
            if (Event_ChooseYesNo(-1, 0) == 1) {
                UiWork_Finalize(rec7, 2);
                Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
                Event_Wait(10);
                Event_ShowMessage(8, 0);
                goto L_02000660;
            } else {
                if ((u32)six00 <= (u32)*(s32 *)(base6_2000240 + 16)) {
                    goto L_0200061e;
                }
                UiWork_Finalize(rec7, 2);
                Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
                Event_Wait(10);
                bump_step(1);
                Audio_PlayCue(113);
                Event_ShowMessage(8, 0);
                goto L_02000660;
            }
            L_0200061e:;
            UiWork_Finalize(rec7, 2);
            Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Event_Wait(10);
            bump_step(3);
            Event_ShowMessage(8, 0);
            Party_GiveItem(235, 0);
            GameFlag_Set(0x8a5);
            Party_AdjustSixDigitCounterA(-six00);
        }
        L_02000660:;
        Event_End();
    }
}

void KareiTorebi_TalkScaryTrip(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKareiHearScaryTrip);
    Event_AskYesNo(8, 0);
    Event_End();
}

void KareiTorebi_TalkSeasickTourists(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKareiTouristsLookUpset);
    Event_AskYesNo(10, 0);
    Event_End();
}

void FieldScene_RunScene3ae_020006c8(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x8a8) != 0) {
        Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Event_SetMessage((s32)MsgKareiThanksMessageParents);
        Event_ShowMessage(11, 0);
        ((void (*)())Engine_EventEnd)();
    } else {
        Event_Wait(20);
        Actor_ShowEmote(11, 0x100, 50);
        Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Event_SetMessage((s32)MsgKareiSilkRoadBlocked);
        Event_ShowMessage(11, 0);
        if (GameFlag_IsSet(0x8a6) != 0) {
            Event_Wait(20);
            Actor_ShowEmote(11, 0x102, 40);
            Event_OpenMessage(11, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                Event_Wait(20);
                Event_ShowMessage(11, 0);
                GameFlag_Set(0x8a8);
                goto L_020007be;
            }
            ((void (*)())Engine_EventWait)(10);
            bump_step(1);
            Event_ShowMessage(11, 0);
            Event_Wait(10);
            Actor_FaceDirection(11, 0, 0);
            Event_Wait(30);
        } else {
            Event_Wait(10);
            Actor_FaceDirection(11, 0, 0);
            Event_Wait(30);
        }
        L_020007be:;
        Event_End();
    }
}
