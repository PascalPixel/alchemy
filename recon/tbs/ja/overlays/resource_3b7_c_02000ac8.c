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
s32 Party_AdjustCoins(s32 amount);
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
        Party_AdjustCoins(-1);
    } else if (choice == 1) {
        PartyInventory_Remove(229);
    }
    result = TorebiIzumi_RunSpringRide(choice);
    if (choice == 0) {
        if (result != 4) {
            Party_AdjustCoins(Data_0200200c[result]);
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
