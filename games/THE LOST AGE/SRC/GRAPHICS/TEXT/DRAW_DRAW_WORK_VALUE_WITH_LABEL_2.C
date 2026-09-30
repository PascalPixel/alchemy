#include "M7_INTERFACES.H"

/* ui/window/set_bounds.c */
void UiWindow_SetBounds(struct WindowBounds *bounds, s32 right, s32 bottom,
    s32 left, s32 top) {
    if (bounds != NULL) {
        bounds->left = left;
        bounds->right = right;
        bounds->top = top;
        bounds->bottom = bottom;
    }
}
