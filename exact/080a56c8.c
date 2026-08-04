#include "types.h"

typedef struct MenuRuntime_080a56c8 {
    u8 padding_00[0x20];
    s32 message_window;
    u8 padding_24[0x1a4];
    u16 entry_ids[256];
} MenuRuntime_080a56c8;

typedef struct MenuPageState_080a56c8 {
    u8 padding_00[8];
    u32 page;
    u8 padding_0c[8];
    u32 entry_count;
} MenuPageState_080a56c8;

extern u8 Value_00000182;

void Func_08015070(s32 window, s32 x, s32 width, s32 height, s32 style);
void Func_08015080(s32 message, s32 window, s32 x, s32 y);
void Func_08015270(s32 window);
void Func_080a21b0(s32 window, s32 count, s32 page_size, s32 page, s32 style);
void Func_080a2324(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);

s32 Func_080a56c8(s32 window, s32 unused, const MenuPageState_080a56c8 *state)
{
    MenuRuntime_080a56c8 *runtime = *(MenuRuntime_080a56c8 **)0x03001f2c;
    u32 page;
    u32 first_entry;
    u32 visible_count;
    u8 row;
    const u16 *entry_id;

    (void)unused;

    Func_08015270(window);
    Func_08015070(window, 0, 11, 16, 11);

    page = state->page;
    first_entry = page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

    Func_080a2324(5, first_entry, window, 116, 34);
    Func_080a21b0(window, state->entry_count, 5, state->page, 15);

    row = 0;
    if (visible_count > row) {
        entry_id = &runtime->entry_ids[first_entry];
        do {
            Func_08015080(
                (entry_id[0] & 0x1ff) + (s32)&Value_00000182,
                runtime->message_window,
                24,
                row * 16 + 8
            );
            row++;
            entry_id++;
        } while (visible_count > row);
    }

    return 1;
}

s32 Func_080a5780(void)
{
    return 1;
}
