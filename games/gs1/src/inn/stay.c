#include "types.h"
#include "gs1_edition.h"
#include "inn_runtime.h"
#include "shop.h"

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

extern struct FieldEffectState *Data_03001ebc;

s32 Func_08077158(s16 *);
void Func_08077230(s32);
struct FieldObject *Runtime_GetObject(s32);
void Func_08077128(s32);
void WaitFrames(s32);
void Func_0808a368(void);
void Func_0808a370(void);
void Audio_PlayCue(s32);
void AudioCommand_WaitForStateByteClear(void);
void Func_0808a360(void);

extern s8 Data_080b4ab6[];

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

extern struct InnGlobalState Data_02000240;
extern char Value_00000d1c;

void Func_080b010c(void);
void Func_080b0204(void);
void Func_080b04dc(s32 message_id);
s32 Func_080b0634(s32);
s32 Func_080b3210(s32);
void Func_080b3398(s32);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void UiWindow_Close(s32, s32);
s32 Func_080150f8(u16, s32, s32, s32);
void UiText_DrawQuantity(s32, s32);
struct InnObject *Scene_GetRecord(s32);

s32 Inn_RoomPrice(s32 mode)
{
    u8 *global = (u8 *)Data_03001f2c;
    u8 *base;
    s32 active = 0;
    s32 factor = Data_080b4ab6[mode];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(global + 0x3A7)) {
        base = global + 2;
        offset = 0x36C;
        do {
            if (*(s16 *)((u8 *)Runtime_GetObject(
                    *(s16 *)(base + offset)) + 56) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(global + 0x3A7));
    }

    return factor *active;
}

s32 Inn_CheckIn(s32 mode, s32 object_id)
{
    struct InnRuntimeState *state;
    struct InnObject *object;
    s32 win;
    s32 amount;
    s32 message_base;

    Func_080b010c();
    state = Data_03001f2c;
    state->active = 1;
    if (mode == 5)
        state->special_active = 1;

    object = Scene_GetRecord(object_id);
    state->resource_id = *object->component->resource_id;
    win = Func_080150f8(state->resource_id, 0, 0, 0);

    amount = Func_080b3210(mode);
    UiText_DrawQuantity(amount, 5);
    message_base = (s32)&Value_00000d1c;
    Func_080b04dc(message_base);
    state->window = UiWindow_CreateFar(0, 16, MESSAGE_WINDOW_ROWS, 4, 2);
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
        UiWindow_Close(win, 2);
        Func_080b3398(amount);

        object = Scene_GetRecord(object_id);
        state->resource_id = *object->component->resource_id;
        win = Func_080150f8(state->resource_id, 0, 0, 0);
        Func_080b04dc(message_base
            + (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
    }

    UiWindow_Close(win, 2);
    Func_080b0204();
    return 0;
}

void Inn_PlaySleep(s32 room_price)
{
    s16 objects[8];
    s32 count;
    s32 index;
    struct FieldObject *object;
    struct FieldEffectState *state;

    count = Func_08077158(objects);
    Func_08077230(-room_price);

    for (index = 0; index < count; index++) {
        object = Runtime_GetObject(objects[index]);
        if (object->x != 0) {
            object->x = object->saved_x;
            object->y = object->saved_y;
            Func_08077128(objects[index]);
        }
    }

    state = Data_03001ebc;
    state->effect = 0x209;
    state->delay = 60;
    WaitFrames(20);
    Func_0808a368();
    Func_0808a370();
    Audio_PlayCue(86);
    AudioCommand_WaitForStateByteClear();
    WaitFrames(10);
    Func_0808a360();
    Func_0808a370();
    WaitFrames(30);
    Data_03001ebc->delay = 16;
}
