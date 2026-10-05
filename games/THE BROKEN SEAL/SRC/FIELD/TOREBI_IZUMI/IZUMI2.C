#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "IO_REG.H"
#include "EDITION.H"
#include "FIELD_SERVICE.H"
#include "GAME_STATE.H"
#include "INVENTORY.H"
#include "MENU_LIST.H"

struct Workspace {
    u8 unknown_000[356];
    struct FieldActor actor;
};

extern const u16 TorebiIzumi_AlphaSteps[];

extern u8 MsgTorebiBadComeBack[];
extern u8 MsgTorebiBadLikeUse[];
extern u8 MsgTorebiComeAgain[];
extern u8 MsgTorebiGetRidItems[];
extern u8 MsgTorebiStepRightEnjoy[];
extern u8 MsgTorebiStepRightTry[];
void ObjectTable_Snapshot(void);
void UiWork_PushValueSlot(s32 value, s32 digits);
void Event_SetPair1c0AndSetValue170(s32 a, s32 b);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);

extern u8 MsgTorebiBuddyWannaTry[];
extern u8 MsgTorebiYaDontEnough[];
s32 Party_GetAverageLevel(void);
void Event_SetPair1c0AndSetValue170(s32 value, s32 mode);

/* Lifts the workspace actor sixteen pixels with a cue, then steps the blend alpha through its eight-entry table. */
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

/* The lucky wheels barker: counts the game tickets (item 228), offers a
 * game, warns when the bag is nearly full (six or fewer free slots) and
 * starts the lucky wheels when the player agrees. */
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
    Event_SetPair1c0AndSetValue170(508, 0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_TorebiIzumi1, 12);
}

/* Tolbi game attendant: quote ten coins per level; without enough coins say so, otherwise record the coins and, if the player accepts, walk in and start the game area. */
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
        Event_SetPair1c0AndSetValue170(0x1fd, 0);
        Party_SetFields1ceAnd1d0((s32)&SceneId_TorebiIzumi1, 13);
    } else {
        gEventWork->message++;
        Engine_EventShowMessage(9, 0);
    }
    Engine_EventEnd();
}

#if EDITION_INTERNATIONAL
/* The localized editions' spring game. The Japanese one, with its counter
   marks beside both numbers, keeps other registers and stays on its
   listing until its branch is written. */
extern u8 MsgTorebiCoins[];
extern u8 MsgTorebiThrow[];
extern u8 MsgTorebiToss[];
extern u8 MsgTorebiWonNumberCoin[];
extern u16 Data_0200200c[];
extern s32 Data_02001fd0[];

void Battle_ResetEffectCounter(void);
void UiText_DrawNumberInWindow(s32 value, s32 digits, struct UiWindow *window, s32 x, s32 y);
s32 Menu_SelectEntry20To21(s32 choice);
void UiWork_FinalizePendingCore(void);
s32 Party_AdjustSixDigitCounterA(s32 amount);
s32 TorebiIzumi_RunSpringRide(s32 side);
void TorebiIzumi_RevealPrize(s32 item);

#if defined(TBS_EDITION_EN)
#define ASK_WIDTH 17
#define NUMBER_X 72
#define TOSS_X 20
#define TOSS_WIDTH 9
#else
#define ASK_WIDTH 19
#define NUMBER_X 88
#define TOSS_X 19
#define TOSS_WIDTH 10
#endif

/* Asks to throw a coin or a ticket into the spring, rides it, and pays a
   coin prize or draws a weighted item prize, halving the drawn weight. */
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
            /* Computed first, so sched2 puts its add between mov r0, fp */
            /* and adds r0, #1; as a pseudo it loses r1 to state + 1. */
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
#endif
