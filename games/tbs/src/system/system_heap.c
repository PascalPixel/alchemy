/* The heap: its two bump allocators and heap slots. Part of system.c. */

/* The heap: two bump allocators, one in IWRAM up to syRamIWRAMHeapEnd and one
   in EWRAM up to its end (EWRAM_END), and 62 heap slots for long-lived blocks
   (HEAP_SLOT). Nothing is freed one block at a time. */

/* Empties the heap: clears the 64 words of HeapSlots (both pool pointers and
   every heap slot) and sets the pools' next addresses to IwramHeap and
   EwramHeap. */
void syHeapInit(void)
{
	HEAP_STATE *state = (HEAP_STATE *)HeapSlots;

	DMA_FILL32(0, state, 256);
	state->heap.nextIwram = IwramHeap;
	state->heap.nextEwram = EwramHeap;
}

/* Returns the IWRAM bytes left to allocate, up to syRamIWRAMHeapEnd (the
   address of CartridgeResetMarker), the end the allocators test. */
Sint32 syHeapGetRemainingIWRAM(void)
{
	HEAP_STATE *state = (HEAP_STATE *)HeapSlots;

	return (Sint32)(*(Uint8 (*)[]) & CartridgeResetMarker) -
		(Sint32)state->heap.nextIwram;
}

/* Returns the EWRAM bytes left to allocate, up to the end of EWRAM. btRun calls
   it around each action and ignores the result. */
Sint32 syHeapGetRemainingEWRAM(void)
{
	HEAP_STATE *state = (HEAP_STATE *)HeapSlots;

	return EWRAM_END - (Sint32)state->heap.nextEwram;
}

/* Returns heap slot `slot`'s block, first allocating `size` bytes for it
   (rounded up to words), from IWRAM when they fit and EWRAM otherwise. */
void *syHeapSlotAllocate(Sint32 slot, Sint32 size)
{
	HEAP_STATE *state;
	Sint32		alignedSize;
	Uint32		address;
	Uint32		nextAddress;
	Uint32		next;
	Uint32		cachedAddress;

	state = (HEAP_STATE *)HeapSlots;
	cachedAddress = (Uint32)state->slots[slot];
	if(cachedAddress == 0) {
		cachedAddress = (Uint32)state->heap.nextIwram;
		alignedSize = (((Uint32)size + 3) >> 2) * 4;
		next = cachedAddress + alignedSize;
		/* A block ending exactly at the pool's end is refused too */
		if(next >= (Uint32)syRamIWRAMHeapEnd) {
			address = (Uint32)state->heap.nextEwram;
			nextAddress = address + alignedSize;
			if(nextAddress >= EWRAM_END) {
				return NULL;
			}
			state->heap.nextEwram = (void *)nextAddress;
			state->slots[slot] = (void *)address;
			return (void *)address;
		}
		state->heap.nextIwram = (void *)next;
		state->slots[slot] = (void *)cachedAddress;
		return (void *)cachedAddress;
	}
	return (void *)cachedAddress;
}

/* As syHeapSlotAllocate, but tries EWRAM first, for blocks that need room more
   than speed: the battle, shop and save state, among others. */
void *syHeapSlotAllocateEWRAM(Sint32 slot, Sint32 size)
{
	HEAP_STATE *state;
	Uint32		alignedSize;
	Uint32		next;
	Uint32		address;
	Uint32		nextAddress;
	Uint32		cachedAddress;

	state = (HEAP_STATE *)HeapSlots;
	cachedAddress = (Uint32)state->slots[slot];
	if(cachedAddress == 0) {
		address = (Uint32)state->heap.nextEwram;
		alignedSize = (((Uint32)size + 3) >> 2) * 4;
		next = address + alignedSize;
		if(next >= EWRAM_END) {
			address = (Uint32)state->heap.nextIwram;
			nextAddress = address + alignedSize;
			if(nextAddress >= (Uint32)syRamIWRAMHeapEnd) {
				return NULL;
			}
			state->heap.nextIwram = (void *)nextAddress;
			state->slots[slot] = (void *)address;
			return (void *)address;
		}
		state->heap.nextEwram = (void *)next;
		state->slots[slot] = (void *)address;
		return (void *)address;
	}
	return (void *)cachedAddress;
}

/* Allocates `size` bytes (rounded up to words) with no heap slot, from IWRAM
   when they fit and EWRAM otherwise, and advances that pool's next address. */
void *syHeapAllocate(Sint32 size)
{
	HEAP_STATE *state = (HEAP_STATE *)HeapSlots;
	Uint32		nextAddress;
	Uint32		next;
	Uint32		allocationAddress;
	Uint32		alignedWords = ((Uint32)size + 3) >> 2;

	allocationAddress = (Uint32)state->heap.nextIwram;
	size = alignedWords << 2;
	next = allocationAddress + size;
	if(next >= (Uint32)syRamIWRAMHeapEnd) {
		allocationAddress = (Uint32)state->heap.nextEwram;
		nextAddress = allocationAddress + size;
		if(nextAddress >= EWRAM_END) {
			return NULL;
		}
		state->heap.nextEwram = (void *)nextAddress;
	} else {
		state->heap.nextIwram = (void *)next;
	}
	return (void *)allocationAddress;
}

/* As syHeapAllocate, but tries EWRAM first. Called mostly by the battle code
   and the credits for buffers too large to want IWRAM. */
void *syHeapAllocateEWRAM(Sint32 size)
{
	HEAP_STATE *state = (HEAP_STATE *)HeapSlots;
	Uint32		alternateNextAddress;
	Uint32		primaryNextAddress;
	Uint32		allocationAddress;
	Uint32		alignedWords = ((Uint32)size + 3) >> 2;

	allocationAddress = (Uint32)state->heap.nextEwram;
	size = alignedWords << 2;
	primaryNextAddress = allocationAddress + size;
	if(primaryNextAddress >= EWRAM_END) {
		allocationAddress = (Uint32)state->heap.nextIwram;
		alternateNextAddress = allocationAddress + size;
		if(alternateNextAddress >= (Uint32)syRamIWRAMHeapEnd) {
			return NULL;
		}
		state->heap.nextIwram = (void *)alternateNextAddress;
	} else {
		state->heap.nextEwram = (void *)primaryNextAddress;
	}
	return (void *)allocationAddress;
}

/* Unused: reserved empty memory export. */
UNUSED void syHeapReservedNoOperation(void)
{
}
