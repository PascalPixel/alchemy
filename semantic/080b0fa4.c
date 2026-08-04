#include "layout_guard.h"
#include "types.h"

typedef struct ItemMetadata_080b0fa4 {
    s16 display_value;
} ItemMetadata_080b0fa4;

typedef struct MenuState_080b0fa4 {
    u8 padding000[0x26c];
    s16 items[129];
    u8 padding36e[0x24];
    u16 previous_page_resource;
    u16 next_page_resource;
    u8 padding396[0x10];
    s8 item_count;
} MenuState_080b0fa4;

typedef struct DisplayObject_080b0fa4 {
    u8 padding00[4];
    s8 x;
    s8 y;
    u8 padding06[6];
    s16 state_0c;
    u8 padding0e;
    s8 draw_order;
} DisplayObject_080b0fa4;

LAYOUT_OFFSET_GUARD(
    MenuState080b0fa4_Items,
    MenuState_080b0fa4,
    items,
    0x26c);
LAYOUT_OFFSET_GUARD(
    MenuState080b0fa4_PreviousPageResource,
    MenuState_080b0fa4,
    previous_page_resource,
    0x392);
LAYOUT_OFFSET_GUARD(
    MenuState080b0fa4_NextPageResource,
    MenuState_080b0fa4,
    next_page_resource,
    0x394);
LAYOUT_OFFSET_GUARD(
    MenuState080b0fa4_ItemCount,
    MenuState_080b0fa4,
    item_count,
    0x3a6);
LAYOUT_OFFSET_GUARD(
    DisplayObject080b0fa4_State,
    DisplayObject_080b0fa4,
    state_0c,
    0x0c);
LAYOUT_OFFSET_GUARD(
    DisplayObject080b0fa4_DrawOrder,
    DisplayObject_080b0fa4,
    draw_order,
    0x0f);

extern MenuState_080b0fa4 *Data_03001f2c;

s32 Func_080022fc(s32 value, s32 divisor);
void Func_08015060(s32 window);
DisplayObject_080b0fa4 *Func_080150c8(
    s32 resource, u32 flags, s32 window, s32 x, s32 y);
DisplayObject_080b0fa4 *Func_080152d0(
    s16 item, s32 mode, s32 window, s32 x, s32 y);
ItemMetadata_080b0fa4 *Func_08077018(s32 item);
DisplayObject_080b0fa4 *Func_080b0744(
    s16 value, s32 window, s32 x, s32 y);

static void ConfigurePageArrow_080b0fa4(
    DisplayObject_080b0fa4 *arrow, s8 y)
{
    arrow->x = 0;
    arrow->y = y;
    arrow->state_0c = 0;
}

/*
 * Rebuild the visible seven-entry item page around the current selection.
 * The item sprite and its numeric value share each 32-pixel row, while small
 * arrows advertise pages above and below the current one.
 */
void Func_080b0fa4(s32 window, s32 selected_index)
{
    MenuState_080b0fa4 *state = Data_03001f2c;
    u32 first_index =
        selected_index - Func_080022fc(selected_index, 7);
    u32 row;

    if (window == 0)
        return;

    Func_08015060(window);

    if (first_index != 0) {
        DisplayObject_080b0fa4 *arrow = Func_080150c8(
            state->previous_page_resource,
            0x40000000,
            window,
            0xd8,
            -0x10);

        ConfigurePageArrow_080b0fa4(arrow, 0x11);
    }

    if ((s32)(first_index + 7) < state->item_count) {
        DisplayObject_080b0fa4 *arrow = Func_080150c8(
            state->next_page_resource,
            0x40000000,
            window,
            0xd8,
            0x18);

        ConfigurePageArrow_080b0fa4(arrow, 0x0f);
    }

    for (row = 0;
         row < 7 && first_index < (u32)state->item_count;
         row++, first_index++) {
        s16 item = state->items[first_index];
        ItemMetadata_080b0fa4 *metadata = Func_08077018(item);
        DisplayObject_080b0fa4 *item_object = Func_080152d0(
            item, 1, window, row * 32, 0);
        DisplayObject_080b0fa4 *value_object;

        item_object->draw_order = -4;
        if (first_index == (u32)selected_index) {
            item_object->y = 9;
            item_object->state_0c = 10;
            item_object->draw_order = -3;
        }

        value_object = Func_080b0744(
            metadata->display_value,
            window,
            row * 32 + 16,
            0);
        value_object->draw_order = -5;
    }
}
