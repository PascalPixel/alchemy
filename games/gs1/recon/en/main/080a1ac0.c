#include "types.h"

#define UiMenu_SlideCursor Func_080a1ac0

struct CursorIcon {
    u8 unknown_00[6];
    u16 x;                          /* 0x06 */
    u16 y;                          /* 0x08 */
    u8 unknown_0a[10];
    u8 attr_y;                      /* 0x14 */
    u8 unknown_15;
    u16 attr_x;                     /* 0x16 */
};

struct CursorWindow {
    u8 unknown_00[12];
    u16 tile_x;                     /* 0x0c */
    u16 tile_y;                     /* 0x0e */
};

struct CursorWork {
    u8 unknown_000[0x10];
    struct CursorWindow *window;    /* 0x010 */
    struct CursorIcon *cursor;      /* 0x014 */
    u8 unknown_018[0x20a];
    u16 skip_slide;                 /* 0x222 */
};

extern struct CursorWork *Data_03001f2c;

s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_080030f8(s32 frames);

void UiMenu_SlideCursor(s32 x, s32 y)
{
    struct CursorWork *work = Data_03001f2c;
    struct CursorIcon *cursor;
    struct CursorWindow *window;
    s32 cnt;
    s32 px;
    s32 py;
    s32 dx;
    s32 dy;

    if (work->skip_slide != 0) {
        work->skip_slide = 0;
        return;
    }
    cursor = work->cursor;
    cursor->x = (cursor->attr_x & 0x1ff) + 64;
    cursor->y = cursor->attr_y + 64;
    x += 64;
    y += 64;
    if (cursor->x - 8 > 0) {
        cursor->x -= 8;
    }
    if (cursor->y - 8 > 0) {
        cursor->y -= 8;
    }
    px = cursor->x << 4;
    py = cursor->y << 4;
    dx = Func_080022ec((x << 4) - px + 1, 2);
    dy = Func_080022ec((y << 4) - py + 1, 2);
    cnt = 2;
    for (;;) {
        window = work->window;
        px += dx;
        cursor->x = (px >> 4) + (window->tile_x << 3) - 56;
        cursor->attr_x = (cursor->attr_x & ~0x1ff) | (cursor->x & 0x1ff);
        py += dy;
        cursor->y = (py >> 4) + (window->tile_y << 3) - 56;
        cursor->attr_y = cursor->y;
        cnt--;
        if (cnt == 0) {
            break;
        }
        Func_080030f8(1);
    }
}
