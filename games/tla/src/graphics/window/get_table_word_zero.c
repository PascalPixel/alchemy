/// @file
/// Windows: return the first entry of the fixed tile table.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "dma.h"
#include "ramcode.h"

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_Decompress4bppCodeSize[];

/// Returns gUi_FixedTileBlocks[0], whatever no is.
s32 Ui_GetTableWordZero(s32 no) {
  if (no != 0)
    no = 0;
  return gUi_FixedTileBlocks[no];
}

/// Copies the 4bpp decompression routine to RAM and runs it on the stream
/// whose address the work holds at 0x604, decoding into the work.
void Func_08045564(u8 *work) {
  void (*routine)(void *, void *);

  RamCode_Load(routine, Tile_Decompress4bppCode, Tile_Decompress4bppCodeSize);
  routine(*(void **)(work + 0x604), work);
  MemFree(routine);
}
