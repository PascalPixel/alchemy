/* Draft: main:080a3ef0, complete 444-byte extent.
 * Remaining: 22 halfwords differ solely in swapped r8/sl assignments
 * for the holder/saved-record and status-style lifetimes. */
#include "TYPES.H"
#include "ITEM.H"
#include "OWNER_STATE.H"

struct EquipPreviewMenu {
    u8 reserved_000[0x24];
    s32 status_window;
};

extern u8 Data_03001388[];
s32 Func_080072f0(void *dst, const void *src, s32 size, void *copy);
static __inline__ s32 CopyWords(void *dst, const void *src, s32 size)
{
    return Func_080072f0(dst, src, size, Data_03001388);
}

extern struct EquipPreviewMenu *gMenuWork;
struct OwnerInventoryState *Owner_GetStateFar(s32 owner);
void *Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *buffer);
s32 Inventory_RemoveFirstUnflagged(s32 owner);
s32 Inventory_AddItemFar(s32 owner, s32 item);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 style);

/* Temporarily equip the selected item on the target, draw its status, then
   restore the complete 0x14c-byte owner record. */
void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 target)
{
    struct EquipPreviewMenu *menu = gMenuWork;
    s32 style = 0;
    struct OwnerInventoryState *state = Owner_GetStateFar(owner);
    s32 item = state->inventory[slot];
    void *saved;
    s32 equipped;

    if (mode == 1)
        style = 0x100;
    switch (Item_Get(item & 0x1ff)->type) {
    case 0:
        Menu_DrawOwnerStatusPanel(menu->status_window, target, slot, style);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
        if (owner == target) {
            style |= 2;
            Menu_DrawOwnerStatusPanel(menu->status_window, target, slot, style);
        } else {
            state = Owner_GetStateFar(target);
            saved = Runtime_BumpAllocate(0x14c);
            CopyWords(saved, state, 0x14c);
            equipped = Inventory_RemoveFirstUnflagged(target);
            if (equipped != 0) {
                item &= ~0x200;
                equipped = Inventory_AddItemFar(target, item);
                if (equipped != -1) {
                    style |= 2;
                    Menu_DrawOwnerStatusPanel(menu->status_window, target, equipped, style);
                } else {
                    Menu_DrawOwnerStatusPanel(menu->status_window, target, slot, style);
                }
            } else {
                Menu_DrawOwnerStatusPanel(menu->status_window, target, slot, style);
            }
            CopyWords(state, saved, 0x14c);
            Runtime_BumpFree(saved);
        }
        break;
    case 6:
        if (target == owner) {
            style |= 4;
            Menu_DrawOwnerStatusPanel(menu->status_window, target, slot, style);
        } else {
            state = Owner_GetStateFar(target);
            saved = Runtime_BumpAllocate(0x14c);
            CopyWords(saved, state, 0x14c);
            equipped = Inventory_RemoveFirstUnflagged(target);
            if (equipped != 0) {
                equipped = Inventory_AddItemFar(target, item);
                if (equipped != -1) {
                    style |= 4;
                    Menu_DrawOwnerStatusPanel(menu->status_window, target, equipped, style);
                } else {
                    Menu_DrawOwnerStatusPanel(menu->status_window, target, slot, style);
                }
            } else {
                Menu_DrawOwnerStatusPanel(menu->status_window, target, slot, style);
            }
            CopyWords(state, saved, 0x14c);
            Runtime_BumpFree(saved);
        }
        break;
    }
}
