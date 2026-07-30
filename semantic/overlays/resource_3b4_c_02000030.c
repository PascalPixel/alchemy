/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `tools/overlay_call_targets.ts`.  The band
 * 0x02002468-0x0200261f is an 8-byte-per-entry import veneer table
 * (`ldr r4,[pc,#0] / bx r4 / .word <main-image address>`); calls landing there
 * are named by that trailing word, as the semantic resource_39f sources do.
 * Calls landing on an overlay prologue keep the overlay's `Func_0200xxxx` name.
 *
 * Link base: resource_3b4 is linked at 0x02008000.  The image is 0x2f84 bytes
 * and the table at 0x02002d40-0x02002f84 holds 59 words of the form
 * `<function offset> + 0x8000 + 1` (Thumb bit), e.g. 0x02008ec9 = Func_02000ec8.
 * So a pool word in 0x02008000-0x0200af83 is in-image data at value - 0x8000.
 * `Data_02000240` lies below the base and is referenced by byte-exact sources
 * in many other overlays, so it is a shared EWRAM global, not overlay data.
 *
 * Return type: `pop {r0} / bx r0` pops the return address into r0, so nothing
 * is returned and the owner is void.
 *
 * Declarations are old-style because imports here are reached with differing
 * argument counts from different call sites.
 */
#include "types.h"

/*
 * resource_3b4 owner at 0x02000030, 60 bytes: code 0x02000030-0x02000067 and
 * the pool word 0x030001d8 at 0x02000068.
 *
 * Euclidean distance between two three-component 16.16 coordinate triples.
 * Each component difference is taken at full precision and then shifted right
 * by 16 (`asrs #16`), so the squares are computed on whole units and cannot
 * overflow; the sum of the three squares is passed in r0 to the routine whose
 * address the pool word holds.
 *
 * That call is `ldr r3,[pc,#8] / bl 0x02002614`, and 0x02002614 is `bx r3` --
 * the overlay's own `call_via r3` slot, not a function.  So this is an
 * indirect call through r3, whose value 0x030001d8 is the square-root helper
 * relocated into IWRAM (HANDOVER records the same literal in that role for
 * other overlays).  It is declared here as a function pointer constant rather
 * than as an opaque argument, which is the faithful spelling.
 *
 * The epilogue is `pop {r5} / pop {r1} / bx r1`: the return address is popped
 * into r1, not r0, so r0 survives the return and the helper's result IS this
 * owner's result.
 *
 * Note: r4 is used as scratch although the prologue only saves r5 and lr.
 * Nothing observable depends on it -- the value dies before the call -- and it
 * matches the `-fcall-used-r4` convention this project builds with, so it is
 * recorded rather than "fixed".
 */

/* The square-root helper relocated into IWRAM at 0x030001d8, reached through
 * the overlay's `call_via r3` slot at 0x02002614. */
#define IwramSquareRoot ((s32 (*)(s32))0x030001d8)

s32 Func_02000030(const s32 *a, const s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dy = (a[1] - b[1]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;

    return IwramSquareRoot(dx * dx + dy * dy + dz * dz);
}
