/* @file */
/* Saving: flash sectors, parts and the save state. */
/* Part of system.c. */

/* The save code's timer, retries, sectors and checksum. */
enum {
	SAVE_FLASH_TIMER = 2, /* the hardware timer the SDK's flash library uses */
	SAVE_FLASH_IDENTIFY_ATTEMPTS = 8, /* tries, a frame apart */
	/* Sectors scanned; as a sector number, "none" in the functions below. */
	SAVE_SECTOR_COUNT = 16,
	SAVE_PART_NONE = 16, /* the part id of a free or invalidated sector */
	/* The highest counter a write keeps; a copy written past it is */
	/* rewritten as 1. */
	SAVE_COUNTER_LIMIT = 65000,
	/* Payload bytes summed into the checksum: all 4080 but the last eight. */
	SAVE_CHECKSUM_BYTES = 4072
};

/* Where a file's second copy of the play time lies in its second part: */
/* mnSaveBuildFileSummary (window/screens_save.c) writes it into */
/* SceneLocalState, which opens the save image's second half, after the part's
 */
/* header. */
#define SAVE_SECOND_PLAY_TIME_OFFSET                                           \
	(sizeof(SAVE_SECTOR_HEADER) + SECOND_PART_PLAY_TIME_WORD * sizeof(u32))

/* Allocates the save state (heap slot HEAP_SLOT_SAVE), installs the SDK's */
/* flash library's SAVE_FLASH_TIMER handler, identifies the flash chip (up to */
/* eight tries, a frame apart) and scans all 16 sectors into the save state's */
/* occupied, partId and counter tables: a sector counts as taken when its */
/* signature and checksum are good, and of two copies of a part only the newer
 */
/* stays taken. */
s32 sySaveInitState(void) {
	SAVE_STATE *state;
	SAVE_SECTOR_HEADER header;
	SAVE_SECTOR_HEADER *slot;
	u32 index;

	state = syHeapSlotAllocEwram(HEAP_SLOT_SAVE, sizeof(*state));
	btDmaFill32(0, state, sizeof(*state));
	SetFlashTimerIntr(SAVE_FLASH_TIMER, &IrqHandlers[IRQ_TIMER2]);

	/* Wait a frame after every failed probe, including the last one, before */
	/* reporting that no flash chip answered. */
	for(index = 0; index < SAVE_FLASH_IDENTIFY_ATTEMPTS; index++) {
		if((u16)IdentifyFlash() == 0)
			break;
		sySynch(1);
	}
	if(index >= SAVE_FLASH_IDENTIFY_ATTEMPTS)
		return 1;

	for(index = 0; index < SAVE_SECTOR_COUNT; index++) {
		u32 status;
		u32 part;

		state->occupied[index] = 0;
		state->partId[index] = SAVE_PART_NONE;
		state->cnt[index] = 0;
		status = sySaveReadSector(index);
		START_DMA(&state->slot, &header, DMA_COPY_WORDS(4));
		WAIT_DMA();
		/* FAKEMATCH(temporary): slot holds &header; as header. at its uses. */
		slot = &header;

		/* Not a save sector, or one for no part, or a damaged one: it stays
		 * free. */
		if(sySaveCompareBytes(slot->signature, (u8 *)Save_Signature, 7) != 0)
			continue;
		state->cnt[index] = slot->cnt;
		part = slot->partId;
		if((u8)part >= SAVE_PART_NONE)
			continue;
		if(status != 0)
			continue;

		state->occupied[index] = 1;
		state->partId[index] = part;
		/* An interrupted replacement can leave both copies valid. */
		for(; status < index; status++) {
			if(state->partId[status] == (u8)part) {
				if(state->cnt[status] < slot->cnt)
					state->occupied[status] = 0;
				else
					state->occupied[index] = 0;
			}
		}
	}
	return 0;
}

/* Picks the free sector to write part `partId` to, at random, or returns */
/* SAVE_SECTOR_COUNT for none. The last free sector is kept for replacing a */
/* part that already has a copy. */
u32 sySaveSelectWriteSector(s32 partId) {
	u32 empty[SAVE_SECTOR_COUNT];
	s32 count;
	u32 index;
	u8 *slot;
	u8 value;

	slot = SaveStateWork->occupied;
	count = 0;
	for(index = 0; index < SAVE_SECTOR_COUNT; index += 1) {
		value = *slot++;
		if(value == 0) {
			empty[count++] = index;
		}
	}
	index = SAVE_SECTOR_COUNT;
	if(count != 0) {
		if(count == 1) {
			index = empty[0];
			if(sySaveFindLatestSector(partId) == SAVE_SECTOR_COUNT) {
				index = SAVE_SECTOR_COUNT;
			}
		}
		else {
			/* A free sector at random, so the writes spread over the free
			 * sectors */
			/* and no sector of the flash chip wears out first; nothing else
			 * depends */
			/* on which free sector a part takes. */
			index = syRandom() % count;
			index = empty[index];
		}
	}
	return index;
}

