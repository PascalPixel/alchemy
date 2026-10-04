/* NONMATCHING: the spring game, 680 bytes; alchemy diff 42 (was 157 this morning).
 * 2026-10-04 sun w3 finisher 1. Settled, each measured:
 *  - the prompt is a goto loop (prompt: ... goto prompt), not a for (;;): loop.c
 *    lifts the A-button exit block out of the key loop to the nearest barrier
 *    before its target at the outer level, and only a goto loop leaves the
 *    prompt's back-jump barrier outside any loop (a for (;;) keeps it inside the
 *    loop notes and the block landed after the choice-0 jump). The choice-0 retry
 *    has its own SetMessage/ShowMessage/Wait/goto tail (crossjumping shares the
 *    call as the reference does);
 *  - the -1 tests compare with a variable (cancel = -1): gcse propagates the
 *    constant into the jumps after cse, so reload materialises the -1 in r3 at
 *    each test and no later -1 argument reuses it. With a literal -1 cse keeps the
 *    compare's -1 in a pseudo and every later -1 argument (ShowMessage,
 *    OpenMessage, Party_AdjustSixDigitCounterA) reuses it;
 *  - one text variable for both windows: text is then global and local-alloc does
 *    not tie text + 1 to it, so the add goes straight into r0 at the call;
 *  - the weight sum is if (i < n) do { ... } while (i < n): i counts up against
 *    a copy of n (ip) and the entry test stays cmp i, n; a for loop is reversed by
 *    loop.c and its entry test folded. The byte is read as u8 and cast (ldrb and
 *    shifts); the roll is Random_Next() * sum (operand order);
 *  - the prize and coin tables are not const (the coin entry is reloaded after
 *    the call).
 * Remaining (42, all but four pool and branch offsets): state + 1. gcse reuses
 * the roll's first read base (fp + 1, kept in r5 across the prize call) for the
 * halving; the reference recomputes mov r0, fp; adds r0, #1 there and uses r1 for
 * the first read. Tried: a weight local (42), a pointer to the weight (79), the
 * state pointer reassigned (58) or gGameState spelled at the halving (38 but the
 * base comes from the pool), the roll loop as do/while (65) or through
 * (s8 *)state + 285 (51), the halving through (s8 *)state + 285 (79). */
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
    s32 text;
    s32 cancel = -1;
    s32 coins;
    s32 tickets;
    struct UiWindow *window;
    s32 result;
    s32 sum;
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
        text = (s32)MsgTorebiCoins;
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
        if (choice == cancel) {
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
        text = (s32)MsgTorebiToss;

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
    if (result == cancel) {
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
        i = 0;
        if (i < result * 3 + 3) {
            do {
                u8 w = ((u8 *)state)[i + 285];
                sum += (s8)w;
                i++;
            } while (i < result * 3 + 3);
        }
        roll = (u32)(Random_Next() * sum) >> 16;
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
        if (state->rest[i + 284] > 1) {
            state->rest[i + 284] /= 2;
        }
    }
end:
    Event_End();
    return 0;
}
