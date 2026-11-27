// Saving: flash sectors, parts and the save workspace. Part of system.c.

// DMA 3's enable bit in its count and control word, which the hardware clears when the transfer is
// done.
#define DMA3_BUSY (DMA_ENABLE << 16)

// Starts DMA 3 copying from source_ to destination_ with control_.
#define BUP_START_DMA(source_, destination_, control_)                                             \
    syDMAXCopy((const void *)(source_), (void *)(destination_), (control_), REG_DMA3)

// Waits until `channel`'s transfer is done.
static __inline__ void syWaitDMA(DMA_REGISTERS volatile *channel)
{
    while ((channel->control & DMA3_BUSY) != 0)
    {
    }
}

// Waits until a BUP_START_DMA transfer is done.
#define WAIT_DMA()                              syWaitDMA(DMA3)

// The bytes of a part's first sector that its BUP_SECTOR_HEADER takes, before the payload.
#define BUP_HEADER_BYTES                        sizeof(BUP_SECTOR_HEADER)

// Where a file's second copy of the play time lies in its part's second sector:
// mnSaveBuildFileSummary writes it to the first word of SceneLocalState, which opens the save
// image's second sector, after the part's header.
#define BUP_SECOND_PLAY_TIME_OFFSET             BUP_HEADER_BYTES
// Where The Broken Seal reads that copy, 256 bytes further into SceneLocalState:
// syBupLoadExtraSummary reads part BUP_PART_EXTRA's there.
#define BUP_BROKEN_SEAL_SECOND_PLAY_TIME_OFFSET (BUP_HEADER_BYTES + 256)

// Allocates the save workspace (heap slot HEAP_SLOT_BUP), installs the BUP_FLASH_TIMER handler of
// the SDK's flash library, identifies the flash chip (BUP_FLASH_IDENTIFY_ATTEMPTS tries, a tick
// apart) and reads each part's place, three sectors at a time, into the workspace's occupied,
// partId and counter tables: a place counts as taken when its signature and checksum are good, and
// of two copies of a part only the newer stays taken. Returns 1 when no flash chip answers, else 0.
Bool syBupInitState(void)
{
    BUP_WORKSPACE     *state;
    BUP_SECTOR_HEADER  header;
    BUP_SECTOR_HEADER *slot;
    Uint32             index;

    state = syHeapSlotAllocateEWRAM(HEAP_SLOT_BUP, sizeof(*state));
    DMA_FILL32(0, state, sizeof(*state));
    SetFlashTimerIntr(BUP_FLASH_TIMER, &IrqHandlers[IRQ_TIMER0 + BUP_FLASH_TIMER]);

    for (index = 0; index < BUP_FLASH_IDENTIFY_ATTEMPTS; index++)
    {
        if ((Uint16)IdentifyFlash() == 0)
            break;
        sySynch(1);
    }
    if (index >= BUP_FLASH_IDENTIFY_ATTEMPTS)
        return TRUE;

    for (index = 0; index < BUP_SECTOR_COUNT; index += BUP_SECTORS_PER_PART)
    {
        Uint32 status;
        Uint16 partId;
        Uint8  id;

        state->occupied[index] = 0;
        state->partId[index]   = BUP_PART_NONE;
        state->count[index]    = 0;
        status                 = syBupReadSector(index);
        BUP_START_DMA(state->slot, &header, DMA_COPY_WORDS(BUP_HEADER_BYTES / 4));
        WAIT_DMA();
        // FAKEMATCH(temporary): slot holds &header; as header. at its uses.
        slot = &header;

        if (syBupCompareBytes(slot->signature, (Uint8 *)BupSignature, sizeof(slot->signature)) != 0)
            continue;
        state->count[index] = slot->count;
        partId              = slot->partId;
        id                  = partId;
        if (id >= BUP_PART_NONE)
            continue;
        if (status != 0)
            continue;

        state->occupied[index] = 1;
        state->partId[index]   = partId;
        if (status < index)
        {
            for (; status < index; status++)
            {
                if (state->partId[status] == (Uint8)partId)
                {
                    if (state->count[status] < slot->count)
                        state->occupied[status] = 0;
                    else
                        state->occupied[index] = 0;
                }
            }
        }
    }
    return FALSE;
}

