/* Saving: flash sectors, parts and the save state. Part of system.c. */

/* The save code's timer, retries, sectors and checksum. */
enum {
	BUP_FLASH_TIMER = 2, /* the hardware timer the SDK's flash library uses */
	BUP_FLASH_IDENTIFY_ATTEMPTS = 8, /* tries, a tick apart */
	/* Sectors scanned; as a sector number, "none" in the functions below. */
	BUP_SECTOR_COUNT = 16,
	BUP_PART_NONE = 16, /* the part id of a free or invalidated sector */
	/* The highest counter a write keeps; a copy written past it is rewritten as
	   1. */
	BUP_COUNTER_LIMIT = 65000,
	/* Payload bytes summed into the checksum: all 4080 but the last eight. */
	BUP_CHECKSUM_BYTES = 4072
};

/* Where a file's second copy of the play time lies in its second part:
   mnSaveBuildFileSummary (window/screens_save.c) writes it into
   SceneLocalState, which opens the save image's second half, after the part's
   header. */
#define BUP_SECOND_PLAY_TIME_OFFSET \
	(sizeof(BUP_SECTOR_HEADER) + SECOND_PART_PLAY_TIME_WORD * sizeof(Uint32))

/* Allocates the save state (heap slot HEAP_SLOT_BUP), installs the SDK's flash
   library's BUP_FLASH_TIMER handler, identifies the flash chip (up to eight
   tries, a tick apart) and scans all 16 sectors into the save state's
   occupied, partId and counter tables: a sector counts as taken when its
   signature and checksum are good, and of two copies of a part only the newer
   stays taken. */
Bool syBupInitState(void)
{
	BUP_STATE		  *state;
	BUP_SECTOR_HEADER  header;
	BUP_SECTOR_HEADER *slot;
	Uint32			   idx;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_BUP, sizeof(*state));
	DMA_FILL32(0, state, sizeof(*state));
	SetFlashTimerIntr(BUP_FLASH_TIMER, &IrqHandlers[IRQ_TIMER2]);

	/* Wait a tick after every failed probe, including the last one, before
	   reporting that no flash chip answered. */
	for(idx = 0; idx < BUP_FLASH_IDENTIFY_ATTEMPTS; idx++) {
		if((Uint16)IdentifyFlash() == 0)
			break;
		sySynch(1);
	}
	if(idx >= BUP_FLASH_IDENTIFY_ATTEMPTS)
		return TRUE;

	for(idx = 0; idx < BUP_SECTOR_COUNT; idx++) {
		Uint32 stat;
		Uint32 part;

		state->occupied[idx] = 0;
		state->partId[idx] = BUP_PART_NONE;
		state->cnt[idx] = 0;
		stat = syBupReadSector(idx);
		START_DMA(&state->slot, &header, DMA_COPY_WORDS(4));
		WAIT_DMA();
		/* FAKEMATCH(temporary): slot holds &header; as header. at its uses. */
		slot = &header;

		/* Not a save sector, or one for no part, or a damaged one: it stays
		   free. */
		if(syBupCompareBytes(slot->signature, (Uint8 *)BupSignature, 7) != 0)
			continue;
		state->cnt[idx] = slot->cnt;
		part = slot->partId;
		if((Uint8)part >= BUP_PART_NONE)
			continue;
		if(stat != 0)
			continue;

		state->occupied[idx] = 1;
		state->partId[idx] = part;
		/* An interrupted replacement can leave both copies valid. */
		for(; stat < idx; stat++) {
			if(state->partId[stat] == (Uint8)part) {
				if(state->cnt[stat] < slot->cnt)
					state->occupied[stat] = 0;
				else
					state->occupied[idx] = 0;
			}
		}
	}
	return FALSE;
}

/* Picks the free sector to write part `partId` to, at random, or returns
   BUP_SECTOR_COUNT for none. The last free sector is kept for replacing a part
   that already has a copy. */
Uint32 syBupSelectWriteSector(Sint32 partId)
{
	Uint32 empty[BUP_SECTOR_COUNT];
	Sint32 cnt;
	Uint32 idx;
	Uint8 *slot;
	Uint8  value;

	slot = BupStateWork->occupied;
	cnt = 0;
	for(idx = 0; idx < BUP_SECTOR_COUNT; idx += 1) {
		value = *slot++;
		if(value == 0) {
			empty[cnt++] = idx;
		}
	}
	idx = BUP_SECTOR_COUNT;
	if(cnt != 0) {
		if(cnt == 1) {
			idx = empty[0];
			if(syBupFindLatestSector(partId) == BUP_SECTOR_COUNT) {
				idx = BUP_SECTOR_COUNT;
			}
		} else {
			/* A free sector at random, so the writes spread over the free
			   sectors and no sector of the flash chip wears out first; nothing
			   else depends on which free sector a part takes. */
			idx = syRandom() % cnt;
			idx = empty[idx];
		}
	}
	return idx;
}

