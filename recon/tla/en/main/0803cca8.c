/*
 * Draft: UiWindow_ClearSlots does not yet match; ⚓️ keeps the tilemap offset in r4, the counter in r1 and zero in r0, and stores the word before the halfword.
 * Links as recon/tla/raw/0803cba8.s.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"

/* Clears the eight window slots kept after the window tilemap: a word and a
   halfword each. */
void UiWindow_ClearSlots(void)
{
    u8 *tiles;
    u16 *flags;
    u32 *words;
    s32 i;

    tiles = Ram_HeapSlots->window_tiles;
    flags = (u16 *)(tiles + 0x136c);
    i = 0;
    words = (u32 *)(tiles + 0x134c);
    do {
        i++;
        *words++ = 0;
        *flags++ = 0;
    } while (i != 8);
}