// Picks the free place, three sectors from a multiple of three, to write part `partId` to, at
// random among them. The last free place only goes to a part that already has a copy, so replacing
// a save always has room. Returns BUP_SECTOR_COUNT when there is no place to use.
Uint32 syBupSelectWriteSector(Sint32 partId)
{
    Uint32 empty[BUP_SECTOR_COUNT];
    Sint32 count;
    Uint32 index;
    Uint8 *occupied;
    Uint8  value;

    occupied = ((BUP_WORKSPACE *)RamHeapSlots->bupState)->occupied;
    count    = 0;
    for (index = 0; index < BUP_SECTOR_COUNT; index += BUP_SECTORS_PER_PART)
    {
        value     = *occupied;
        occupied += BUP_SECTORS_PER_PART;
        if (value == 0)
            empty[count++] = index;
    }
    index = BUP_SECTOR_COUNT;
    if (count != 0)
    {
        if (count == 1)
        {
            index = empty[0];
            if (syBupFindLatestSector(partId) == BUP_SECTOR_COUNT)
                index = BUP_SECTOR_COUNT;
        }
        else
        {
            // A free place at random, so the writes spread over the flash chip.
            index = syRandom() % count;
            index = empty[index];
        }
    }
    return index;
}

// Programs the workspace's three-sector buffer into flash from sector `sector` on and verifies
// each, through the SDK's flash library's function for this chip, calling the progress function
// before the first and after each. Returns 1 when a sector fails, else 0.
Uint32 syBupWriteSector(Sint32 sector)
{
    BUP_WORKSPACE *state               = RamHeapSlots->bupState;
    void           (*progress)(Sint32) = state->progress;

    if (progress != 0)
        progress(0);
    if (ProgramFlashSectorWork(sector, state->slot) != 0
        || VerifyFlashSector(sector, state->slot) != 0)
        return 1;
    if (progress != 0)
        progress(1);
    if (ProgramFlashSectorWork(sector + 1, state->slot + BUP_SECTOR_SIZE) != 0
        || VerifyFlashSector(sector + 1, state->slot + BUP_SECTOR_SIZE) != 0)
        return 1;
    if (progress != 0)
        progress(1);
    if (ProgramFlashSectorWork(sector + 2, state->slot + 2 * BUP_SECTOR_SIZE) != 0
        || VerifyFlashSector(sector + 2, state->slot + 2 * BUP_SECTOR_SIZE) != 0)
        return 1;
    if (progress != 0)
        progress(1);
    return 0;
}

// Reads the three flash sectors from `index` on into the workspace's sector buffer. Returns 0 when
// their checksum matches the header's.
Sint32 syBupReadSector(Sint32 index)
{
    BUP_WORKSPACE    *state = RamHeapSlots->bupState;
    BUP_SECTOR_HEADER header;

    ReadFlash(index, 0, state->slot, BUP_SECTOR_SIZE);
    ReadFlash(index + 1, 0, state->slot + BUP_SECTOR_SIZE, BUP_SECTOR_SIZE);
    ReadFlash(index + 2, 0, state->slot + 2 * BUP_SECTOR_SIZE, BUP_SECTOR_SIZE);
    BUP_START_DMA(state->slot, &header, DMA_COPY_WORDS(BUP_HEADER_BYTES / 4));
    WAIT_DMA();
    return (Uint16)syBupChecksumSector() - header.checksum;
}

// Saves the 12,272 bytes at `src`, three sectors less the header, as part `partId`: writes the new
// copy with the next counter, then invalidates the old one, updating the workspace's sector tables.
// Returns 0, or the step that failed: 1 no free place, 2 the new copy, 3 invalidating the old one,
// and, once the counter passes BUP_COUNTER_LIMIT and the copy is written again with counter 1 over
// the old place, 4 that write and 5 invalidating the new copy.
Sint32 syBupWritePart(Sint32 partId, void *src)
{
    BUP_WORKSPACE    *state = RamHeapSlots->bupState;
    BUP_SECTOR_HEADER header;
    volatile Uint32   zero;
    Uint32            current;
    Uint32            sector;

    zero = 0;
    BUP_START_DMA(&zero, state->slot, DMA_FILL_WORDS(sizeof(state->slot) / 4));
    WAIT_DMA();
    current = syBupFindLatestSector(partId);
    sector  = syBupSelectWriteSector(partId);
    if (sector >= BUP_SECTOR_COUNT)
        return 1;
    BUP_START_DMA(src,
                  state->slot + BUP_HEADER_BYTES,
                  DMA_COPY_WORDS((sizeof(state->slot) - BUP_HEADER_BYTES) / 4));
    WAIT_DMA();
    BUP_START_DMA(BupHeaderTemplate, &header, DMA_COPY_WORDS(sizeof(BupHeaderTemplate) / 4));
    WAIT_DMA();
    header.partId   = partId;
    header.checksum = syBupChecksumSector();
    header.count    = syBupGetLatestCounter(partId) + 1;
    BUP_START_DMA(&header, state->slot, DMA_COPY_WORDS(BUP_HEADER_BYTES / 4));
    WAIT_DMA();
    state->sectorCount = 0;
    state->sectorTotal = BUP_SECTORS_PER_PART;
    if (current < BUP_SECTOR_COUNT)
        state->sectorTotal = 2 * BUP_SECTORS_PER_PART;
    if (syBupWriteSector(sector) != 0)
        return 2;
    if (current < BUP_SECTOR_COUNT && syBupInvalidateSector(current) != 0)
        return 3;
    if (header.count > BUP_COUNTER_LIMIT)
    {
        header.count = 1;
        BUP_START_DMA(&header, state->slot, DMA_COPY_WORDS(BUP_HEADER_BYTES / 4));
        WAIT_DMA();
        if (syBupWriteSector(current) != 0)
            return 4;
        if (syBupInvalidateSector(sector) != 0)
            return 5;
        sector = current;
    }
    state->occupied[sector] = 1;
    state->partId[sector]   = partId;
    state->count[sector]    = header.count;
    return 0;
}

