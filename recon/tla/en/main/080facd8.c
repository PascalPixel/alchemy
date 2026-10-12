/* Near miss, differ 2 of 32, one reordered pair: the native loads the menu before it builds the state 13
 * (ldr r7, [r3]; movs r1, #13); this form builds the 13 first. The tagged empty asm after the state store keeps
 * the saved counter's reload after that store (strb r1, [r3, #5]; ldr r2, [sp]); without it the reload comes
 * first as well (differ 4). An empty asm after the menu load fixes the first pair but pulls the frame's sub sp
 * before the load; a one-pass block around the load leaves movs r1, #13 two rows late; an empty one-pass block
 * before the load, after it, or before the loop changes nothing (differ 2 each, 2026-10-12). The permuter found
 * nothing in four minutes (2,374 candidates) on the form without the asm. The same pair is what is left of
 * PsynergyMenu_BuildPageResult (080fd80c): the native schedules a load ahead of the first instruction of a
 * constant. ItemMenu_Collect (080fad88) had it too and matches with an empty one-pass block before its first
 * loop. */
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
            /* FAKEMATCH: the empty asm keeps the counter's reload after the state store (differ 4 without it). */
            __asm__("");
        }
    }
}
