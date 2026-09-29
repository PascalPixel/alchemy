/* NONMATCHING: 680 of 680 bytes, 211 differing halfwords, 123 aligned edits
 * (2026-09-27). Whole owner 02000ac8..02000d70; pool 02000d4c..02000d70.
 * Own-ROM callees/pool audited. WalkLeaderToSpring calls this controller;
 * its side argument is passed to the separately matched RunSpringRide.
 * Registered as torebi-spring-game-candidate, not credited.
 *
 * Three bounded structural trials:
 * 1. Unsigned weight storage with explicit signed consumption: identical
 *    680/212/125. Signedness alone does not change the sum-loop lowering.
 * 2. State-owned signed weight array for the sum, deriving the selection
 *    pointer only after RandomNext: 684 bytes, 241 differing halfwords,
 *    124 aligned edits. Sum now uses ldrb/lsls/asrs, but remains a countdown;
 *    selection entry and tail addressing worsen. Original full-size baseline
 *    retained despite the single-edit alignment improvement.
 * 3. Explicit wait/read-key labels, one persistent A-key mask and local B
 *    mask: 668 bytes, 245 differing halfwords, 130 aligned edits. B no longer
 *    hoists, but the pressed block moves into the poll and keypad base reloads.
 *    Rejected. Full normalized diff reviewed for every trial.
 *
 * Remaining: choice/message exchange sl/r9; shared -1 crosses prompt calls;
 * poll-loop entry/hoisting; sum's bound must move to ip, and the state+1
 * selection pointer must not survive RandomNext.
 * Stop these three axes; do not repeat type or declaration spelling sweeps.
 *
 * H1 (2026-09-27): exact TOREBI_IZUMI/OFFER_WHEELS.C preserves the s32
 * Engine_EventOpenMessage result contract. Replace both old void function
 * pointer casts with FIELD_EVENT.H's Event_OpenMessage. The first prompt's
 * r1-before-final-r0 argument setup now matches; full owner improves from
 * 212/125 to 211/123 with all nine pool words still exact. The second prompt
 * still inherits the choice-comparison -1 in r5. MENU/CONFIRM_SELECTION.C
 * confirms Menu_SelectEntry20To21 is s32(s32), unchanged by this transfer.
 * H2: transfer INVENTORY.H and exact GAME/INVENTORY/{COUNT,ITEMS,MUTATION}.C
 * plus GAME/PARTY/COUNTERS.C. Debit/removal had incorrect void declarations;
 * canonical s32 calls and automatic registered-import resolution reproduce
 * H1's complete bytes and normalized diff. Four opaque bindings are obsolete.
 * The remaining -1 is pseudo 52 from the choice comparison (5 uses/24 insns/
 * 2 calls), not a mismatched callee return. Choice has 15 uses/302 insns/31
 * calls; message has 8/146/15. No allocator tie admits declaration changes.
 * Exact spring callers and menu siblings reveal no new input-read ownership
 * boundary beyond the already closed polling/weight models. Interface pass
 * stopped with the H1 argument-order correction retained; no new DONE bytes.
 * Post-rewrite sibling audit: CONFIRM_SELECTION.C and the spring callers
 * introduce no changed ABI. FUNE_HEYA/COND_ACT.C's delayed repeated-speaker
 * lifetime has no matching one-off phase here: message is already loaded at
 * its first repeated use. RTL insn 136 creates comparison pseudo 52 after
 * FinalizePending; local allocation reuses it in prompt insn 253. This is
 * not a symbolic/numeric constant split. The exact window pointer interfaces
 * in TEXT/RENDER.C and WINDOW/FINALIZE.C do not justify another type trial:
 * the window already occupies the reference r6 in both phases. No new model
 * admitted; preserve 680/211/123 rather than repeat the closed searches.
 *
 * Guarded-do model: own-ROM 02000cbc/02000cd4 separates the entry guard
 * from the upward back-edge; approved loop.c's variable-bound countdown
 * conversion requires loop->vtop. Prediction/admission: an explicit guard
 * plus do/while retains the upward counter. One compile, no spelling sweep;
 * acceptance remains the complete 680-byte owner and compare/coverage/verify.
 * Admitted: adds r5,#1; cmp r5,r0; blt, replacing the countdown. Complete
 * normalized diff: still 680/211/123, all nine pool words exact. Retain this
 * ordinary control-flow correction without credit. Negative fact: it does
 * not move the bound to ip, change the ldrsb to ldrb/lsls/asrs, or stop r7's
 * state+1 pointer crossing RandomNext. The bound/pointer ancestry remains
 * missing; closed weight/type/polling axes were not reopened. Stop here.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "INVENTORY.H"
extern u8 MsgTorebiCoins[];
extern u8 MsgTorebiThrow[];
extern u8 MsgTorebiToss[];
extern u8 MsgTorebiWonNumberCoin[];

void Main_0808a460(void);
s32 Main_08015010(s32 x, s32 y, s32 width, s32 height, s32 flags);
void Main_08015080(s32 text, s32 window, s32 x, s32 y);
void Main_080150b0(s32 value, s32 digits, s32 window, s32 x, s32 y);
s32 Main_08015398(s32 choice);
void Main_08015018(s32 window, s32 mode);
void Main_08015140(void);
s32 Party_AdjustSixDigitCounterA(s32 amount);
void Main_08015120(s32 value, s32 digits);
s32 TorebiIzumi_Func0200173c(s32 side);
void Func_020004bc(s32 item);

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u16 Data_0200a00c[];
extern s32 Data_02009fd0[];
extern volatile u32 Data_03001c94;
/* MsgTorebiThrow, as a link-time value; the prompts that follow it are derived from it. */

