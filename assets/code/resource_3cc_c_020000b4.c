#include "resource_3cc.h"

typedef signed int s32;

/*
 * resource_3cc owner at 0x020000b4, 4 bytes: `movs r0, #0 / bx lr`.
 *
 * THIS IS THE OVERLAY'S ENTRY DRIVER, not an ordinary leaf. The word at
 * image offset 4 -- the one the opening `ldr r4,[pc,#0] / bx r4` veneer
 * jumps through -- is 0x020080b5, which is this address plus the Thumb bit
 * under the base + 0x8000 spelling. The loader calls it; nothing in the
 * image does.
 *
 * It is the one driver in all 96 that does not open with a `push`, and this
 * is why: there is no body to save registers for. resource_3cc's entry
 * returns 0 and does nothing else. Every other overlay's driver sequences
 * its scene; this one declines to.
 *
 * CORRECTION TO AN EARLIER COUNT. All 96 entry drivers were reported as
 * drafted because `overlay_driver.ts --all --unowned` reported 0, but its
 * `ownerOf` accepts "inventory row" -- merely appearing in
 * the census -- as ownership, so an undrafted driver is not UNOWNED to it.
 * Two drivers are in that state, this one and resource_373's 0x02002a54.
 * The blind spot was in what the tool ACCEPTS, not in what it scans, which
 * is the same fault already identified in sweep B and covered by sweep D's tail.
 *
 * Reached through leaf-residue analysis rather than as a driver: sweep B saw
 * the header word as a publication and discarded the target for opening
 * with no `push`. The two populations meet here.
 *
 * Complete owner: both instructions. No prologue, no stack frame, no
 * literal pool, no callees, no argument read.
 */

s32 Func_020000b4(void)
{
    return 0;
}