// Loads the current copy of part `partId` (12,272 bytes) into `dest`. Returns 1 when there is none
// and 0 otherwise.
Uint32 syBupReadPart(Sint32 partId, void *dest)
{
    BUP_WORKSPACE *state = RamHeapSlots->bupState;
    Uint32         index;

    index = syBupFindLatestSector(partId);
    if (index >= BUP_SECTOR_COUNT)
        return 1;
    syBupReadSector(index);
    BUP_START_DMA(state->slot + BUP_HEADER_BYTES,
                  dest,
                  DMA_COPY_WORDS((sizeof(state->slot) - BUP_HEADER_BYTES) / 4));
    WAIT_DMA();
    return 0;
}

// Invalidates the current copy of part `partId`. Nothing calls it.
Uint32 syBupDeletePart(Sint32 partId)
{
    BUP_WORKSPACE *state = RamHeapSlots->bupState;
    Sint32         index;

    index = syBupFindLatestSector(partId);
    if ((Uint32)index >= BUP_SECTOR_COUNT)
        return 1;
    state->sectorCount = 0;
    state->sectorTotal = BUP_SECTORS_PER_PART;
    if (syBupInvalidateSector(index))
        return 1;
    return 0;
}

// Sums the 12,272 payload bytes of the workspace's three-sector buffer, the checksum its header
// keeps.
Sint32 syBupChecksumSector(void)
{
    BUP_WORKSPACE *state = RamHeapSlots->bupState;
    Uint32         limit;
    Uint32         offset;
    Sint32         sum;

    sum    = 0;
    offset = 0;
    limit  = BUP_CHECKSUM_BYTES - 1;
    for (; offset <= limit; offset += 8)
    {
        sum += state->slot[BUP_HEADER_BYTES + offset + 0];
        sum += state->slot[BUP_HEADER_BYTES + offset + 1];
        sum += state->slot[BUP_HEADER_BYTES + offset + 2];
        sum += state->slot[BUP_HEADER_BYTES + offset + 3];
        sum += state->slot[BUP_HEADER_BYTES + offset + 4];
        sum += state->slot[BUP_HEADER_BYTES + offset + 5];
        sum += state->slot[BUP_HEADER_BYTES + offset + 6];
        sum += state->slot[BUP_HEADER_BYTES + offset + 7];
    }
    return sum;
}

// Returns the first sector of the newest copy of part `partId`, or BUP_SECTOR_COUNT when it has
// none.
Uint32 syBupFindLatestSector(Sint32 partId)
{
    BUP_WORKSPACE *state = RamHeapSlots->bupState;
    Uint32         latestCounter;
    Uint32         latestSector;
    Uint32         i;

    latestCounter = 0;
    latestSector  = BUP_SECTOR_COUNT;
    for (i = 0; i < BUP_SECTOR_COUNT; i += BUP_SECTORS_PER_PART)
    {
        if (state->occupied[i] != 0 && partId == state->partId[i])
        {
            if (latestCounter < state->count[i])
            {
                latestCounter = state->count[i];
                latestSector  = i;
            }
        }
    }
    return latestSector;
}

