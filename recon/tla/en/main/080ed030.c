// Draws one column of the world map picture on BG1 (21 rows) from the
// decoded rows in the cell buffer, for scrolling; the column twin of
// Func_080ecf74 (games/tla/src/battle/effect/world_map_row.c).
//
// Near miss, differ 2 of 51 (2026-10-19; was 16). The native keeps the line
// in r0 and its wrapped copy in r5 only when line is reused for the screen's
// base (the permuter's find: a plain 0x06004000 + (tile << 6) gives the line
// r5, differ 10), and adds column + source in that operand order only
// through an inline (the plain sum is adds r5, r5, r7: differ 4). Left: the
// native loads the cell buffer's address before that sum, this after it.
// Both forms here would be devices. Earlier: the source address computed
// before the cell strength-reduces the loop (62); a conditional expression,
// a reused source variable and the body as an inline were all 16.
// With the buffer pinned to r2 behind an asm reading source, the load comes
// first but the last sum turns to adds r0, r2, r5 (2); a plain barrier hoists
// the buffer out of the loop (36). Five minutes of permuter: nothing better.
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"

static inline s32 Add(s32 a, s32 b) { return a + b; }

void Func_080ed030(s32 column, s32 row) {
  u32 i;

  for (i = 0; i <= 20; i++) {
    s32 line = row + i;
    s32 source = line;
    s32 tile;

    if (line > 23)
      source = line - 24;
    tile = ((column & 31) + (line << 5) + 128) % 768;
    source <<= 6;
    source = Add(column, source);
    source <<= 6;
    tile <<= 6;
    line = 0x06004000;
    Dma_Set(Ram_MapCellBuffer + source, (u8 *)line + tile,
            (DMA_ENABLE | DMA_32BIT) << 16 | 16, REG_DMA3);
  }
}
