/* Near miss, differ 4 of 32, two reordered pairs: the native loads the menu before it builds the state 13
 * (ldr r7, [r3]; movs r1, #13) and reloads the saved counter after the state store (strb r1, [r3, #5];
 * ldr r2, [sp]); this form has both the other way round. An empty asm after the menu load fixes the first pair
 * but pulls the frame's sub sp before the load; a one-pass block around the load leaves movs r1, #13 two rows
 * late. Neither moves the reload. The permuter found nothing in four minutes (2,374 candidates). The same
 * first pair is what is left of PsynergyMenu_BuildPageResult (080fd80c) and ItemMenu_Collect (080fad88): the
 * native schedules a load ahead of the first instruction of a constant. */
#include "types.h"
#include "ram_buffer.h"

struct ItemEntryIcon {
    u8 unknown_00[5];
    u8 state;
    u8 unknown_06[8];
    u8 slot;
};

struct ItemEntryMenu {
    u8 unknown_000[0x4c];
    struct ItemEntryIcon *entry_icons[32];
};

void UiIcon_PrepareObject(struct ItemEntryIcon *icon);

void Menu_HideEmptyEntryIcons(const u16 *items)
{
    struct ItemEntryMenu *menu = Ram_HeapSlots->menu_runtime;
    s32 slot;

    for (slot = 0; slot < 32; slot++) {
        if (items[slot] == 0) {
            UiIcon_PrepareObject(menu->entry_icons[slot]);
            menu->entry_icons[slot]->state = 13;
        }
    }
}