s32 TorebiIzumi_RunSpringGame(void)
{
    s32 choice = 0;
    s32 message;
    union GameStateRows *state;
    s32 window;
    s32 text;
    s32 coins;
    s32 tickets;
    s32 result;
    s32 sum;
    s32 n;
    s32 i;
    s32 roll;
    s8 *weights;

    Engine_EventBegin();
    Main_0808a460();
    message = (s32)MsgTorebiThrow;
    state = &Data_02000240_t;
ask:
    coins = state->words[4];
    tickets = PartyInventory_CountItem(229);
    Engine_EventSetMessage(message);
    Event_OpenMessage(-1, 0);
    window = Main_08015010(0, 0, 17, 4, 2);
    text = (s32)MsgTorebiCoins;
    Main_08015080(text, window, 0, 0);
    Main_080150b0(coins, 6, window, 72, 0);
    Main_08015080(text + 1, window, 0, 8);
    Main_080150b0(tickets, 6, window, 72, 8);
    choice = Main_08015398(choice);
    Main_08015018(window, 2);
    Main_08015140();
    if (choice == -1) {
        goto end;
    }
    if (choice == 0) {
        if (coins != 0) {
            goto play;
        }
        n = message + 1;
    } else {
        if (choice != 1) {
            goto play;
        }
        if (tickets != 0) {
            goto check_room;
        }
        n = message + 2;
    }
    Engine_EventSetMessage(n);
    Call2((void (*)())Engine_EventShowMessage, -1, 0);
    Engine_TaskWait(1);
    goto ask;
pressed:
    Engine_AudioPlayCue(112);
    result = 0;
    goto close;
check_room:
    if (PartyInventory_CountFreeSlots() == 0) {
        Engine_EventSetMessage(message + 4);
        Event_OpenMessage(-1, 0);
        if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) != 0) {
            goto end;
        }
    }
play:
    window = Main_08015010(20, 15, 9, 4, 2);
    text = (s32)MsgTorebiToss;
    Main_08015080(text, window, 0, 0);
    Main_08015080(text + 1, window, 0, 8);
    Engine_TaskWait(5);
    Engine_AudioPlayCue(116);
    for (;;) {
        if (Data_03001c94 & 1) {
            goto pressed;
        }
        if (Data_03001c94 & 2) {
            break;
        }
        Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(113);
    result = -1;
close:
    Main_08015018(window, 2);
    if (result == -1) {
        goto end;
    }
    if (choice == 0) {
        Party_AdjustSixDigitCounterA(-1);
    } else if (choice == 1) {
        PartyInventory_Remove(229);
    }
    result = TorebiIzumi_Func0200173c(choice);
    if (choice == 0) {
        if (result != 4) {
            Party_AdjustSixDigitCounterA(Data_0200a00c[result]);
            Engine_AudioPlayCue(91);
            Main_08015120(Data_0200a00c[result], 5);
            Engine_EventSetMessage((s32)MsgTorebiWonNumberCoin);
            Call2((void (*)())Engine_EventShowMessage, -1, 0);
        } else {
            Engine_AudioPlayCue(113);
            Engine_EventWait(10);
        }
    } else {
        weights = (s8 *)&state->bytes[0][1];
        n = result * 3 + 3;
        sum = 0;
        i = 0;
        if (i < n) {
            do {
                sum += weights[i + 284];
                i++;
            } while (i < n);
        }
        roll = (u32)(sum * Engine_RandomNext()) >> 16;
        for (i = 0; i < 15; i++) {
            roll -= weights[i + 284];
            if (roll < 0) {
                break;
            }
        }
        if (i == 15) {
            i = 14;
        }
        Func_020004bc(Data_02009fd0[i]);
        if (weights[i + 284] > 1) {
            weights[i + 284] /= 2;
        }
    }
end:
    Engine_EventEnd();
    return 0;
}
