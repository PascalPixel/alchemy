#include "layout_guard.h"
#include "menu_result.h"
#include "types.h"

struct MenuState_080a6a98 {
    u8 padding00[0x20];
    void *selection_grid;
    u8 padding24[8];
    s32 preview_window;
    u8 padding30[0x198];
    u16 action_list[40];
};

LAYOUT_OFFSET_GUARD(
    MenuState080a6a98_SelectionGrid,
    struct MenuState_080a6a98,
    selection_grid,
    0x20);
LAYOUT_OFFSET_GUARD(
    MenuState080a6a98_PreviewWindow,
    struct MenuState_080a6a98,
    preview_window,
    0x2c);
LAYOUT_OFFSET_GUARD(
    MenuState080a6a98_ActionList,
    struct MenuState_080a6a98,
    action_list,
    0x1c8);

s32 Func_080770c0(s32);
void Func_080770d0(s32);
void Func_08015270(s32);
void Func_080030f8(u32);
void Func_08015080(s32, s32, s32, s32);
void Func_080a2268(void *, s32, s32, s32, s32, u32);

/*
 * Refresh the five-row action preview.  `page->value8` chooses a group of
 * five actions and `value10` chooses the highlighted row.  The selected row
 * uses palette 14; the other four use palette 15.
 *
 * The first two arguments are scratch values kept by the surrounding modal;
 * this renderer does not consume them.
 */
s32 Func_080a6a98(
    s32 unused_selected_info,
    s32 unused_preview_scratch,
    struct MenuResult *page)
{
    struct MenuState_080a6a98 *state =
        *(struct MenuState_080a6a98 **)0x03001f2c;
    s32 row;

    (void)unused_selected_info;
    (void)unused_preview_scratch;

    page->value18 = page->value8 * 5 + page->value10;

    if (Func_080770c0(0x151) == 0) {
        u16 action = state->action_list[page->value18];

        Func_08015270(state->preview_window);
        Func_080030f8(1);
        if (action != 0) {
            Func_08015080(
                (action & 0x01ff) + 0x053a,
                state->preview_window,
                0,
                0);
        }
    } else {
        Func_080770d0(0x02ff);
    }

    for (row = 0; row < 5; row++) {
        u32 palette = row == page->value10 ? 14 : 15;
        Func_080a2268(state->selection_grid, 0, row * 2 + 1, 15, 1, palette);
    }

    Func_080030f8(1);
    return 1;
}
