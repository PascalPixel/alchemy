/* @file */
/* The heap: its two bump allocators and heap slots. */
/* Part of system.c. */

/* The heap: two bump allocators, one in IWRAM up to syRamIwramHeapEnd and one
 */
/* in EWRAM up to its end (EWRAM_END), and 62 heap slots for long-lived */
/* blocks (HEAP_SLOT). Nothing is freed one block at a time. */

/* Empties the heap: clears the 64 words of HeapSlots (both pool pointers and */
/* every heap slot) and sets the pools' next addresses to IwramHeap and */
/* EwramHeap. */
void syHeapInit(void) {
	HEAP_STATE *state = (HEAP_STATE *)HeapSlots;

	btDmaFill32(0, state, 256);
	state->heap.nextIwram = IwramHeap;
	state->heap.nextEwram = EwramHeap;
}

/* Returns the IWRAM bytes left to allocate, up to syRamIwramHeapEnd (the */
/* address of CartridgeResetMarker), the end the allocators test. */
s32 syHeapGetRemainingIwram(void) {
	HEAP_STATE *state = (HEAP_STATE *)HeapSlots;

	return (s32)(*(u8 (*)[]) & CartridgeResetMarker) -
		   (s32)state->heap.nextIwram;
}

/* Returns the EWRAM bytes left to allocate, up to the end of EWRAM. */
/* btRun calls it around each action and ignores the result. */
s32 syHeapGetRemainingEwram(void) {
	HEAP_STATE *state = (HEAP_STATE *)HeapSlots;

	return EWRAM_END - (s32)state->heap.nextEwram;
}

/* Returns heap slot `slot`'s block, first allocating `size` bytes for it */
/* (rounded up to words), from IWRAM when they fit and EWRAM otherwise. */
void *syHeapSlotAlloc(s32 slot, s32 size) {
	HEAP_STATE *state;
	s32 alignedSize;
	u32 address;
	u32 nextAddress;
	u32 next;
	u32 cachedAddress;

	state = (HEAP_STATE *)HeapSlots;
	cachedAddress = (u32)state->slots[slot];
	if(cachedAddress == 0) {
		cachedAddress = (u32)state->heap.nextIwram;
		alignedSize = (((u32)size + 3) >> 2) * 4;
		next = cachedAddress + alignedSize;
		/* A block ending exactly at the pool's end is refused too */
		if(next >= (u32)syRamIwramHeapEnd) {
			address = (u32)state->heap.nextEwram;
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

/* As syHeapSlotAlloc, but tries EWRAM first, for blocks that need */
/* room more than speed: the battle, shop and save state, among others. */
void *syHeapSlotAllocEwram(s32 slot, s32 size) {
	HEAP_STATE *state;
	u32 alignedSize;
	u32 next;
	u32 address;
	u32 nextAddress;
	u32 cachedAddress;

	state = (HEAP_STATE *)HeapSlots;
	cachedAddress = (u32)state->slots[slot];
	if(cachedAddress == 0) {
		address = (u32)state->heap.nextEwram;
		alignedSize = (((u32)size + 3) >> 2) * 4;
		next = address + alignedSize;
		if(next >= EWRAM_END) {
			address = (u32)state->heap.nextIwram;
			nextAddress = address + alignedSize;
			if(nextAddress >= (u32)syRamIwramHeapEnd) {
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

/* Allocates `size` bytes (rounded up to words) with no heap slot, from IWRAM */
/* when they fit and EWRAM otherwise, and advances that pool's next address. */
void *syHeapAlloc(s32 size) {
	HEAP_STATE *state = (HEAP_STATE *)HeapSlots;
	u32 nextAddress;
	u32 next;
	u32 allocationAddress;
	u32 alignedWords = ((u32)size + 3) >> 2;

	allocationAddress = (u32)state->heap.nextIwram;
	size = alignedWords << 2;
	next = allocationAddress + size;
	if(next >= (u32)syRamIwramHeapEnd) {
		allocationAddress = (u32)state->heap.nextEwram;
		nextAddress = allocationAddress + size;
		if(nextAddress >= EWRAM_END) {
			return NULL;
		}
		state->heap.nextEwram = (void *)nextAddress;
	}
	else {
		state->heap.nextIwram = (void *)next;
	}
	return (void *)allocationAddress;
}

/* As syHeapAlloc, but tries EWRAM first. Called mostly by the battle code and
 */
/* the credits for buffers too large to want IWRAM. */
void *syHeapAllocEwram(s32 size) {
	HEAP_STATE *state = (HEAP_STATE *)HeapSlots;
	u32 alternateNextAddress;
	u32 primaryNextAddress;
	u32 allocationAddress;
	u32 alignedWords = ((u32)size + 3) >> 2;

	allocationAddress = (u32)state->heap.nextEwram;
	size = alignedWords << 2;
	primaryNextAddress = allocationAddress + size;
	if(primaryNextAddress >= EWRAM_END) {
		allocationAddress = (u32)state->heap.nextIwram;
		alternateNextAddress = allocationAddress + size;
		if(alternateNextAddress >= (u32)syRamIwramHeapEnd) {
			return NULL;
		}
		state->heap.nextIwram = (void *)alternateNextAddress;
	}
	else {
		state->heap.nextEwram = (void *)primaryNextAddress;
	}
	return (void *)allocationAddress;
}

/* Unused: reserved empty memory export. */
UNUSED void syHeapReservedNoOp(void) {}
