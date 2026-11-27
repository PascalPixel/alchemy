/* @file */
/* Saving: flash sectors, parts and the save workspace. */
/* Part of system.c. */

/* DMA 3's enable bit in its count and control word, which the hardware */
/* clears when the transfer is done. */
#define DMA3_BUSY (DMA_ENABLE << 16)

/* Starts DMA 3 copying from source_ to destination_ with control_. */
#define SAVE_START_DMA(source_, destination_, control_)                        \
	syDMAXCopy((const void *)(source_), (void *)(destination_), (control_),    \
			   REG_DMA3)

/* Waits until `channel`'s transfer is done. */
static __inline__ void WaitDma(DMA_REGISTERS volatile *channel) {
	while((channel->ctrl & DMA3_BUSY) != 0) {
	}
}

/* Waits until a SAVE_START_DMA transfer is done. */
#define WAIT_DMA() WaitDma(DMA3)

/* The bytes of a part's first sector that its SAVE_SECTOR_HEADER takes, before
 */
/* the payload. */
#define SAVE_HEADER_BYTES sizeof(SAVE_SECTOR_HEADER)

/* Where a file's second copy of the play time lies in its part's second */
/* sector: mnSaveBuildFileSummary writes it to the first word of */
/* SceneLocalState, which opens the save image's second sector, after the */
/* part's header. */
#define SAVE_SECOND_PLAY_TIME_OFFSET SAVE_HEADER_BYTES
/* Where The Broken Seal reads that copy, 256 bytes further into */
/* SceneLocalState: sySaveLoadExtraSummary reads part SAVE_PART_EXTRA's there.
 */
#define SAVE_BROKEN_SEAL_SECOND_PLAY_TIME_OFFSET (SAVE_HEADER_BYTES + 256)

/* Allocates the save workspace (heap slot HEAP_SLOT_SAVE), installs the */
/* SAVE_FLASH_TIMER handler of the SDK's flash library, identifies the flash */
/* chip (SAVE_FLASH_IDENTIFY_ATTEMPTS tries, a frame apart) and reads each */
/* part's place, three sectors at a time, into the workspace's occupied, */
/* partId and counter tables: a place counts as taken when its signature and */
/* checksum are good, and of two copies of a part only the newer stays taken. */
/* Returns 1 when no flash chip answers, else 0. */
s32 sySaveInitState(void) {
	SAVE_WORKSPACE *state;
	SAVE_SECTOR_HEADER header;
	SAVE_SECTOR_HEADER *slot;
	u32 index;

	state = syHeapSlotAllocEwram(HEAP_SLOT_SAVE, sizeof(*state));
	btDmaFill32(0, state, sizeof(*state));
	SetFlashTimerIntr(SAVE_FLASH_TIMER,
					  &IrqHandlers[IRQ_TIMER0 + SAVE_FLASH_TIMER]);

	for(index = 0; index < SAVE_FLASH_IDENTIFY_ATTEMPTS; index++) {
		if((u16)IdentifyFlash() == 0)
			break;
		sySynch(1);
	}
	if(index >= SAVE_FLASH_IDENTIFY_ATTEMPTS)
		return 1;

	for(index = 0; index < SAVE_SECTOR_COUNT; index += SAVE_SECTORS_PER_PART) {
		u32 status;
		u16 partId;
		u8 id;

		state->occupied[index] = 0;
		state->partId[index] = SAVE_PART_NONE;
		state->cnt[index] = 0;
		status = sySaveReadSector(index);
		SAVE_START_DMA(state->slot, &header,
					   DMA_COPY_WORDS(SAVE_HEADER_BYTES / 4));
		WAIT_DMA();
		/* FAKEMATCH(temporary): slot holds &header; as header. at its uses. */
		slot = &header;

		if(sySaveCompareBytes(slot->signature, (u8 *)Save_Signature,
							  sizeof(slot->signature)) != 0)
			continue;
		state->cnt[index] = slot->cnt;
		partId = slot->partId;
		id = partId;
		if(id >= SAVE_PART_NONE)
			continue;
		if(status != 0)
			continue;

		state->occupied[index] = 1;
		state->partId[index] = partId;
		if(status < index) {
			for(; status < index; status++) {
				if(state->partId[status] == (u8)partId) {
					if(state->cnt[status] < slot->cnt)
						state->occupied[status] = 0;
					else
						state->occupied[index] = 0;
				}
			}
		}
	}
	return 0;
}

