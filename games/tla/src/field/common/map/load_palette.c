#include "types.h"
#include "mem.h"
#include "resource.h"
#include "dma.h"
#include "io_reg.h"

extern const u16 gData_0802f380[][6];
extern char ResourceId_Map000;

/// Decodes the palette resource of a map's header row (its second halfword)
/// and copies its 224 colors to destination, keeping the current backdrop
/// color in the first entry.
void Func_0802a650(s32 map, void *destination) {
  /* FAKEMATCH: the table address is a word local added after the row offset,
   * as in Map_GetHeaderSize; the native loads it before it scales the row. An
   * empty asm that passes both through ranks the load first; without it the
   * scaling comes first (differ 2). */
  u32 table = (u32)gData_0802f380;
  const u16 *entry;
  u16 *buffer;
  s16 backdrop;

  /* FAKEMATCH: the barrier of the table load above. */
  asm("" : "+l"(table), "+l"(map));
  entry = (const u16 *)(map * 12 + table);
  buffer = MemAlloc(0x200);
  backdrop = *(s16 *)0x05000000;
  Resource_DecodeType01(
      Resource_GetTableEntry(entry[1] + (u32)&ResourceId_Map000), buffer);
  buffer[0] = backdrop;
  Dma_Set(buffer, destination, 0x84000070, REG_DMA3);
  MemFree(buffer);
}