/* Programs the save state's sector buffer into flash sector `sector` and
   verifies it, through the SDK's flash library's function for this chip. */
Uint32 syBupWriteSector(Sint32 sector)
{
	BUP_STATE *state = BupStateWork;
	Uint16	   idx = sector;

	if(ProgramFlashSectorWork(idx, state->slot.bytes) != 0)
		return 1;
	if(VerifyFlashSector(idx, state->slot.bytes) != 0)
		return 1;
	return 0;
}

/* Reads flash sector `index` into the save state's sector buffer. Returns 0
   when its checksum is good. */
Sint32 syBupReadSector(Sint32 index)
{
	BUP_STATE		 *state;
	BUP_SECTOR_HEADER header;
	Uint32			  checksum;

	state = BupStateWork;
	ReadFlash(index, 0, state->slot.bytes, sizeof(state->slot));
	syDMAXCopy(&state->slot, &header, DMA_COPY_WORDS(4), REG_DMA3);
	WAIT_DMA();
	checksum = syBupChecksumSector();
	return (Uint16)checksum - header.checksum;
}

/* Unused: erases flash sector `sector` with the SDK's flash library's function
   for this chip. */
UNUSED Uint16 syBupEraseSector(Uint16 sector)
{
	return EraseFlashSectorWork(sector);
}

/* Saves 4080 bytes from `src` as part `partId`: writes the new copy with the
   next counter, then invalidates the old one, updating the save state's sector
   tables. */
Bool syBupWritePart(Sint32 partId, void *src)
{
	BUP_STATE		 *state;
	BUP_SECTOR_HEADER header;
	Uint32			  cur;
	Uint32			  slot;

	state = BupStateWork;
	DMA_FILL32(0, &state->slot, sizeof(state->slot));
	WAIT_DMA();
	cur = syBupFindLatestSector(partId);
	slot = syBupSelectWriteSector(partId);
	if(slot >= BUP_SECTOR_COUNT)
		return TRUE;

	/* Build the new sector before changing which old sector counts as
	   current. */
	START_DMA(src, state->slot.part.payload,
		DMA_COPY_WORDS(sizeof(state->slot.part.payload) / 4));
	WAIT_DMA();
	START_DMA(BupHeaderTemplate, &header, DMA_COPY_WORDS(2));
	WAIT_DMA();
	header.partId = partId;
	header.checksum = syBupChecksumSector();
	header.cnt = syBupGetLatestCounter(partId) + 1;
	START_DMA(&header, &state->slot.part.header, DMA_COPY_WORDS(4));
	WAIT_DMA();

	if(syBupWriteSector(slot) != 0)
		return TRUE;
	/* A power loss before invalidation leaves two valid copies; the next scan
	   chooses the new one by its higher counter. */
	if(cur < BUP_SECTOR_COUNT && syBupInvalidateSector(cur) != 0)
		return TRUE;

	if(header.cnt > BUP_COUNTER_LIMIT) {
		/* Past the limit the count starts again at 1, rewritten over the old
		   copy: the scan compares counters plainly, so a wrapped one would
		   lose. */
		header.cnt = 1;
		START_DMA(&header, &state->slot.part.header, DMA_COPY_WORDS(4));
		WAIT_DMA();
		if(syBupWriteSector(cur) != 0)
			return TRUE;
		if(syBupInvalidateSector(slot) != 0)
			return TRUE;
		slot = cur;
	}

	state->occupied[slot] = 1;
	state->partId[slot] = partId;
	state->cnt[slot] = header.cnt;
	return FALSE;
}

/* Loads the current copy of part `partId` (4080 bytes) into `dst`.
   Returns 1 when there is none and 0 otherwise. */
Uint32 syBupReadPart(Sint32 partId, void *dst)
{
	BUP_STATE *state;
	Uint32	   idx;

	state = BupStateWork;
	idx = syBupFindLatestSector(partId);
	if(idx >= BUP_SECTOR_COUNT)
		return 1;
	syBupReadSector(idx);
	START_DMA(state->slot.part.payload, dst,
		DMA_COPY_WORDS(sizeof(state->slot.part.payload) / 4));
	WAIT_DMA();
	return 0;
}

/* Invalidates the current copy of part `partId`. Called when the file list
   erases a file. */
Uint32 syBupDeletePart(Sint32 partId)
{
	Sint32 idx;

	idx = syBupFindLatestSector(partId);
	if((Uint32)idx >= BUP_SECTOR_COUNT) {
		return 1U;
	}
	if(syBupInvalidateSector(idx) != 0) {
		return 1;
	}
	return 0;
}

/* Sums the first BUP_CHECKSUM_BYTES payload bytes of the save state's sector
   buffer; the final eight payload bytes are excluded. */
