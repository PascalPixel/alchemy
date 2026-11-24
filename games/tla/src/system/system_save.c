/// @file
/// Saving: flash sectors, parts and the save workspace.
// Part of system.c.

#include "system_internal.h"

/// DMA 3's enable bit in its count and control word, which the hardware
/// clears when the transfer is done.
#define DMA3_BUSY (DMA_ENABLE << 16)

/// Starts DMA 3 copying from source_ to destination_ with control_.
#define SAVE_START_DMA(source_, destination_, control_)                        \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_), REG_DMA3)

/// Waits until `channel`'s transfer is done.
static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA3_BUSY) != 0) {
  }
}

/// Waits until a SAVE_START_DMA transfer is done.
#define WAIT_DMA() WaitDma(DMA3)

/// The bytes of a part's first sector that its SaveSectorHeader takes, before
/// the payload.
#define SAVE_HEADER_BYTES sizeof(struct SaveSectorHeader)

/// Where a file's second copy of the play time lies in its part's second
/// sector: Save_BuildFileSummary writes it to the first word of
/// gSceneLocalState, which opens the save image's second sector, after the
/// part's header.
#define SAVE_SECOND_PLAY_TIME_OFFSET SAVE_HEADER_BYTES
/// Where The Broken Seal reads that copy, 256 bytes further into
/// gSceneLocalState: Save_LoadExtraSummary reads part SAVE_PART_EXTRA's there.
#define SAVE_BROKEN_SEAL_SECOND_PLAY_TIME_OFFSET (SAVE_HEADER_BYTES + 256)

