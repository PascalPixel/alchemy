#include "EDITION.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "UI.H"
#include "SHOP.H"
#include "PARTY_STATE.H"
#include "GLOBAL_CELLS.H"
#include "SOUND_IDS.H"
#include "BATTLE_RUNTIME.H"
#include "OBJECT_RUNTIME.H"

extern struct ShopRuntime *gMenuWork;
s32 Shop_CanServe(s32 selection, s32 variant);
extern u8 MsgReviveService;
extern u8 MsgCurePoisonService;
extern u8 MsgExorcismService;
extern u8 MsgRemoveCurseService;
s32 BattleFx_GetResourceIdFar(u16);
void UiWork_FinalizePendingCoreFar(void);
s32 Shop_MsgByMode(s32 value);
void UiText_OpenMessageWindowFar(s32, s32, s32, s32);
extern u8 MsgSanctumWelcome[];
extern u8 MsgSanctumMoreAid[];
extern u8 MsgSanctumFarewell[];
struct ObjectRuntime *Object_GetByIdFar(s32 unit_id);
s32 UiWindow_CreateWithSideObjectFar(s32 resource, s32 x, s32 y, s32 flags);
void SideObject_CreateFar(s32 a, s32 b, s32 c, s32 window, s32 d, s32 e);
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 target_x, s32 target_y);
void UiMessage_ShowResolvedAndWait(s32 message);
void Shop_InitializeCursorWork(void);
void Inn_Cleanup(void);
s32 Shop_CountUnits(void);
s32 Menu_SelectEntry19To1cFar(s32 prev);
void UiWork_FinalizeFar(s32 window, s32 style);

extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
void UiWork_PushValueSlotFar(s32 value, s32 slot);
s32 Shop_CanServe(s32 unit_id, s32 kind);
s32 Shop_ServicePrice(s32 unit_id, s32 kind);
void BattleUnit_ResetStateByMode(s32 unit_id, s32 mode);
void Shop_HiliteUnit(s32 enabled, s32 selected);
void Shop_DrawSelMsg(s32 target, s32 selection);
void UiMessage_ShowResolvedAndRestoreState(s32 message);
s32 UiMessage_ShowChoiceVariant(s32 arg0);
void Shop_RunPartyMemberIconBurst(s32 member);
s32 Party_AdjustSixDigitCounterAFar(s32 amount);
/* Four-word void helper; this caller keeps its legacy extra word. */
void PsynergyMenu_InitializeEntryObjectsFar();
void Menu_ReleaseEntryObjectsFar(void);
extern char MsgReviveDonation;
extern u8 MsgWhoToRevive[];

s32 Sanctum_RunPartyService(void);

s32 Shop_ServicePrice(s32 entry_no, s32 kind)
{
    u8 value = Owner_GetStateFar(entry_no)->level;
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
    struct BattleUnit *entry = Owner_GetStateFar(entry_no);
    s32 result = 0;

    if ((kind == 0 && entry->hp <= 0)
        || (kind == 1 && entry->poison != 0)
        || (kind == 2 && entry->evil_spirit != 0)
        || (kind == 3 && (s8)entry->restraint != 0)) {
        result = 1;
    }
    return result;
}

s32 Shop_CountUnits(void)
{
    struct ShopRuntime *shop = gMenuWork;
    s32 active = 0;
    s32 index;

    for (index = 0; index < shop->party_member_count; index++) {
        if (Shop_CanServe(shop->party_member_ids[index], shop->party_action))
            active++;
    }
    return active;
}

s32 Shop_MsgByMode(s32 value)
{
    s8 mode = gMenuWork->party_action;

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

    no = BattleFx_GetResourceIdFar(gMenuWork->keeper_resource);
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
    struct ShopRuntime *state;
    struct RenderOutput **slot;
    s32 value;
    u8 saved;

    state = gMenuWork;
    slot = &state->cursor.anchor;
    saved = (*slot)->active;
    value = BattleFx_GetResourceIdFar(state->keeper_resource);
    arg0 = Shop_MsgByMode(arg0);
    (*slot)->active = 13;
    UiWork_FinalizePendingCoreFar();
    UiText_OpenMessageWindowFar(arg0, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
    state->cursor.anchor->active = saved;
}

/* Run the shop's yes/no party-action confirmation prompt for one unit. */
s32 Shop_ConfirmAct(s32 unit_id)
{
    s32 party_action = 0;
    s32 list_window = 0;
    struct ShopRuntime *shop;
    struct RenderOutput *cursor_anchor;

    Shop_InitializeCursorWork();
    shop = ((struct ShopRuntime *)gMenuWork);
    shop->party_action = list_window;

    {
        s32 shown =
            (u16)((struct AnimationObject *)Object_GetByIdFar(unit_id)->animation)->entries[0]->anim_id;
        shop->keeper_resource = shown;
    }

    list_window = UiWindow_CreateWithSideObjectFar(shop->keeper_resource, 0, 0, 0);
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    }
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(0, 0, 5, 5, 2);
        SideObject_CreateFar(2, 0, 0, list_window, -4, -4);
    }

    cursor_anchor = RenderOutput_CreateFar(
        shop->cursor_icon,
        0x40000000,
        (struct RenderInput *)list_window,
        0,
        0);
    cursor_anchor->active = 1;
    cursor_anchor->kind = 0;
    ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    UiMessage_ShowResolvedAndWait((s32)MsgSanctumWelcome);

