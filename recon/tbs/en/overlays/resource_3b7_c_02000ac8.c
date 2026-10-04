/* NONMATCHING: the spring game, 680 bytes; alchemy diff 111 (was 157).
 * 2026-10-04 sun w3 finisher 1: the prompt is a goto loop (prompt: ... goto
 * prompt), not a for (;;). That is what places the A-button block where the
 * English reference has it: loop.c lifts that exit block out of the key loop to
 * the nearest barrier before its target at the outer level, and only a goto loop
 * leaves the prompt's back-jump barrier outside any loop (a for (;;) keeps it
 * inside the loop notes, so the block went after the choice-0 jump instead). With
 * the goto loop, choice/message take r10/r9 and coins/tickets r8/r7 as in the
 * reference, and the coin text loads inside the loop; the choice-0 retry has its
 * own SetMessage/ShowMessage/Wait/goto tail (crossjumping shares the call as the
 * reference does). The prize and coin tables are not const (the reference reloads
 * the coin table entry after the call), and the weight sum reads a u8 then casts
 * (ldrb and shifts, not ldrsb).
 * Remaining (111):
 *  - the -1 constants: here cse keeps the choice test's -1 in a pseudo and reuses
 *    it for the later Event_ShowMessage/Event_OpenMessage(-1, 0) (and the result
 *    test's -1 for Party_AdjustSixDigitCounterA(-1)); the reference materialises a
 *    fresh -1 at every use and the compare's in r3. A switch on choice (case -1, 0,
 *    1) avoids the carry but emits a balanced decision tree (0 first), and an
 *    if (choice == -1) before a switch carries again;
 *  - the sum loop: the reference keeps i counting up against a copy of the
 *    count in ip and does not fold the entry test (cmp i, n with i = 0); here
 *    loop.c reverses it and cse folds the entry test to n <= 0;
 *  - text + 1 (both windows): the reference adds 1 at the use (adds r0, r5, #1),
 *    here the text register itself is incremented before the call;
 *  - state + 1 kept across the prize call for the halving (the reference
 *    recomputes fp + 1); the roll loop index r5 against r6.
 * Earlier: a for (;;) prompt scored 157 (A block misplaced, r9/r10 and r7/r8
 * swapped, text hoisted). */
#include "TYPES.H"
#include "IO_REG.H"
#include "EDITION.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"
#include "GAME_STATE.H"
#include "INVENTORY.H"
#include "MENU_LIST.H"

extern u8 MsgTorebiCoins[];
extern u8 MsgTorebiThrow[];
extern u8 MsgTorebiToss[];
extern u8 MsgTorebiWonNumberCoin[];
extern u8 MsgTorebiCounterMarker[];
extern u16 Data_0200200c[];
extern s32 Data_02001fd0[];

void Battle_ResetEffectCounter(void);
void UiText_DrawNumberInWindow(s32 value, s32 digits, struct UiWindow *window, s32 x, s32 y);
s32 Menu_SelectEntry20To21(s32 choice);
void UiWork_FinalizePendingCore(void);
s32 Party_AdjustSixDigitCounterA(s32 amount);
void UiWork_PushValueSlot(s32 value, s32 digits);
s32 TorebiIzumi_RunSpringRide(s32 side);
void TorebiIzumi_RevealPrize(s32 item);

struct StateBytes {
    u8 first;
    s8 rest[15];
    s32 coins;
};


#if defined(TBS_EDITION_EN) || !EDITION_INTERNATIONAL
#define ASK_WIDTH 17
#else
#define ASK_WIDTH 19
#endif
#if !EDITION_INTERNATIONAL
#define NUMBER_X 56
#define TOSS_X 22
#define TOSS_WIDTH 7
#elif defined(TBS_EDITION_EN)
#define NUMBER_X 72
#define TOSS_X 20
#define TOSS_WIDTH 9
#else
#define NUMBER_X 88
#define TOSS_X 19
#define TOSS_WIDTH 10
#endif

