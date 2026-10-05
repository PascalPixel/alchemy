/* NONMATCHING (Japanese edition only, id gate): TorebiIzumi_RunSpringGame.
 * 2026-10-05 sun deep finisher 18. This body matches the Japanese listing
 * instruction for instruction (an opcode, register and offset compare of
 * the .ifeq EDITION_INTERNATIONAL body of resource_3b7_overlay.s, 290 of
 * 290, branch and pool offsets included), but the build's id gate refuses
 * it: the counter word is written text + 2, loop.c hoists it and reload
 * rematerializes it from its REG_EQUIV as the pool word MsgTorebiCoins + 2
 * (the same 4052 as MsgTorebiCounterMarker; O3 wants the id whole).
 * What the native needed, each measured:
 *  - the menu is a for (;;) loop that runs on through the throw window, the
 *    key loop and UiWork_Finalize and ends with break; the retries are
 *    continue. loop.c moves the key loop's A-button exit block to the
 *    nearest barrier before its target at the target's loop level; with
 *    the target inside the menu loop that is the barrier after the retry's
 *    back jump, as the native has it (the goto loop of the localized
 *    editions gets it the same way at the outer level, but a goto loop has
 *    no loop.c hoisting). With the loop ending before UiWork_Finalize: 72;
 *    with the whole payout inside it the roll loop changes (55);
 *  - text and state are assigned at the loop top and hoisted by loop.c in
 *    that order (native order Throw, Coins, gCell); text assigned before
 *    the loop lets GCSE PRE hoist text + 1 too;
 *  - the throw window's text lives in result, a pseudo already global, so
 *    result + 1 goes straight into r0 (a local toss variable is tied to
 *    the sum: adds r5, #1; sharing text keeps the toss in r9);
 *  - global allocation of the high registers goes 1st r8, 2nd r10, 3rd r9,
 *    4th r11 (pass 0 reuses sl), so the priority order must be choice,
 *    message, text, state, and the counter word last (rematerialized).
 *    Refs/live: message 9/174, text 8/164 (the 8th is the hoisted
 *    text + 2), state 8/194, counter 7/160;
 *  - the halving: w pinned to r0 and an empty asm copying state into it
 *    ("=r"(w) : "0"(state)) stops GCSE PRE's reuse of the roll loop's
 *    state + 1 with one state reference instead of the localized "+r"
 *    device's two (with those two state outranks text: o8 form, 26), and
 *    the pin keeps w (8 refs, so it outranks the halved value) out of r2.
 *    Without the pin: 14 (w r2, value r0); a separate copy t: 38 (t in r3
 *    and a second spill register, r2, in the reload rotation).
 * With the counter word as (s32)MsgTorebiCounterMarker the gate passes but
 * text has 7 refs (.085) and loses to state (.138) and the counter (.0875):
 * 20 (state r9, text rematerialized, counter fp). Tried and not matching:
 * the counter hoisted first through a mark variable (18: text then beats
 * the counter but not state), mark set before the loop (18), text also set
 * before the loop (the earlier set is dead: 20), text pinned to r9 (loop.c
 * leaves the Coins load in the loop: 8; set before the loop PRE spills
 * text + 1: 21), a "+r"(message) device with an "r"(text) use (text 13
 * refs outranks message: 40, 50), "+r"(text) in the loop (54), text++
 * (67). The counter word needs one more reference to text before the loop
 * than the whole id gives, or the gate's leave for that addend.
 * The earlier Japanese history follows. */
