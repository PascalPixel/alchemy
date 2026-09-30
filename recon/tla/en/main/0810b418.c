#include "TYPES.H"
#include "SYSTEM.H"

struct FieldEffectState {
    u8 padding0[0x1C0];
    s32 effect;
    u8 padding1C4[4];
    s32 delay;
};

struct FieldObject {
    u8 padding0[0x34];
    u16 saved_x;
    u16 saved_y;
    s16 x;
    u16 y;
};

extern struct FieldEffectState *gEventWork;

s32 Party_ListActiveOwnersFar(s16 *);
void Party_AdjustSixDigitCounterAFar(s32);
struct FieldObject *Owner_GetStateFar(s32);
void Owner_RecalculateRatiosFar(s32);
void Event_ClearStatus1c6Far(void);
void Event_WaitValue1c8FramesFar(void);
void Audio_PlayCue(s32);
void AudioCommand_WaitForStateByteClear(void);
void Event_SetStatus1c6Far(void);

extern s8 Inn_PriceMultipliers[];

#if defined(TBS_EDITION_JA)
#define MESSAGE_WINDOW_ROWS 11
#else
#define MESSAGE_WINDOW_ROWS 12
#endif

enum InnMessageId {
    INN_MESSAGE_WELCOME = 0xd1c,
    INN_MESSAGE_STAY_COMPLETE,
    INN_MESSAGE_NOT_ENOUGH_COINS,
    INN_MESSAGE_GOODBYE,
    INN_MESSAGE_REST_COMPLETE,
};

struct InnGlobalState {
    u8 padding_00[0x10];
    u32 limit;
};

struct InnObjectComponent {
    u8 padding_00[0x28];
    u16 *resource_id;
};

struct InnObject {
    u8 padding_00[0x50];
    struct InnObjectComponent *component;
};

extern struct InnGlobalState gGameState;
extern char MsgInnWelcome;

void Shop_InitializeCursorWork(void);
void Inn_Cleanup(void);
void UiMessage_ShowAndWait(s32 message_id);
s32 UiMessage_ShowChoice(s32);
s32 Inn_RoomPrice(s32);
void Inn_PlaySleep(s32);
void UiWork_FinalizeFar(s32, s32);
s32 UiWindow_CreateWithSideObjectFar(u16, s32, s32, s32);
void UiWork_PushValueSlotFar(s32, s32);
struct InnObject *Object_GetByIdFar(s32);

s32 Inn_CheckIn(s32 mode, s32 object_id)
{
    struct InnRuntimeState *state;
    struct InnObject *object;
    s32 win;
    s32 amount;
    s32 message_base;

    Shop_InitializeCursorWork();
    state = gMenuWork;
    state->active = 1;
    if (mode == 5)
        state->special_active = 1;

    object = Object_GetByIdFar(object_id);
    state->resource_id = *object->component->resource_id;
    win = UiWindow_CreateWithSideObjectFar(state->resource_id, 0, 0, 0);

    amount = Inn_RoomPrice(mode);
    UiWork_PushValueSlotFar(amount, 5);
    message_base = (s32)&MsgInnWelcome;
    UiMessage_ShowAndWait(message_base);
    state->window = UiWindow_CreateFar(0, 16, MESSAGE_WINDOW_ROWS, 4, 2);
    Shop_DrawMoney();

    if (UiMessage_ShowChoice(0) != 0) {
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
        UiWork_FinalizeFar(state->window, 2);
    } else if ((u32)amount > gGameState.limit) {
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
        UiWork_FinalizeFar(state->window, 2);
    } else {
        UiWork_FinalizeFar(state->window, 2);
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_STAY_COMPLETE - INN_MESSAGE_WELCOME));
        UiWork_FinalizeFar(win, 2);
        Inn_PlaySleep(amount);

        object = Object_GetByIdFar(object_id);
        state->resource_id = *object->component->resource_id;
        win = UiWindow_CreateWithSideObjectFar(state->resource_id, 0, 0, 0);
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
    }

    UiWork_FinalizeFar(win, 2);
    Inn_Cleanup();
    return 0;
}
