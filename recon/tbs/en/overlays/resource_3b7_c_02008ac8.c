/* Pin-free TorebiIzumi_RunSpringGame trial: English 280/280/279 common,
 * Japanese 290/290/289, differ 2 in each. The state+1 increment precedes
 * the slot addition; the native places it immediately after that addition.
 * Removing only the slot or pointer binding leaves 10/16 differences.
 * Shared pointer-first declarations reduce both editions to this one swap.
 * Tied pointer/slot operands, moving the slot across the existing barrier,
 * one-pass pointer/index/halving blocks and zero-cancellation did not match.
 * The updated overlay permuter compiled 4231 candidates in 2 minutes (seed
 * 379), finding the same pointer-first order but no zero. This draft now
 * retains staging's prize_state copy and removal of the international
 * prompt barrier; the copy is used only in the international final block.
 * Moving the existing barrier onto w, taking slot as an input and incrementing
 * w afterwards orders the operations correctly but differs 14 in EN: w takes
 * r2 instead of r0, and the signed byte takes r0 instead of r2. Early-clobber
 * and generic constraints retain 14; passing both w and slot through differs
 * 44. Moving slot before the existing state barrier and adding it as a
 * low-register input leaves 10 in EN (280 built); a prize_state output
 * instead leaves 16, and a volatile state output leaves 66.
 * Further EN trials: an index output with state input leaves 42 (278
 * built), joint state/index outputs leave 24 (280 built). Replacing the
 * state boundary with a slot output and pointer input before w++ leaves
 * 31 (279 built); retaining the state boundary as well leaves 50. A
 * pointer output with slot input and the state boundary leaves 42;
 * input-only pointer/slot operands retain 14, and joint outputs leave 44.
 * Linked source keeps its bindings pending all-edition byte identity. */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "call.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "global_cells.h"
#include "edition.h"
#include "field_service.h"
#include "game_state.h"
#include "inventory.h"
#include "menu_list.h"
#include "iwram_call.h"


void TorebiIzumi_RevealPrize(s32 item);
s32 TorebiIzumi_RunSpringRide(s32 side);

void Battle_ResetEffectCounter(void);
void UiText_DrawNumberInWindow(s32 value, s32 digits, struct UiWindow *window, s32 x, s32 y);
void UiWork_FinalizePendingCore(void);
s32 Party_AdjustCoins(s32 amount);

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
    struct GameState *prize_state;

    Event_Begin();
    Battle_ResetEffectCounter();
    message = (s32)MsgTorebiThrow;
#if EDITION_INTERNATIONAL
    state = &gGameState;
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
        UiText_DrawCharacterAtOffset(text + 1, window, 0, 8);
        UiText_DrawNumberInWindow(tickets, 6, window, NUMBER_X, 8);
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
    Event_PlayCue(116);
    for (;;) {
        if (gKeyState & KEY_A) {
            Event_PlayCue(112);
            result = 0;
            break;
        }
        if (gKeyState & KEY_B) {
            Event_PlayCue(113);
            result = -1;
            break;
        }
        Task_Wait(1);
    }
    UiWork_Finalize(window, 2);
#else
    /* FAKEMATCH: whole marker ids swap message/label registers; keep message in r10 and label in r9. */
    asm("" : "+r"(message));
    for (;;) {
        text = (s32)MsgTorebiCoins;
        /* FAKEMATCH: keep the coin label in r9; otherwise the repeated marker takes fp and moves state into r9. */
        asm("" : "+r"(text));
        state = &gGameState;
        coins = state->coins;
        tickets = PartyInventory_CountItem(229);
        Event_SetMessage(message);
        Event_OpenMessage(-1, 0);
        window = UiWindow_Create(0, 0, ASK_WIDTH, 4, 2);
        UiText_DrawCharacterAtOffset(text, window, 0, 0);
        UiText_DrawNumberInWindow(coins, 6, window, NUMBER_X, 0);
        UiText_DrawCharacterAtOffset((s32)MsgTorebiCounterMarker, window, 104, 0);
        UiText_DrawCharacterAtOffset(text + 1, window, 0, 8);
        UiText_DrawNumberInWindow(tickets, 6, window, NUMBER_X, 8);
        UiText_DrawCharacterAtOffset((s32)MsgTorebiCounterMarker, window, 104, 8);
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
        Event_PlayCue(116);
        for (;;) {
            if (gKeyState & KEY_A) {
                Event_PlayCue(112);
                result = 0;
                break;
            }
            if (gKeyState & KEY_B) {
                Event_PlayCue(113);
                result = -1;
                break;
            }
            Task_Wait(1);
        }
        UiWork_Finalize(window, 2);
        break;
    }
#endif
    if (result == cancel) {
        goto end;
    }
    if (choice == 0) {
        Party_AdjustCoins(-1);
    } else if (choice == 1) {
        PartyInventory_Remove(229);
    }
    result = TorebiIzumi_RunSpringRide(choice);
    if (choice == 0) {
        if (result != 4) {
            Party_AdjustCoins(Data_0200200c[result]);
            Event_PlayCue(91);
            UiWork_PushValueSlot(Data_0200200c[result], 5);
            Event_SetMessage((s32)MsgTorebiWonNumberCoin);
            Event_ShowMessage(-1, 0);
        } else {
            Event_PlayCue(113);
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
        /* FAKEMATCH: preserving the pre-device pointer copy keeps message in r9 and state in fp without the international prompt barrier. */
        prize_state = state;

        /* FAKEMATCH: post-call state recomputation retained; one scheduling swap remains. */
        __asm__("" : "+r"(state));
        {
#if EDITION_INTERNATIONAL
            s8 *w = (s8 *)prize_state + 1;
#else
            s8 *w = (s8 *)state + 1;
#endif
            s32 slot = i + 284;
            if (w[slot] > 1) {
                w[slot] /= 2;
            }
        }
    }
end:
    Event_End();
    return 0;
}
