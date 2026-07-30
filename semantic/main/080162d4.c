typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct WindowWork_080162d4 {
    s32 field00;
    struct WindowWork_080162d4 *field04;
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 field10;
    u16 field12;
    u16 field14;
    u16 flags;
    s16 frame;
    s16 duration;
    u16 previous_x;
    u16 previous_y;
    u16 previous_width;
    u16 previous_height;
};

void Func_080030f8(u32);
void Func_08016230(struct WindowWork_080162d4 *);
void Func_080163ec(struct WindowWork_080162d4 *);
void Func_080173ac(void);

/*
 * Allocate one of the eight window-work records, initialize its geometry and
 * presentation flags, then either publish it immediately or run the opening
 * transition synchronously.
 */
struct WindowWork_080162d4 *Func_080162d4(
    u16 x,
    u16 y,
    u16 width,
    u16 height,
    s32 options
) {
    u8 *engine;
    struct WindowWork_080162d4 *window;
    s32 index;

    engine = *(u8 **)0x03001E8C;
    window = (struct WindowWork_080162d4 *)(engine + 0x500);
    for (index = 0; index < 8; index++, window++) {
        if ((window->flags & 1) == 0 && window->duration == 0) {
            break;
        }
    }
    if (index == 8) {
        return 0;
    }

    window->field00 = 0;
    window->field04 = window;
    window->width = width;
    window->height = height;
    window->x = x;
    window->y = y;
    window->field10 = 1;
    window->field14 = 0;
    window->flags = 1;
    Func_080173ac();

    window->flags |= options & (8 | 0x20 | 0x40 | 0x80 | 0x100);
    if (options & 2) {
        window->flags |= 2;
        window->frame = 0;
        window->duration = 1;
        Func_08016230(window);
    } else {
        window->frame = 7;
        window->duration = 8;
        Func_080163ec(window);
        Func_080030f8(1);
    }
    return window;
}