/// Allocates the save workspace (heap slot HEAP_SLOT_SAVE), installs the flash
/// library's SAVE_FLASH_TIMER handler, identifies the flash chip
/// (SAVE_FLASH_IDENTIFY_ATTEMPTS tries, a frame apart) and reads each part's
/// place, three sectors at a time, into the workspace's occupied, partId and
/// counter tables: a place counts as taken when its signature and checksum are
/// good, and of two copies of a part only the newer stays taken. Returns 1 when
/// no flash chip answers, else 0.
s32 Save_InitializeState(void) {
  struct SaveWorkspace *state;
  struct SaveSectorHeader header;
  struct SaveSectorHeader *slot;
  u32 index;

  state = HeapSlot_AllocEwram(HEAP_SLOT_SAVE, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  SetFlashTimerIntr(SAVE_FLASH_TIMER,
                    &gIrqHandlers[IRQ_TIMER0 + SAVE_FLASH_TIMER]);

  for (index = 0; index < SAVE_FLASH_IDENTIFY_ATTEMPTS; index++) {
    if ((u16)IdentifyFlash() == 0)
      break;
    wait(1);
  }
  if (index >= SAVE_FLASH_IDENTIFY_ATTEMPTS)
    return 1;

  for (index = 0; index < SAVE_SECTOR_COUNT; index += SAVE_SECTORS_PER_PART) {
    u32 status;
    u16 partId;
    u8 id;

    state->occupied[index] = 0;
    state->partId[index] = SAVE_PART_NONE;
    state->counter[index] = 0;
    status = Save_ReadSector(index);
    SAVE_START_DMA(state->slot, &header, DMA_COPY_WORDS(SAVE_HEADER_BYTES / 4));
    WAIT_DMA();
    slot = &header;

    if (Save_CompareBytes(slot->signature, (u8 *)gSave_Signature,
                          sizeof(slot->signature)) != 0)
      continue;
    state->counter[index] = slot->counter;
    partId = slot->partId;
    id = partId;
    if (id >= SAVE_PART_NONE)
      continue;
    if (status != 0)
      continue;

    state->occupied[index] = 1;
    state->partId[index] = partId;
    if (status < index) {
      for (; status < index; status++) {
        if (state->partId[status] == (u8)partId) {
          if (state->counter[status] < slot->counter)
            state->occupied[status] = 0;
          else
            state->occupied[index] = 0;
        }
      }
    }
  }
  return 0;
}

/// Picks the free place, three sectors from a multiple of three, to write part
/// `partId` to, at random among them. The last free place only goes to a part
/// that already has a copy, so replacing a save always has room. Returns
/// SAVE_SECTOR_COUNT when there is no place to use.
u32 Save_SelectWriteSector(s32 partId) {
  u32 empty[SAVE_SECTOR_COUNT];
  s32 count;
  u32 index;
  u8 *occupied;
  u8 value;

  occupied = ((struct SaveWorkspace *)Ram_HeapSlots->saveState)->occupied;
  count = 0;
  for (index = 0; index < SAVE_SECTOR_COUNT; index += SAVE_SECTORS_PER_PART) {
    value = *occupied;
    occupied += SAVE_SECTORS_PER_PART;
    if (value == 0)
      empty[count++] = index;
  }
  index = SAVE_SECTOR_COUNT;
  if (count != 0) {
    if (count == 1) {
      index = empty[0];
      if (Save_FindLatestSector(partId) == SAVE_SECTOR_COUNT)
        index = SAVE_SECTOR_COUNT;
    } else {
      // A free place at random, so the writes spread over the flash chip.
      index = Random16() % count;
      index = empty[index];
    }
  }
  return index;
}

/// Programs the workspace's three-sector buffer into flash from sector
/// `sector` on and verifies each, through the flash library's routine for this
/// chip, calling the progress routine before the first and after each. Returns
/// 1 when a sector fails, else 0.
u32 Save_WriteSector(s32 sector) {
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;
  void (*progress)(s32) = state->progress;

  if (progress != 0)
    progress(0);
  if (gProgramFlashSector(sector, state->slot) != 0 ||
      Flash_VerifySector(sector, (s32)state->slot) != 0)
    return 1;
  if (progress != 0)
    progress(1);
  if (gProgramFlashSector(sector + 1, state->slot + SAVE_SECTOR_SIZE) != 0 ||
      Flash_VerifySector(sector + 1, (s32)(state->slot + SAVE_SECTOR_SIZE)) !=
          0)
    return 1;
  if (progress != 0)
    progress(1);
  if (gProgramFlashSector(sector + 2, state->slot + 2 * SAVE_SECTOR_SIZE) !=
          0 ||
      Flash_VerifySector(sector + 2,
                         (s32)(state->slot + 2 * SAVE_SECTOR_SIZE)) != 0)
    return 1;
  if (progress != 0)
    progress(1);
  return 0;
}

/// Reads the three flash sectors from `index` on into the workspace's sector
/// buffer. Returns 0 when their checksum matches the header's.
s32 Save_ReadSector(s32 index) {
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;
  struct SaveSectorHeader header;

  ReadFlash(index, 0, state->slot, SAVE_SECTOR_SIZE);
  ReadFlash(index + 1, 0, state->slot + SAVE_SECTOR_SIZE, SAVE_SECTOR_SIZE);
  ReadFlash(index + 2, 0, state->slot + 2 * SAVE_SECTOR_SIZE, SAVE_SECTOR_SIZE);
  SAVE_START_DMA(state->slot, &header, DMA_COPY_WORDS(SAVE_HEADER_BYTES / 4));
  WAIT_DMA();
  return (u16)Save_ChecksumSector() - header.checksum;
}

/// Saves the 12,272 bytes at `source`, three sectors less the header, as part
/// `partId`: writes the new copy with the next counter, then invalidates the
/// old one, updating the workspace's sector tables. Returns 0, or the step that
/// failed: 1 no free place, 2 the new copy, 3 invalidating the old one, and,
/// once the counter passes SAVE_COUNTER_LIMIT and the copy is written again
/// with counter 1 over the old place, 4 that write and 5 invalidating the new
/// copy.
s32 Save_WritePart(s32 partId, void *source) {
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;
  struct SaveSectorHeader header;
  volatile u32 zero;
  u32 current;
  u32 sector;

  zero = 0;
  SAVE_START_DMA(&zero, state->slot, DMA_FILL_WORDS(sizeof(state->slot) / 4));
  WAIT_DMA();
  current = Save_FindLatestSector(partId);
  sector = Save_SelectWriteSector(partId);
  if (sector >= SAVE_SECTOR_COUNT)
    return 1;
  SAVE_START_DMA(source, state->slot + SAVE_HEADER_BYTES,
                 DMA_COPY_WORDS((sizeof(state->slot) - SAVE_HEADER_BYTES) / 4));
  WAIT_DMA();
  SAVE_START_DMA(gSave_HeaderTemplate, &header,
                 DMA_COPY_WORDS(sizeof(gSave_HeaderTemplate) / 4));
  WAIT_DMA();
  header.partId = partId;
  header.checksum = Save_ChecksumSector();
  header.counter = Save_GetLatestCounter(partId) + 1;
  SAVE_START_DMA(&header, state->slot, DMA_COPY_WORDS(SAVE_HEADER_BYTES / 4));
  WAIT_DMA();
  state->sectorCount = 0;
  state->sectorTotal = SAVE_SECTORS_PER_PART;
  if (current < SAVE_SECTOR_COUNT)
    state->sectorTotal = 2 * SAVE_SECTORS_PER_PART;
  if (Save_WriteSector(sector) != 0)
    return 2;
  if (current < SAVE_SECTOR_COUNT && Save_InvalidateSector(current) != 0)
    return 3;
  if (header.counter > SAVE_COUNTER_LIMIT) {
    header.counter = 1;
    SAVE_START_DMA(&header, state->slot, DMA_COPY_WORDS(SAVE_HEADER_BYTES / 4));
    WAIT_DMA();
    if (Save_WriteSector(current) != 0)
      return 4;
    if (Save_InvalidateSector(sector) != 0)
      return 5;
    sector = current;
  }
  state->occupied[sector] = 1;
  state->partId[sector] = partId;
  state->counter[sector] = header.counter;
  return 0;
}

/// Loads the current copy of part `partId` (12,272 bytes) into `destination`.
/// Returns 1 when there is none and 0 otherwise.
u32 Save_ReadPart(s32 partId, void *destination) {
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;
  u32 index;

  index = Save_FindLatestSector(partId);
  if (index >= SAVE_SECTOR_COUNT)
    return 1;
  Save_ReadSector(index);
  SAVE_START_DMA(state->slot + SAVE_HEADER_BYTES, destination,
                 DMA_COPY_WORDS((sizeof(state->slot) - SAVE_HEADER_BYTES) / 4));
  WAIT_DMA();
  return 0;
}

/// Invalidates the current copy of part `partId`. Nothing calls it.
u32 Save_DeletePart(s32 partId) {
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;
  s32 index;
  s32 result;

  index = Save_FindLatestSector(partId);
  if ((u32)index >= SAVE_SECTOR_COUNT)
    return 1;
  state->sectorCount = 0;
  state->sectorTotal = SAVE_SECTORS_PER_PART;
  result = Save_InvalidateSector(index);
  return NONZERO(result);
}

/// Sums the 12,272 payload bytes of the workspace's three-sector buffer, the
/// checksum its header keeps.
s32 Save_ChecksumSector(void) {
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;
  u32 limit;
  u32 offset;
  s32 sum;

  sum = 0;
  offset = 0;
  limit = SAVE_CHECKSUM_BYTES - 1;
  for (; offset <= limit; offset += 8) {
    sum += state->slot[SAVE_HEADER_BYTES + offset + 0];
    sum += state->slot[SAVE_HEADER_BYTES + offset + 1];
    sum += state->slot[SAVE_HEADER_BYTES + offset + 2];
    sum += state->slot[SAVE_HEADER_BYTES + offset + 3];
    sum += state->slot[SAVE_HEADER_BYTES + offset + 4];
    sum += state->slot[SAVE_HEADER_BYTES + offset + 5];
    sum += state->slot[SAVE_HEADER_BYTES + offset + 6];
    sum += state->slot[SAVE_HEADER_BYTES + offset + 7];
  }
  return sum;
}

/// Returns the first sector of the newest copy of part `partId`, or
/// SAVE_SECTOR_COUNT when it has none.
u32 Save_FindLatestSector(s32 partId) {
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;
  u32 latestCounter;
  u32 latestSector;
  u32 i;

  latestCounter = 0;
  latestSector = SAVE_SECTOR_COUNT;
  for (i = 0; i < SAVE_SECTOR_COUNT; i += SAVE_SECTORS_PER_PART) {
    if (state->occupied[i] != 0 && partId == state->partId[i]) {
      if (latestCounter < state->counter[i]) {
        latestCounter = state->counter[i];
        latestSector = i;
      }
    }
  }
  return latestSector;
}

/// Overwrites sector `index` with a header for no part (SAVE_PART_NONE) and
/// clears its occupied entry. Returns 0 on success and 1 on failure.
s32 Save_InvalidateSector(s32 index) {
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;
  struct SaveSectorHeader header;
  volatile u32 zero;

  zero = 0;
  SAVE_START_DMA(&zero, &header, DMA_FILL_WORDS(SAVE_HEADER_BYTES / 4));
  WAIT_DMA();
  SAVE_START_DMA(gSave_HeaderTemplate, &header,
                 DMA_COPY_WORDS(sizeof(gSave_HeaderTemplate) / 4));
  WAIT_DMA();
  header.partId = SAVE_PART_NONE;
  header.counter = 0;
  SAVE_START_DMA(&header, state->slot, DMA_COPY_WORDS(SAVE_HEADER_BYTES / 4));
  WAIT_DMA();
  if (Save_WriteSector(index) != 0)
    return 1;
  state->occupied[index] = 0;
  state->partId[index] = SAVE_PART_NONE;
  state->counter[index] = 0;
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
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;
  u32 latestCounter;
  u32 i;

  latestCounter = 0;
  for (i = 0; i < SAVE_SECTOR_COUNT; i += SAVE_SECTORS_PER_PART) {
    if (state->occupied[i] != 0 && partId == state->partId[i]) {
      if (latestCounter < state->counter[i])
        latestCounter = state->counter[i];
    }
  }
  return latestCounter;
}

/// Reads the summary of the latest save in each of the SAVE_FILE_COUNT files,
/// with its second play time from its next sector, clearing it first; returns
/// how many files have one.
s32 Save_LoadFileSummaries(void) {
  struct SaveWorkspace *state = Ram_HeapSlots->saveState;
  struct SaveSummary *summary;
  s32 count;
  u32 file;

  summary = state->summary;
  count = 0;
  for (file = 0; file < SAVE_FILE_COUNT; file++) {
    u32 sector;

    Dma_Fill32(0, summary, sizeof(*summary));
    sector = Save_FindLatestSector(file);
    if (sector < SAVE_SECTOR_COUNT) {
      ReadFlash(sector, 0, (u8 *)summary, sizeof(*summary));
      ReadFlash(sector + 1, SAVE_SECOND_PLAY_TIME_OFFSET,
                (u8 *)&summary->secondPartPlayTime,
                sizeof(summary->secondPartPlayTime));
      count++;
    }
    summary++;
  }
  return count;
}

/// Unused: reads part SAVE_PART_EXTRA's summary into the extra summary of the
/// save's state, with its second play time from its next sector; returns 1, or
/// 0 when the part has no copy.
s32 Save_LoadExtraSummary(void) {
  struct SaveWorkspace *state;
  struct SaveSummary *summary;
  u32 index;

  state = Ram_HeapSlots->saveState;
  summary = &state->extra;
  Dma_Fill32(0, summary, sizeof(*summary));
  index = Save_FindLatestSector(SAVE_PART_EXTRA);
  if (index < SAVE_SECTOR_COUNT) {
    ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
    // XXX (no evidence): whether part SAVE_PART_EXTRA keeps The Broken Seal's
    // layout; nothing in this game calls this reader, and no code names a
    // writer of the part.
    ReadFlash(index + 1, SAVE_BROKEN_SEAL_SECOND_PLAY_TIME_OFFSET,
              (u8 *)&state->extra.secondPartPlayTime,
              sizeof(state->extra.secondPartPlayTime));
    return 1;
  }
  return 0;
}

/// Removes the flash timer handler and frees the save workspace's heap slot,
/// with every block allocated after it in the same pool.
void Save_ReleaseState(void) {
  Irq_SetHandler(IRQ_TIMER0 + SAVE_FLASH_TIMER, 0, NULL);
  HeapSlot_Release(HEAP_SLOT_SAVE);
}

#undef DMA3_BUSY
#undef WAIT_DMA
#undef SAVE_START_DMA
