#include "types.h"
#include "ram_buffer.h"
#include "save.h"
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
extern const u8 gSave_Signature[];
extern const u8 gSave_HeaderTemplate[];

u32 Random16(void);
s32 Save_CompareBytes(u8 *left, u8 *right, s32 count);
s32 Save_ChecksumSector(void);
s32 Save_ReadSector(s32 index);
u32 Save_WriteSector(s32 sector);
s32 Save_InvalidateSector(s32 index);

u32 Save_SelectWriteSector(s32 partId);
u32 Save_GetLatestCounter(s32 partId);

s32 Func_08015ca4(s32 partId, void *source) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  struct SaveSectorHeader header;
  volatile u32 zero;
  u32 current;
  u32 sector;

  zero = 0;
  START_DMA(&zero, work->slot,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
                (sizeof(work->slot) / 4));
  WAIT_DMA();
  current = Save_FindLatestSector(partId);
  sector = Save_SelectWriteSector(partId);
  if (sector > 14)
    return 1;
  START_DMA(source, work->slot + 16,
            DMA_COPY_WORDS((sizeof(work->slot) - 16) / 4));
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, DMA_COPY_WORDS(2));
  WAIT_DMA();
  header.partId = partId;
  header.checksum = Save_ChecksumSector();
  header.counter = Save_GetLatestCounter(partId) + 1;
  START_DMA(&header, work->slot, DMA_COPY_WORDS(4));
  WAIT_DMA();
  work->sectorCount = 0;
  work->sectorTotal = 3;
  if (current <= 14)
    work->sectorTotal = 6;
  if (Save_WriteSector(sector) != 0)
    return 2;
  if (current <= 14 && Save_InvalidateSector(current) != 0)
    return 3;
  if (header.counter > 0xfde8) {
    header.counter = 1;
    START_DMA(&header, work->slot, DMA_COPY_WORDS(4));
    WAIT_DMA();
    if (Save_WriteSector(current) != 0)
      return 4;
    if (Save_InvalidateSector(sector) != 0)
      return 5;
    sector = current;
  }
  work->occupied[sector] = 1;
  work->partId[sector] = partId;
  work->counter[sector] = header.counter;
  return 0;
}
