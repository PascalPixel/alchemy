// Draft, differ 4 of 57: The Broken Seal's Map_RenderPaletteMappedRow, and both
// loops match once the colour table is the fixed address Ram_MapCellBuffer (as
// the symbol gMapCellBuffer the address is hoisted into r6: differ 71). Left:
// the native loads 0x06004000 into r5, the register the loops then use for the
// colour table, right after the map address is formed (ldr r5; movs r3, #62;
// ands; lsls; adds r1, r3, r5); this form loads it into r2 two rows later.
// Writing the sum the other way round, a separate base variable, and sharing
// one variable between the base and the table through a macro (which hoists
// again, differ 72) measured the same or worse.
#include "types.h"
#include "ram_buffer.h"

extern u8 gMapBlocks[];

static __inline__ void CopyEntry(u8 *map, u8 *destination) {
  u32 palette = *(u16 *)map;
  u16 *colors = (u16 *)Ram_MapCellBuffer;

  colors += palette * 2;
  *(u16 *)destination = *colors++;
  *(u16 *)(destination + 64) = *colors;
}

void Map_RenderPaletteMappedRow(u32 value) {
  u8 *map;
  u8 *destination;
  u32 counter;

  map = gMapBlocks + ((((s32)value / 2) & 31) << 7);
  destination = (u8 *)0x06004000 + ((value & 62) << 6);
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
