#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "btlsys.h"
#include "types.h"
#include "psynergy_menu.h"
#include "system.h"
#include "battle_unit.h"


s32 PsynergyMenu_ClassifySelectedPsynergy(void)
{
    struct BtlAct *psynergy;
    s32 diff;
    s32 ret;

    psynergy = BtlActGet(
        0x3fff & ((struct PsynergyMenuState *)gMenuWork)->pane_action[0]);
    if (BattleFx_HasMatchingEvent5Far(psynergy->type_0c) != 0) {
        return 0;
    }
    ret = 2;
    if (psynergy->rng != 0xff) {
        u8 kind = psynergy->target_mode;
        diff = kind ^ 2;
        ret = (0 - diff) | diff;
        ret = (u32)ret >> 0x1f;
        ret = 1 - ret;
    }
    return ret;
}


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
    struct BtlUnit *owner;
    u16 *actions;
    u8 *counts;
    s32 badge;
    s32 result;
    s32 icon_offset2;

    result = 0;
    icon = *(struct RenderOutput **)((u8 *)menu + icon_offset);
    icon->active = 1;
    icon->anim_step = result;
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
    owner = UnitGetFar(owner_id);
    actions = menu->psynergies;
    badge = PsynergyMenu_CollectActions(owner, actions, 2);
    menu->psynergy_count = badge;
    result = PsynergyMenu_SetupActionIcons(menu->owner_table, actions);

    icon_offset2 = party_slot * sizeof(menu->pane_icon[0]) +
                   ((u8 *)menu->pane_icon - (u8 *)menu);
    icon = *(struct RenderOutput **)((u8 *)menu + icon_offset2);
    UiIcon_PrepareObject(icon);
    WaitFrames(1);
    return result;
}