// Overwrites sector `index` with a header for no part (BUP_PART_NONE) and clears its occupied
// entry. Returns 0 on success and 1 on failure.
Bool syBupInvalidateSector(Sint32 index)
{
    BUP_WORKSPACE    *state = RamHeapSlots->bupState;
    BUP_SECTOR_HEADER header;
    volatile Uint32   zero;

    zero = 0;
    BUP_START_DMA(&zero, &header, DMA_FILL_WORDS(BUP_HEADER_BYTES / 4));
    WAIT_DMA();
    BUP_START_DMA(BupHeaderTemplate, &header, DMA_COPY_WORDS(sizeof(BupHeaderTemplate) / 4));
    WAIT_DMA();
    header.partId = BUP_PART_NONE;
    header.count  = 0;
    BUP_START_DMA(&header, state->slot, DMA_COPY_WORDS(BUP_HEADER_BYTES / 4));
    WAIT_DMA();
    if (syBupWriteSector(index) != 0)
        return TRUE;
    state->occupied[index] = 0;
    state->partId[index]   = BUP_PART_NONE;
    state->count[index]    = 0;
    return FALSE;
}

// Compares `count` bytes, as memcmp does.
Sint32 syBupCompareBytes(Uint8 *left, Uint8 *right, Sint32 count)
{
    Sint32 difference = 0;

    while (count != 0)
    {
        difference = *left - *right;
        if (difference != 0)
            break;
        count--;
        left++;
        right++;
    }
    return difference;
}

// Returns the highest counter among the copies of part `partId`.
Uint32 syBupGetLatestCounter(Sint32 partId)
{
    BUP_WORKSPACE *state = RamHeapSlots->bupState;
    Uint32         latestCounter;
    Uint32         i;

    latestCounter = 0;
    for (i = 0; i < BUP_SECTOR_COUNT; i += BUP_SECTORS_PER_PART)
    {
        if (state->occupied[i] != 0 && partId == state->partId[i])
        {
            if (latestCounter < state->count[i])
                latestCounter = state->count[i];
        }
    }
    return latestCounter;
}

// Reads the summary of the latest save in each of the BUP_FILE_COUNT files, with its second play
// time from its next sector, clearing it first; returns how many files have one.
Sint32 syBupLoadFileSummaries(void)
{
    BUP_WORKSPACE *state = RamHeapSlots->bupState;
    BUP_SUMMARY   *summary;
    Sint32         count;
    Uint32         file;

    summary = state->summary;
    count   = 0;
    for (file = 0; file < BUP_FILE_COUNT; file++)
    {
        Uint32 sector;

        DMA_FILL32(0, summary, sizeof(*summary));
        sector = syBupFindLatestSector(file);
        if (sector < BUP_SECTOR_COUNT)
        {
            ReadFlash(sector, 0, (Uint8 *)summary, sizeof(*summary));
            ReadFlash(sector + 1,
                      BUP_SECOND_PLAY_TIME_OFFSET,
                      (Uint8 *)&summary->secondPartPlayTime,
                      sizeof(summary->secondPartPlayTime));
            count++;
        }
        summary++;
    }
    return count;
}

// Unused: reads part BUP_PART_EXTRA's summary into the extra summary of the save's state, with its
// second play time from its next sector; returns 1, or 0 when the part has no copy.
Bool syBupLoadExtraSummary(void)
{
    BUP_WORKSPACE *state;
    BUP_SUMMARY   *summary;
    Uint32         index;

    state   = RamHeapSlots->bupState;
    summary = &state->extra;
    DMA_FILL32(0, summary, sizeof(*summary));
    index = syBupFindLatestSector(BUP_PART_EXTRA);
    if (index < BUP_SECTOR_COUNT)
    {
        ReadFlash(index, 0, (Uint8 *)summary, sizeof(*summary));
        // XXX (no evidence): whether part BUP_PART_EXTRA keeps The Broken Seal's layout; nothing in
        // this game calls this reader, and no code names a writer of the part.
        ReadFlash(index + 1,
                  BUP_BROKEN_SEAL_SECOND_PLAY_TIME_OFFSET,
                  (Uint8 *)&state->extra.secondPartPlayTime,
                  sizeof(state->extra.secondPartPlayTime));
        return TRUE;
    }
    return FALSE;
}

// Removes the flash timer handler and frees the save workspace's heap slot, with every block
// allocated after it in the same pool.
void syBupReleaseState(void)
{
    syIntFunction(IRQ_TIMER0 + BUP_FLASH_TIMER, 0, NULL);
    syHeapSlotRelease(HEAP_SLOT_BUP);
}

#undef DMA3_BUSY
#undef WAIT_DMA
#undef BUP_START_DMA
