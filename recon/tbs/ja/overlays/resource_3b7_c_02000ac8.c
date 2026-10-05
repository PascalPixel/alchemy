/* NONMATCHING (Japanese edition only): TorebiIzumi_RunSpringGame.
 * 2026-10-05 sun deep finisher 7: the localized editions are adopted in
 * IZUMI2.C (three tagged S2 devices; the third, slot pinned to r1 with a
 * register variable, settles the last two: computed before state + 1 it
 * keeps LUID order for sched2 and r1 for itself). The Japanese build of
 * this same source still differs from its listing (about 208 of 290
 * instructions by an opcode-and-register comparison against the
 * .ifeq EDITION_INTERNATIONAL body of resource_3b7_overlay.s): the native
 * keeps choice in r8, message in sl, MsgTorebiCoins (text) in r9, state in
 * fp, coins in r7 and tickets in r5 with a 4-byte frame; this source spills
 * (8-byte frame) and keeps text in r7 and the counter mark in r5. The two
 * counter-mark calls change the pressure across the prompt, so the message
 * and text devices need their own Japanese measurement. The earlier
 * localized history follows. */
/* Localized history: the spring game, 680 bytes; alchemy diff 2 with two tagged S2
 * devices (42 without them; 157 on 2026-10-04 morning).
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
 * (s8 *)state + 285 (51), the halving through (s8 *)state + 285 (79).
 * 2026-10-05 sun w5 finisher 1, measured with -da: the reuse is GCSE's PRE,
 * not cse. jump.c's copy of the roll loop's exit test (the first read, before
 * the loop) computes state + 1 into a pseudo; PRE then finds the halving's
 * state + 1 (and the loop body's) fully redundant and copy-propagates that
 * pseudo, which global gives r5 across the prize call. Every roll-loop
 * spelling that still reads base state + 1 and index i + 284 keeps the copy
 * available at the halving. The roll loop through a constant address
 * ((struct StateBytes *)&gGameState)->rest[i + 284] stops the PRE but the
 * halving then loads gGameState from the pool (cprop) and the roll loop
 * loses fp (53; 102 with every access spelled that way).
 * 2026-10-05 sun deep finisher 4:
 *  - the weights are a real GameState field at 0x11d (prize_weights, the
 *    15 values CHARACTER.C initialises 4,4,4,8,...,64): state->prize_weights[i]
 *    expands to exactly (state + 1) + (i + 284), the split StateBytes faked.
 *    Spelling the sum loop that way too lets PRE hoist state + 1 into it (28,
 *    wrong shape), so the sum loop keeps the u8 read at state + 285.
 *  - -fno-gcse (diagnostic only) leaves just the halving's r0/r1 swap, so the
 *    PRE reuse and the halving allocation are separate causes.
 *  - __asm__("" : "+r"(state)) after the prize call stops the PRE (14); it adds
 *    two references to state, which then outranks message in global
 *    allocation (message fp, state r9: 30) unless state is assigned first
 *    (right registers, wrong load order: 14) or message gets its own "+r"
 *    (10). A copy t = state with the "+r" on t instead keeps message ahead
 *    without a second device (t10 form: 4, t lands in r3, not tied to w).
 *  - the halving swap is global-alloc priority: base state + 1 (3 refs, born
 *    after the index) outranks i + 284 and takes r1. Base first (w = state + 1
 *    then slot = i + 284) gives r0/r1 as the native but sched2 then keeps
 *    adds r0, #1 before the index add (LUID order): the 2 left. Index first
 *    (slot then w) schedules as the native but swaps r0/r1 again (10). Tried
 *    and worse: slot as roll (24: the roll loop swaps roll and pointer),
 *    sum (16), a block local (10), i += 284 (14), (state->rest + 284)[i],
 *    *(rest + (i + 284)), (&rest[284])[i], w = state->rest then w[i + 284]
 *    (all 79, combine folds to fp + i + 285), "+r" on slot (12, slot takes
 *    r2), a "r" use of slot (12), "=r"(w) : "0"(state) then w++ (16), a
 *    register asm pin (70, loop opts change), a "+h" copy (cc1 aborts).
 *    devsearch from the 14 and the 2 forms (2100 iterations) found nothing. */
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
    struct GameState *state;

    Event_Begin();
    Battle_ResetEffectCounter();
    message = (s32)MsgTorebiThrow;
    state = &gGameState;
    /* The halving device below raises state above message in priority. */
    /* FAKEMATCH: keeps message in r9 and state in fp, as the native does. */
    __asm__("" : "+r"(message));
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
            roll -= state->prize_weights[i];
            if (roll < 0) {
                break;
            }
        }
        if (i == 15) {
            i = 14;
        }
        TorebiIzumi_RevealPrize(Data_02001fd0[i]);
        /* Without it GCSE PRE reuses the roll loop's state + 1 from r5. */
        /* FAKEMATCH: the native recomputes state + 1 into r0 after the call. */
        __asm__("" : "+r"(state));
        {
        /* FAKEMATCH: keeps the slot in r1 and state + 1 in r0. */
        register s32 slot asm("r1") = i + 284;
        s8 *w = (s8 *)state + 1;
        if (w[slot] > 1) {
            w[slot] /= 2;
        }
        }
    }
end:
    Event_End();
    return 0;
}
