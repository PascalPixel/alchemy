#include "TYPES.H"

void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 style);

#define PAGE_LABEL_FIRST 49
#define PAGE_CAP_LEFT 0xf128
#define PAGE_CAP_RIGHT 0xf129

void Menu_DrawPageIndicator(
    s32 window,
    s32 item_count,
    s32 page_size,
    s32 selected_page,
    s32 right_edge
)
{
    s32 page_count;
    s32 page;
    s32 x;
    s32 tile;

    x = right_edge;
    tile = PAGE_LABEL_FIRST;
    page_count = Math_Div(item_count, page_size);
    if (Math_Mod(item_count, page_size) != 0)
        page_count++;

    x -= page_count;
    if (page_count > 1) {
        UiWindow_SetTilemapEntryFar(window, PAGE_CAP_LEFT, x - 1, -1, 0);

        for (page = 0; page < page_count; page++) {
            if (page == selected_page)
                UiWindow_SetTilemapEntryFar(window, tile, x, -1, 2);
            else
                UiWindow_SetTilemapEntryFar(window, tile, x, -1, 3);
            tile++;
            x++;
        }

        UiWindow_SetTilemapEntryFar(window, PAGE_CAP_RIGHT, x, -1, 0);
    }
}
