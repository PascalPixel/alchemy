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

/* psynergy_menu/select_party_slot.c */
struct Rec5 { u8 pad[5]; unsigned int flag : 8; };
struct Cur { unsigned short mark : 8; };

extern struct PsynergyMenuState *gMenuWork;
void *Owner_GetStateFar(s32);
s32 UiMenu_SlideCursor(s32, s32);
void UiIcon_PrepareObject(void *cursor);

s32 PsynergyMenu_SelectPartySlot(s32 party_slot)
{
    struct PsynergyMenuState *menu = gMenuWork;
    void *icon;
    u8 byte_val;
    s32 owner_index;
    s32 combined_offset;
    s32 obj_off;
    s32 obj_id;
    void *obj_ptr;
    void *p456;
    s32 badge;
    s32 result;

    result = 0;
    icon = menu->pane_icon[party_slot];
    *(u8 *)(icon + 5) = 1;
    *(u16 *)(icon + 12) = result;
    ((struct Rec5 *)menu->cursor_icon)->flag = 13;
    owner_index = menu->tab_index[party_slot];
    byte_val = ((struct Cur *)&menu->owner_count)->mark;
    menu->tab_counts[party_slot] = byte_val;

    if (owner_index == -1) {
        menu->tab_index[party_slot] = 0;
        combined_offset = 0;
    } else {
        combined_offset = owner_index * 2;
        UiMenu_SlideCursor(owner_index * 24 - 10, 16);
    }

    obj_off = combined_offset + 520;
    obj_id = *(u16 *)((u8 *)menu + obj_off);
    obj_ptr = Owner_GetStateFar(obj_id);
    p456 = menu->psynergies;
    badge = PsynergyMenu_CollectActions(obj_ptr, p456, 2);
    menu->psynergy_count = (u8)badge;
    result = PsynergyMenu_SetupActionIcons(menu->owner_table, p456);

    icon = menu->pane_icon[party_slot];
    UiIcon_PrepareObject(icon);
    WaitFrames(1);
    return result;
}
