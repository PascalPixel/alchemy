#include "layout_guard.h"
#include "types.h"

typedef struct MenuWindow_080a1a40 {
    u8 padding00[0x0c];
    u16 tile_x;
    u16 tile_y;
} MenuWindow_080a1a40;

typedef struct CursorObject_080a1a40 {
    u8 padding00[6];
    u16 x;
    u16 y;
    u8 padding0a[0x0a];
    u8 screen_y;
    u8 padding15;
    u16 attr1;
} CursorObject_080a1a40;

typedef struct MenuState_080a1a40 {
    u8 padding00[0x10];
    MenuWindow_080a1a40 *window;
    CursorObject_080a1a40 *cursor;
} MenuState_080a1a40;

LAYOUT_OFFSET_GUARD(
    MenuWindow080a1a40_TileX,
    MenuWindow_080a1a40,
    tile_x,
    0x0c);
LAYOUT_OFFSET_GUARD(
    CursorObject080a1a40_X,
    CursorObject_080a1a40,
    x,
    6);
LAYOUT_OFFSET_GUARD(
    CursorObject080a1a40_ScreenY,
    CursorObject_080a1a40,
    screen_y,
    0x14);
LAYOUT_OFFSET_GUARD(
    CursorObject080a1a40_Attr1,
    CursorObject_080a1a40,
    attr1,
    0x16);
LAYOUT_OFFSET_GUARD(
    MenuState080a1a40_Window,
    MenuState_080a1a40,
    window,
    0x10);

extern MenuState_080a1a40 *Data_03001f2c;
extern volatile u32 Data_03001e40;

/* Position the shared menu cursor, including its eight-frame wobble. */
void Func_080a1a40(s32 x, s32 y)
{
    static const u8 *const wobble_x = (const u8 *)0x080af294;
    static const u8 *const wobble_y = (const u8 *)0x080af29d;
    MenuState_080a1a40 *state = Data_03001f2c;
    CursorObject_080a1a40 *cursor = state->cursor;
    u32 frame = (Data_03001e40 >> 1) & 7;
    u32 screen_x =
        wobble_x[frame] + x + state->window->tile_x * 8 + 8;
    u32 screen_y =
        wobble_y[frame] + y + state->window->tile_y * 8 + 8;

    cursor->x = (u16)screen_x;
    cursor->attr1 =
        (cursor->attr1 & 0xfe00) | (screen_x & 0x01ff);
    cursor->y = (u16)screen_y;
    cursor->screen_y = (u8)screen_y;
}
