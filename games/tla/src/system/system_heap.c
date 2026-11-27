/* The heap: its two bump allocators and heap slots. Part of system.c. */

/* Empties the heap: clears the heap slots (both pool pointers and every slot)
   and sets the pools' next addresses to IwramHeap and EwramHeap. */
void syHeapInit(void)
{
	HEAP_SLOTS *state = Ram_HeapSlots;

	DMA_FILL32(0, state, sizeof(HEAP_SLOTS));
	Ram_HeapSlots->nextIwram = IwramHeap;
	Ram_HeapSlots->nextEwram = EwramHeap;
}

/* Returns the IWRAM bytes left to allocate, up to the sound mixer's workspace
   (IwramSoundMixWorkspace). */
Sint32 syHeapGetRemainingIWRAM(void)
{
	HEAP_SLOTS *state = Ram_HeapSlots;

	return (Sint32)IwramSoundMixWorkspace - (Sint32)state->nextIwram;
}

/* Returns the EWRAM bytes left to allocate, up to the end of EWRAM. Nothing
   calls it or its stub. */
Sint32 syHeapGetRemainingEWRAM(void)
{
	HEAP_SLOTS *state = Ram_HeapSlots;

	return EWRAM_END - (Sint32)state->nextEwram;
}

/* Returns heap slot `slot`'s block, first allocating `size` bytes for it
   (rounded up to words), from IWRAM when they fit and EWRAM otherwise. */
void *syHeapSlotAlloc(Sint32 slot, Sint32 size)
{
	HEAP_SLOTS *state;
	Sint32		alignedSize;
	Uint32		address;
	Uint32		nextAddress;
	Uint32		next;
	Uint32		block;

	state = Ram_HeapSlots;
	block = (Uint32)HEAP_SLOT_AT(state, slot);
	if(block == 0) {
		block = (Uint32)state->nextIwram;
		alignedSize = (((Uint32)size + 3) >> 2) * 4;
		next = block + alignedSize;
		/* A block ending exactly at the pool's end is refused too. */
		if(next >= (Uint32)Ram_IwramHeapEnd) {
			address = (Uint32)state->nextEwram;
			nextAddress = address + alignedSize;
			if(nextAddress >= (Uint32)EWRAM_END) {
				return NULL;
			}
			state->nextEwram = (void *)nextAddress;
			HEAP_SLOT_AT(state, slot) = (void *)address;
			return (void *)address;
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
	Uint32		address;
	Uint32		nextAddress;
	Uint32		block;

	state = Ram_HeapSlots;
	block = (Uint32)HEAP_SLOT_AT(state, slot);
	if(block == 0) {
		alignedSize = (Uint32)size + 3;
		address = (Uint32)state->nextEwram;
		alignedSize = (alignedSize >> 2) * 4;
		next = address + alignedSize;
		if(next >= (Uint32)EWRAM_END) {
			address = (Uint32)state->nextIwram;
			nextAddress = address + alignedSize;
			if(nextAddress > (Uint32)(Ram_IwramHeapEnd - 1)) {
				return NULL;
			}
			state->nextIwram = (void *)nextAddress;
			HEAP_SLOT_AT(state, slot) = (void *)address;
			return (void *)address;
		}
		state->nextEwram = (void *)next;
		HEAP_SLOT_AT(state, slot) = (void *)address;
		return (void *)address;
	}
	return (void *)block;
}

/* Fills the free space of both heaps, from each pool's next address to its end,
   with `value`. */
void syHeapFillFreeWords(Uint32 value)
{
	HEAP_SLOTS *state = Ram_HeapSlots;
	Uint32		address;

	address = (Uint32)state->nextIwram;
	syWordsFillEntry(
		(void *)address, (Uint32)Ram_IwramHeapEnd - address, value);
	address = (Uint32)state->nextEwram;
	syWordsFillEntry((void *)address, (Uint32)EWRAM_END - address, value);
}

/* Allocates `size` bytes (rounded up to words) with no heap slot, from IWRAM
   when they fit and EWRAM otherwise, and advances that pool's next address. */
void *syHeapAlloc(Sint32 size)
{
	HEAP_SLOTS *state = Ram_HeapSlots;
	Uint32		nextAddress;
	Uint32		next;
	Uint32		address;
	Uint32		words;
	Uint32		limit;

	words = (Uint32)size + 3;
	address = (Uint32)state->nextIwram;
	words >>= 2;
	limit = (Uint32)(Ram_IwramHeapEnd - 1);
	size = (Sint32)(words << 2);
	next = address + (Uint32)size;
	if(next > limit) {
		address = (Uint32)state->nextEwram;
		nextAddress = address + (Uint32)size;
		if(nextAddress >= (Uint32)EWRAM_END) {
			return NULL;
		}
		state->nextEwram = (void *)nextAddress;
	} else {
		state->nextIwram = (void *)next;
	}
	return (void *)address;
}

/* As syHeapAlloc, but tries EWRAM first. Called mostly by the battle engine and
   the credits for buffers too large to want IWRAM. */
void *syHeapAllocEWRAM(Sint32 size)
{
	HEAP_SLOTS *state = Ram_HeapSlots;
	Uint32		nextAddress;
	Uint32		next;
	Uint32		address;
	Uint32		words;

	words = (Uint32)size + 3;
	address = (Uint32)state->nextEwram;
	size = (Sint32)((words >> 2) << 2);
	next = address + (Uint32)size;
	if(next >= (Uint32)EWRAM_END) {
		address = (Uint32)state->nextIwram;
		nextAddress = address + (Uint32)size;
		if(nextAddress > (Uint32)(Ram_IwramHeapEnd - 1)) {
			return NULL;
		}
		state->nextIwram = (void *)nextAddress;
	} else {
		state->nextEwram = (void *)next;
	}
	return (void *)address;
}
