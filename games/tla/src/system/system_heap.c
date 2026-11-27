/* @file */
/* The heap: its two bump allocators and heap slots. */
/* Part of system.c. */

/* Empties the heap: clears the heap slots (both pool pointers and every slot)
 */
/* and sets the pools' next addresses to IwramHeap and EwramHeap. */
void syHeapInit(void) {
	HEAP_SLOTS *state = Ram_HeapSlots;

	btDmaFill32(0, state, sizeof(HEAP_SLOTS));
	Ram_HeapSlots->nextIwram = IwramHeap;
	Ram_HeapSlots->nextEwram = EwramHeap;
}

/* Returns the IWRAM bytes left to allocate, up to the sound mixer's */
/* workspace (IwramSoundMixWorkspace). */
s32 syHeapGetRemainingIwram(void) {
	HEAP_SLOTS *state = Ram_HeapSlots;

	return (s32)IwramSoundMixWorkspace - (s32)state->nextIwram;
}

/* Returns the EWRAM bytes left to allocate, up to the end of EWRAM. */
/* Nothing calls it or its stub. */
s32 syHeapGetRemainingEwram(void) {
	HEAP_SLOTS *state = Ram_HeapSlots;

	return EWRAM_END - (s32)state->nextEwram;
}

/* Returns heap slot `slot`'s block, first allocating `size` bytes for it */
/* (rounded up to words), from IWRAM when they fit and EWRAM otherwise. */
void *syHeapSlotAlloc(s32 slot, s32 size) {
	HEAP_SLOTS *state;
	s32 alignedSize;
	u32 address;
	u32 nextAddress;
	u32 next;
	u32 block;

	state = Ram_HeapSlots;
	block = (u32)HEAP_SLOT_AT(state, slot);
	if(block == 0) {
		block = (u32)state->nextIwram;
		alignedSize = (((u32)size + 3) >> 2) * 4;
		next = block + alignedSize;
		/* A block ending exactly at the pool's end is refused too. */
		if(next >= (u32)Ram_IwramHeapEnd) {
			address = (u32)state->nextEwram;
			nextAddress = address + alignedSize;
			if(nextAddress >= (u32)EWRAM_END) {
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

/* As syHeapSlotAlloc, but tries EWRAM first, for blocks that need */
/* room more than speed: the battle, shop and save state, among others. */
void *syHeapSlotAllocEwram(s32 slot, s32 size) {
	HEAP_SLOTS *state;
	u32 alignedSize;
	u32 next;
	u32 address;
	u32 nextAddress;
	u32 block;

	state = Ram_HeapSlots;
	block = (u32)HEAP_SLOT_AT(state, slot);
	if(block == 0) {
		alignedSize = (u32)size + 3;
		address = (u32)state->nextEwram;
		alignedSize = (alignedSize >> 2) * 4;
		next = address + alignedSize;
		if(next >= (u32)EWRAM_END) {
			address = (u32)state->nextIwram;
			nextAddress = address + alignedSize;
			if(nextAddress > (u32)(Ram_IwramHeapEnd - 1)) {
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

/* Fills the free space of both heaps, from each pool's next address to its */
/* end, with `value`. */
void syHeapFillFreeWords(u32 value) {
	HEAP_SLOTS *state = Ram_HeapSlots;
	u32 address;

	address = (u32)state->nextIwram;
	syWordsFillEntry((void *)address, (u32)Ram_IwramHeapEnd - address, value);
	address = (u32)state->nextEwram;
	syWordsFillEntry((void *)address, (u32)EWRAM_END - address, value);
}

/* Allocates `size` bytes (rounded up to words) with no heap slot, from IWRAM */
/* when they fit and EWRAM otherwise, and advances that pool's next address. */
void *syHeapAlloc(s32 size) {
	HEAP_SLOTS *state = Ram_HeapSlots;
	u32 nextAddress;
	u32 next;
	u32 address;
	u32 words;
	u32 limit;

	words = (u32)size + 3;
	address = (u32)state->nextIwram;
	words >>= 2;
	limit = (u32)(Ram_IwramHeapEnd - 1);
	size = (s32)(words << 2);
	next = address + (u32)size;
	if(next > limit) {
		address = (u32)state->nextEwram;
		nextAddress = address + (u32)size;
		if(nextAddress >= (u32)EWRAM_END) {
			return NULL;
		}
		state->nextEwram = (void *)nextAddress;
	}
	else {
		state->nextIwram = (void *)next;
	}
	return (void *)address;
}

/* As syHeapAlloc, but tries EWRAM first. Called mostly by the battle engine */
/* and the credits for buffers too large to want IWRAM. */
void *syHeapAllocEwram(s32 size) {
	HEAP_SLOTS *state = Ram_HeapSlots;
	u32 nextAddress;
	u32 next;
	u32 address;
	u32 words;

	words = (u32)size + 3;
	address = (u32)state->nextEwram;
	size = (s32)((words >> 2) << 2);
	next = address + (u32)size;
	if(next >= (u32)EWRAM_END) {
		address = (u32)state->nextIwram;
		nextAddress = address + (u32)size;
		if(nextAddress > (u32)(Ram_IwramHeapEnd - 1)) {
			return NULL;
		}
		state->nextIwram = (void *)nextAddress;
	}
	else {
		state->nextEwram = (void *)next;
	}
	return (void *)address;
}
