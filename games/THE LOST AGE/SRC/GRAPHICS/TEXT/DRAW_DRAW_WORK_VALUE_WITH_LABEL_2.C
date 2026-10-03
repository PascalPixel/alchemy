#include "M7_INTERFACES.H"

/* ui/window/set_bounds.c */
void UiWindow_SetBounds(struct RenderInput *window, s32 x, s32 y,
    s32 width, s32 height)
{
    if (window != NULL) {
        window->width = width;
        window->x = x;
        window->height = height;
        window->y = y;
    }
}
