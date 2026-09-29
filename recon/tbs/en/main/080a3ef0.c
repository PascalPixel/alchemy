/* Draft: main:080a3ef0, complete 444-byte extent (2026-09-29).
 * The record copies now go through the checked Iwram_CopyWords entry
 * instead of a four-argument _call_via_r3 spelling; every copy call matches
 * (26 aligned edits, was 22 with the old spelling).
 * Remaining: r8 and sl are swapped. The ROM keeps owner (then the saved
 * record) in r8 and the style in sl; here style takes r8. Global allocation
 * priorities from the lreg dump: style 19 refs over 246 insns (0.309),
 * owner 4 over 28 (0.286), slot 9 over 103 (0.262), saved 8 over 92
 * (0.261), so style is allocated first. The ROM order needs style below
 * slot: fewer style references or a longer style lifetime. A permuter found
 * 11 edits only by moving the case-6 style update after its draw call,
 * which changes behaviour, so it was rejected.
 * 2026-09-29: alchemy permute scores it 230 (22 register-only, 2
 * reordered); 64,145 and then 39,695 candidates (the second run with the
 * shared-temporary rewrite) found none lower. The thumb order hands out
 * r8, then sl, then r9, so the ROM needs owner (4 refs, 28 insns: 2857)
 * allocated before style (19 refs, 246 insns: 3089): style with at most
 * 17 references, or owner with a fifth. Sharing the two owner-path draws
 * through one label drops style below slot as well (scores 480 and 885). */
#include "TYPES.H"
#include "ITEM.H"
#include "OWNER_STATE.H"
#include "IWRAM_CALL.H"

struct EquipPreviewMenu {
    u8 reserved_000[0x24];
    s32 status_window;
};

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
            Iwram_CopyWords(saved, state, 0x14c);
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
            Iwram_CopyWords(state, saved, 0x14c);
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
            Iwram_CopyWords(saved, state, 0x14c);
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
            Iwram_CopyWords(state, saved, 0x14c);
            Runtime_BumpFree(saved);
        }
        break;
    }
}
