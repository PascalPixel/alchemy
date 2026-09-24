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

extern u8 UiMenu_CursorBobX[];
extern u8 UiMenu_CursorBobY[];
extern volatile u32 gFrameCount;
extern struct MenuCursorWork *gMenuWork;


/* Places the menu cursor at the given pixel offset from the top-left tile of
 * the menu window, plus eight pixels and a small diagonal bob that follows
 * the frame counter. */
void UiMenu_PositionCursor(s32 x_offset, s32 y_offset)
{
    struct MenuCursorWork *work = gMenuWork;

    work->cursor->attributes.x = work->cursor->x =
        UiMenu_CursorBobX[(gFrameCount >> 1) & 7] + x_offset
        + work->window->x * 8 + 8;
    work->cursor->attributes.y = work->cursor->y =
        UiMenu_CursorBobY[(gFrameCount >> 1) & 7] + y_offset
        + work->window->y * 8 + 8;
}