/* Picks the free place, three sectors from a multiple of three, to write part
 */
/* `partId` to, at random among them. The last free place only goes to a part */
/* that already has a copy, so replacing a save always has room. Returns */
/* SAVE_SECTOR_COUNT when there is no place to use. */
u32 sySaveSelectWriteSector(s32 partId) {
	u32 empty[SAVE_SECTOR_COUNT];
	s32 count;
	u32 index;
	u8 *occupied;
	u8 value;

	occupied = ((SAVE_WORKSPACE *)Ram_HeapSlots->saveState)->occupied;
	count = 0;
	for(index = 0; index < SAVE_SECTOR_COUNT; index += SAVE_SECTORS_PER_PART) {
		value = *occupied;
		occupied += SAVE_SECTORS_PER_PART;
		if(value == 0)
			empty[count++] = index;
	}
	index = SAVE_SECTOR_COUNT;
	if(count != 0) {
		if(count == 1) {
			index = empty[0];
			if(sySaveFindLatestSector(partId) == SAVE_SECTOR_COUNT)
				index = SAVE_SECTOR_COUNT;
		}
		else {
			/* A free place at random, so the writes spread over the flash chip.
			 */
			index = syRandom() % count;
			index = empty[index];
		}
	}
	return index;
}

/* Programs the workspace's three-sector buffer into flash from sector */
/* `sector` on and verifies each, through the SDK's flash library's function */
/* for this chip, calling the progress function before the first and after */
/* each. Returns 1 when a sector fails, else 0. */
u32 sySaveWriteSector(s32 sector) {
	SAVE_WORKSPACE *state = Ram_HeapSlots->saveState;
	void (*progress)(s32) = state->progress;

	if(progress != 0)
		progress(0);
	if(ProgramFlashSectorWork(sector, state->slot) != 0 ||
	   VerifyFlashSector(sector, state->slot) != 0)
		return 1;
	if(progress != 0)
		progress(1);
	if(ProgramFlashSectorWork(sector + 1, state->slot + SAVE_SECTOR_SIZE) !=
		   0 ||
	   VerifyFlashSector(sector + 1, state->slot + SAVE_SECTOR_SIZE) != 0)
		return 1;
	if(progress != 0)
		progress(1);
	if(ProgramFlashSectorWork(sector + 2, state->slot + 2 * SAVE_SECTOR_SIZE) !=
		   0 ||
	   VerifyFlashSector(sector + 2, state->slot + 2 * SAVE_SECTOR_SIZE) != 0)
		return 1;
	if(progress != 0)
		progress(1);
	return 0;
}

/* Reads the three flash sectors from `index` on into the workspace's sector */
/* buffer. Returns 0 when their checksum matches the header's. */
s32 sySaveReadSector(s32 index) {
	SAVE_WORKSPACE *state = Ram_HeapSlots->saveState;
	SAVE_SECTOR_HEADER header;

	ReadFlash(index, 0, state->slot, SAVE_SECTOR_SIZE);
	ReadFlash(index + 1, 0, state->slot + SAVE_SECTOR_SIZE, SAVE_SECTOR_SIZE);
	ReadFlash(index + 2, 0, state->slot + 2 * SAVE_SECTOR_SIZE,
			  SAVE_SECTOR_SIZE);
	SAVE_START_DMA(state->slot, &header, DMA_COPY_WORDS(SAVE_HEADER_BYTES / 4));
	WAIT_DMA();
	return (u16)sySaveChecksumSector() - header.checksum;
}

/* Saves the 12,272 bytes at `source`, three sectors less the header, as part */
/* `partId`: writes the new copy with the next counter, then invalidates the */
/* old one, updating the workspace's sector tables. Returns 0, or the step that
 */
