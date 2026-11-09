#include "types.h"
#include "heap_release.h"
#include "resource.h"
#include "dma.h"
#include "io_reg.h"

extern const u16 gData_0802f380[][6];
extern char ResourceId_Map000;

/// Decodes the palette resource of a map's header row (its second halfword)
/// and copies its 224 colors to destination, keeping the current backdrop
/// color in the first entry.
void Func_0802a650(s32 map, void *destination) {
  const u16 *entry;
  u16 *buffer;
  s16 backdrop;

  entry = gData_0802f380[map];
  buffer = Heap_Alloc(0x200);
  backdrop = *(s16 *)0x05000000;
  Resource_DecodeType01(
      Resource_GetTableEntry(entry[1] + (u32)&ResourceId_Map000), buffer);
  buffer[0] = backdrop;
  Dma_Set(buffer, destination, DMA_COPY_WORDS(112), REG_DMA3);
  Heap_Release(buffer);
}
