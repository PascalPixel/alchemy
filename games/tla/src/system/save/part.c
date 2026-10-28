#include "types.h"
#include "ram_buffer.h"
#include "save.h"
#include "savedata.h"
#include "dma.h"
#include "flash.h"

#define DMA3 ((volatile struct DmaRegisters *)&REG_DMA3SAD)
#define DMA3_BUSY (DMA_ENABLE << 16)

#define START_DMA(source_, destination_, control_)                             \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_), REG_DMA3)

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

u32 Save_ReadPart(s32 partId, void *destination) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  u32 index;

  index = Save_FindLatestSector(partId);
  if (index > 14)
    return 1;
  Save_ReadSector(index);
  START_DMA(work->slot + 16, destination, DMA_COPY_WORDS(3068));
  WAIT_DMA();
  return 0;
}

u32 Save_DeletePart(s32 partId) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  s32 index;
  s32 result;

  index = Save_FindLatestSector(partId);
  if ((u32)index > 14)
    return 1;
  work->sectorCount = 0;
  work->sectorTotal = 3;
  result = Save_InvalidateSector(index);
  return (u32)((0 - result) | result) >> 31;
}

s32 Save_ChecksumSector(void) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  u32 limit;
  u32 offset;
  s32 sum;

  sum = 0;
  offset = 0;
  limit = 0x2fef;
  do {
    sum += work->slot[16 + offset + 0];
    sum += work->slot[16 + offset + 1];
    sum += work->slot[16 + offset + 2];
    sum += work->slot[16 + offset + 3];
    sum += work->slot[16 + offset + 4];
    sum += work->slot[16 + offset + 5];
    sum += work->slot[16 + offset + 6];
    sum += work->slot[16 + offset + 7];
    offset += 8;
  } while (offset <= limit);
  return sum;
}

u32 Save_FindLatestSector(s32 partId) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  u32 latestCounter;
  u32 latestSlot;
  u32 i;

  latestCounter = 0;
  latestSlot = 15;
  for (i = 0; i <= 14; i += 3) {
    if (work->occupied[i] != 0 && partId == work->partId[i]) {
      if (latestCounter < work->counter[i]) {
        latestCounter = work->counter[i];
        latestSlot = i;
      }
    }
  }
  return latestSlot;
}

s32 Save_InvalidateSector(s32 index) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  struct SaveSectorHeader header;
  volatile u32 zero;

  zero = 0;
  START_DMA(&zero, &header, (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 4);
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, DMA_COPY_WORDS(2));
  WAIT_DMA();
  header.partId = 16;
  header.counter = 0;
  START_DMA(&header, work->slot, DMA_COPY_WORDS(4));
  WAIT_DMA();
  if (Save_WriteSector(index) != 0)
    return 1;
  work->occupied[index] = 0;
  work->partId[index] = 16;
  work->counter[index] = 0;
  return 0;
}
