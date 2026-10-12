/* Near miss of UiWindow_UpdateInterpolatedGeometry (The Broken Seal twin in graphics/window/window.c; first written
 * 2026-10-12): differ 10 of 92, order only. After each of three ratio calls the native loads the next operand
 * (window->x, window->height, window->y) before it stores the result to the scratch (ldrsh; str r0, [r5, #8]), and it
 * stores the third scaled part after the duration is loaded; the draft stores first each time. x and y are signed
 * here (ldrsh), unlike the twin. Measured: the result in a local with the origin loaded before the store (differ 44
 * and 46: the shift leaves r0), the scratch as a word array (same 10). */
#include "types.h"
#include "iwram_call.h"

struct UiWindow {
    s32 unknown_00;
    s32 unknown_04;
    u16 width;
    u16 height;
    s16 x;
    s16 y;
    u16 unknown_10;
    u16 unknown_12;
    u16 state;
    u16 flags;
    s16 frame;
    s16 duration;
    s16 previous_x;
    s16 previous_y;
    s16 previous_width;
    s16 previous_height;
};

void UiWindow_DrawFrame(s32 x, s32 y, s32 width, s32 height);

// 開閉の途中の枠を描く
void UiWindow_UpdateInterpolatedGeometry(void *arg, s32 save_position)
{
    struct UiWindow *window = arg;
    s32 scratch[3];
    s32 frame;
    s32 duration;
    s32 remaining;
    s32 x;
    s32 y;
    s32 width;
    s32 height;

    frame = (s16)window->frame;
    duration = window->duration;
    remaining = duration - frame;
    scratch[0] = (frame * window->width) << 16;
    scratch[1] = duration << 17;
    scratch[2] = Iwram_RatioMulQ14(scratch[1], scratch[0]);
    x = (scratch[2] >> 16) + window->x;

    scratch[0] = (remaining * window->width) << 16;
    scratch[2] = Iwram_RatioMulQ14(scratch[1], scratch[0]);
    width = scratch[2] >> 15;

    scratch[0] = (frame * window->height) << 16;
    scratch[1] = window->duration << 17;
    scratch[2] = Iwram_RatioMulQ14(scratch[1], scratch[0]);
    y = (scratch[2] >> 16) + window->y;

    scratch[0] = (remaining * window->height) << 16;
    scratch[2] = Iwram_RatioMulQ14(scratch[1], scratch[0]);
    height = scratch[2] >> 15;

    UiWindow_DrawFrame(x, y, width, height);
    if (save_position != 0) {
        window->previous_x = x;
        window->previous_y = y;
        window->previous_width = width;
        window->previous_height = height;
    }
}
