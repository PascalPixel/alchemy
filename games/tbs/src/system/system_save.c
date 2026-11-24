/// @file
/// Saving: flash sectors, parts and the save state.
// Part of system.c.

#include "system_internal.h"
#include "macros.h"

/// The save code's timer, retries, sectors and checksum.
enum {
  SAVE_FLASH_TIMER = 2, ///< the hardware timer handed to the flash library
  SAVE_FLASH_IDENTIFY_ATTEMPTS = 8, ///< tries, a frame apart
  /// Sectors scanned; as a sector number, "none" in the functions below.
  SAVE_SECTOR_COUNT = 16,
  SAVE_PART_NONE = 16, ///< the part id of a free or invalidated sector
  /// The highest counter a write keeps; a copy written past it is
  /// rewritten as 1.
  SAVE_COUNTER_LIMIT = 65000,
  /// Payload bytes summed into the checksum: all 4080 but the last eight.
  SAVE_CHECKSUM_BYTES = 4072
};

/// Where a file's second copy of the play time lies in its second part:
/// Save_BuildFileSummary (window/screens_save.c) writes it into
/// gSceneLocalState, which opens the save image's second half, after the part's
/// header.
#define SAVE_SECOND_PLAY_TIME_OFFSET                                           \
  (sizeof(struct SaveSectorHeader) + SECOND_PART_PLAY_TIME_WORD * sizeof(u32))

/// Allocates the save state (heap slot HEAP_SLOT_SAVE), installs the flash
/// library's SAVE_FLASH_TIMER handler, identifies the flash chip (up to eight
/// tries, a frame apart) and scans all 16 sectors into the save state's
/// occupied, partId and counter tables: a sector counts as taken when its
/// signature and checksum are good, and of two copies of a part only the newer
/// stays taken.
s32 Save_InitializeState(void) {
  struct SaveState *state;
  u32 buffer[4];
  struct SaveSectorHeader *header;
  u32 index;

  state = HeapSlot_AllocEwram(HEAP_SLOT_SAVE, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
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

    state->occupied[index] = 0;
    state->partId[index] = SAVE_PART_NONE;
    state->counter[index] = 0;
    status = Save_ReadSector(index);
    START_DMA(&state->slot, buffer, DMA_COPY_WORDS(4));
    WAIT_DMA();
    header = (struct SaveSectorHeader *)buffer;

    // Not a save sector, or one for no part, or a damaged one: it stays free.
    if (Save_CompareBytes(header->signature, (u8 *)gSave_Signature, 7) != 0)
      continue;
    state->counter[index] = header->counter;
    part = header->partId;
    if ((u8)part >= SAVE_PART_NONE)
      continue;
    if (status != 0)
      continue;

    state->occupied[index] = 1;
    state->partId[index] = part;
    // An interrupted replacement can leave both copies valid.
    for (; status < index; status++) {
      if (state->partId[status] == (u8)part) {
        if (state->counter[status] < header->counter)
          state->occupied[status] = 0;
        else
          state->occupied[index] = 0;
      }
    }
  }
  return 0;
}

