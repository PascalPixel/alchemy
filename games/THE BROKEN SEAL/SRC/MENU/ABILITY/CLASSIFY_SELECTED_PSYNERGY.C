#include "BATTLE_RUNTIME.H"
#include "TYPES.H"
#include "PSYNERGY_MENU.H"
#include "SYSTEM.H"
#include "BATTLE_UNIT.H"

/* menu/psynergy_menu/classify_selected_psynergy.c */
struct BattleAction *BattleAction_Get(s32 action);
s32 BattleFx_HasMatchingEvent5Far(u8 effect);

s32 PsynergyMenu_ClassifySelectedPsynergy(void)
{
    struct BattleAction *psynergy;
    s32 diff;
    s32 ret;

    psynergy = BattleAction_Get(
        (s32)(0x3fff &
              gMenuWork
                  ->pane_action[0]));
    if (BattleFx_HasMatchingEvent5Far(psynergy->type_0c) != 0) {
        return 0;
    }
    ret = 2;
    if (psynergy->range != 0xff) {
        u8 kind = psynergy->target_mode;
        diff = kind ^ 2;
        ret = (0 - diff) | diff;
        ret = (s32)((u32)ret >> 0x1f);
        ret = 1 - ret;
    }
    return ret;
}

/* Select the current party tab and build that owner's Psynergy icons. */
void UiMenu_SlideCursor(s32 x, s32 y);
void UiIcon_PrepareObject(struct RenderOutput *icon);

s32 PsynergyMenu_SelectPartySlot(s32 party_slot)
{
    /* FAKEMATCH: retain the original scalar tab/icon address lanes. Direct indexed owner fields change the 244-byte native object to 246 bytes and reorder loads around the cursor call. */
    struct PsynergyMenuState *menu = gMenuWork;
    s32 tab_offset = party_slot + ((u8 *)menu->tab_index - (u8 *)menu);
    s32 icon_offset = party_slot * sizeof(menu->pane_icon[0]) +
                      ((u8 *)menu->pane_icon - (u8 *)menu);
    struct RenderOutput *icon;
    u8 byte_val;
    s32 tab;
    s32 owner_offset;
    s32 table_offset;
    s32 owner_id;
    struct BattleUnit *owner;
    u16 *actions;
    u8 *counts;
    s32 badge;
    s32 result;
    s32 icon_offset2;

    result = 0;
    icon = *(struct RenderOutput **)((u8 *)menu + icon_offset);
    icon->active = 1;
    icon->unknown_0c = result;
    menu->cursor_icon->active = 13;
    counts = (u8 *)menu + ((u8 *)menu->tab_counts - (u8 *)menu->tab_index);
    tab = *(s8 *)((u8 *)menu + tab_offset);
    byte_val = menu->owner_count;
    counts[tab_offset] = byte_val;

    if (tab == -1) {
        *((u8 *)menu + tab_offset) = 0;
        owner_offset = 0;
    } else {
        owner_offset = tab * sizeof(menu->owner_table[0]);
        UiMenu_SlideCursor(tab * 24 - 10, 16);
    }

    table_offset = owner_offset + ((u8 *)menu->owner_table - (u8 *)menu);
    owner_id = *(u16 *)((u8 *)menu + table_offset);
    owner = Owner_GetStateFar(owner_id);
    actions = menu->psynergies;
    badge = (u8)PsynergyMenu_CollectActions(owner, actions, 2);
    menu->psynergy_count = (u8)badge;
    result = PsynergyMenu_SetupActionIcons(menu->owner_table, actions);

    icon_offset2 = party_slot * sizeof(menu->pane_icon[0]) +
                   ((u8 *)menu->pane_icon - (u8 *)menu);
    icon = *(struct RenderOutput **)((u8 *)menu + icon_offset2);
    UiIcon_PrepareObject(icon);
    WaitFrames(1);
    return result;
}
