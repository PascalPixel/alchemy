#include "types.h"
#include "menu.h"
#include "render_input.h"
#include "ram_buffer.h"

struct ShopRuntime {
    u8 unknown_000[0x4dc];
    struct RenderOutput *anchor;
};

s32 Func_08108630(s32 arg0)
{
    struct RenderOutput **slot = &((struct ShopRuntime *)Ram_HeapSlots->menu_runtime)->anchor;
    u8 saved = (*slot)->active;

    UiIcon_PrepareObjectFar(*slot);
#if defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    arg0 = Menu_RunConfirmSelectionAtFar(6, 5, arg0);
#else
    arg0 = Menu_RunConfirmSelectionAtFar(7, 5, arg0);
#endif
    (*slot)->active = saved;
    return arg0;
}

s32 Func_08108660(s32 arg0)
{
    struct RenderOutput **slot = &((struct ShopRuntime *)Ram_HeapSlots->menu_runtime)->anchor;
    u8 saved = (*slot)->active;

    UiIcon_PrepareObjectFar(*slot);
    arg0 = Menu_RunConfirmSelectionAtFar(7, 7, arg0);
    (*slot)->active = saved;
    return arg0;
}
