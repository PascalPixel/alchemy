/// @file
/// Map: unpack decoded block data into gMapBlocks.

#include "types.h"
extern u32 gMapBlocks[];
#include "ram_buffer.h"
#include "mem.h"
#include "ram_code.h"
#include "iwram_call.h"
extern u8 gMapCellBuffer[];

/// Decoded buffer [1] selects the method; halfword data starts at [2].
/// size: length including the method byte (even means no data).
/// Method 0: copy unchanged.
/// Method 1: high bytes followed by low bytes; reconstruct by XOR with prior
/// values. Method 2: reconstruct by XOR with prior values.
void Tilemap_DecodeStagedBuffer(s32 size) {
  u16 *dst;
  u16 *src;
  s32 n;
  s32 i;

  src = (u16 *)(Ram_MapCellBuffer + 2);
  dst = (u16 *)gMapBlocks;
  n = (size - 1) / 2;
  if (size & 1) {
    switch (((u8 *)gMapCellBuffer)[1]) {
    case 0:
      for (i = 0; i < n; i++)
        *dst++ = *src++;
      break;
    case 1: {
      u32 prev = 0;
      u8 *hi = Ram_MapCellBuffer + 2;
      u8 *lo = hi + n;

      for (i = 0; i < n; i++) {
        u32 v = ((*hi++ << 8) | *lo++) ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    case 2: {
      s32 prev = 0;

      for (i = 0; i < n; i++) {
        s32 v = *src++ ^ prev;
        *dst++ = v;
        prev = v;
      }
      break;
    }
    }
  }
}

typedef void (*ConvertFn)(void *destination, void *source, void *saved);

extern u8 Render_ConvertMapCode[];
extern u8 Render_ConvertMapCodeSize[];

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH: through an inline, as ☀️'s IWRAM call layer, the map cell
   * buffer's address is loaded again for the conversion; called directly its
   * constant is kept in a saved register (differ 20). */
  return Iwram_CopyWordsEntry(destination, source, size);
}

/// Converts the map cell buffer's 32 KB in place with the ARM routine copied
/// to RAM, keeping a copy of the buffer for it to read.
void Tilemap_ConvertBuffer(void) {
  u8 *saved;
  ConvertFn routine;
  u32 size;

  size = 0x8000;
  saved = Heap_AllocEwram(size);
  CopyWords(saved, Ram_MapCellBuffer, size);
  RamCode_Load(routine, Render_ConvertMapCode, Render_ConvertMapCodeSize);
  routine(((u8 *)gMapCellBuffer + 0x8000), Ram_MapCellBuffer, saved);
  Heap_Release(routine);
  Heap_Release(saved);
}