/* NONMATCHING (Japanese edition only): TorebiIzumi_RunSpringGame.
 * 2026-10-05 sun deep finisher 7: the localized editions are adopted in
 * IZUMI2.C (three tagged S2 devices; the third, slot pinned to r1 with a
 * register variable, settles the last two: computed before state + 1 it
 * keeps LUID order for sched2 and r1 for itself). Measured here by an
 * opcode-and-register LCS against the .ifeq EDITION_INTERNATIONAL body of
 * resource_3b7_overlay.s (290 instructions), compiled with
 * -DTBS_EDITION_JA:
 *  - the adopted localized source as it stands: 118 (spills, 8-byte frame);
 *  - the native keeps choice r8, message sl, MsgTorebiCoins r9 loaded once
 *    before the prompt, state fp, coins r7, tickets r5, and reloads the
 *    counter mark from the pool at each use: a hoisted invariant that
 *    lost allocation and is rematerialized from its REG_EQUIV;
 *  - text assigned before the goto label: 68; GCSE PRE then hoists
 *    text + 1 too (rematerialized as MsgTorebiCoins + 1), which the native
 *    computes in the loop as mov r0, r9; adds r0, #1. Adding a mark
 *    variable before the label: 24 (state and text swap fp/r9);
 *  - the prompt as for (;;) with continue (this file, 16): loop.c hoists
 *    text (r9) and, with text = MsgTorebiCoins; state = &gGameState; as
 *    the loop's first statements, in the native order (text, then state).
 *    Left: (1) the toss window reuses text's hoisted r9 where the native
 *    loads MsgTorebiToss into r5 (a separate toss variable or the literal
 *    symbols drop text's priority below the mark's: 30, 32); (2) loop.c
 *    moves the A-button exit block of the key loop to the barrier after
 *    Party_AdjustSixDigitCounterA(-1) instead of after the prompt's back
 *    jump, because that barrier is now inside the prompt loop's notes
 *    (the localized finding that forces the goto loop there);
 *  - without the message device: 42 (as for (;;)).
 * The earlier localized history follows. */
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
    for (;;) {
        text = (s32)MsgTorebiCoins;
        state = &gGameState;
        coins = state->coins;
        tickets = PartyInventory_CountItem(229);
        Event_SetMessage(message);
        Event_OpenMessage(-1, 0);
        window = UiWindow_Create(0, 0, ASK_WIDTH, 4, 2);
        UiText_DrawCharacterAtOffset(text, window, 0, 0);
        UiText_DrawNumberInWindow(coins, 6, window, NUMBER_X, 0);
        UiText_DrawCharacterAtOffset(text + 2, window, 104, 0);
        UiText_DrawCharacterAtOffset(text + 1, window, 0, 8);
        UiText_DrawNumberInWindow(tickets, 6, window, NUMBER_X, 8);
        UiText_DrawCharacterAtOffset(text + 2, window, 104, 8);
        choice = Menu_SelectEntry20To21(choice);
        UiWork_Finalize(window, 2);
        UiWork_FinalizePendingCore();
        if (choice == cancel) {
            goto end;
        }
        if (choice == 0) {
            if (coins == 0) {
                Event_SetMessage(message + 1);
                Event_ShowMessage(-1, 0);
                Task_Wait(1);
                continue;
            }
        } else if (choice == 1) {
            if (tickets == 0) {
                Event_SetMessage(message + 2);
                Event_ShowMessage(-1, 0);
                Task_Wait(1);
                continue;
            }
            if (PartyInventory_CountFreeSlots() == 0) {
                Event_SetMessage(message + 4);
                Event_OpenMessage(-1, 0);
                if (Event_ChooseYesNo(0, 0) != 0) {
                    goto end;
                }
            }
        }
        window = UiWindow_Create(TOSS_X, 15, TOSS_WIDTH, 4, 2);
        result = (s32)MsgTorebiToss;
        UiText_DrawCharacterAtOffset(result, window, 0, 0);
        UiText_DrawCharacterAtOffset(result + 1, window, 0, 8);
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
        break;
    }
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
        {
            /* FAKEMATCH: keeps state + 1 in r0 as the native does. */
            register s8 *w asm("r0");
            s32 slot = i + 284;

            /* FAKEMATCH: the native recomputes state + 1 after the call. */
            __asm__("" : "=r"(w) : "0"(state));
            w++;
            if (w[slot] > 1) {
                w[slot] /= 2;
            }
        }
    }
end:
    Event_End();
    return 0;
}
