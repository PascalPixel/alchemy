#include "EDITION.H"
#include "TYPES.H"
#include "SCENE.H"
#include "M7_INTERFACES.H"
#include "TBS_EDITION.H"

/* ui/text/draw/draw_work_value_with_label.c */
struct SharedWork080a23c0 {
    u8 padding_00[0x10];
    s32 resource;
};

extern void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
void UiWindow_DrawDividerLineFar(s32 window, s32 x1, s32 y1, s32 x2, s32 y2);
extern struct SharedWork080a23c0 gGameState;
extern u8 MsgCoinsLabel[];

void UiText_DrawWorkValueWithLabel(s32 work)
{
    UiText_DrawNumberAtOffsetFar(gGameState.resource, 7, work, 8, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCoinsLabel, work, 0x40, 0);
#if !EDITION_INTERNATIONAL
    /* The Japanese window closes the coin count off with a short rule. */
    UiWindow_DrawDividerLineFar(work, 12, 0, 12, 2);
#endif
}

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
