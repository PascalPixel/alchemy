#include "inventory_menu.h"
#include "owner_state.h"

s32 InventoryMenu_GetItemQuantity(s32 owner_id, s32 item_id) {
    s32 slot_index;
    s32 quantity;
    u16 *slots;
    s32 encoded_item;
    u16 slot;
    s32 occupied_slot;
    s32 item_mask;
    s32 quantity_mask;

    quantity = 0;
    slots = (u16 *)OwnerState_GetFar(owner_id);
    item_mask = 0x1FF;
    quantity_mask = 0xF800;
    slot_index = 0;
    slots += 0x6C;
loop_1:
    encoded_item = *slots;
    slot = encoded_item;
    occupied_slot = slot;
    slots += 1;
    if ((occupied_slot != 0) && ((item_mask & encoded_item) == item_id)) {
        quantity = (u32) (quantity_mask & encoded_item) >> 0xB;
        quantity += 1;
    } else {
        slot_index += 1;
        if (slot_index <= 0xE) { goto loop_1; }
    }
    return quantity;
}
