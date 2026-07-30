typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct Window_08019aa0 {
    u8 padding00[8];
    u16 width;
    u16 height;
};

typedef char Window_08019aa0_size[
    sizeof(struct Window_08019aa0) == 0x0C ? 1 : -1
];

void Func_080030f8(s32);
struct Window_08019aa0 *Func_080162d4(
    s32, s32, s32, s32, s32);
void Func_08016418(struct Window_08019aa0 *, s32);
void *Func_080165d8(
    struct Window_08019aa0 *, s32, s32, s32, const u16 *, s32);
s32 Func_08017364(void);
s32 Func_08017394(struct Window_08019aa0 *);
s32 Func_08018038(s32, s32);
s32 Func_080187ac(s32, s32 *, s32 *, s32 *, s32 *);

void Func_08019aa0(s32 text, s32 transition, s32 row_offset)
{
    u8 *engine;
    struct Window_08019aa0 *window;
    s32 column;
    s32 row;
    s32 width;
    s32 height;
    s32 text_index;

    engine = *(u8 **)0x03001E8C;
    column = 8;
    row = 8;

    text_index = Func_08018038(text, 1);
    if (*(u16 *)(engine + 0xEB0 + text_index * 2) == 0)
        return;

    Func_080187ac(text, &column, &row, &width, &height);
    column = (30 - width) >> 1;
    row = ((15 - height) >> 1) + row_offset;

    if (transition != 0) {
        window = Func_080162d4(column, row, width, height, 0);
    } else {
        window = Func_080162d4(column, row, 0, 0, 2);
        window->width = 0;
        window->height = 0;
    }

    if (Func_080165d8(window, text_index, 0, 0, 0, 0) == 0) {
        Func_08016418(window, 1);
        return;
    }

    while (Func_08017364() == 0)
        Func_080030f8(1);

    if (transition != 0) {
        Func_08016418(window, 0);
        while (Func_08017394(window) == 0)
            Func_080030f8(1);
    } else {
        Func_08016418(window, 1);
    }

    *(u16 *)(engine + 0x12F4) = 0;
    *(u16 *)(engine + 0x12F6) = 0;
}
