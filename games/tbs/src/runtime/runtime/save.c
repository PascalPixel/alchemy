/// @file
/// Saving: flash sectors, records and the save workspace.
// Part of runtime.c.

#include "../runtime_internal.h"

// Saving. The flash chip holds 16 sectors of 4 KB; each holds one record: a
// 16-byte header (signature, record id, checksum of the payload,
// counter) and its payload. A save file is records n and n + 3, for n below
// SAVE_FILE_COUNT. Writing a record goes to a free sector before the old copy
// is invalidated, and the highest counter of a record id is its current
// copy.

enum {
  SAVE_FLASH_TIMER = 2, ///< the hardware timer handed to the flash library
  SAVE_FLASH_IDENTIFY_ATTEMPTS = 8, ///< tries, a frame apart
  /// Sectors scanned; as a sector number, "none" in the functions below.
  SAVE_SECTOR_COUNT = 16,
  SAVE_RECORD_NONE = 16, ///< the record id of a free or invalidated sector
  SAVE_FILE_COUNT = 3,   ///< save files; file n is records n and n + 3
  /// The highest counter a write keeps; a copy written past it is
  /// rewritten as 1.
  SAVE_COUNTER_LIMIT = 65000,
  /// Payload bytes summed into the checksum: all 0xff0 but the last eight.
  SAVE_CHECKSUM_BYTES = 0xfe8
};

