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
    struct PsynergyMenuState *menu = gMenuWork;
    struct RenderOutput *icon;
    struct BattleUnit *owner;
    s32 tab;
    s32 result;

    result = 0;
    icon = menu->pane_icon[party_slot];
    icon->active = 1;
    icon->unknown_0c = result;
    menu->cursor_icon->active = 13;
    tab = menu->tab_index[party_slot];
    menu->tab_counts[party_slot] = menu->owner_count;
    if (tab == -1) {
        menu->tab_index[party_slot] = 0;
        tab = 0;
    } else {
        UiMenu_SlideCursor(tab * 24 - 10, 16);
    }
    owner = Owner_GetStateFar(menu->owner_table[tab]);
    menu->psynergy_count = (u8)PsynergyMenu_CollectActions(
        owner, menu->psynergies, 2);
    result = PsynergyMenu_SetupActionIcons(menu->owner_table, menu->psynergies);
    icon = menu->pane_icon[party_slot];
    UiIcon_PrepareObject(icon);
    WaitFrames(1);
    return result;
}
