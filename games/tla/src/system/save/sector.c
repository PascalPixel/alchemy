#include "types.h"
#include "ram_buffer.h"
#include "save.h"
#include "savedata.h"
#include "dma.h"
#include "flash.h"

#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)
#define DMA3_BUSY 0x80000000

#define START_DMA(source_, destination_, control_)                             \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_),         \
          (volatile u32 *)0x040000d4)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA3_BUSY) != 0) {
  }
}

#define WAIT_DMA() WaitDma(DMA3)

extern u16 (*gFlash_Handler0)(u16 sector, u8 *source);

u32 Random16(void);

s32 Save_CompareBytes(u8 *left, u8 *right, s32 count);

s32 Save_ChecksumSector(void);

s32 Save_ReadSector(s32 index);

u32 Save_WriteSector(s32 sector);

s32 Save_InvalidateSector(s32 index);

/// Picks the free slot to write record `mode` to, at random among every third
/// of the 15 slots. The last free slot only goes to a record that already has
/// a copy, so replacing a save always has room. Returns 15 when there is no
/// slot to use.
// As ☀️'s Save_SelectWriteSector.
u32 Save_SelectWriteSector(s32 mode) {
  u32 empty[15];
  s32 count;
  u32 index;
  u8 *slot;
  u8 value;

  slot = ((struct SaveWorkspace *)Ram_HeapSlots->serialWork)->occupied;
  /* FAKEMATCH(one-pass): the count's clearing in a block of its own, so the
   * workspace pointer loads before the prologue's sub sp; as count = 0, 2 of
   * 39 instructions differ in tla-en. */
  do {
    count = 0;
  } while (0);
  index = 0;
  do {
    value = *slot;
    slot += 3;
    if (value == 0)
      empty[count++] = index;
    index += 3;
  } while (index <= 14);
  index = 15;
  if (count != 0) {
    if (count == 1) {
      index = empty[0];
      if (Save_FindLatestSector(mode) == 15)
        index = 15;
    } else {
      // A free slot at random, so the writes spread over the flash chip.
      index = Random16() % count;
      index = empty[index];
    }
  }
  return index;
}

u32 Save_WriteSector(s32 sector) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  void (*progress)(s32) = work->progress;

  if (progress != 0)
    progress(0);
  if (gFlash_Handler0(sector, work->slot) != 0 ||
      Flash_VerifySector(sector, (s32)work->slot) != 0)
    return 1;
  if (progress != 0)
    progress(1);
  if (gFlash_Handler0(sector + 1, work->slot + 0x1000) != 0 ||
      Flash_VerifySector(sector + 1, (s32)(work->slot + 0x1000)) != 0)
    return 1;
  if (progress != 0)
    progress(1);
  if (gFlash_Handler0(sector + 2, work->slot + 0x2000) != 0 ||
      Flash_VerifySector(sector + 2, (s32)(work->slot + 0x2000)) != 0)
    return 1;
  if (progress != 0)
    progress(1);
  return 0;
}

s32 Save_ReadSector(s32 index) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  struct SaveSlotHeader header;

  ReadFlash(index, 0, work->slot, 0x1000);
  ReadFlash(index + 1, 0, work->slot + 0x1000, 0x1000);
  ReadFlash(index + 2, 0, work->slot + 0x2000, 0x1000);
  START_DMA(work->slot, &header, 0x84000004);
  WAIT_DMA();
  return (u16)Save_ChecksumSector() - header.checksum;
}
