/* NONMATCHING: Japanese ticket-counter coin labels, 2026-10-01.
 * The complete opening callback compiles with approved TBS flags. Separate
 * whole enum arguments rematerialize the unit label and shorten the owner
 * by twelve bytes. Production updates one label selector between the two
 * catalog entries, preserving the saved register through the number draw.
 */
#include "TBS_EDITION.H"
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/KAREI.H"
#include "text/MSG_IDS.H"
TEXT_MESSAGE_ENUM(MsgYourCoins);
TEXT_MESSAGE_ENUM(MsgCoins);
#include "SCENE_IDS.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

extern const struct SceneEntrance gKareiTorebiEntrances1[];
extern const struct SceneEntrance gKareiTorebiEntrances2[];
extern const struct SceneEntrance gKareiTorebiEntrances3[];
extern const struct SceneEntrance gKareiTorebiEntrancesOther[];

extern const struct ScenePlacement gKareiTorebiPlacements1[];
extern const struct ScenePlacement gKareiTorebiPlacements1Flag93e[];
extern const struct ScenePlacement gKareiTorebiPlacements2[];
extern const struct ScenePlacement gKareiTorebiPlacements2Flag93e[];
extern const struct ScenePlacement gKareiTorebiPlacements2Flag950[];
extern const struct ScenePlacement gKareiTorebiPlacements3[];
extern const struct ScenePlacement gKareiTorebiPlacements3Flag950[];
extern const struct ScenePlacement gKareiTorebiPlacementsOther[];

extern const struct SceneEvent gKareiTorebiEvents1[];
extern const struct SceneEvent gKareiTorebiEvents1Flag93e[];
extern const struct SceneEvent gKareiTorebiEvents2[];
extern const struct SceneEvent gKareiTorebiEvents2Flag93e[];
extern const struct SceneEvent gKareiTorebiEvents2Flag950[];
extern const struct SceneEvent gKareiTorebiEvents3[];
extern const struct SceneEvent gKareiTorebiEvents3Flag950[];
extern const struct SceneEvent gKareiTorebiEventsOther[];

extern u8 MsgKareiFinishedOutHereBoardShip[];
extern u8 MsgKareiGoingTakeShip[];
extern u8 MsgKareiPassMessageDaughter[];
extern u8 MsgKareiWantGoToTolbi[];

extern u8 MsgKareiWantGoAshore[];
extern u8 MsgKareiTicketsPlease[];

extern u8 MsgKareiAfterHandingOverTicket[];
extern u8 MsgKareiBoardWaitSetSail[];
extern u8 MsgKareiHandInTicketPlank[];
extern u8 MsgKareiHaveTicketsGiveHim[];
extern u8 MsgKareiHearScaryTrip[];
extern u8 MsgKareiLookingForTickets[];
extern u8 MsgKareiSilkRoadBlocked[];
extern u8 MsgKareiThanksMessageParents[];
extern u8 MsgKareiTouristsLookUpset[];

extern const u16 KareiTorebi_LeaveCells1[];
extern const u16 KareiTorebi_LeaveCells3[];

void FieldScene_ConfigureFlaggedActors(void);

void FieldScene_PlaceSlots14And15(void);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

/* The sprite's attribute bytes as the scene scripts write them. */
struct SpriteBytes {
    u8 unknown_00[9];
    u8 priority;
    u8 unknown_0a[11];
    u8 second_priority;
    u8 unknown_16[8];
    u16 rotation;
    u8 unknown_20[6];
    u8 flags;
};

extern u8 KareiTorebi_ActorSixteenScript[];
extern u8 MsgKareiIncredibleOcean[];
extern u8 MsgKareiReturnedTicketCost[];

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_2000240;
    s32 six00;

    six00 = 0x258;
    Engine_EventBegin();
    if (GameFlag_IsSet(0x8a5) != 0) {
        Engine_EventSetMessage((s32)MsgKareiHandInTicketPlank);
        Event_ShowMessage(8, 0);
    } else {
        Engine_EventSetMessage((s32)MsgKareiLookingForTickets);
        Event_OpenMessage(8, 0);
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            Event_ShowMessageAndWait(8, 0, 10);
        } else {
            bump_step(1);
            UiWork_PushValueSlot(six00, 5);
            Event_OpenMessage(8, 0);
            rec7 = UiWindow_Create(TICKET_COUNTER_WINDOW_X, 8, TICKET_COUNTER_WIDTH, 4, 2);
            UiText_DrawCharacterAtOffset(MsgYourCoins, rec7, 0, 0);
            base6_2000240 = (s32)Data_02000240;
            UiText_DrawNumberInWindow(*(s32 *)(base6_2000240 + 16), 6, rec7, TICKET_COUNTER_X, 8);
#if defined(TBS_EDITION_JA)
            UiText_DrawCharacterAtOffset(MsgCoins, rec7, 48, 8);
#endif
            if (Engine_EventChooseYesNo(-1, 0) == 1) {
                UiWork_Finalize(rec7, 2);
                Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
                Engine_EventWait(10);
                Event_ShowMessage(8, 0);
                goto L_02000660;
            } else {
                if ((u32)six00 <= (u32)*(s32 *)(base6_2000240 + 16)) {
                    goto L_0200061e;
                }
                UiWork_Finalize(rec7, 2);
                Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
                Engine_EventWait(10);
                bump_step(1);
                Audio_PlayCue(113);
                Event_ShowMessage(8, 0);
                goto L_02000660;
            }
            L_0200061e:;
            UiWork_Finalize(rec7, 2);
            Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
            Engine_EventWait(10);
            bump_step(3);
            Event_ShowMessage(8, 0);
            Engine_PartyGiveItem(235, 0);
            GameFlag_Set(0x8a5);
            Party_AdjustSixDigitCounterA(-six00);
        }
        L_02000660:;
        Engine_EventEnd();
    }
}
