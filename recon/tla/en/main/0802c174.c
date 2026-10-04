/* Draft: Map_RenderPaletteMappedRow and Map_RenderPaletteMappedColumn
 * (0802c174, 0802c1f4) are The Broken Seal's MAP4.C text exactly, which
 * matches ☀️ when moved to COMMON. ⚓️ differs by 2 in each: the 0x06004000
 * base loads after the mask (row) and the block address adds before it
 * (column). The block table and cell palette must fold as constants
 * (0x02020000, Ram_MapCellBuffer). A volatile "+r" barrier on the base and
 * an r5 pin leave one operand order; not adopted.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"

extern u8 gMapBlocks[];

void Map_RenderPaletteMappedRow(u32 value);
void Map_RenderPaletteMappedColumn(u32 value);

/* Writes one map entry's two tile halfwords, looked up in the decoded cell
   palette, to a character row and the row below it. */
static __inline__ void CopyEntry(u8 *map, u8 *destination)
{
    u32 palette = *(u16 *)map;
    u16 *colors = (u16 *)Ram_MapCellBuffer;

    colors += palette * 2;
    *(u16 *)destination = *colors++;
    *(u16 *)(destination + 64) = *colors;
}

/* Redraws the world map's row of 64 palette-mapped entries that the view has
   just crossed, in both halves of the BG screen. */
void Map_RenderPaletteMappedRow(u32 value)
{
    u8 *map;
    u8 *destination;
    u32 counter;

    map = ((u8 *)0x02020000) + ((((s32)value / 2) & 31) << 7);
    destination = (u8 *)(0x06004000 + ((value & 62) << 6));
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 2;
        map += 4;
    } while (counter <= 31);

    destination += 0xfc0;
    map += 0xf80;
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 2;
        map += 4;
    } while (counter <= 31);
}

/* Redraws the world map's column of 64 palette-mapped entries that the view
   has just crossed. */
void Map_RenderPaletteMappedColumn(u32 value)
{
    u8 *map;
    u8 *destination;
    u32 counter;

    map = ((u8 *)0x02020000) + ((((s32)value / 2) & 31) << 2);
    destination = (u8 *)(0x06004000 + (value & 62));
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 128;
        map += 128;
    } while (counter <= 63);
}
