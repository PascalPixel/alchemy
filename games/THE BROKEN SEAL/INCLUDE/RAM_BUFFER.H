#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

#include "TYPES.H"

/* Fixed RAM buffers that Camelot's code addressed as constants. GCC folds a
 * constant base with its offsets where it cannot fold a label: it loads each
 * offset address from the pool, where from a label it adds the offsets to one
 * loaded base. Code the ROM shows doing the former reaches its buffer through
 * the entry here; the build checks each entry against the object named beside
 * it. Everywhere else the label serves. */
#define Ram_MapCellBuffer ((u8 *)0x02010000) /* gMapCellBuffer */
#define Ram_MapBlocks ((u8 *)0x02020000) /* gMapBlocks */
#define Ram_MapCollision ((u8 *)0x0202c000) /* gMapCollision */
#define Ram_BgTileBuffer ((u8 *)0x02038000) /* gBgTileBuffer */

#endif
