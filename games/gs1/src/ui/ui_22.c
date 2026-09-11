#include "types.h"
#include "scene.h"
#include "m7_interfaces.h"
#include "gs1_edition.h"

/* ui/text/draw/draw_work_value_with_label.c */
struct SharedWork080a23c0 {
    u8 padding_00[0x10];
    s32 resource;
};

extern void UiText_DrawAt(s32, s32, s32, s32);
extern struct SharedWork080a23c0 gCell;

void UiText_DrawWorkValueWithLabel(s32 work)
{
    Ui_SetRange(gCell.resource, 7, work, 8, 0);
    UiText_DrawAt(0xB0B, work, 0x40, 0);
}

/* ui/window/set_bounds.c */
void WindowBounds_Set(struct WindowBounds *bounds, s32 right, s32 bottom,
    s32 left, s32 top) {
    if (bounds != NULL) {
        bounds->left = left;
        bounds->right = right;
        bounds->top = top;
        bounds->bottom = bottom;
    }
}

/* ui/runtime/set/set_menu_busy.c */
extern u8 *volatile gIw;

void UiWork_SetMenuBusy(void)
{
    u8 *base = gIw;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 1;
    *p = flag;
}

void Ui_Run(void)
{
    u8 *base = gIw;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 0;
    *p = flag;
}
