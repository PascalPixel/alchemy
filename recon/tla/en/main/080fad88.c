/* Near miss, differ 10 of 34, all from one reordered pair: the native loads the first zero before it copies items
 * (ldr r2; adds r3, r5, #0), this form copies first, as The Broken Seal does. That ldr two bytes earlier is what
 * puts the pool of the two halfword zeros at the second loop top with a branch around it; here it lands inside
 * the loop after the copy. An empty asm between the loops and before count = 0 places the pool as the native
 * (differ 2, only the pair left); after count = 0 it is differ 6; a hand-written downward clear is differ 28. */
#include "types.h"
#include "ram_buffer.h"
#include "owner_state.h"

struct ItemEntryIcon {
    u8 unknown_00[5];
    u8 state;
    u8 unknown_06[8];
    u8 slot;
};

struct ItemEntryMenu {
    u8 unknown_000[0x24];
    s32 item_window;
    u8 unknown_028[0x24];
    struct ItemEntryIcon *entry_icons[32];
    u8 unknown_0cc[0xf8];
    u16 items[32];
    u8 unknown_204[0x10];
    u8 item_count;
};

void UiIcon_PrepareObject(struct ItemEntryIcon *icon);
void Func_08038288(s32 mode, s32 id, s32 slot, s32 flags);
void ItemMenu_RefreshEntry(s32 mode);

s32 ItemMenu_Collect(struct OwnerInventoryState *owner, u16 *items, s32 mode)
{
    s32 count;
    s32 i;

    for (i = 0; i < 32; i++) {
        items[i] = 0;
    }
    count = 0;
    for (i = 0; i < 15; i++) {
        items[i] = 0;
        if (owner->inventory[i] != 0) {
            items[count] = owner->inventory[i];
            count++;
        }
    }
    return count;
}
