#include "types.h"

s32 UiWindow_Create(s32, s32, s32, s32, s32);
extern u8 Data_03001f38;

void Menu_LayoutResourceEntries(s32 x, s32 y, s32 w, s32 h)
{
    u8 *state;
    s32 i;

    state = *(u8 **)&Data_03001f38;

    *(u16 *)(state + 144) = (u16)((u32)w + 2);
    *(u16 *)(state + 146) = (u16)h;
    *(u16 *)(state + 148) = (u16)y;

    for (i = 0; i < *(s16 *)(state + 142); i++) {
        *(u16 *)(state + i * 20 + 14) = (u16)((u32)y << 3);
        *(u16 *)(state + i * 20 + 12) = (u16)((u32)x << 3);
        x = (s32)((u32)x + 3);
    }

    *(s32 *)(state + 120) =
        UiWindow_Create(x, y, *(s16 *)(state + 144), 3, 2);
}
