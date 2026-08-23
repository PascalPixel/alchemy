#include "types.h"

#define PAGE_LABEL_FIRST 49
#define PAGE_CAP_LEFT 0xf128
#define PAGE_CAP_RIGHT 0xf129

s32 FixedPoint_Ratio(s32 numerator, s32 denominator);
s32 Modulo(s32 numerator, s32 denominator);
void Func_08015280(
    s32 window,
    s32 tile,
    s32 x,
    s32 y,
    s32 style
);

void Func_080a21b0(
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
    page_count = FixedPoint_Ratio(item_count, page_size);
    if (Modulo(item_count, page_size) != 0)
        page_count++;

    x -= page_count;
    if (page_count > 1) {
        Func_08015280(window, PAGE_CAP_LEFT, x - 1, -1, 0);

        for (page = 0; page < page_count; page++) {
            if (page == selected_page)
                Func_08015280(window, tile, x, -1, 2);
            else
                Func_08015280(window, tile, x, -1, 3);
            tile++;
            x++;
        }

        Func_08015280(window, PAGE_CAP_RIGHT, x, -1, 0);
    }
}
