#include "scene.h"
#include "item_menu.h"
#include "global_cells.h"
#include "types.h"
#include "gs1_edition.h"

/* item_menu/page_result.c */
s32 Runtime_GetObject(s32);
s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);

static __inline__ u8 LoadByte(s32 base, s32 offset)
{
    return *(u8 *)(base + offset);
}

static __inline__ s8 LoadSignedByte(s32 base, s32 offset)
{
    return *(s8 *)(base + offset);
}

s32 ItemMenu_PageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    s32 base = *(s32 *)ADDR_03001F2C;
    s32 offset = index + 0x218;
    s32 entries = base + 2;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    limit = ItemMenu_Count(LoadByte(entries, offset));
    encoded = Runtime_GetObject(LoadByte(entries, offset));
    value = LoadSignedByte(base, LoadByte(entries, offset) + 0x260);
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    quotient = FixedPoint_Ratio(value, 5);
    remainder = Modulo(value, 5);
    groups = FixedPoint_Ratio(limit, 5);
    if (Modulo(limit, 5) != 0) {
        groups++;
    }
    result->owner_state = encoded;
    result->page = quotient;
    result->page_count = groups;
    result->row = remainder;
    result->entry_count = limit;
    result->selected_index = value;
    return 1;
}

/* item_menu/draw/detail.c */
extern void *gIw;
extern u8 Value_00000075;

s32 ItemMenu_DrawItemDetailPage(s32 arg0, s32 arg1, void *state)
{
    void *menu;
    s32 page;
    s32 combined;
    s32 off;
    s32 row;

    page = *(s32 *)(state + 8);
    menu = gIw;
    combined = page * 5;
    combined += *(s32 *)(state + 16);
    *(s32 *)(state + 24) = combined;

    Sys_Do(*(s32 *)(menu + 44));
    Sys_unk2_2(1);

    combined = *(s32 *)(state + 24);
    off = combined * 2 + 456;
    if (*(u16 *)((char *)menu + off) != 0) {
        s32 masked = (*(u16 *)((char *)menu + off) & 0x1ff) + (s32)&Value_00000075;
        Sys_SetMode(masked, *(s32 *)(menu + 44), 0, 0);
    }

    row = 0;
    do {
        if (row == *(s32 *)(state + 16)) {
            Sys_SetRect(*(s32 *)(menu + 32), 1, row * 2 + 1, 14, 1, 14);
        } else {
            Sys_SetRect(*(s32 *)(menu + 32), 1, row * 2 + 1, 14, 1, 15);
        }
        row++;
    } while (row <= 4);

    Sys_unk2_2(1);
    return 1;
}

/* item_menu/draw/name_page.c */
#if defined(GS1_EDITION_JA)
#define PAGE_X  120
#define ENTRY_X 32
#else
#define PAGE_X  116
#define ENTRY_X 24
#endif

extern u8 Value_00000182;

void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);
void UiWindow_Commit(s32 window);

s32 ItemMenu_DrawNamePage(
    s32 window,
    s32 unused,
    const struct MenuResult *state)
{
    struct ItemMenuState *menu =
        *(struct ItemMenuState **)ADDR_03001F2C;
    u32 page;
    u32 first_entry;
    u32 visible_count;
    u8 row;
    const u16 *item_id;

    (void)unused;

    UiWindow_Commit(window);
    Sys_SetRange(window, 0, 11, 16, 11);

    page = state->page;
    first_entry = page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

    Menu_SetPageIcons(5, first_entry, window, PAGE_X, 34);
    Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 15);

    row = 0;
    if (visible_count > row) {
        item_id = &menu->items[first_entry];
        do {
            UiText_DrawAt(
                (item_id[0] & 0x1ff) + (s32)&Value_00000182,
                menu->item_window,
                ENTRY_X,
                row * 16 + 8
            );
            row++;
            item_id++;
        } while (visible_count > row);
    }

    return 1;
}

s32 ItemMenu_NamePageOk(void)
{
    return 1;
}