/// Picks the free sector to write part `partId` to, at random, or returns
/// SAVE_SECTOR_COUNT for none. The last free sector is kept for replacing a
/// part that already has a copy.
u32 Save_SelectWriteSector(s32 partId) {
  u32 empty[SAVE_SECTOR_COUNT];
  s32 count;
  u32 index;
  u8 *slot;
  u8 value;

  slot = gSaveState->occupied;
  count = 0;
  for (index = 0; index < SAVE_SECTOR_COUNT; index += 1) {
    value = *slot++;
    if (value == 0) {
      empty[count++] = index;
    }
  }
  index = SAVE_SECTOR_COUNT;
  if (count != 0) {
    if (count == 1) {
      index = empty[0];
      if (Save_FindLatestSector(partId) == SAVE_SECTOR_COUNT) {
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

/// Programs the save state's sector buffer into flash sector `sector` and
/// verifies it, through the flash library's routine for this chip.
u32 Save_WriteSector(s32 sector) {
  struct SaveState *state = gSaveState;
  u16 index = sector;
  s32 result;

  if (gProgramFlashSector(index, state->slot.bytes) != 0)
    return 1;
  result = Flash_VerifySector(index, state->slot.bytes);
  return NONZERO(result);
}

/// Reads flash sector `index` into the save state's sector buffer. Returns 0
/// when its checksum is good.
s32 Save_ReadSector(s32 index) {
  struct SaveState *state;
  struct SaveSectorHeader header;
  u32 checksum;

  state = gSaveState;
  ReadFlash(index, 0, state->slot.bytes, sizeof(state->slot));
  Dma_Set(&state->slot, &header, DMA_COPY_WORDS(4), REG_DMA3);
  WAIT_DMA();
  checksum = Save_ChecksumSector();
  return (u16)checksum - header.checksum;
}

/// Unused: erases flash sector `sector` with the flash library's routine for
/// this chip.
UNUSED u16 Save_EraseSector(u16 sector) { return gEraseFlashSector(sector); }

/// Saves 4080 bytes from `source` as part `partId`: writes the new copy with
/// the next counter, then invalidates the old one, updating the save state's
/// sector tables.
s32 Save_WritePart(s32 partId, void *source) {
  struct SaveState *state;
  struct SaveSectorHeader header;
  u32 current;
  u32 slot;

  state = gSaveState;
  Dma_Fill32(0, &state->slot, sizeof(state->slot));
  WAIT_DMA();
  current = Save_FindLatestSector(partId);
  slot = Save_SelectWriteSector(partId);
  if (slot >= SAVE_SECTOR_COUNT)
    return 1;

  // Build the new sector before changing which old sector counts as current.
  START_DMA(source, state->slot.part.payload,
            DMA_COPY_WORDS(sizeof(state->slot.part.payload) / 4));
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, DMA_COPY_WORDS(2));
  WAIT_DMA();
  header.partId = partId;
  header.checksum = Save_ChecksumSector();
  header.counter = Save_GetLatestCounter(partId) + 1;
  START_DMA(&header, &state->slot.part.header, DMA_COPY_WORDS(4));
  WAIT_DMA();

  if (Save_WriteSector(slot) != 0)
    return 1;
  // A power loss before invalidation leaves two valid copies; the next
  // scan chooses the new one by its higher counter.
  if (current < SAVE_SECTOR_COUNT && Save_InvalidateSector(current) != 0)
    return 1;

  if (header.counter > SAVE_COUNTER_LIMIT) {
    // Past the limit the count starts again at 1, rewritten over the old
    // copy: the scan compares counters plainly, so a wrapped one would lose.
    header.counter = 1;
    START_DMA(&header, &state->slot.part.header, DMA_COPY_WORDS(4));
    WAIT_DMA();
    if (Save_WriteSector(current) != 0)
      return 1;
    if (Save_InvalidateSector(slot) != 0)
      return 1;
    slot = current;
  }

  state->occupied[slot] = 1;
  state->partId[slot] = partId;
  state->counter[slot] = header.counter;
  return 0;
}

/// Loads the current copy of part `partId` (4080 bytes) into `destination`.
/// Returns 1 when there is none and 0 otherwise.
u32 Save_ReadPart(s32 partId, void *destination) {
  struct SaveState *state;
  u32 index;

  state = gSaveState;
  index = Save_FindLatestSector(partId);
  if (index >= SAVE_SECTOR_COUNT)
    return 1;
  Save_ReadSector(index);
  START_DMA(state->slot.part.payload, destination,
            DMA_COPY_WORDS(sizeof(state->slot.part.payload) / 4));
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
  return NONZERO(deletionResult);
}

/// Sums the first SAVE_CHECKSUM_BYTES payload bytes of the save state's sector
/// buffer; the final eight payload bytes are excluded.
s32 Save_ChecksumSector(void) {
  struct SaveState *state;
  u32 limit;
  u32 offset;
  s32 sum;

  state = gSaveState;
  limit = SAVE_CHECKSUM_BYTES - 1;
  sum = 0;
  for (offset = 0; offset <= limit; offset += 8) {
    sum += state->slot.part.payload[offset + 0];
    sum += state->slot.part.payload[offset + 1];
    sum += state->slot.part.payload[offset + 2];
    sum += state->slot.part.payload[offset + 3];
    sum += state->slot.part.payload[offset + 4];
    sum += state->slot.part.payload[offset + 5];
    sum += state->slot.part.payload[offset + 6];
    sum += state->slot.part.payload[offset + 7];
  }
  return sum;
}

/// Returns the occupied sector holding the newest copy of part `partId`, or
/// SAVE_SECTOR_COUNT.
u32 Save_FindLatestSector(s32 partId) {
  struct SaveState *state;
  u32 latestCounter;
  u32 latestSlot;
  u32 i;

  state = gSaveState;
  latestSlot = SAVE_SECTOR_COUNT;
  latestCounter = 0;
  for (i = 0; i < SAVE_SECTOR_COUNT; i++) {
    if (state->occupied[i] != 0 && partId == state->partId[i]) {
      if (latestCounter < state->counter[i]) {
        latestCounter = state->counter[i];
        latestSlot = i;
      }
    }
  }
  return latestSlot;
}

/// Overwrites sector `index` with a header for no part (SAVE_PART_NONE) and
/// clears its occupied entry. Returns 0 on success and 1 on failure.
s32 Save_InvalidateSector(s32 index) {
  struct SaveState *state;
  struct SaveSectorHeader header;

  state = gSaveState;
  Dma_Fill32(0, &header, sizeof(header));
  WAIT_DMA();
  START_DMA(gSave_HeaderTemplate, &header, DMA_COPY_WORDS(2));
  WAIT_DMA();
  header.partId = SAVE_PART_NONE;
  header.counter = 0;
  START_DMA(&header, &state->slot.part.header, DMA_COPY_WORDS(4));
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
  struct SaveState *state;
  u32 latestCounter;
  u32 i;

  state = gSaveState;
  latestCounter = 0;
  for (i = 0; i < SAVE_SECTOR_COUNT; i++) {
    if (state->occupied[i] != 0 && partId == state->partId[i]) {
      if (latestCounter < state->counter[i])
        latestCounter = state->counter[i];
    }
  }
  return latestCounter;
}

/// Reads the summary at the start of each of the three save files into the
/// save state's summary table for the file list, and each file's second copy
/// of the play time, from SAVE_SECOND_PLAY_TIME_OFFSET in its second part,
/// into its secondPartPlayTime (zero when it has none).
s32 Save_LoadFileSummaries(void) {
  struct SaveState *state;
  struct SaveFileSummary *summary;
  u32 group;
  s32 count;

  state = gSaveState;
  summary = state->summary;
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
      ReadFlash(index, SAVE_SECOND_PLAY_TIME_OFFSET,
                (u8 *)&summary->secondPartPlayTime,
                sizeof(summary->secondPartPlayTime));
    else
      summary->secondPartPlayTime = 0;
    group++;
    summary++;
  } while (group < SAVE_FILE_COUNT);
  return count;
}

/// Removes the flash timer handler and frees the save state's heap slot,
/// with every block allocated after it in the same pool.
u32 Save_ReleaseState(void) {
  Irq_SetHandler(IRQ_TIMER2, 0, NULL);
  HeapSlot_Release(HEAP_SLOT_SAVE);
  // UB: nothing is returned, and no caller reads the result
#ifdef UBFIX
  return 0;
#endif
}

#undef SAVE_SECOND_PLAY_TIME_OFFSET
