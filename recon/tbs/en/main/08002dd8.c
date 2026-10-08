/*
 * Runtime_ReleaseHeapBlock / MemFree: English Thumb C draft, not linked.
 * Re-derived from the current listing and our ROM on 2026-10-09.
 * Approved agscc and K1's common Thumb options, measured with:
 *   alchemy diff recon/tbs/en/main/08002dd8.c heapfree
 *                --game tbs --edition tbs-en
 * Native: 17 instructions; draft: 20; common: 3; residual: 31.
 * The first body gains push/pop and a cmp; both bodies differ in allocation
 * and scheduling. The native placed run is 40 bytes, including a two-byte
 * linker alignment gap and one shared gWorkSlot literal. This C emits 48 bytes and separate literal
 * words for the two functions. The DE/ES ARM tail is not part of this draft.
 * No register pins, barriers, asm or alternative compiler options.
 * Trials: a u32-slots view adds an address instruction (21 instructions,
 * residual 32); the ordinary void-pointer view retained here is smaller.
 */

#include "types.h"
#include "mem.h"
#include "heap_state.h"

void Runtime_ReleaseHeapBlock(s32 slot)
{
	void *buffer;
	u32 pool;

	buffer = gWorkSlot[slot];
	pool = (u32)buffer >> 22;
	if (pool != 0)
	{
		gWorkSlot[slot] = NULL;
		*(void **)((u8 *)gWorkSlot + (pool & 4)) = buffer;
	}
}

void MemFree(void *buffer)
{
	*(void **)((u8 *)gWorkSlot + (((u32)buffer >> 22) & 4)) = buffer;
}