#if EDITION_INTERNATIONAL
    shop->money_window = UiWindow_CreateFar(16, 11, 12, 4, 2);
#else
    shop->money_window = UiWindow_CreateFar(16, 11, 11, 4, 2);
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

/* Lets the player choose which party member receives the chosen sanctum
 * service, starting on the first member who needs it; left and right move the
 * cursor and B leaves. A on a member who needs the service quotes the
 * donation. Declining it, or being unable to pay, says so and starts the
 * choice again; paying treats the member and, while others still need the
 * service, starts the choice again. */
s32 Sanctum_RunPartyService(void)
{
    struct ShopRuntime *shop = ((struct ShopRuntime *)gMenuWork);
    s32 price_window;
    s32 redraw;
    s32 kind;
    s32 list_window;
    s32 selection;
    s32 unit_id;
    s32 retry;
    s32 price;
    s32 message;

    price_window = 0;
    redraw = 1;
    kind = shop->party_action;
    UiMessage_ShowResolvedAndWait((s32)MsgWhoToRevive);

    list_window = UiWindow_CreateFar(1, 12, 13, 3, 2);
    shop->cursor.anchor->active = 4;
    shop->mode = redraw;
    PsynergyMenu_InitializeEntryObjectsFar(list_window, 2, 0, 8, price_window);
#if defined(TBS_EDITION_DE)
    price_window = UiWindow_CreateFar(0, 16, 30, 3, 2);
#elif defined(TBS_EDITION_FR)
    price_window = UiWindow_CreateFar(1, 16, 25, 3, 2);
#else
    price_window = UiWindow_CreateFar(1, 16, 23, 3, 2);
#endif

    selection = 0;
    unit_id = 0;
    retry = 0;
    while (selection < shop->party_member_count) {
        unit_id = shop->party_member_ids[selection];
        if (Shop_CanServe(unit_id, kind) != 0)
            break;
        selection++;
    }

    redraw = 1;
    for (;;) {
        if (retry != 0) {
            retry = 0;
            UiMessage_ShowResolvedAndWait((s32)MsgWhoToRevive);
            redraw = 1;
            selection = 0;
            while (selection < shop->party_member_count) {
                unit_id = shop->party_member_ids[selection];
                if (Shop_CanServe(unit_id, kind) != 0)
                    break;
                selection++;
            }
        }

        if (redraw != 0) {
            redraw = 0;
            selection = (selection + shop->party_member_count) % shop->party_member_count;
            unit_id = shop->party_member_ids[selection];
            Shop_PlaceCursor((void *)list_window, selection * 24 - 12, 0);
            shop->mode = 3;
            Shop_HiliteUnit(list_window, selection);
            Shop_DrawSelMsg(price_window, unit_id);
        }

        if ((gKeyState & 1) != 0) {
            WaitFrames(1);
            price = Shop_ServicePrice(unit_id, kind);
            if (Shop_CanServe(unit_id, kind) == 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                continue;
            }
            UiWork_PushValueSlotFar(unit_id, 1);
            UiWork_PushValueSlotFar(price, 5);
            message = (s32)&MsgReviveDonation;
            UiMessage_ShowResolvedAndWait(message);
            if (UiMessage_ShowChoiceVariant(0) != 0) {
                UiMessage_ShowResolvedAndRestoreState(message + 2);
                retry = 1;
                continue;
            }
            if ((u32)price > (u32)gGameState.coins) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                UiMessage_ShowResolvedAndRestoreState(message + 1);
                retry = 1;
                continue;
            }
            UiWork_PushValueSlotFar(unit_id, 1);
            UiMessage_ShowResolvedAndWait(message + 3);
            UiWork_FinalizePendingCoreFar();
            BattleUnit_ResetStateByMode(unit_id, kind);
            Shop_RunPartyMemberIconBurst(selection);
            Party_AdjustSixDigitCounterAFar(-price);
            Shop_DrawMoney();
            UiWork_PushValueSlotFar(unit_id, 1);
            UiMessage_ShowResolvedAndWait(message + 4);
            if (Shop_CountUnits() != 0) {
                retry = 1;
                continue;
            }
            break;
        } else if ((gKeyState & 2) != 0) {
            Audio_PlayCue(SOUND_MENU_CANCEL);
            break;
        } else {
            if ((gKeysRepeat & 0x20) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
                selection -= 1;
            }
            if ((gKeysRepeat & 0x10) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
                selection += 1;
            }
            WaitFrames(1);
            continue;
        }
    }

    Menu_ReleaseEntryObjectsFar();
    UiWork_FinalizeFar(price_window, 2);
    UiWork_FinalizeFar(list_window, 2);
    WaitFrames(1);
    return 0;
}
