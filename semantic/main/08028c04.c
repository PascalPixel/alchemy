#include "layout_guard.h"
#include "types.h"

struct Window_08028c04;

struct SelectionDialogState_08028c04 {
    u8 padding00[0x7c];
    struct Window_08028c04 *secondary_window;
    struct Window_08028c04 *primary_window;
    u8 padding84[0x12];
    u16 selection;
};

LAYOUT_OFFSET_GUARD(
    SelectionDialog08028c04_Secondary,
    struct SelectionDialogState_08028c04,
    secondary_window,
    0x7c);
LAYOUT_OFFSET_GUARD(
    SelectionDialog08028c04_Primary,
    struct SelectionDialogState_08028c04,
    primary_window,
    0x80);
LAYOUT_OFFSET_GUARD(
    SelectionDialog08028c04_Selection,
    struct SelectionDialogState_08028c04,
    selection,
    0x96);

extern struct SelectionDialogState_08028c04 *Data_03001f38;

void Func_080284dc(void);
void Func_080287a8(s32);
void Func_08028808(s32, s32, s32);
void Func_080041d8(const void *, s32);
void Func_08004278(const void *);
struct Window_08028c04 *Func_080162d4(u32, u32, u32, u32, u32);
void Func_0801e74c(s32, struct Window_08028c04 *, s32, s32);
s32 Func_08028574(s32);
void Func_08016478(struct Window_08028c04 *);
void Func_08016418(struct Window_08028c04 *, s32);
void Func_080030f8(s32);
void Func_0802851c(void);

/*
 * Build and run either the compact two-resource dialog or the extended
 * three-resource dialog.  Both variants own a primary and secondary window,
 * install a variant-specific callback, then close the same two windows after
 * the blocking selector returns.
 */
s32 Func_08028c04(s32 extended)
{
    struct SelectionDialogState_08028c04 *state;
    const void *callback;
    s32 result;

    Func_080284dc();
    state = Data_03001f38;

    if (extended == 0) {
        Func_080287a8(0x2c);
        Func_080287a8(0x2d);
    } else {
        Func_080287a8(0x2e);
        Func_080287a8(0x2f);
        Func_080287a8(0x30);
    }
    Func_08028808(0x11, 7, 0);

    if (extended != 0) {
        callback = (const void *)0x08028aa9;
        Func_080041d8(callback, 0x0c76);
        state->selection = 0xffff;

        state->primary_window = Func_080162d4(7, 0, 0x11, 4, 2);
        Func_0801e74c(0x0c77, state->primary_window, 0, 4);

        state->secondary_window = Func_080162d4(3, 4, 0x19, 0x0c, 2);
        Func_0801e74c(0x0c78, state->secondary_window, 8, 0);
        Func_0801e74c(0x0c79, state->secondary_window, 8, 0x0b);
        Func_0801e74c(0x0c7a, state->secondary_window, 8, 0x16);
    } else {
        callback = (const void *)0x08028b81;
        Func_080041d8(callback, 0x0c76);
        state->selection = 0xffff;

        state->primary_window = Func_080162d4(6, 0, 0x12, 4, 2);
        Func_0801e74c(0x0c76, state->primary_window, 2, 4);
        state->secondary_window = Func_080162d4(1, 5, 0x1c, 7, 2);
    }

    result = Func_08028574(0);
    Func_08004278(callback);

    Func_08016478(state->primary_window);
    Func_08016478(state->secondary_window);
    Func_08016418(state->primary_window, 2);
    Func_08016418(state->secondary_window, 2);
    Func_080030f8(1);
    Func_0802851c();
    return result;
}
