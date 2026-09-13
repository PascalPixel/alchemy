#include "types.h"

void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height,
                   s32 value)
{
    u32 row = 0;
    s16 *dst = (s16 *)0x06002000;

    if (row < height) {
        do {
            u32 column = 0;
            if (column < width) {
                do {
                    column++;
                    *dst = value;
                    dst++;
                } while (column < width);
            }
            row++;
            dst += 32 - width;
        } while (row < height);
    }
}
#include "global_cells.h"
#include "gs1_edition.h"

struct UiWindowBounds {
    s32 handle;
    u16 left;
    u16 top;
    u16 right;
    u16 height;
    u16 flags;
};

extern s32 Func_080b50c8(s32);
extern s32 Func_08077148(void);

void UiWindow_BuildLayoutBounds(s32 flags)
{
    void **slot = (void **)ADDR_03001E90;
    struct UiWindowBounds *state = *slot;
    u8 *base = *(u8 **)(slot - 1);
    s32 height = 4;
    s32 n;
    s32 right;
    s32 left;

    if (base[RENDER_MENU_STATE_OFS] != 0) {
        n = Func_080b50c8(0);
        height = 3;
    } else {
        n = Func_08077148();
    }
    if (flags & 1)
        height++;
    else
        flags &= -3;

    n *= 6;
    right = n + 1;
    if (flags & 2)
        right += 5;

    left = 30;
    left -= right;
    state->left = left;
    state->top = 0;
    state->right = right;
    state->height = height;
    state->flags = flags;
}



void Func_0801eea0(s32 flags);
void Func_0801f200(s32);
void *Runtime_AllocateBlock(s32 flags, s32 arg1);
s32 UiWindow_Create(u16, u16, u16, u16, s32);

void UiWindow_CreateWithLayoutBounds(s32 flags)
{
    s32 zero;
    struct UiWindowBounds *window;
    s8 *busy;

    window = Runtime_AllocateBlock(0x10, 0x10);
    busy = (s8 *)((u8 *)*(void **)ADDR_03001E8C + RENDER_MENU_BUSY_OFS);
    zero = 0;
    *busy = 1;
    Func_0801eea0(flags);
    window->handle = UiWindow_Create(
        window->left, window->top, window->right, window->height, 6);
    Func_0801f200(flags);
    *busy = zero;
}
