#include "types.h"
#include "scene.h"
#include "shop.h"

/* shop/service_price_2.c */
/* shop/service_price.c */
/* shop/price/service.c */
u8 *Runtime_GetObject(void);

s32 Shop_ServicePrice(s32 unused, s32 kind)
{
    u8 value = Runtime_GetObject()[0xF];
    s32 result = 0;

    if (kind == 0) {
        result = value * 20;
    } else if (kind == 1) {
        result = 10;
    } else if (kind == 2) {
        result = 50;
    } else if (kind == 3) {
        result = value * 10;
    }
    return result;
}

/* shop/unit/can_serve.c */
u8 *Runtime_GetObject(s32);

s32 Shop_CanServe(s32 entry_no, s32 kind)
{
    u8 *entry = Runtime_GetObject(entry_no);
    s32 result = 0;

    if ((kind == 0 && *(s16 *)(entry + 56) <= 0)
        || (kind == 1 && *(s8 *)(entry + 305) != 0)
        || (kind == 2 && entry[320] != 0)
        || (kind == 3 && *(s8 *)(entry + 304) != 0)) {
        result = 1;
    }
    return result;
}

/* shop/unit/count.c */
extern u8 *gIw;

s32 Shop_CountUnits(void)
{
    u8 *work = gIw;
    u8 *base;
    s32 active = 0;
    s32 variant = (s8)work[0x3AA];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(work + 0x3A7)) {
        base = work + 2;
        offset = 0x36C;
        do {
            if (Sys_Apply(*(s16 *)(base + offset), variant) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(work + 0x3A7));
    }

    return active;
}

/* shop/draw/msg_mode.c */
struct Work_080b2884 {
    u8 padding[0x3AA];
    s8 mode;
};

extern struct Work_080b2884 *gIw;
extern u8 Value_00000d24;
extern u8 Value_00000d2e;
extern u8 Value_00000d38;
extern u8 Value_00000d42;

s32 Shop_MsgByMode(s32 value)
{
    s8 mode = gIw->mode;

    if (mode == 1) {
        value += (u32)&Value_00000d2e - (u32)&Value_00000d24;
    }
    if (mode == 2) {
        value += (u32)&Value_00000d38 - (u32)&Value_00000d24;
    }
    if (mode == 3) {
        value += (u32)&Value_00000d42 - (u32)&Value_00000d24;
    }
    return value;
}

/* ui/message/message_show_resolved_and_wait.c */
/* ui/message/show_resolved_and_wait.c */
struct Work_080b28d4 {
    u8 unknown_000[0x3a4];
    u16 value_3a4;
};

void UiWork_FinalizePending(void);

void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(u32);

extern struct Work_080b28d4 *gIw;

void UiMessage_ShowResolvedAndWait(s32 value)
{
    s32 no;

    no = Ui_Check(gIw->value_3a4);
    UiWork_FinalizePending();
    value = Shop_MsgByMode(value);
    UiWork_Create(value, 5, 0, (no << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
}

/* ui/message/show_resolved_and_restore_state.c */
void UiWork_FinalizePending(void);

void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(u32);

extern void *gIw;

struct State080b2928 {
    u8 padding0[0x380];
    void *mode_state;
    u8 padding1[0x20];
    u16 value;
};

void UiMessage_ShowResolvedAndRestoreState(s32 arg0)
{
    struct State080b2928 *state;
    void **slot;
    s32 value;
    u8 saved;

    state = gIw;
    slot = &state->mode_state;
    saved = *(u8 *)((u8 *)*slot + 5);
    value = Ui_Check(state->value);
    arg0 = Shop_MsgByMode(arg0);
    *(u8 *)((u8 *)*slot + 5) = 13;
    UiWork_FinalizePending();
    UiWork_Create(arg0, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
    *(u8 *)((u8 *)state->mode_state + 5) = saved;
}

/* shop/sel/confirm.c */
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);

struct ShopCursorAnchor *Sys_Run(
    u32 resource,
    u32 flags,
    s32 window,
    s32 x,
    s32 y);

void UiWindow_Close(s32 window, s32 style);

/* Run the shop's yes/no party-action confirmation prompt for one unit. */
s32 Shop_ConfirmAct(s32 unit_id)
{
    s32 party_action = 0;
    s32 list_window = 0;
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *cursor_anchor;

    Sys_unk2_4();
    shop = SHOP_RUNTIME;
    shop->party_action = list_window;

    {
        s32 shown =
            *(u16 *)(*(u32 *)(*(u32 *)((u8 *)Sys_Check(unit_id) + 80) + 40));
        *(u16 *)((u8 *)shop + 0x3a4) = shown;
    }

    list_window = Sys_SetMode(*(u16 *)((u8 *)shop + 0x3a4), 0, 0, 0);
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    }
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(0, 0, 5, 5, 2);
        Sys_SetRect(2, 0, 0, list_window, -4, -4);
    }

    cursor_anchor = Sys_Run(
        *(u16 *)((u8 *)shop + 0x390),
        0x40000000,
        list_window,
        0,
        0);
    cursor_anchor->kind = 1;
    cursor_anchor->unknown_00[4] = 0;
    Sys_Place(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    Sys_Do(0xd21);

    shop->money_window = UiWindow_CreateFar(16, 11, 12, 4, 2);
    Shop_DrawMoney();

    for (;;) {
        party_action = Sys_unk2(party_action);
        shop->party_action = party_action;
        if (party_action == -1)
            break;

        {
            s32 base = 0xd24;
            s32 message = base;

            base = 0;
            Sys_Do(message);
            if (Shop_CountUnits() == 0) {
                Sys_Do(message + 1);
            } else {
                Sys_unk3_4();
            }
        }
        shop->party_action = 0;
        Sys_Place(&shop->cursor, -32, 112);
        Sys_Do(0xd22);
    }

    Sys_Do(0xd23);
    UiWindow_Close(shop->money_window, 2);
    UiWindow_Close(list_window, 2);
    Inn_Cleanup();
    return 0;
}
