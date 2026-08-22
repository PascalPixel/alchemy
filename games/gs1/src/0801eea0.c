#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

struct State_0801eea0 {
    u8 padding[4];
    u16 left;
    u16 zero;
    u16 right;
    u16 height;
    u16 flags;
};

extern s32 Func_080b50c8(s32);
extern s32 Func_08077148(void);

void Func_0801eea0(s32 flags)
{
    void **slot = (void **)ADDR_03001E90;
    struct State_0801eea0 *state = *slot;
    u8 *base = *(u8 **)(slot - 1);
    s32 height = 4;
    s32 value;
    s32 right;
    s32 left;

    if (base[RENDER_MENU_STATE_OFS] != 0) {
        value = Func_080b50c8(0);
        height = 3;
    } else {
        value = Func_08077148();
    }
    if (flags & 1)
        height++;
    else
        flags &= -3;

    value *= 6;
    right = value + 1;
    if (flags & 2)
        right += 5;

    left = 30;
    left -= right;
    state->left = left;
    state->zero = 0;
    state->right = right;
    state->height = height;
    state->flags = flags;
}