/// Allocates the save workspace (heap slot HEAP_SLOT_SAVE), installs the flash
/// library's SAVE_FLASH_TIMER handler, identifies the flash chip (up to eight
/// tries, a frame apart) and scans all 16 sectors into the workspace's
/// occupied, recordId and counter tables: a sector counts as taken when its
/// signature and checksum are good, and of two copies of a record only the
/// newer stays taken. Returns 1 when no flash chip answers, 0 otherwise; either
/// way the caller ends with Save_ReleaseWork. Called by the save
/// screens (window/screens/save.c) before every save, load, copy or
/// delete. The workspace is EWRAM first (AllocHeapSlotEwram) and not cleared
/// by the allocator, so it is cleared here; the record and sector-table
/// functions in this file read it through gSaveWork, the slot's word in
/// gHeapSlots, and need it allocated.
s32 Save_InitializeWork(void) {
  struct SaveWork *work;
  struct SaveSectorHeader headerStorage;
  struct SaveSectorHeader *header;
  u32 index;

  work = AllocHeapSlotEwram(HEAP_SLOT_SAVE, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  SetFlashTimerIntr(SAVE_FLASH_TIMER, &gIrqHandlers[IRQ_TIMER2]);

  // Yield after every failed probe, including the last one, before
  // reporting that no flash chip answered.
  for (index = 0; index < SAVE_FLASH_IDENTIFY_ATTEMPTS; index++) {
    if ((u16)IdentifyFlash() == 0)
      break;
    WaitFrames(1);
  }
  if (index >= SAVE_FLASH_IDENTIFY_ATTEMPTS)
    return 1;

  for (index = 0; index < SAVE_SECTOR_COUNT; index++) {
    u32 status;
    u32 record;
    u8 id;

    work->occupied[index] = 0;
    work->recordId[index] = SAVE_RECORD_NONE;
    work->counter[index] = 0;
    status = Save_ReadSector(index);
    START_DMA(&work->slot, &headerStorage, DMA_COPY_WORDS(4));
    WAIT_DMA();
    header = &headerStorage;

    // Not a save sector, or one for no record, or a damaged one: it stays
    // free. A sector with the signature still keeps its counter.
    if (Save_CompareBytes(header->signature, (u8 *)gSave_Signature, 7) != 0)
      continue;
    work->counter[index] = header->counter;
    record = header->recordId;
    id = record;
    if (id >= SAVE_RECORD_NONE)
      continue;
    if (status != 0)
      continue;

    work->occupied[index] = 1;
    work->recordId[index] = record;
    // An interrupted replacement can leave both copies valid. Retain the
    // larger counter, preferring the earlier sector when they tie; status
    // is the successful checksum result (0), reused as the scan cursor.
    if (status < index) {
      do {
        if (work->recordId[status] == (u8)record) {
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

/// Picks the free sector to write record `mode` to, at random. The last free
/// sector only goes to a record that already has a copy, so replacing a save
/// always has room. Called by Save_WriteRecord. Returns SAVE_SECTOR_COUNT
/// when there is no sector to use.
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
      // on which free sector a record takes.
      index = Random16() % count;
      index = empty[index];
    }
  }
  return index;
}

/// Programs the workspace's sector buffer into flash sector `sector` and
/// verifies it, through the flash library's routine for this chip. Called by
/// Save_WriteRecord and Save_InvalidateSector. Returns 0 on success
/// and 1 on failure.
u32 Save_WriteSector(s32 sector) {
  s32 *param = &gProgramFlashSector;
  s32 result;
  struct SaveWork *work;
  s32 value;

  work = gSaveWork;
  value = (u16)sector;
  // The chip's program routine, from gProgramFlashSector, reports an error in
  // its result's low halfword
  if ((_call_via_r3(value, (s32)&work->slot, (s32)param, *param) << 0x10) !=
      0) {
    return 1U;
  }
  result = Flash_VerifySector(value, (s32)&work->slot);
  return (u32)((0 - result) | result) >> 0x1F;
}

/// Reads flash sector `index` into the workspace's sector buffer. Called by
/// Save_InitializeWork and Save_ReadRecord. Returns 0
/// when the payload matches the header's checksum, and the difference of the
/// two checksums otherwise.
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

typedef u16 (*EraseSectorProc)(u16);

/// Unused: erases flash sector `sector` with the flash library's routine for
/// this chip. Nothing calls it.
UNUSED u16 Save_EraseSector(u16 sector) {
  return ((EraseSectorProc)gEraseFlashSector)(sector);
}

/// Saves 0xff0 bytes from `source` as record `recordId`: writes the new copy
/// with the next counter, then invalidates the old one, updating the
/// workspace's sector tables. When the counter passes 65000 it restarts at 1,
/// written back over the old sector. Called by the save screens. Returns 0 on
/// success and 1 on failure.
s32 Save_WriteRecord(s32 recordId, void *source) {
  struct SaveWork *work;
  struct SaveSectorHeader header;
  u32 current;
  u32 slot;

  work = gSaveWork;
  Dma_Fill32(0, &work->slot, sizeof(work->slot));
  WAIT_DMA();
  current = Save_FindLatestSector(recordId);
  slot = Save_SelectWriteSector(recordId);
  if (slot >= SAVE_SECTOR_COUNT)
    return 1;

  // Build the new sector before changing which old sector counts as current.
  START_DMA(source, work->slot.record.payload, DMA_COPY_WORDS(0x3fc));
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, DMA_COPY_WORDS(2));
  WAIT_DMA();
  header.recordId = recordId;
  header.checksum = Save_ChecksumSector();
  header.counter = Save_GetLatestCounter(recordId) + 1;
  START_DMA(&header, &work->slot.record.header, DMA_COPY_WORDS(4));
  WAIT_DMA();

  if (Save_WriteSector(slot) != 0)
    return 1;
  // A power loss before invalidation leaves two valid copies; the next
  // scan chooses the new one by its higher counter.
  if (current < SAVE_SECTOR_COUNT && Save_InvalidateSector(current) != 0)
    return 1;

  if (header.counter > SAVE_COUNTER_LIMIT) {
    // Counter order is ordinary, not modular. Keep the high-counter copy
    // valid until the old sector has been rewritten and verified as 1, then
    // invalidate the high copy so it cannot win the next scan.
    header.counter = 1;
    START_DMA(&header, &work->slot.record.header, DMA_COPY_WORDS(4));
    WAIT_DMA();
    if (Save_WriteSector(current) != 0)
      return 1;
    if (Save_InvalidateSector(slot) != 0)
      return 1;
    slot = current;
  }

  work->occupied[slot] = 1;
  work->recordId[slot] = recordId;
  work->counter[slot] = header.counter;
  return 0;
}

/// Loads the current copy of record `recordId` (0xff0 bytes) into
/// `destination`. Called by the save screens to load or copy a file. Returns
/// 1 when there is none and 0 otherwise. The checksum result of the read is
/// ignored: a sector counts as occupied only after the scan's checksum or a
/// verified write.
u32 Save_ReadRecord(s32 recordId, void *destination) {
  struct SaveWork *work;
  u32 index;

  work = gSaveWork;
  index = Save_FindLatestSector(recordId);
  if (index >= SAVE_SECTOR_COUNT)
    return 1;
  Save_ReadSector(index);
  START_DMA(work->slot.record.payload, destination, DMA_COPY_WORDS(0x3fc));
  WAIT_DMA();
  return 0;
}

/// Invalidates the current copy of record `recordId`. Called when the
/// file list erases a file. Returns 0 on success and 1 when there is none or
/// the write failed.
u32 Save_DeleteRecord(s32 recordId) {
  s32 index;
  s32 deletionResult;

  index = Save_FindLatestSector(recordId);
  if ((u32)index >= SAVE_SECTOR_COUNT) {
    return 1U;
  }
  deletionResult = Save_InvalidateSector(index);
  return (u32)((0 - deletionResult) | deletionResult) >> 0x1F;
}

/// Sums the first 0xfe8 payload bytes of the workspace's sector buffer; the
/// final eight payload bytes are excluded. Called when a sector is read and
/// before one is written.
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
    sum += work->slot.record.payload[offset + 0];
    sum += work->slot.record.payload[offset + 1];
    sum += work->slot.record.payload[offset + 2];
    sum += work->slot.record.payload[offset + 3];
    sum += work->slot.record.payload[offset + 4];
    sum += work->slot.record.payload[offset + 5];
    sum += work->slot.record.payload[offset + 6];
    sum += work->slot.record.payload[offset + 7];
    offset += 8;
  } while (offset <= limit);
  return sum;
}

