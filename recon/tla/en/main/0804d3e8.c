/* Near miss, differ 2 of 75 (one reordered pair), 2026-10-15: The Broken
 * Seal's Menu_CenterResourceEntries, written fresh from it. Remaining: the
 * native stores the row before it builds the count's offset (movs r1, #142);
 * this builds the offset first. An empty asm after the row store fixes the
 * order but holds the prologue's sub sp above it; one-pass blocks break
 * CSE (differ 38 to 42); operand-tied empty asms after each statement and
 * the device search found nothing lower.
 */
#include "mem.h"
#include "types.h"
#include "resource.h"
#include "resource_menu.h"
#include "ram_buffer.h"

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resource_base)
{
    struct ResourceMenuWork *menu = Ram_HeapSlots->menu_select_work;
    s32 x;
    s32 i;
    s32 count;

    menu->width = width + 2;
    menu->resource_base = resource_base;
    menu->row = row;
    count = menu->count;
    x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
    for (i = 0; i < menu->count; i++) {
        struct ResourceMenuEntry *entry = &menu->entries[i];

        entry->x = x * 8;
        entry->y = row * 8;
        x += 3;
    }
    menu->window = UiWindow_Create(x, row, menu->width, 3, 2);
}
