#include "types.h"

struct Window_0801d9d4 {
    u8 padding0[0x0c];
    u16 width;
    u16 height;
};

struct RenderObject_0801d9d4;

s32 Func_080770c0(s32 flag);
struct Window_0801d9d4 *Func_080162d4(
    s32 type, s32 top, s32 width, s32 height, s32 mode);
void Func_0801e41c(
    struct Window_0801d9d4 *window,
    s32 left,
    s32 top,
    s32 right,
    s32 bottom);
void Func_0801e74c(
    s32 resource, struct Window_0801d9d4 *window, s32 x, s32 y);
s32 Func_08004080(void);
void Func_08003fa4(s32 resource, s32 size, const void *source);
struct RenderObject_0801d9d4 *Func_0801eadc(
    s32 resource,
    u32 mode,
    struct Window_0801d9d4 *window,
    s32 x,
    s32 y);
void Func_080b0038(
    struct RenderObject_0801d9d4 **object, s32 width, s32 height);
void *Func_08021750(
    s32 value,
    s32 mode,
    struct Window_0801d9d4 *window,
    s32 y,
    s32 x);

struct Window_0801d9d4 *Func_0801d9d4(void)
{
    u8 *runtime = *(u8 **)0x03001ea0;
    s32 eventActive = Func_080770c0(0x17e);
    s32 tableOffset = 0;
    s32 rowCount = 3;
    s32 top;
    s32 height;
    struct Window_0801d9d4 *window;
    s32 y;
    s32 resource;
    s32 index;

    if (eventActive != 0) {
        tableOffset = 2;
        rowCount = 1;
    }
    if (*(u8 *)0x03001f54 != 0)
        rowCount += 3;

    top = 8 - rowCount;
    height = rowCount * 3 + 1;
    if (top + height > 19) {
        top = 1;
        height = 19;
    }

    window = Func_080162d4(5, top, 20, height, 2);

    if (rowCount > 1) {
        s32 remaining = rowCount - 1;

        y = 3;
        do {
            Func_0801e41c(window, 0, y, 19, y);
            y += 3;
            remaining--;
        } while (remaining != 0);
    }

    y = 4;
    if (eventActive == 0) {
        Func_0801e74c(0x0c23, window, 48, 4);
        Func_0801e74c(0x0c24, window, 48, 28);
        y = 52;
    }

    Func_0801e74c(0x0c25, window, 48, y);
    y += 24;
    if (*(u8 *)0x03001f54 != 0) {
        Func_0801e74c(0x0c27, window, 48, y);
        y += 24;
        Func_0801e74c(0x0c28, window, 48, y);
        y += 24;
        Func_0801e74c(0x0c29, window, 48, y);
    }

    resource = Func_08004080();
    if (resource <= 95) {
        struct RenderObject_0801d9d4 **object =
            (struct RenderObject_0801d9d4 **)(runtime + 0x5a4);

        Func_08003fa4(resource, 0x80, (const void *)0x080310a4);
        *object = Func_0801eadc(
            resource, 0x40000000, window, 0, 0);
        Func_080b0038(
            object, window->width * 8, window->height * 8 + 16);
    }

    for (index = 0; index < rowCount; index++) {
        s8 value = *(s8 *)(0x080367dc + tableOffset + index);

        *(void **)(runtime + 0x610 + index * 4) =
            Func_08021750(value, 0, window, 12, index * 24 - 4);
    }

    return window;
}
