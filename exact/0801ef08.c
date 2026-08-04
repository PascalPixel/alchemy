#include "types.h"

struct Window {
    s32 handle;
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
};

void Func_0801eea0(s32 flags);
void Func_0801f200(s32);
void *Func_080048f4(s32 arg0, s32 arg1);
s32 Func_080162d4(u16, u16, u16, u16, s32);

void Func_0801ef08(s32 arg0) {
    s32 zero;
    struct Window *window;
    s8 *busy;

    window = Func_080048f4(0x10, 0x10);
    busy = (s8 *)((u8 *)*(void **)0x03001E8C + 0xEA6);
    zero = 0;
    *busy = 1;
    Func_0801eea0(arg0);
    window->handle = Func_080162d4(
        window->left, window->top, window->right, window->bottom, 6);
    Func_0801f200(arg0);
    *busy = zero;
}
