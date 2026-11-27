/* The heap: its two bump allocators and heap slots. Part of system.c. */

/* Empties the heap: clears the heap slots (both pool pointers and every slot)
   and sets the pools' next addresses to IwramHeap and EwramHeap. */
void syHeapInit(void)
{
	HEAP_SLOTS *state = RamHeapSlots;

	DMA_FILL32(0, state, sizeof(HEAP_SLOTS));
	RamHeapSlots->nextIwram = IwramHeap;
	RamHeapSlots->nextEwram = EwramHeap;
}

/* Returns the IWRAM bytes left to allocate, up to the sound mixer's workspace
   (IwramSoundMixWorkspace). */
Sint32 syHeapGetRemainingIWRAM(void)
{
	HEAP_SLOTS *state = RamHeapSlots;

	return (Sint32)IwramSoundMixWorkspace - (Sint32)state->nextIwram;
}

/* Returns the EWRAM bytes left to allocate, up to the end of EWRAM. Nothing
   calls it or its stub. */
Sint32 syHeapGetRemainingEWRAM(void)
{
	HEAP_SLOTS *state = RamHeapSlots;

	return EWRAM_END - (Sint32)state->nextEwram;
}

/* Returns heap slot `slot`'s block, first allocating `size` bytes for it
   (rounded up to words), from IWRAM when they fit and EWRAM otherwise. */
void *syHeapSlotAlloc(Sint32 slot, Sint32 size)
{
	HEAP_SLOTS *state;
	Sint32		alignedSize;
	Uint32		adr;
	Uint32		nextAdr;
	Uint32		next;
	Uint32		block;

	state = RamHeapSlots;
	block = (Uint32)HEAP_SLOT_AT(state, slot);
	if(block == 0) {
		block = (Uint32)state->nextIwram;
		alignedSize = (((Uint32)size + 3) >> 2) * 4;
		next = block + alignedSize;
		/* A block ending exactly at the pool's end is refused too. */
		if(next >= (Uint32)RamIwramHeapEnd) {
			adr = (Uint32)state->nextEwram;
			nextAdr = adr + alignedSize;
			if(nextAdr >= (Uint32)EWRAM_END) {
				return NULL;
			}
			state->nextEwram = (void *)nextAdr;
			HEAP_SLOT_AT(state, slot) = (void *)adr;
			return (void *)adr;
		}
		state->nextIwram = (void *)next;
		HEAP_SLOT_AT(state, slot) = (void *)block;
		return (void *)block;
	}
	return (void *)block;
}

/* As syHeapSlotAlloc, but tries EWRAM first, for blocks that need room more
   than speed: the battle, shop and save state, among others. */
void *syHeapSlotAllocEWRAM(Sint32 slot, Sint32 size)
{
	HEAP_SLOTS *state;
	Uint32		alignedSize;
	Uint32		next;
	Uint32		adr;
	Uint32		nextAdr;
	Uint32		block;

	state = RamHeapSlots;
	block = (Uint32)HEAP_SLOT_AT(state, slot);
	if(block == 0) {
		alignedSize = (Uint32)size + 3;
		adr = (Uint32)state->nextEwram;
		alignedSize = (alignedSize >> 2) * 4;
		next = adr + alignedSize;
		if(next >= (Uint32)EWRAM_END) {
			adr = (Uint32)state->nextIwram;
			nextAdr = adr + alignedSize;
			if(nextAdr > (Uint32)(RamIwramHeapEnd - 1)) {
				return NULL;
			}
			state->nextIwram = (void *)nextAdr;
			HEAP_SLOT_AT(state, slot) = (void *)adr;
			return (void *)adr;
		}
		state->nextEwram = (void *)next;
		HEAP_SLOT_AT(state, slot) = (void *)adr;
		return (void *)adr;
	}
	return (void *)block;
}

/* Fills the free space of both heaps, from each pool's next address to its end,
   with `value`. */
void syHeapFillFreeWords(Uint32 value)
{
	HEAP_SLOTS *state = RamHeapSlots;
	Uint32		adr;

	adr = (Uint32)state->nextIwram;
	syWordsFillEntry((void *)adr, (Uint32)RamIwramHeapEnd - adr, value);
	adr = (Uint32)state->nextEwram;
	syWordsFillEntry((void *)adr, (Uint32)EWRAM_END - adr, value);
}

/* Allocates `size` bytes (rounded up to words) with no heap slot, from IWRAM
   when they fit and EWRAM otherwise, and advances that pool's next address. */
void *syHeapAlloc(Sint32 size)
{
	HEAP_SLOTS *state = RamHeapSlots;
	Uint32		nextAdr;
	Uint32		next;
	Uint32		adr;
	Uint32		words;
	Uint32		limit;

	words = (Uint32)size + 3;
	adr = (Uint32)state->nextIwram;
	words >>= 2;
	limit = (Uint32)(RamIwramHeapEnd - 1);
	size = (Sint32)(words << 2);
	next = adr + (Uint32)size;
	if(next > limit) {
		adr = (Uint32)state->nextEwram;
		nextAdr = adr + (Uint32)size;
		if(nextAdr >= (Uint32)EWRAM_END) {
			return NULL;
		}
		state->nextEwram = (void *)nextAdr;
	} else {
		state->nextIwram = (void *)next;
	}
	return (void *)adr;
}

/* As syHeapAlloc, but tries EWRAM first. Called mostly by the battle engine and
   the credits for buffers too large to want IWRAM. */
void *syHeapAllocEWRAM(Sint32 size)
{
	HEAP_SLOTS *state = RamHeapSlots;
	Uint32		nextAdr;
	Uint32		next;
	Uint32		adr;
	Uint32		words;

	words = (Uint32)size + 3;
	adr = (Uint32)state->nextEwram;
	size = (Sint32)((words >> 2) << 2);
	next = adr + (Uint32)size;
	if(next >= (Uint32)EWRAM_END) {
		adr = (Uint32)state->nextIwram;
		nextAdr = adr + (Uint32)size;
		if(nextAdr > (Uint32)(RamIwramHeapEnd - 1)) {
			return NULL;
		}
		state->nextIwram = (void *)nextAdr;
	} else {
		state->nextEwram = (void *)next;
	}
	return (void *)adr;
}