/* Programs the save state's sector buffer into flash sector `sector` and */
/* verifies it, through the SDK's flash library's function for this chip. */
u32 sySaveWriteSector(s32 sector) {
	SAVE_STATE *state = SaveStateWork;
	u16 index = sector;

	if(ProgramFlashSectorWork(index, state->slot.bytes) != 0)
		return 1;
	if(VerifyFlashSector(index, state->slot.bytes) != 0)
		return 1;
	return 0;
}

/* Reads flash sector `index` into the save state's sector buffer. Returns 0 */
/* when its checksum is good. */
s32 sySaveReadSector(s32 index) {
	SAVE_STATE *state;
	SAVE_SECTOR_HEADER header;
	u32 checksum;

	state = SaveStateWork;
	ReadFlash(index, 0, state->slot.bytes, sizeof(state->slot));
	syDMAXCopy(&state->slot, &header, DMA_COPY_WORDS(4), REG_DMA3);
	WAIT_DMA();
	checksum = sySaveChecksumSector();
	return (u16)checksum - header.checksum;
}

/* Unused: erases flash sector `sector` with the SDK's flash library's */
/* function for this chip. */
UNUSED u16 sySaveEraseSector(u16 sector) {
	return EraseFlashSectorWork(sector);
}

/* Saves 4080 bytes from `source` as part `partId`: writes the new copy with */
/* the next counter, then invalidates the old one, updating the save state's */
/* sector tables. */
s32 sySaveWritePart(s32 partId, void *source) {
	SAVE_STATE *state;
	SAVE_SECTOR_HEADER header;
	u32 current;
	u32 slot;

	state = SaveStateWork;
	btDmaFill32(0, &state->slot, sizeof(state->slot));
	WAIT_DMA();
	current = sySaveFindLatestSector(partId);
	slot = sySaveSelectWriteSector(partId);
	if(slot >= SAVE_SECTOR_COUNT)
		return 1;

	/* Build the new sector before changing which old sector counts as current.
	 */
	START_DMA(source, state->slot.part.payload,
			  DMA_COPY_WORDS(sizeof(state->slot.part.payload) / 4));
	WAIT_DMA();
	START_DMA(Save_HeaderTemplate, &header, DMA_COPY_WORDS(2));
	WAIT_DMA();
	header.partId = partId;
	header.checksum = sySaveChecksumSector();
	header.cnt = sySaveGetLatestCounter(partId) + 1;
	START_DMA(&header, &state->slot.part.header, DMA_COPY_WORDS(4));
	WAIT_DMA();

	if(sySaveWriteSector(slot) != 0)
		return 1;
	/* A power loss before invalidation leaves two valid copies; the next */
	/* scan chooses the new one by its higher counter. */
	if(current < SAVE_SECTOR_COUNT && sySaveInvalidateSector(current) != 0)
		return 1;

	if(header.cnt > SAVE_COUNTER_LIMIT) {
		/* Past the limit the count starts again at 1, rewritten over the old */
		/* copy: the scan compares counters plainly, so a wrapped one would
		 * lose. */
		header.cnt = 1;
		START_DMA(&header, &state->slot.part.header, DMA_COPY_WORDS(4));
		WAIT_DMA();
		if(sySaveWriteSector(current) != 0)
			return 1;
		if(sySaveInvalidateSector(slot) != 0)
			return 1;
		slot = current;
	}

	state->occupied[slot] = 1;
	state->partId[slot] = partId;
	state->cnt[slot] = header.cnt;
	return 0;
}

/* Loads the current copy of part `partId` (4080 bytes) into `destination`. */
/* Returns 1 when there is none and 0 otherwise. */
u32 sySaveReadPart(s32 partId, void *destination) {
	SAVE_STATE *state;
	u32 index;

	state = SaveStateWork;
	index = sySaveFindLatestSector(partId);
	if(index >= SAVE_SECTOR_COUNT)
		return 1;
	sySaveReadSector(index);
	START_DMA(state->slot.part.payload, destination,
			  DMA_COPY_WORDS(sizeof(state->slot.part.payload) / 4));
	WAIT_DMA();
	return 0;
}

