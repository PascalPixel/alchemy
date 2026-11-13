/// @file
/// Saving: flash sectors, parts and the save workspace.
// Part of system.c.

#include "../system_internal.h"

// The save code starts each DMA through Dma_Set, where serial.h's START_DMA
// writes DMA3's registers itself.
#define DMA3_BUSY (DMA_ENABLE << 16)

#define SAVE_START_DMA(source_, destination_, control_)                        \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_), REG_DMA3)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA3_BUSY) != 0) {
  }
}

#define WAIT_DMA() WaitDma(DMA3)

extern const u8 gSave_HeaderTemplate[];

s32 Func_0801596c(void) {
  struct SaveWorkspace *work;
  struct SaveSectorHeader header;
  struct SaveSectorHeader *slot;
  u32 index;

  work = HeapSlot_AllocEwram(204, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  SetFlashTimerIntr(2, &gIrqHandlers[5]);

  for (index = 0; index < 8; index++) {
    if ((u16)IdentifyFlash() == 0)
      break;
    wait(1);
  }
  if (index >= 8)
    return 1;

  for (index = 0; index < 15; index += 3) {
    u32 status;
    u16 partId;
    u8 id;

    work->occupied[index] = 0;
    work->partId[index] = 16;
    work->counter[index] = 0;
    status = Save_ReadSector(index);
    SAVE_START_DMA(work->slot, &header, DMA_COPY_WORDS(4));
    WAIT_DMA();
    slot = &header;

    if (Save_CompareBytes(slot->signature, (u8 *)gSave_Signature, 7) != 0)
      continue;
    work->counter[index] = slot->counter;
    partId = slot->partId;
    id = partId;
    if (id >= 16)
      continue;
    if (status != 0)
      continue;

    work->occupied[index] = 1;
    work->partId[index] = partId;
    if (status < index) {
      do {
        if (work->partId[status] == (u8)partId) {
          if (work->counter[status] < slot->counter)
            work->occupied[status] = 0;
          else
            work->occupied[index] = 0;
        }
        status++;
      } while (status < index);
    }
  }
  return 0;
}

/// Picks the free sector to write part `mode` to, at random among every third
/// of the 15 sectors. The last free sector only goes to a part that already has
/// a copy, so replacing a save always has room. Returns 15 when there is no
/// sector to use.
// As ☀️'s Save_SelectWriteSector.
u32 Save_SelectWriteSector(s32 mode) {
  u32 empty[15];
  s32 count;
  u32 index;
  u8 *slot;
  u8 value;

  slot = ((struct SaveWorkspace *)Ram_HeapSlots->serialWork)->occupied;
  count = 0;
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
      // A free sector at random, so the writes spread over the flash chip.
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
  struct SaveSectorHeader header;

  ReadFlash(index, 0, work->slot, 0x1000);
  ReadFlash(index + 1, 0, work->slot + 0x1000, 0x1000);
  ReadFlash(index + 2, 0, work->slot + 0x2000, 0x1000);
  SAVE_START_DMA(work->slot, &header, DMA_COPY_WORDS(4));
  WAIT_DMA();
  return (u16)Save_ChecksumSector() - header.checksum;
}

s32 Func_08015ca4(s32 partId, void *source) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  struct SaveSectorHeader header;
  volatile u32 zero;
  u32 current;
  u32 sector;

  zero = 0;
  SAVE_START_DMA(&zero, work->slot,
                 (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
                     (sizeof(work->slot) / 4));
  WAIT_DMA();
  current = Save_FindLatestSector(partId);
  sector = Save_SelectWriteSector(partId);
  if (sector > 14)
    return 1;
  SAVE_START_DMA(source, work->slot + 16,
                 DMA_COPY_WORDS((sizeof(work->slot) - 16) / 4));
  WAIT_DMA();
  SAVE_START_DMA(gSave_HeaderTemplate, &header, DMA_COPY_WORDS(2));
  WAIT_DMA();
  header.partId = partId;
  header.checksum = Save_ChecksumSector();
  header.counter = Save_GetLatestCounter(partId) + 1;
  SAVE_START_DMA(&header, work->slot, DMA_COPY_WORDS(4));
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
    SAVE_START_DMA(&header, work->slot, DMA_COPY_WORDS(4));
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

u32 Save_ReadPart(s32 partId, void *destination) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  u32 index;

  index = Save_FindLatestSector(partId);
  if (index > 14)
    return 1;
  Save_ReadSector(index);
  SAVE_START_DMA(work->slot + 16, destination,
                 DMA_COPY_WORDS((sizeof(work->slot) - 16) / 4));
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
  SAVE_START_DMA(&zero, &header,
                 (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 4);
  WAIT_DMA();
  SAVE_START_DMA(gSave_HeaderTemplate, &header, DMA_COPY_WORDS(2));
  WAIT_DMA();
  header.partId = 16;
  header.counter = 0;
  SAVE_START_DMA(&header, work->slot, DMA_COPY_WORDS(4));
  WAIT_DMA();
  if (Save_WriteSector(index) != 0)
    return 1;
  work->occupied[index] = 0;
  work->partId[index] = 16;
  work->counter[index] = 0;
  return 0;
}

s32 Save_CompareBytes(u8 *left, u8 *right, s32 count) {
  s32 difference = 0;

  while (count != 0) {
    difference = *left - *right;
    if (difference != 0)
      break;
    count--;
    left++;
    right++;
  }
  return difference;
}

u32 Save_GetLatestCounter(s32 partId) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  u32 latestCounter;
  u32 i;

  latestCounter = 0;
  for (i = 0; i <= 14; i += 3) {
    if (work->occupied[i] != 0 && partId == work->partId[i]) {
      if (latestCounter < work->counter[i])
        latestCounter = work->counter[i];
    }
  }
  return latestCounter;
}

/// Reads the summary of the latest save in each of the three files, clearing
/// it first; returns how many files have one.
s32 Save_LoadFileSummaries(void) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  struct SaveSummary *summary;
  s32 count;
  u32 group;

  summary = work->summary;
  count = 0;
  for (group = 0; group <= 2; group++) {
    u32 slot;

    Dma_Fill32(0, summary, sizeof(*summary));
    slot = Save_FindLatestSector(group);
    if (slot <= 14) {
      ReadFlash(slot, 0, (u8 *)summary, sizeof(*summary));
      ReadFlash(slot + 1, 16, summary->unk38, sizeof(summary->unk38));
      count++;
    }
    summary++;
  }
  return count;
}

s32 Save_LoadExtraSummary(void) {
  struct SaveWorkspace *work;
  struct SaveSummary *summary;
  u32 index;

  work = Ram_HeapSlots->serialWork;
  summary = &work->extra;
  Dma_Fill32(0, summary, sizeof(*summary));
  index = Save_FindLatestSector(3);
  if (index <= 14) {
    ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
    ReadFlash(index + 1, 0x110, work->extra.unk38, sizeof(work->extra.unk38));
    return 1;
  }
  return 0;
}

void Save_ReleaseWork(void) {
  Irq_SetHandler(5, 0, NULL);
  HeapSlot_Release(204);
}

#undef DMA3_BUSY
#undef WAIT_DMA
#undef SAVE_START_DMA
