/*
 * Runtime_ReleaseHeapBlock / MemFree: English Thumb C draft, not linked.
 * Re-derived from the current listing and our ROM on 2026-10-09.
 * Approved agscc and K1's common Thumb options, measured with:
 *   alchemy diff recon/tbs/en/main/08002dd8.c heapfree --show
 * Native: 17 instructions; draft: 20; common: 4; residual: 29.
 * MemFree still has the native six instructions, but its table is r2 and
 * its pool is r3 rather than r4/r2, and the literal load is scheduled late.
 * The native placed run is 40 bytes: release 22, alignment 2, free 12,
 * and one final gWorkSlot literal 4. The draft emits 48 bytes: release
 * 28 with its own literal 4, then free 12 with another literal 4.
 * The release body also adds push/pop and a cmp. Approved agscc's Thumb
 * conditional branch patterns always emit cmp/cmn (arm.md cbranchsi4 and
 * negated_cbranchsi4), unlike the native lsrs/beq. Its arm_reorg processes
 * a function's literal pool separately, unlike the native shared word.
 * The DE/ES ARM tail is a different function and stays in its listing.
 * No register pins, barriers, asm or alternative compiler options.
 * Trials: pointer/u16 view, declaration orders, return-buffer and plain
 * register hints: residual 31, 48 bytes; 64-bit value shifted before its
 * mask: 28 instructions/residual 41; table+4 with offset-4: 22/residual 33.
 * Named signed mask/pool (permutation search): 20/residual 29, retained.
 * Widened mask/pool and final address casts (144 distinct PERM choices,
 * seed 137, two jobs, one minute): no improvement over residual 29.
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
	/* FAKEMATCH: Signed mask/pool temporaries retain native movs r1, #4.
	   The pointer expression or u16 pool allocates the mask to r2; over
	   the two functions the measured residual improves from 31 to 29. */
	s32 mask = 4;
	s32 pool = (u32)buffer >> 22;
	void **table = gWorkSlot;
	*(void **)((u8 *)table + (pool & mask)) = buffer;
}