/* Invalidates the current copy of part `partId`. Called when the file list */
/* erases a file. */
u32 sySaveDeletePart(s32 partId) {
	s32 index;

	index = sySaveFindLatestSector(partId);
	if((u32)index >= SAVE_SECTOR_COUNT) {
		return 1U;
	}
	if(sySaveInvalidateSector(index) != 0) {
		return 1;
	}
	return 0;
}

/* Sums the first SAVE_CHECKSUM_BYTES payload bytes of the save state's sector
 */
/* buffer; the final eight payload bytes are excluded. */
s32 sySaveChecksumSector(void) {
	SAVE_STATE *state;
	u32 limit;
	u32 offset;
	s32 sum;

	state = SaveStateWork;
	limit = SAVE_CHECKSUM_BYTES - 1;
	sum = 0;
	for(offset = 0; offset <= limit; offset += 8) {
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

/* Returns the occupied sector holding the newest copy of part `partId`, or */
/* SAVE_SECTOR_COUNT. */
u32 sySaveFindLatestSector(s32 partId) {
	SAVE_STATE *state;
	u32 latestCounter;
	u32 latestSlot;
	u32 i;

	state = SaveStateWork;
	latestSlot = SAVE_SECTOR_COUNT;
	latestCounter = 0;
	for(i = 0; i < SAVE_SECTOR_COUNT; i++) {
		if(state->occupied[i] != 0 && partId == state->partId[i]) {
			if(latestCounter < state->cnt[i]) {
				latestCounter = state->cnt[i];
				latestSlot = i;
			}
		}
	}
	return latestSlot;
}

/* Overwrites sector `index` with a header for no part (SAVE_PART_NONE) and */
/* clears its occupied entry. Returns 0 on success and 1 on failure. */
s32 sySaveInvalidateSector(s32 index) {
	SAVE_STATE *state;
	SAVE_SECTOR_HEADER header;

	state = SaveStateWork;
	btDmaFill32(0, &header, sizeof(header));
	WAIT_DMA();
	START_DMA(Save_HeaderTemplate, &header, DMA_COPY_WORDS(2));
	WAIT_DMA();
	header.partId = SAVE_PART_NONE;
	header.cnt = 0;
	START_DMA(&header, &state->slot.part.header, DMA_COPY_WORDS(4));
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
	SAVE_STATE *state;
	u32 latestCounter;
	u32 i;

	state = SaveStateWork;
	latestCounter = 0;
	for(i = 0; i < SAVE_SECTOR_COUNT; i++) {
		if(state->occupied[i] != 0 && partId == state->partId[i]) {
			if(latestCounter < state->cnt[i])
				latestCounter = state->cnt[i];
		}
	}
	return latestCounter;
}

/* Reads the summary at the start of each of the three save files into the */
/* save state's summary table for the file list, and each file's second copy */
/* of the play time, from SAVE_SECOND_PLAY_TIME_OFFSET in its second part, */
/* into its secondPartPlayTime (zero when it has none). */
s32 sySaveLoadFileSummaries(void) {
	SAVE_STATE *state;
	SAVE_FILE_SUMMARY *summary;
	u32 group;
	s32 count;

	state = SaveStateWork;
	summary = state->summary;
	count = 0;
	group = 0;
	do {
		u32 index;

		btDmaFill32(0, summary, sizeof(*summary));
		index = sySaveFindLatestSector(group);
		if(index < SAVE_SECTOR_COUNT) {
			ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
			count++;
		}
		index = sySaveFindLatestSector(group + SAVE_FILE_COUNT);
		if(index < SAVE_SECTOR_COUNT)
			ReadFlash(index, SAVE_SECOND_PLAY_TIME_OFFSET,
					  (u8 *)&summary->secondPartPlayTime,
					  sizeof(summary->secondPartPlayTime));
		else
			summary->secondPartPlayTime = 0;
		group++;
		summary++;
	} while(group < SAVE_FILE_COUNT);
	return count;
}

/* Removes the flash timer handler and frees the save state's heap slot, */
/* with every block allocated after it in the same pool. */
u32 sySaveReleaseState(void) {
	syIntFunction(IRQ_TIMER2, 0, NULL);
	syHeapSlotRelease(HEAP_SLOT_SAVE);
	/* UB: nothing is returned, and no caller reads the result */
#ifdef UBFIX
	return 0;
#endif
}

#undef SAVE_SECOND_PLAY_TIME_OFFSET