/* failed: 1 no free place, 2 the new copy, 3 invalidating the old one, and, */
/* once the counter passes SAVE_COUNTER_LIMIT and the copy is written again */
/* with counter 1 over the old place, 4 that write and 5 invalidating the new */
/* copy. */
s32 sySaveWritePart(s32 partId, void *source) {
	SAVE_WORKSPACE *state = Ram_HeapSlots->saveState;
	SAVE_SECTOR_HEADER header;
	volatile u32 zero;
	u32 current;
	u32 sector;

	zero = 0;
	SAVE_START_DMA(&zero, state->slot, DMA_FILL_WORDS(sizeof(state->slot) / 4));
	WAIT_DMA();
	current = sySaveFindLatestSector(partId);
	sector = sySaveSelectWriteSector(partId);
	if(sector >= SAVE_SECTOR_COUNT)
		return 1;
	SAVE_START_DMA(
		source, state->slot + SAVE_HEADER_BYTES,
		DMA_COPY_WORDS((sizeof(state->slot) - SAVE_HEADER_BYTES) / 4));
	WAIT_DMA();
	SAVE_START_DMA(Save_HeaderTemplate, &header,
				   DMA_COPY_WORDS(sizeof(Save_HeaderTemplate) / 4));
	WAIT_DMA();
	header.partId = partId;
	header.checksum = sySaveChecksumSector();
	header.cnt = sySaveGetLatestCounter(partId) + 1;
	SAVE_START_DMA(&header, state->slot, DMA_COPY_WORDS(SAVE_HEADER_BYTES / 4));
	WAIT_DMA();
	state->sectorCnt = 0;
	state->sectorTotal = SAVE_SECTORS_PER_PART;
	if(current < SAVE_SECTOR_COUNT)
		state->sectorTotal = 2 * SAVE_SECTORS_PER_PART;
	if(sySaveWriteSector(sector) != 0)
		return 2;
	if(current < SAVE_SECTOR_COUNT && sySaveInvalidateSector(current) != 0)
		return 3;
	if(header.cnt > SAVE_COUNTER_LIMIT) {
		header.cnt = 1;
		SAVE_START_DMA(&header, state->slot,
					   DMA_COPY_WORDS(SAVE_HEADER_BYTES / 4));
		WAIT_DMA();
		if(sySaveWriteSector(current) != 0)
			return 4;
		if(sySaveInvalidateSector(sector) != 0)
			return 5;
		sector = current;
	}
	state->occupied[sector] = 1;
	state->partId[sector] = partId;
	state->cnt[sector] = header.cnt;
	return 0;
}

/* Loads the current copy of part `partId` (12,272 bytes) into `destination`. */
/* Returns 1 when there is none and 0 otherwise. */
u32 sySaveReadPart(s32 partId, void *destination) {
	SAVE_WORKSPACE *state = Ram_HeapSlots->saveState;
	u32 index;

	index = sySaveFindLatestSector(partId);
	if(index >= SAVE_SECTOR_COUNT)
		return 1;
	sySaveReadSector(index);
	SAVE_START_DMA(
		state->slot + SAVE_HEADER_BYTES, destination,
		DMA_COPY_WORDS((sizeof(state->slot) - SAVE_HEADER_BYTES) / 4));
	WAIT_DMA();
	return 0;
}

/* Invalidates the current copy of part `partId`. Nothing calls it. */
u32 sySaveDeletePart(s32 partId) {
	SAVE_WORKSPACE *state = Ram_HeapSlots->saveState;
	s32 index;

	index = sySaveFindLatestSector(partId);
	if((u32)index >= SAVE_SECTOR_COUNT)
		return 1;
	state->sectorCnt = 0;
	state->sectorTotal = SAVE_SECTORS_PER_PART;
	if(sySaveInvalidateSector(index))
		return 1;
	return 0;
}

