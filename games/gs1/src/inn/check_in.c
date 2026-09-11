#include "types.h"
#include "scene.h"
#include "abi/inn/check_in.h"
#include "gs1_edition.h"
#include "inn.h"
#include "shop.h"

#if defined(GS1_EDITION_JA)
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

extern struct InnGlobalState gCell;
extern char Value_00000d1c;

void Inn_Cleanup(void);

s32 Inn_RoomPrice(s32);
void Inn_PlaySleep(s32);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void UiWindow_Close(s32, s32);

void UiText_DrawQuantity(s32, s32);
struct InnObject *Scene_GetRecord(s32);

s32 Inn_CheckIn(s32 mode, s32 object_id)
{
    struct InnState *state;
    struct InnObject *object;
    s32 win;
    s32 amount;
    s32 message_base;

    Sys_Run();
    state = gIw;
    state->active = 1;
    if (mode == 5)
        state->special_active = 1;

    object = Scene_GetRecord(object_id);
    state->resource_id = *object->component->resource_id;
    win = Sys_SetMode(state->resource_id, 0, 0, 0);

    amount = Inn_RoomPrice(mode);
    UiText_DrawQuantity(amount, 5);
    message_base = (s32)&Value_00000d1c;
    Sys_Do(message_base);
    state->window = UiWindow_CreateFar(0, 16, MESSAGE_WINDOW_ROWS, 4, 2);
    Shop_DrawMoney();

    if (Sys_Check(0) != 0) {
        Sys_Do(message_base
            + (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
        UiWindow_Close(state->window, 2);
    } else if ((u32)amount > gCell.limit) {
        Sys_Do(message_base
            + (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
        UiWindow_Close(state->window, 2);
    } else {
        UiWindow_Close(state->window, 2);
        Sys_Do(message_base
            + (INN_MESSAGE_STAY_COMPLETE - INN_MESSAGE_WELCOME));
        UiWindow_Close(win, 2);
        Inn_PlaySleep(amount);

        object = Scene_GetRecord(object_id);
        state->resource_id = *object->component->resource_id;
        win = Sys_SetMode(state->resource_id, 0, 0, 0);
        Sys_Do(message_base
            + (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
    }

    UiWindow_Close(win, 2);
    Inn_Cleanup();
    return 0;
}
