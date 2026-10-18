// Part of runtime.c, the runtime object, compiled only through it.
// Saving: flash sectors, records and the save workspace.

#include "../runtime_internal.h"

// Saving. The flash chip holds 16 sectors of 4 KB; each holds one record: a
// 16-byte header (signature, record id, checksum of the payload, sequence
// number) and its payload. A save file is records n and n + 3 (n = 0 to 2).
// Writing a record goes to a free sector before the old copy is invalidated,
// and the highest sequence number of a record id is its current copy.

enum {
  SAVE_FLASH_IDENTIFY_ATTEMPTS = 8,
  SAVE_SECTOR_COUNT = 16,
  SAVE_RECORD_NONE = 16,
  SAVE_FILE_COUNT = 3,
  SAVE_SEQUENCE_LIMIT = 65000,
  SAVE_CHECKSUM_BYTES = 0xfe8
};

// Allocates the save workspace, identifies the flash chip (up to eight
// tries) and scans all 16 sectors: a sector counts as taken when its
// signature and checksum are good, and of two copies of a record only the
// newer stays taken. Returns 1 when no flash chip answers, 0 otherwise.
s32 SaveState_InitializeWorkspace(void) {
  struct SaveWorkspace *work;
  struct SaveSlotHeader header;
  struct SaveSlotHeader *slot;
  u32 index;

  work = Runtime_AllocateBlock(HEAP_SLOT_SAVE, sizeof(*work));
  Dma_Fill32(0, work, sizeof(*work));
  SetFlashTimerIntr(2, &gIrqHandlers[IRQ_TIMER2]);

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
    work->sequence[index] = 0;
    status = SaveState_ReadSlotAndCheckChecksum(index);
    START_DMA(&work->slot, &header, (DMA_ENABLE | DMA_32BIT) << 16 | 4);
    WAIT_DMA();
    slot = &header;

    if (SaveState_CompareBytes(slot->signature, (u8 *)gSave_Signature, 7) != 0)
      continue;
    work->sequence[index] = slot->sequence;
    record = slot->recordId;
    id = record;
    if (id >= SAVE_RECORD_NONE)
      continue;
    if (status != 0)
      continue;

    work->occupied[index] = 1;
    work->recordId[index] = record;
    if (status < index) {
      do {
        if (work->recordId[status] == (u8)record) {
          if (work->sequence[status] < slot->sequence)
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

// Picks the free sector to write record `mode` to, at random. The last free
// sector only goes to a record that already has a copy, so replacing a save
// always has room. Returns 16 when there is no sector to use.
u32 SaveState_SelectWriteSlot(s32 mode) {
  u32 empty[SAVE_SECTOR_COUNT];
  s32 count;
  u32 index;
  u8 *slot;
  u8 value;

  slot = gSaveWorkspace->occupied;
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
      if (SaveState_FindLatestSlot(mode) == SAVE_SECTOR_COUNT) {
        index = SAVE_SECTOR_COUNT;
      }
    } else {
      index = Random16() % count;
      index = empty[index];
    }
  }
  return index;
}

// Programs the workspace's sector buffer into flash sector `sector` and
// verifies it. Returns 0 on success and 1 on failure.
u32 SaveState_WriteWorkspaceSlot(s32 sector) {
  s32 *param = &gFlash_Handler0;
  s32 result;
  struct SaveWorkspace *work;
  s32 value;

  work = gSaveWorkspace;
  value = (u16)sector;
  if ((_call_via_r3(value, (s32)&work->slot, (s32)param, *param) << 0x10) !=
      0) {
    return 1U;
  }
  result = Flash_VerifySector(value, (s32)&work->slot);
  return (u32)((0 - result) | result) >> 0x1F;
}

// Reads flash sector `index` into the workspace's sector buffer. Returns 0
// when the payload matches the header's checksum.
s32 SaveState_ReadSlotAndCheckChecksum(s32 index) {
  struct SaveWorkspace *work;
  struct SaveSlotHeader header;
  u32 checksum;

  work = gSaveWorkspace;
  ReadFlash(index, 0, work->slot.bytes, sizeof(work->slot));
  Dma_Set(&work->slot, &header, (DMA_ENABLE | DMA_32BIT) << 16 | 4, REG_DMA3);
  WAIT_DMA();
  checksum = SaveState_ChecksumWorkspace();
  return (u16)checksum - header.checksum;
}

typedef u16 (*EraseSectorProc)(u16);

// Erases flash sector `sector` with the flash library's routine for this chip.
u16 SaveState_EraseSlotSector(u16 sector) {
  return ((EraseSectorProc)gEraseFlashSector)(sector);
}

// Saves 0xff0 bytes from `source` as record `recordId`: writes the new copy
// with the next sequence number, then invalidates the old one. When the
// sequence passes 65000 it restarts at 1, written back over the old sector.
// Returns 0 on success and 1 on failure.
s32 SaveState_WriteRecord(s32 recordId, void *source) {
  struct SaveWorkspace *work;
  struct SaveSlotHeader header;
  u32 current;
  u32 slot;

  work = gSaveWorkspace;
  Dma_Fill32(0, &work->slot, sizeof(work->slot));
  WAIT_DMA();
  current = SaveState_FindLatestSlot(recordId);
  slot = SaveState_SelectWriteSlot(recordId);
  if (slot >= SAVE_SECTOR_COUNT)
    return 1;

  // Build the new sector before changing which old sector counts as current.
  START_DMA(source, work->slot.record.payload,
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x3fc);
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, (DMA_ENABLE | DMA_32BIT) << 16 | 2);
  WAIT_DMA();
  header.recordId = recordId;
  header.checksum = SaveState_ChecksumWorkspace();
  header.sequence = SaveState_GetLatestSequence(recordId) + 1;
  START_DMA(&header, &work->slot.record.header,
            (DMA_ENABLE | DMA_32BIT) << 16 | 4);
  WAIT_DMA();

  if (SaveState_WriteWorkspaceSlot(slot) != 0)
    return 1;
  if (current < SAVE_SECTOR_COUNT && SaveState_InvalidateSlot(current) != 0)
    return 1;

  if (header.sequence > SAVE_SEQUENCE_LIMIT) {
    // Restart sequence numbering only after the replacement was written.
    header.sequence = 1;
    START_DMA(&header, &work->slot.record.header,
              (DMA_ENABLE | DMA_32BIT) << 16 | 4);
    WAIT_DMA();
    if (SaveState_WriteWorkspaceSlot(current) != 0)
      return 1;
    if (SaveState_InvalidateSlot(slot) != 0)
      return 1;
    slot = current;
  }

  work->occupied[slot] = 1;
  work->recordId[slot] = recordId;
  work->sequence[slot] = header.sequence;
  return 0;
}

// Loads the current copy of record `recordId` into `destination`. Returns 1
// when there is none and 0 otherwise.
u32 SaveState_ReadRecordPayload(s32 recordId, void *destination) {
  struct SaveWorkspace *work;
  u32 index;

  work = gSaveWorkspace;
  index = SaveState_FindLatestSlot(recordId);
  if (index >= SAVE_SECTOR_COUNT)
    return 1;
  SaveState_ReadSlotAndCheckChecksum(index);
  START_DMA(work->slot.record.payload, destination,
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x3fc);
  WAIT_DMA();
  return 0;
}

// Invalidates the current copy of record `recordId`. Returns 0 on success
// and 1 when there is none or the write failed.
u32 SaveState_DeleteRecord(s32 recordId) {
  s32 index;
  s32 deletionResult;

  index = SaveState_FindLatestSlot(recordId);
  if ((u32)index >= SAVE_SECTOR_COUNT) {
    return 1U;
  }
  deletionResult = SaveState_InvalidateSlot(index);
  return (u32)((0 - deletionResult) | deletionResult) >> 0x1F;
}

// Sums the first 0xfe8 payload bytes; the final eight payload bytes are
// excluded.
s32 SaveState_ChecksumWorkspace(void) {
  struct SaveWorkspace *work;
  u32 limit;
  u32 offset;
  s32 sum;

  work = gSaveWorkspace;
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

// Returns the sector holding the newest copy of record `recordId`, or 16.
u32 SaveState_FindLatestSlot(s32 recordId) {
  struct SaveWorkspace *work;
  u32 latestSequence;
  u32 latestSlot;
  u32 i;

  work = gSaveWorkspace;
  latestSlot = SAVE_SECTOR_COUNT;
  latestSequence = 0;
  for (i = 0; i < SAVE_SECTOR_COUNT; i++) {
    if (work->occupied[i] != 0 && recordId == work->recordId[i]) {
      if (latestSequence < work->sequence[i]) {
        latestSequence = work->sequence[i];
        latestSlot = i;
      }
    }
  }
  return latestSlot;
}

// Overwrites sector `index` with a header for no record (id 16) and marks it
// free. Returns 0 on success and 1 on failure.
s32 SaveState_InvalidateSlot(s32 index) {
  struct SaveWorkspace *work;
  struct SaveSlotHeader header;

  work = gSaveWorkspace;
  Dma_Fill32(0, &header, sizeof(header));
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, (DMA_ENABLE | DMA_32BIT) << 16 | 2);
  WAIT_DMA();
  header.recordId = SAVE_RECORD_NONE;
  header.sequence = 0;
  START_DMA(&header, &work->slot.record.header,
            (DMA_ENABLE | DMA_32BIT) << 16 | 4);
  WAIT_DMA();
  if (SaveState_WriteWorkspaceSlot(index) != 0)
    return 1;
  work->occupied[index] = 0;
  work->recordId[index] = SAVE_RECORD_NONE;
  work->sequence[index] = 0;
  return 0;
}

// Compares `count` bytes, as memcmp does.
s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count) {
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

// Returns the highest sequence number among the copies of record `recordId`.
u32 SaveState_GetLatestSequence(s32 recordId) {
  struct SaveWorkspace *work;
  u32 latestSequence;
  u32 i;

  work = gSaveWorkspace;
  latestSequence = 0;
  for (i = 0; i < SAVE_SECTOR_COUNT; i++) {
    if (work->occupied[i] != 0 && recordId == work->recordId[i]) {
      if (latestSequence < work->sequence[i])
        latestSequence = work->sequence[i];
    }
  }
  return latestSequence;
}

// Reads the summary at the start of each of the three save files for the
// file select screen, and four bytes from offset 0x110 of each file's second
// record. Returns how many save files exist.
s32 SaveState_LoadSummaryRecords(void) {
  struct SaveWorkspace *work;
  struct SaveSummary *summary;
  u32 group;
  s32 count;

  work = gSaveWorkspace;
  summary = work->summary;
  count = 0;
  group = 0;
  do {
    u32 index;

    Dma_Fill32(0, summary, sizeof(*summary));
    index = SaveState_FindLatestSlot(group);
    if (index < SAVE_SECTOR_COUNT) {
      ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
      count++;
    }
    index = SaveState_FindLatestSlot(group + SAVE_FILE_COUNT);
    if (index < SAVE_SECTOR_COUNT)
      ReadFlash(index, 0x110, summary->unk38, sizeof(summary->unk38));
    else
      *(u32 *)summary->unk38 = 0;
    group++;
    summary++;
  } while (group < SAVE_FILE_COUNT);
  return count;
}

// Removes the flash timer handler and frees the save workspace. Declared as
// returning a value, it returns none (pop {r1}; bx r1).
u32 SaveState_ReleaseWorkspace(void) {
  Runtime_SetIrqHandler(IRQ_TIMER2, 0, NULL);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_SAVE);
}
