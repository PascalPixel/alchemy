#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "field_event.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "edition.h"
#include "field_service.h"
#include "game_state.h"
#include "inventory.h"
#include "menu_list.h"
#include "text/msg_ids.h"

#define Event_RequestDestination ((void (*)(s32,s32))Event_RequestDestination)
#define Party_GetAverageLevel ((s32 (*)(void))Party_GetAverageLevel)
#define Event_RequestDestination ((void (*)(s32,s32))Event_RequestDestination)

struct Workspace {
    u8 unknown_000[356];
    struct FieldActor actor;
};


void ObjectTable_Snapshot(void);
void UiWork_PushValueSlot(s32 value, s32 digits);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);


void TorebiIzumi_RiseAndFadeIn(void)
{
    u8 *p = gMapWork[0];
    s32 i;

    Engine_AudioPlayCue(216);
    p += 356;
    for (i = 0; i < 16; i++) {
        ((struct FieldActor *)p)->y.fixed -= 0x10000;
        Engine_TaskWait(4);
    }
    p = (u8 *)TorebiIzumi_AlphaSteps;
    for (i = 0; i < 8; i++) {
        {
            s32 mode = 0x3f42;

            REG_BLDCNT = mode;
        }
        REG_BLDALPHA = *(u16 *)p;
        p += 2;
        /* FAKEMATCH: the empty do/while keeps the step pointer's advance ahead of the wait's argument. */
        do { } while (0);
        Engine_TaskWait(8);
    }
}

void TorebiIzumi_OfferLuckyWheels(s32 spoken)
{
    s32 tickets = PartyInventory_CountItem(228);
    s32 room;

    ObjectTable_Snapshot();
    if (spoken == 0) {
        s32 message = (s32)MsgTorebiStepRightTry;

        Engine_EventSetMessage(message);
        Engine_EventShowMessage(8, 0);
        if (tickets == 0) {
            Engine_EventShowMessage(8, 0);
            return;
        }
        Engine_EventSetMessage(message + 2);
        UiWork_PushValueSlot(tickets, 5);
        Engine_EventOpenMessage(8, 0);
        if (Engine_EventChooseYesNo(0, 0) != 0) {
            Engine_EventShowMessage(8, 0);
            return;
        }
        room = PartyInventory_CountFreeSlots();
        if (room == 0) {
            Engine_EventSetMessage(message + 4);
            Engine_EventOpenMessage(8, 0);
        } else {
            if (room > 6) {
                goto play;
            }
            Engine_EventSetMessage(message + 5);
            Engine_EventOpenMessage(8, 0);
        }
        if (room > 6 || Engine_EventChooseYesNo(0, 0) == 0) {
            goto play;
        }
        Engine_EventSetMessage((s32)MsgTorebiGetRidItems);
        Engine_EventShowMessage(8, 0);
        return;
    }
    if (tickets == 0) {
        Engine_EventSetMessage((s32)MsgTorebiBadComeBack);
        Engine_EventShowMessage(8, 0);
        return;
    }
    Engine_EventSetMessage((s32)MsgTorebiBadLikeUse);
    Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        Engine_EventSetMessage((s32)MsgTorebiComeAgain);
        Engine_EventShowMessage(8, 0);
        return;
    }
play:
    Engine_EventSetMessage((s32)MsgTorebiStepRightEnjoy);
    Engine_EventShowMessage(8, 0);
    Event_RequestDestination(508, 0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_TorebiIzumi1, 12);
}

void TorebiIzumi_PayToPlay(void)
{
    u32 cost;
    u32 coins;

    cost = Party_GetAverageLevel() * 10;
    Engine_EventBegin();
    coins = gGameState.coins;
    if (coins < cost) {
        Engine_EventSetMessage((s32)MsgTorebiYaDontEnough);
        Engine_EventOpenMessage(9, 0);
        return;
    }
    *(u32 *)gSceneState = coins;
    ObjectTable_Snapshot();
    Engine_EventSetMessage((s32)MsgTorebiBuddyWannaTry);
    UiWork_PushValueSlot(cost, 5);
    Engine_EventOpenMessage(9, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventShowMessage(9, 0);
        Engine_ActorWalkToAndWait(0, 120, 128);
        Engine_ActorWalkToAndWait(0, 120, 152);
        Engine_ActorFaceDirection(0, 0x8000, 0);
        Engine_EventWait(20);
        Event_RequestDestination(0x1fd, 0);
        Party_SetFields1ceAnd1d0((s32)&SceneId_TorebiIzumi1, 13);
    } else {
        gEventWork->message++;
        Engine_EventShowMessage(9, 0);
    }
    Engine_EventEnd();
}

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

    Event_Begin();
    Battle_ResetEffectCounter();
    message = (s32)MsgTorebiThrow;
#if EDITION_INTERNATIONAL
    state = &gGameState;
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

#if EDITION_INTERNATIONAL
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
#else
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
#endif
    }
end:
    Event_End();
    return 0;
}
