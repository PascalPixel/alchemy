/* Draft, differ 4 of 32: The Broken Seal's Map_RenderPaletteMappedColumn
 * with the colour table at the fixed Ram_MapCellBuffer, as in the row
 * draft 0802c174 (as the symbol: differ 36). Left, as there: the native loads
 * 0x06004000 into r5, the register the loop then uses for the colour table,
 * between the map offset's shift and add; this loads it into r2 just before
 * its add. A pin of the base to r5 with an operand barrier gives r5 (differ
 * 2), but the load still follows the map address. */
#include "types.h"
#include "ram_buffer.h"

extern u8 gMapBlocks[];
extern u8 gMapCellBuffer[];

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
