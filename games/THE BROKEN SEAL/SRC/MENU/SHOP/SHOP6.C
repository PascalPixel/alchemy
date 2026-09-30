#include "TYPES.H"
#include "SYSTEM.H"
#include "UI.H"
#include "SHOP.H"

struct ShopServiceWork {
    u8 unk_000[0x380];
    void *mode_state;
    u8 unk_384[0x20];
    u16 value;
    u8 unk_3a6[4];
    s8 mode;
};

u8 *Owner_GetStateFar(s32);
extern struct ShopServiceWork *gMenuWork;
s32 Shop_CanServe(s32 selection, s32 variant);
extern u8 MsgReviveService;
extern u8 MsgCurePoisonService;
extern u8 MsgExorcismService;
extern u8 MsgRemoveCurseService;
s32 BattleFx_GetResourceIdFar(u16);
void UiWork_FinalizePendingCoreFar(void);
s32 Shop_MsgByMode(s32 value);
void UiText_OpenMessageWindowFar(s32, s32, s32, s32);

extern u8 Data_03001f2c[];
extern u8 MsgSanctumWelcome[];
extern u8 MsgSanctumMoreAid[];
extern u8 MsgSanctumFarewell[];
s32 Object_GetByIdFar(s32 unit_id);
s32 UiWindow_CreateWithSideObjectFar(s32 resource, s32 x, s32 y, s32 flags);
void SideObject_CreateFar(s32 a, s32 b, s32 c, s32 window, s32 d, s32 e);
struct ShopCursorAnchor *RenderOutput_CreateFar(
    u32 resource,
    u32 flags,
    s32 window,
    s32 x,
    s32 y);
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 target_x, s32 target_y);
void UiMessage_ShowResolvedAndWait(s32 message);
void Shop_InitializeCursorWork(void);
void Inn_Cleanup(void);
s32 Shop_CountUnits(void);
void Sanctum_RunPartyService(void);
s32 Menu_SelectEntry19To1cFar(s32 prev);
void UiWork_FinalizeFar(s32 window, s32 style);

s32 Shop_ServicePrice(s32 entry_no, s32 kind)
{
    u8 value = Owner_GetStateFar(entry_no)[0xF];
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

s32 Shop_CanServe(s32 entry_no, s32 kind)
{
    u8 *entry = Owner_GetStateFar(entry_no);
    s32 result = 0;

    if ((kind == 0 && *(s16 *)(entry + 56) <= 0)
        || (kind == 1 && *(s8 *)(entry + 305) != 0)
        || (kind == 2 && entry[320] != 0)
        || (kind == 3 && *(s8 *)(entry + 304) != 0)) {
        result = 1;
    }
    return result;
}

s32 Shop_CountUnits(void)
{
    u8 *work = (u8 *)gMenuWork;
    u8 *base;
    s32 active = 0;
    s32 variant = (s8)work[0x3AA];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(work + 0x3A7)) {
        base = work + 2;
        offset = 0x36C;
        do {
            if (Shop_CanServe(*(s16 *)(base + offset), variant) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(work + 0x3A7));
    }

    return active;
}

s32 Shop_MsgByMode(s32 value)
{
    s8 mode = gMenuWork->mode;

    if (mode == 1) {
        value += (u32)&MsgCurePoisonService - (u32)&MsgReviveService;
    }
    if (mode == 2) {
        value += (u32)&MsgExorcismService - (u32)&MsgReviveService;
    }
    if (mode == 3) {
        value += (u32)&MsgRemoveCurseService - (u32)&MsgReviveService;
    }
    return value;
}

void UiMessage_ShowResolvedAndWait(s32 value)
{
    s32 no;

    no = BattleFx_GetResourceIdFar(gMenuWork->value);
    UiWork_FinalizePendingCoreFar();
    value = Shop_MsgByMode(value);
    UiText_OpenMessageWindowFar(value, 5, 0, (no << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
}

void UiMessage_ShowResolvedAndRestoreState(s32 arg0)
{
    struct ShopServiceWork *state;
    void **slot;
    s32 value;
    u8 saved;

    state = gMenuWork;
    slot = &state->mode_state;
    saved = *(u8 *)((u8 *)*slot + 5);
    value = BattleFx_GetResourceIdFar(state->value);
    arg0 = Shop_MsgByMode(arg0);
    *(u8 *)((u8 *)*slot + 5) = 13;
    UiWork_FinalizePendingCoreFar();
    UiText_OpenMessageWindowFar(arg0, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
    *(u8 *)((u8 *)state->mode_state + 5) = saved;
}

/* Run the shop's yes/no party-action confirmation prompt for one unit. */
s32 Shop_ConfirmAct(s32 unit_id)
{
    s32 party_action = 0;
    s32 list_window = 0;
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *cursor_anchor;

    Shop_InitializeCursorWork();
    shop = ((struct ShopRuntime *)gMenuWork);
    shop->party_action = list_window;

    {
        s32 shown =
            *(u16 *)(*(u32 *)(*(u32 *)((u8 *)Object_GetByIdFar(unit_id) + 80) + 40));
        *(u16 *)((u8 *)shop + 0x3a4) = shown;
    }

    list_window = UiWindow_CreateWithSideObjectFar(*(u16 *)((u8 *)shop + 0x3a4), 0, 0, 0);
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    }
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(0, 0, 5, 5, 2);
        SideObject_CreateFar(2, 0, 0, list_window, -4, -4);
    }

    cursor_anchor = RenderOutput_CreateFar(
        *(u16 *)((u8 *)shop + 0x390),
        0x40000000,
        list_window,
        0,
        0);
    cursor_anchor->kind = 1;
    cursor_anchor->unknown_00[4] = 0;
    ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    UiMessage_ShowResolvedAndWait((s32)MsgSanctumWelcome);

#if defined(TBS_EDITION_JA)
    shop->money_window = UiWindow_CreateFar(16, 11, 11, 4, 2);
#else
    shop->money_window = UiWindow_CreateFar(16, 11, 12, 4, 2);
#endif
    Shop_DrawMoney();

    for (;;) {
        party_action = Menu_SelectEntry19To1cFar(party_action);
        shop->party_action = party_action;
        if (party_action == -1)
            break;

        {
            s32 base = (s32)&MsgReviveService;
            s32 message = base;

            base = 0;
            UiMessage_ShowResolvedAndWait(message);
            if (Shop_CountUnits() == 0) {
                UiMessage_ShowResolvedAndWait(message + 1);
            } else {
                Sanctum_RunPartyService();
            }
        }
        shop->party_action = 0;
        ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
        UiMessage_ShowResolvedAndWait((s32)MsgSanctumMoreAid);
    }

    UiMessage_ShowResolvedAndWait((s32)MsgSanctumFarewell);
    UiWork_FinalizeFar(shop->money_window, 2);
    UiWork_FinalizeFar(list_window, 2);
    Inn_Cleanup();
    return 0;
}