Sint32 syBupChecksumSector(void)
{
	BUP_STATE *state;
	Uint32	   limit;
	Uint32	   ofs;
	Sint32	   sum;

	state = BupStateWork;
	limit = BUP_CHECKSUM_BYTES - 1;
	sum = 0;
	for(ofs = 0; ofs <= limit; ofs += 8) {
		sum += state->slot.part.payload[ofs + 0];
		sum += state->slot.part.payload[ofs + 1];
		sum += state->slot.part.payload[ofs + 2];
		sum += state->slot.part.payload[ofs + 3];
		sum += state->slot.part.payload[ofs + 4];
		sum += state->slot.part.payload[ofs + 5];
		sum += state->slot.part.payload[ofs + 6];
		sum += state->slot.part.payload[ofs + 7];
	}
	return sum;
}

/* Returns the occupied sector holding the newest copy of part `partId`, or
   BUP_SECTOR_COUNT. */
Uint32 syBupFindLatestSector(Sint32 partId)
{
	BUP_STATE *state;
	Uint32	   latestCounter;
	Uint32	   latestSlot;
	Uint32	   i;

	state = BupStateWork;
	latestSlot = BUP_SECTOR_COUNT;
	latestCounter = 0;
	for(i = 0; i < BUP_SECTOR_COUNT; i++) {
		if(state->occupied[i] != 0 && partId == state->partId[i]) {
			if(latestCounter < state->cnt[i]) {
				latestCounter = state->cnt[i];
				latestSlot = i;
			}
		}
	}
	return latestSlot;
}

/* Overwrites sector `idx` with a header for no part (BUP_PART_NONE) and
   clears its occupied entry. Returns 0 on success and 1 on failure. */
Bool syBupInvalidateSector(Sint32 idx)
{
	BUP_STATE		 *state;
	BUP_SECTOR_HEADER header;

	state = BupStateWork;
	DMA_FILL32(0, &header, sizeof(header));
	WAIT_DMA();
	START_DMA(BupHeaderTemplate, &header, DMA_COPY_WORDS(2));
	WAIT_DMA();
	header.partId = BUP_PART_NONE;
	header.cnt = 0;
	START_DMA(&header, &state->slot.part.header, DMA_COPY_WORDS(4));
	WAIT_DMA();
	if(syBupWriteSector(idx) != 0)
		return TRUE;
	state->occupied[idx] = 0;
	state->partId[idx] = BUP_PART_NONE;
	state->cnt[idx] = 0;
	return FALSE;
}

#include "system/bup_compare_bytes.c"

/* Returns the highest counter among the copies of part `partId`. */
Uint32 syBupGetLatestCounter(Sint32 partId)
{
	BUP_STATE *state;
	Uint32	   latestCounter;
	Uint32	   i;

	state = BupStateWork;
	latestCounter = 0;
	for(i = 0; i < BUP_SECTOR_COUNT; i++) {
		if(state->occupied[i] != 0 && partId == state->partId[i]) {
			if(latestCounter < state->cnt[i])
				latestCounter = state->cnt[i];
		}
	}
	return latestCounter;
}

/* Reads the summary at the start of each of the three save files into the save
   state's summary table for the file list, and each file's second copy of the
   play time, from BUP_SECOND_PLAY_TIME_OFFSET in its second part, into its
   secondPartPlayTime (zero when it has none). */
Sint32 syBupLoadFileSummaries(void)
{
	BUP_STATE		 *state;
	BUP_FILE_SUMMARY *summary;
	Uint32			  group;
	Sint32			  cnt;

	state = BupStateWork;
	summary = state->summary;
	cnt = 0;
	group = 0;
	do {
		Uint32 idx;

		DMA_FILL32(0, summary, sizeof(*summary));
		idx = syBupFindLatestSector(group);
		if(idx < BUP_SECTOR_COUNT) {
			ReadFlash(idx, 0, (Uint8 *)summary, sizeof(*summary));
			cnt++;
		}
		idx = syBupFindLatestSector(group + BUP_FILE_COUNT);
		if(idx < BUP_SECTOR_COUNT)
			ReadFlash(idx, BUP_SECOND_PLAY_TIME_OFFSET,
				(Uint8 *)&summary->secondPartPlayTime,
				sizeof(summary->secondPartPlayTime));
		else
			summary->secondPartPlayTime = 0;
		group++;
		summary++;
	} while(group < BUP_FILE_COUNT);
	return cnt;
}

/* Removes the flash timer handler and frees the save state's heap slot, with
   every block allocated after it in the same pool. */
Uint32 syBupReleaseState(void)
{
	syIntFunction(IRQ_TIMER2, 0, NULL);
	syHeapSlotRelease(HEAP_SLOT_BUP);
	/* UB: nothing is returned, and no caller reads the result */
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

#undef BUP_SECOND_PLAY_TIME_OFFSET
