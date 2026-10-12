/* Draft, differ 4 of 32: The Broken Seal's Map_RenderPaletteMappedColumn with the colour table as
 * the fixed address Ram_MapCellBuffer; the loop matches. Left, as in Map_RenderPaletteMappedRow
 * (0802c174): the native loads 0x06004000 into r5 before the map address is added (ldr r5; adds
 * r4, r3, r2; ... adds r1, r3, r5), this form into r2 two rows later. */
#include "types.h"
#include "ram_buffer.h"

extern u8 gMapBlocks[];

static __inline__ void CopyEntry(u8 *map, u8 *destination)
{
    u32 palette = *(u16 *)map;
    u16 *colors = (u16 *)Ram_MapCellBuffer;

    colors += palette * 2;
    *(u16 *)destination = *colors++;
    *(u16 *)(destination + 64) = *colors;
}

void Map_RenderPaletteMappedColumn(u32 value)
{
    u8 *map;
    u8 *destination;
    u32 counter;

    map = gMapBlocks + ((((s32)value / 2) & 31) << 2);
    destination = (u8 *)(0x06004000 + (value & 62));
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 128;
        map += 128;
    } while (counter <= 63);
}
