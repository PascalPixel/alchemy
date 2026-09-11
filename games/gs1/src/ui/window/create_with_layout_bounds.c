#include "types.h"
#include "scene.h"
#include "abi/ui/window/create_with_layout_bounds.h"
#include "global_cells.h"
#include "gs1_edition.h"

struct Window {
    s32 handle;
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
};

void *Runtime_AllocateBlock(s32 flags, s32 arg1);
s32 UiWindow_Create(u16, u16, u16, u16, s32);

void UiWindow_CreateWithLayoutBounds(s32 flags)
{
    s32 zero;
    struct Window *window;
    s8 *busy;

    window = Runtime_AllocateBlock(0x10, 0x10);
    busy = (s8 *)((u8 *)*(void **)ADDR_03001E8C + RENDER_MENU_BUSY_OFS);
    zero = 0;
    *busy = 1;
    Ui_Do(flags);
    window->handle = UiWindow_Create(
        window->left, window->top, window->right, window->bottom, 6);
    Ui_Do2(flags);
    *busy = zero;
}