/// Returns the occupied sector holding the newest copy of record `recordId`,
/// or SAVE_SECTOR_COUNT. Called by every record operation in this file.
u32 Save_FindLatestSector(s32 recordId) {
  struct SaveWork *work;
  u32 latestCounter;
  u32 latestSlot;
  u32 i;

  work = gSaveWork;
  latestSlot = SAVE_SECTOR_COUNT;
  latestCounter = 0;
  for (i = 0; i < SAVE_SECTOR_COUNT; i++) {
    if (work->occupied[i] != 0 && recordId == work->recordId[i]) {
      if (latestCounter < work->counter[i]) {
        latestCounter = work->counter[i];
        latestSlot = i;
      }
    }
  }
  return latestSlot;
}

/// Overwrites sector `index` with a header for no record (SAVE_RECORD_NONE) and
/// clears its occupied entry. Called by Save_WriteRecord and
/// Save_DeleteRecord. Returns 0 on success and 1 on failure.
s32 Save_InvalidateSector(s32 index) {
  struct SaveWork *work;
  struct SaveSectorHeader header;

  work = gSaveWork;
  Dma_Fill32(0, &header, sizeof(header));
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, DMA_COPY_WORDS(2));
  WAIT_DMA();
  header.recordId = SAVE_RECORD_NONE;
  header.counter = 0;
  START_DMA(&header, &work->slot.record.header, DMA_COPY_WORDS(4));
  WAIT_DMA();
  if (Save_WriteSector(index) != 0)
    return 1;
  work->occupied[index] = 0;
  work->recordId[index] = SAVE_RECORD_NONE;
  work->counter[index] = 0;
  return 0;
}

/// Compares `count` bytes, as memcmp does. Called by
/// Save_InitializeWork to check each sector's signature.
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

/// Returns the highest counter among the copies of record `recordId`.
/// Called by Save_WriteRecord to number the new copy.
u32 Save_GetLatestCounter(s32 recordId) {
  struct SaveWork *work;
  u32 latestCounter;
  u32 i;

  work = gSaveWork;
  latestCounter = 0;
  for (i = 0; i < SAVE_SECTOR_COUNT; i++) {
    if (work->occupied[i] != 0 && recordId == work->recordId[i]) {
      if (latestCounter < work->counter[i])
        latestCounter = work->counter[i];
    }
  }
  return latestCounter;
}

/// Reads the summary at the start of each of the three save files into the
/// workspace's summary table for the file list, and four bytes from
/// offset 0x110 of each file's second record into its secondRecordPlayTime
/// (zero when it has none). A file with no first record keeps a cleared
/// summary, so its level reads 0. Called by the save screens. Returns how many
/// save files exist.
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
      ReadFlash(index, 0x110, summary->secondRecordPlayTime,
                sizeof(summary->secondRecordPlayTime));
    else
      *(u32 *)summary->secondRecordPlayTime = 0;
    group++;
    summary++;
  } while (group < SAVE_FILE_COUNT);
  return count;
}

/// Removes the flash timer handler and frees the save workspace's heap slot,
/// with every block allocated after it in the same pool. Called by the save
/// screens after every save operation. It returns no value, whatever its
/// declaration says; no caller reads one.
u32 Save_ReleaseWork(void) {
  Irq_SetHandler(IRQ_TIMER2, 0, NULL);
  FreeHeapSlot(HEAP_SLOT_SAVE);
  // UB: nothing is returned, and no caller reads the result
#ifdef UBFIX
  return 0;
#endif
}
