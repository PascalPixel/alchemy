/* Draft: Menu_LayoutResourceEntries, demoted from SELECTION.C when ⚓️ moved to -mtune=arm9tdmi
   (Pascal, 2026-10-05). Under the new tuning the row store and the count address tie at priority 43 in sched2 and the independent movs wins, 3 instructions from the listing; barriers, volatile stores, register pins and 2000 hill-climb steps did not fix it. */
#include "TLA_EDITION.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"

/* ☀️'s, reaching the selection work through ⚓️'s heap slot. */
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 w, s32 h)
{
    u8 *state;
    s32 i;

    state = Ram_HeapSlots->menu_select_work;

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
