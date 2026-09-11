#include "types.h"
#include "scene.h"
#include "gs1_edition.h"
#include "inn.h"
#include "shop.h"

/* inn/room_price.c */
extern u8 *gIw;
extern s8 gRom[];

u8 *Runtime_GetObject(s32);

s32 Inn_RoomPrice(s32 mode)
{
    u8 *global = gIw;
    u8 *base;
    s32 active = 0;
    s32 factor = gRom[mode];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(global + 0x3A7)) {
        base = global + 2;
        offset = 0x36C;
        do {
            if (*(s16 *)(Runtime_GetObject(
                    *(s16 *)(base + offset)) + 56) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(global + 0x3A7));
    }

    return factor *active;
}

/* inn/check_in.c */
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

/* inn/sleep.c */
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

extern struct FieldEffectState *gWork;

struct FieldObject *Runtime_GetObject(s32);

void WaitFrames(s32);

void Audio_PlayCue(s32);
void AudioCommand_WaitForStateByteClear(void);

void Inn_PlaySleep(s32 room_price)
{
    s16 objects[8];
    s32 count;
    s32 index;
    struct FieldObject *object;
    struct FieldEffectState *state;

    count = Sys_Check(objects);
    Sys_Do(-room_price);

    for (index = 0; index < count; index++) {
        object = Runtime_GetObject(objects[index]);
        if (object->x != 0) {
            object->x = object->saved_x;
            object->y = object->saved_y;
            Sys_unk2_2(objects[index]);
        }
    }

    state = gWork;
    state->effect = 0x209;
    state->delay = 60;
    WaitFrames(20);
    Sys_Run();
    Sys_unk2_4();
    Audio_PlayCue(86);
    AudioCommand_WaitForStateByteClear();
    WaitFrames(10);
    Sys_unk3_4();
    Sys_unk2_4();
    WaitFrames(30);
    gWork->delay = 16;
}