s32 TorebiIzumi_RunSpringGame(void)
{
    s32 choice = 0;
    s32 coins;
    s32 tickets;
    struct UiWindow *window;
    s32 result;
    s32 sum;
    s32 count;
    s32 i;
    s32 roll;
    s32 message;
    struct StateBytes *state;

    Event_Begin();
    Battle_ResetEffectCounter();
    message = (s32)MsgTorebiThrow;
    state = (struct StateBytes *)&gGameState;
prompt:
        coins = state->coins;
        tickets = PartyInventory_CountItem(229);
        Event_SetMessage(message);
        Event_OpenMessage(-1, 0);
        window = UiWindow_Create(0, 0, ASK_WIDTH, 4, 2);
      {
        s32 text = (s32)MsgTorebiCoins;
        UiText_DrawCharacterAtOffset(text, window, 0, 0);
        UiText_DrawNumberInWindow(coins, 6, window, NUMBER_X, 0);
#if !EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffset((s32)MsgTorebiCounterMarker, window, 104, 0);
#endif
        UiText_DrawCharacterAtOffset(text + 1, window, 0, 8);
        UiText_DrawNumberInWindow(tickets, 6, window, NUMBER_X, 8);
#if !EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffset((s32)MsgTorebiCounterMarker, window, 104, 8);
#endif
      }
        choice = Menu_SelectEntry20To21(choice);
        UiWork_Finalize(window, 2);
        UiWork_FinalizePendingCore();
        if (choice == -1) {
            goto end;
        }
        if (choice == 0) {
            if (coins != 0) {
                goto play;
            }
            Event_SetMessage(message + 1);
            Event_ShowMessage(-1, 0);
            Task_Wait(1);
            goto prompt;
        }
        if (choice != 1) {
            goto play;
        }
        if (tickets != 0) {
            goto room;
        }
        Event_SetMessage(message + 2);
        Event_ShowMessage(-1, 0);
        Task_Wait(1);
        goto prompt;
room:
    if (PartyInventory_CountFreeSlots() == 0) {
        Event_SetMessage(message + 4);
        Event_OpenMessage(-1, 0);
        if (Event_ChooseYesNo(0, 0) != 0) {
            goto end;
        }
    }
play:
    window = UiWindow_Create(TOSS_X, 15, TOSS_WIDTH, 4, 2);
    {
        s32 text = (s32)MsgTorebiToss;

        UiText_DrawCharacterAtOffset(text, window, 0, 0);
        UiText_DrawCharacterAtOffset(text + 1, window, 0, 8);
    }
    Task_Wait(5);
    Audio_PlayCue(116);
    for (;;) {
        if (gKeyState & KEY_A) {
            Audio_PlayCue(112);
            result = 0;
            break;
        }
        if (gKeyState & KEY_B) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }
        Task_Wait(1);
    }
    UiWork_Finalize(window, 2);
    if (result == -1) {
        goto end;
    }
    if (choice == 0) {
        Party_AdjustSixDigitCounterA(-1);
    } else if (choice == 1) {
        PartyInventory_Remove(229);
    }
    result = TorebiIzumi_RunSpringRide(choice);
    if (choice == 0) {
        if (result != 4) {
            Party_AdjustSixDigitCounterA(Data_0200200c[result]);
            Audio_PlayCue(91);
            UiWork_PushValueSlot(Data_0200200c[result], 5);
            Event_SetMessage((s32)MsgTorebiWonNumberCoin);
            Event_ShowMessage(-1, 0);
        } else {
            Audio_PlayCue(113);
            Event_Wait(10);
        }
    } else {
        sum = 0;
        for (i = 0; i < result * 3 + 3; i++) {
            u8 w = ((u8 *)state)[i + 285];
            sum += (s8)w;
        }
        roll = (u32)(sum * Random_Next()) >> 16;
        for (i = 0; i < 15; i++) {
            roll -= state->rest[i + 284];
            if (roll < 0) {
                break;
            }
        }
        if (i == 15) {
            i = 14;
        }
        TorebiIzumi_RevealPrize(Data_02001fd0[i]);
        {
            s8 weight = state->rest[i + 284];

            if (state->rest[i + 284] > 1) {
                state->rest[i + 284] /= 2;
            }
        }
    }
end:
    Event_End();
    return 0;
}
