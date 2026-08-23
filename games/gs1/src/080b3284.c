#include "types.h"
#include "gs1_edition.h"
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

struct RuntimeState_080b3284 {
    u8 padding_000[0x0c];
    s32 window;
    u8 padding_010[0x394];
    u16 resource_id;
    u8 padding_3a6[3];
    u8 active;
    u8 padding_3aa[2];
    u8 special_active;
};

struct GlobalState_080b3284 {
    u8 padding_00[0x10];
    u32 limit;
};

struct ObjectComponent_080b3284 {
    u8 padding_00[0x28];
    u16 *resource_id;
};

struct Object_080b3284 {
    u8 padding_00[0x50];
    struct ObjectComponent_080b3284 *component;
};

extern struct RuntimeState_080b3284 *Data_03001f2c;
extern struct GlobalState_080b3284 Data_02000240;
extern char Value_00000d1c;

void Func_080b010c(void);
void Func_080b0204(void);
void Func_080b04dc(s32 message_id);
s32 Func_080b0634(s32);
#define Inn_CalculateRoomPrice Func_080b3210
#define Inn_PlaySleepSequence Func_080b3398
s32 Inn_CalculateRoomPrice(s32);
void Inn_PlaySleepSequence(s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
void UiWindow_Close(s32, s32);
s32 Func_080150f8(u16, s32, s32, s32);
void UiText_DrawQuantity(s32, s32);
struct Object_080b3284 *Scene_GetRecord(s32);

#define Inn_CheckIn Func_080b3284

s32 Func_080b3284(s32 mode, s32 object_id)
{
    struct RuntimeState_080b3284 *state;
    struct Object_080b3284 *object;
    s32 resource_window;
    s32 amount;
    s32 message_base;

    Func_080b010c();
    state = Data_03001f2c;
    state->active = 1;
    if (mode == 5)
        state->special_active = 1;

    object = Scene_GetRecord(object_id);
    state->resource_id = *object->component->resource_id;
    resource_window = Func_080150f8(state->resource_id, 0, 0, 0);

    amount = Inn_CalculateRoomPrice(mode);
    UiText_DrawQuantity(amount, 5);
    message_base = (s32)&Value_00000d1c;
    Func_080b04dc(message_base);
    state->window = Func_08015010(0, 16, MESSAGE_WINDOW_ROWS, 4, 2);
    Shop_DrawMoney();

    if (Func_080b0634(0) != 0) {
        Func_080b04dc(message_base
            + (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
        UiWindow_Close(state->window, 2);
    } else if ((u32)amount > Data_02000240.limit) {
        Func_080b04dc(message_base
            + (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
        UiWindow_Close(state->window, 2);
    } else {
        UiWindow_Close(state->window, 2);
        Func_080b04dc(message_base
            + (INN_MESSAGE_STAY_COMPLETE - INN_MESSAGE_WELCOME));
        UiWindow_Close(resource_window, 2);
        Inn_PlaySleepSequence(amount);

        object = Scene_GetRecord(object_id);
        state->resource_id = *object->component->resource_id;
        resource_window = Func_080150f8(state->resource_id, 0, 0, 0);
        Func_080b04dc(message_base
            + (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
    }

    UiWindow_Close(resource_window, 2);
    Func_080b0204();
    return 0;
}
