/// @file
/// Saving: flash sectors, parts and the save workspace.
// Part of system.c.

#include "../system_internal.h"

// Saving.

enum {
  SAVE_FLASH_TIMER = 2, ///< the hardware timer handed to the flash library
  SAVE_FLASH_IDENTIFY_ATTEMPTS = 8, ///< tries, a frame apart
  /// Sectors scanned; as a sector number, "none" in the functions below.
  SAVE_SECTOR_COUNT = 16,
  SAVE_PART_NONE = 16, ///< the part id of a free or invalidated sector
  SAVE_FILE_COUNT = 3, ///< save files; file n is parts n and n + 3
  /// The highest counter a write keeps; a copy written past it is
  /// rewritten as 1.
  SAVE_COUNTER_LIMIT = 65000,
  /// Payload bytes summed into the checksum: all 4080 but the last eight.
  SAVE_CHECKSUM_BYTES = 4072
};

/// Allocates the save workspace (heap slot HEAP_SLOT_SAVE), installs the flash
/// library's SAVE_FLASH_TIMER handler, identifies the flash chip (up to eight
/// tries, a frame apart) and scans all 16 sectors into the workspace's
/// occupied, partId and counter tables: a sector counts as taken when its
/// signature and checksum are good, and of two copies of a part only the newer
/// stays taken.
s32 Save_InitializeWork(void) {
  struct SaveWork *work;
  struct SaveSectorHeader headerStorage;
  struct SaveSectorHeader *header;
  u32 index;

  work = HeapSlot_AllocEwram(HEAP_SLOT_SAVE, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  SetFlashTimerIntr(SAVE_FLASH_TIMER, &gIrqHandlers[IRQ_TIMER2]);

  // Wait a frame after every failed probe, including the last one, before
  // reporting that no flash chip answered.
  for (index = 0; index < SAVE_FLASH_IDENTIFY_ATTEMPTS; index++) {
    if ((u16)IdentifyFlash() == 0)
      break;
    wait(1);
  }
  if (index >= SAVE_FLASH_IDENTIFY_ATTEMPTS)
    return 1;

  for (index = 0; index < SAVE_SECTOR_COUNT; index++) {
    u32 status;
    u32 part;

    work->occupied[index] = 0;
    work->partId[index] = SAVE_PART_NONE;
    work->counter[index] = 0;
    status = Save_ReadSector(index);
    START_DMA(&work->slot, &headerStorage, DMA_COPY_WORDS(4));
    WAIT_DMA();
    /* FAKEMATCH(temporary): header holds &headerStorage; as headerStorage
     * used directly, 98 of 140 instructions differ in tbs-en. */
    header = &headerStorage;

    // Not a save sector, or one for no part, or a damaged one: it stays free.
    if (Save_CompareBytes(header->signature, (u8 *)gSave_Signature, 7) != 0)
      continue;
    work->counter[index] = header->counter;
    part = header->partId;
    if ((u8)part >= SAVE_PART_NONE)
      continue;
    if (status != 0)
      continue;

    work->occupied[index] = 1;
    work->partId[index] = part;
    // An interrupted replacement can leave both copies valid.
    if (status < index) {
      do {
        if (work->partId[status] == (u8)part) {
          if (work->counter[status] < header->counter)
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

/// Picks the free sector to write part `mode` to, at random.
u32 Save_SelectWriteSector(s32 mode) {
  u32 empty[SAVE_SECTOR_COUNT];
  s32 count;
  u32 index;
  u8 *slot;
  u8 value;

  slot = gSaveWork->occupied;
  count = 0;
  index = 0;
  do {
    value = *slot++;
    if (value == 0) {
      empty[count++] = index;
    }
    index += 1;
  } while (index < SAVE_SECTOR_COUNT);
  index = SAVE_SECTOR_COUNT;
  if (count != 0) {
    if (count == 1) {
      index = empty[0];
      if (Save_FindLatestSector(mode) == SAVE_SECTOR_COUNT) {
        index = SAVE_SECTOR_COUNT;
      }
    } else {
      // A free sector at random, so the writes spread over the free sectors
      // and no sector of the flash chip wears out first; nothing else depends
      // on which free sector a part takes.
      index = Random16() % count;
      index = empty[index];
    }
  }
  return index;
}

/// Programs the workspace's sector buffer into flash sector `sector` and
/// verifies it, through the flash library's routine for this chip.
u32 Save_WriteSector(s32 sector) {
  struct SaveWork *work = gSaveWork;
  u16 index = sector;
  s32 result;

  if (gProgramFlashSector(index, work->slot.bytes) != 0)
    return 1;
  result = Flash_VerifySector(index, work->slot.bytes);
  return (u32)(-result | result) >> 31; // 1 for any nonzero result
}

/// Reads flash sector `index` into the workspace's sector buffer.
s32 Save_ReadSector(s32 index) {
  struct SaveWork *work;
  struct SaveSectorHeader header;
  u32 checksum;

  work = gSaveWork;
  ReadFlash(index, 0, work->slot.bytes, sizeof(work->slot));
  Dma_Set(&work->slot, &header, DMA_COPY_WORDS(4), REG_DMA3);
  WAIT_DMA();
  checksum = Save_ChecksumSector();
  return (u16)checksum - header.checksum;
}

/// Unused: erases flash sector `sector` with the flash library's routine for
/// this chip.
UNUSED u16 Save_EraseSector(u16 sector) { return gEraseFlashSector(sector); }

/// Saves 4080 bytes from `source` as part `partId`: writes the new copy with
/// the next counter, then invalidates the old one, updating the workspace's
/// sector tables.
s32 Save_WritePart(s32 partId, void *source) {
  struct SaveWork *work;
  struct SaveSectorHeader header;
  u32 current;
  u32 slot;

  work = gSaveWork;
  Dma_Fill32(0, &work->slot, sizeof(work->slot));
  WAIT_DMA();
  current = Save_FindLatestSector(partId);
  slot = Save_SelectWriteSector(partId);
  if (slot >= SAVE_SECTOR_COUNT)
    return 1;

  // Build the new sector before changing which old sector counts as current.
  START_DMA(source, work->slot.part.payload,
            DMA_COPY_WORDS(sizeof(work->slot.part.payload) / 4));
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, DMA_COPY_WORDS(2));
  WAIT_DMA();
  header.partId = partId;
  header.checksum = Save_ChecksumSector();
  header.counter = Save_GetLatestCounter(partId) + 1;
  START_DMA(&header, &work->slot.part.header, DMA_COPY_WORDS(4));
  WAIT_DMA();

  if (Save_WriteSector(slot) != 0)
    return 1;
  // A power loss before invalidation leaves two valid copies; the next
  // scan chooses the new one by its higher counter.
  if (current < SAVE_SECTOR_COUNT && Save_InvalidateSector(current) != 0)
    return 1;

  if (header.counter > SAVE_COUNTER_LIMIT) {
    // Counter order is ordinary, not modular.
    header.counter = 1;
    START_DMA(&header, &work->slot.part.header, DMA_COPY_WORDS(4));
    WAIT_DMA();
    if (Save_WriteSector(current) != 0)
      return 1;
    if (Save_InvalidateSector(slot) != 0)
      return 1;
    slot = current;
  }

  work->occupied[slot] = 1;
  work->partId[slot] = partId;
  work->counter[slot] = header.counter;
  return 0;
}

/// Loads the current copy of part `partId` (4080 bytes) into `destination`.
/// Returns 1 when there is none and 0 otherwise.
u32 Save_ReadPart(s32 partId, void *destination) {
  struct SaveWork *work;
  u32 index;

  work = gSaveWork;
  index = Save_FindLatestSector(partId);
  if (index >= SAVE_SECTOR_COUNT)
    return 1;
  Save_ReadSector(index);
  START_DMA(work->slot.part.payload, destination,
            DMA_COPY_WORDS(sizeof(work->slot.part.payload) / 4));
  WAIT_DMA();
  return 0;
}

/// Invalidates the current copy of part `partId`. Called when the file list
/// erases a file.
u32 Save_DeletePart(s32 partId) {
  s32 index;
  s32 deletionResult;

  index = Save_FindLatestSector(partId);
  if ((u32)index >= SAVE_SECTOR_COUNT) {
    return 1U;
  }
  deletionResult = Save_InvalidateSector(index);
  return (u32)((0 - deletionResult) | deletionResult) >> 31;
}

/// Sums the first 4072 payload bytes of the workspace's sector buffer; the
/// final eight payload bytes are excluded.
s32 Save_ChecksumSector(void) {
  struct SaveWork *work;
  u32 limit;
  u32 offset;
  s32 sum;

  work = gSaveWork;
  limit = SAVE_CHECKSUM_BYTES - 1;
  sum = 0;
  offset = 0;
  do {
    sum += work->slot.part.payload[offset + 0];
    sum += work->slot.part.payload[offset + 1];
    sum += work->slot.part.payload[offset + 2];
    sum += work->slot.part.payload[offset + 3];
    sum += work->slot.part.payload[offset + 4];
    sum += work->slot.part.payload[offset + 5];
    sum += work->slot.part.payload[offset + 6];
    sum += work->slot.part.payload[offset + 7];
    offset += 8;
  } while (offset <= limit);
  return sum;
}

/// Returns the occupied sector holding the newest copy of part `partId`, or
/// SAVE_SECTOR_COUNT.
u32 Save_FindLatestSector(s32 partId) {
  struct SaveWork *work;
  u32 latestCounter;
  u32 latestSlot;
  u32 i;

  work = gSaveWork;
  latestSlot = SAVE_SECTOR_COUNT;
  latestCounter = 0;
  for (i = 0; i < SAVE_SECTOR_COUNT; i++) {
    if (work->occupied[i] != 0 && partId == work->partId[i]) {
      if (latestCounter < work->counter[i]) {
        latestCounter = work->counter[i];
        latestSlot = i;
      }
    }
  }
  return latestSlot;
}

/// Overwrites sector `index` with a header for no part (SAVE_PART_NONE) and
/// clears its occupied entry. Returns 0 on success and 1 on failure.
s32 Save_InvalidateSector(s32 index) {
  struct SaveWork *work;
  struct SaveSectorHeader header;

  work = gSaveWork;
  Dma_Fill32(0, &header, sizeof(header));
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, DMA_COPY_WORDS(2));
  WAIT_DMA();
  header.partId = SAVE_PART_NONE;
  header.counter = 0;
  START_DMA(&header, &work->slot.part.header, DMA_COPY_WORDS(4));
  WAIT_DMA();
  if (Save_WriteSector(index) != 0)
    return 1;
  work->occupied[index] = 0;
  work->partId[index] = SAVE_PART_NONE;
  work->counter[index] = 0;
  return 0;
}

/// Compares `count` bytes, as memcmp does.
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

/// Returns the highest counter among the copies of part `partId`.
u32 Save_GetLatestCounter(s32 partId) {
  struct SaveWork *work;
  u32 latestCounter;
  u32 i;

  work = gSaveWork;
  latestCounter = 0;
  for (i = 0; i < SAVE_SECTOR_COUNT; i++) {
    if (work->occupied[i] != 0 && partId == work->partId[i]) {
      if (latestCounter < work->counter[i])
        latestCounter = work->counter[i];
    }
  }
  return latestCounter;
}

/// Reads the summary at the start of each of the three save files into the
/// workspace's summary table for the file list, and four bytes from offset
/// 0x110 of each file's second part into its secondPartPlayTime (zero when it
/// has none).
s32 Save_LoadFileSummaries(void) {
  struct SaveWork *work;
  struct SaveFileSummary *summary;
  u32 group;
  s32 count;

  work = gSaveWork;
  summary = work->summary;
  count = 0;
  group = 0;
  do {
    u32 index;

    Dma_Fill32(0, summary, sizeof(*summary));
    index = Save_FindLatestSector(group);
    if (index < SAVE_SECTOR_COUNT) {
      ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
      count++;
    }
    index = Save_FindLatestSector(group + SAVE_FILE_COUNT);
    if (index < SAVE_SECTOR_COUNT)
      ReadFlash(index, 0x110, summary->secondPartPlayTime,
                sizeof(summary->secondPartPlayTime));
    else
      *(u32 *)summary->secondPartPlayTime = 0;
    group++;
    summary++;
  } while (group < SAVE_FILE_COUNT);
  return count;
}

/// Removes the flash timer handler and frees the save workspace's heap slot,
/// with every block allocated after it in the same pool.
u32 Save_ReleaseWork(void) {
  Irq_SetHandler(IRQ_TIMER2, 0, NULL);
  HeapSlot_Release(HEAP_SLOT_SAVE);
  // UB: nothing is returned, and no caller reads the result
#ifdef UBFIX
  return 0;
#endif
}