/* Sums the 12,272 payload bytes of the workspace's three-sector buffer, the */
/* checksum its header keeps. */
s32 sySaveChecksumSector(void) {
	SAVE_WORKSPACE *state = Ram_HeapSlots->saveState;
	u32 limit;
	u32 offset;
	s32 sum;

	sum = 0;
	offset = 0;
	limit = SAVE_CHECKSUM_BYTES - 1;
	for(; offset <= limit; offset += 8) {
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

/* Returns the first sector of the newest copy of part `partId`, or */
/* SAVE_SECTOR_COUNT when it has none. */
u32 sySaveFindLatestSector(s32 partId) {
	SAVE_WORKSPACE *state = Ram_HeapSlots->saveState;
	u32 latestCounter;
	u32 latestSector;
	u32 i;

	latestCounter = 0;
	latestSector = SAVE_SECTOR_COUNT;
	for(i = 0; i < SAVE_SECTOR_COUNT; i += SAVE_SECTORS_PER_PART) {
		if(state->occupied[i] != 0 && partId == state->partId[i]) {
			if(latestCounter < state->cnt[i]) {
				latestCounter = state->cnt[i];
				latestSector = i;
			}
		}
	}
	return latestSector;
}

/* Overwrites sector `index` with a header for no part (SAVE_PART_NONE) and */
/* clears its occupied entry. Returns 0 on success and 1 on failure. */
s32 sySaveInvalidateSector(s32 index) {
	SAVE_WORKSPACE *state = Ram_HeapSlots->saveState;
	SAVE_SECTOR_HEADER header;
	volatile u32 zero;

	zero = 0;
	SAVE_START_DMA(&zero, &header, DMA_FILL_WORDS(SAVE_HEADER_BYTES / 4));
	WAIT_DMA();
	SAVE_START_DMA(Save_HeaderTemplate, &header,
				   DMA_COPY_WORDS(sizeof(Save_HeaderTemplate) / 4));
	WAIT_DMA();
	header.partId = SAVE_PART_NONE;
	header.cnt = 0;
	SAVE_START_DMA(&header, state->slot, DMA_COPY_WORDS(SAVE_HEADER_BYTES / 4));
	WAIT_DMA();
	if(sySaveWriteSector(index) != 0)
		return 1;
	state->occupied[index] = 0;
	state->partId[index] = SAVE_PART_NONE;
	state->cnt[index] = 0;
	return 0;
}

/* Compares `count` bytes, as memcmp does. */
s32 sySaveCompareBytes(u8 *left, u8 *right, s32 count) {
	s32 difference = 0;

	while(count != 0) {
		difference = *left - *right;
		if(difference != 0)
			break;
		count--;
		left++;
		right++;
	}
	return difference;
}

/* Returns the highest counter among the copies of part `partId`. */
u32 sySaveGetLatestCounter(s32 partId) {
	SAVE_WORKSPACE *state = Ram_HeapSlots->saveState;
	u32 latestCounter;
	u32 i;

	latestCounter = 0;
	for(i = 0; i < SAVE_SECTOR_COUNT; i += SAVE_SECTORS_PER_PART) {
		if(state->occupied[i] != 0 && partId == state->partId[i]) {
			if(latestCounter < state->cnt[i])
				latestCounter = state->cnt[i];
		}
	}
	return latestCounter;
}

/* Reads the summary of the latest save in each of the SAVE_FILE_COUNT files, */
/* with its second play time from its next sector, clearing it first; returns */
/* how many files have one. */
s32 sySaveLoadFileSummaries(void) {
	SAVE_WORKSPACE *state = Ram_HeapSlots->saveState;
	SAVE_SUMMARY *summary;
	s32 count;
	u32 file;

	summary = state->summary;
	count = 0;
	for(file = 0; file < SAVE_FILE_COUNT; file++) {
		u32 sector;

		btDmaFill32(0, summary, sizeof(*summary));
		sector = sySaveFindLatestSector(file);
		if(sector < SAVE_SECTOR_COUNT) {
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

/* Unused: reads part SAVE_PART_EXTRA's summary into the extra summary of the */
/* save's state, with its second play time from its next sector; returns 1, or
 */
/* 0 when the part has no copy. */
s32 sySaveLoadExtraSummary(void) {
	SAVE_WORKSPACE *state;
	SAVE_SUMMARY *summary;
	u32 index;

	state = Ram_HeapSlots->saveState;
	summary = &state->extra;
	btDmaFill32(0, summary, sizeof(*summary));
	index = sySaveFindLatestSector(SAVE_PART_EXTRA);
	if(index < SAVE_SECTOR_COUNT) {
		ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
		/* XXX (no evidence): whether part SAVE_PART_EXTRA keeps The Broken
		 * Seal's */
		/* layout; nothing in this game calls this reader, and no code names a
		 */
		/* writer of the part. */
		ReadFlash(index + 1, SAVE_BROKEN_SEAL_SECOND_PLAY_TIME_OFFSET,
				  (u8 *)&state->extra.secondPartPlayTime,
				  sizeof(state->extra.secondPartPlayTime));
		return 1;
	}
	return 0;
}

/* Removes the flash timer handler and frees the save workspace's heap slot, */
/* with every block allocated after it in the same pool. */
void sySaveReleaseState(void) {
	syIntFunction(IRQ_TIMER0 + SAVE_FLASH_TIMER, 0, NULL);
	syHeapSlotRelease(HEAP_SLOT_SAVE);
}

#undef DMA3_BUSY
#undef WAIT_DMA
#undef SAVE_START_DMA
