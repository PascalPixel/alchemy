#include "TYPES.H"

struct MenuCursorAttributes {
    u16 y : 8;
    u16 affine_mode : 2;
    u16 object_mode : 2;
    u16 mosaic : 1;
    u16 palette_256 : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 matrix : 5;
    u16 size : 2;
};

struct MenuCursorSprite {
    u8 reserved_00[6];
    u16 x;
    u16 y;
    u8 reserved_0a[10];
    struct MenuCursorAttributes attributes;
};

struct MenuCursorWindow {
    u8 reserved_00[12];
    u16 x;
    u16 y;
};

struct MenuCursorWork {
    u8 reserved_00[16];
    struct MenuCursorWindow *window;
    struct MenuCursorSprite *cursor;
};

extern u8 Data_080af294[];
extern u8 Data_080af29d[];
extern volatile u32 Data_03001e40;
extern struct MenuCursorWork *Data_03001f2c;


/* Places the menu cursor at the given pixel offset from the top-left tile of
 * the menu window, plus eight pixels and a small diagonal bob that follows
 * the frame counter. */
void UiMenu_PositionCursor(s32 x_offset, s32 y_offset)
{
    struct MenuCursorWork *work = Data_03001f2c;

    work->cursor->attributes.x = work->cursor->x =
        Data_080af294[(Data_03001e40 >> 1) & 7] + x_offset
        + work->window->x * 8 + 8;
    work->cursor->attributes.y = work->cursor->y =
        Data_080af29d[(Data_03001e40 >> 1) & 7] + y_offset
        + work->window->y * 8 + 8;
}
