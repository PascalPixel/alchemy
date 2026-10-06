/* ARM compiler study for Resource_PatchThumbBranch, the not-yet-C listing
 * recon/tbs/raw/08002d5c.S.
 * Native: 124 bytes, 31 ARM instructions, no literal pool.
 * Fixed stock agscc control: normal TBS options with -mthumb replaced by -marm;
 * listing 31, draft 30, common 1, differ 59. No assembler diagnostics.
 * Fixed agbcc_arm control: -O2 -mthumb-interwork -fomit-frame-pointer
 * -fcall-used-r4; listing 31, draft 31, common 3, differ 56.
 * Scores use alchemy instruction normalization and the existing assembly
 * object as the reference. No production routing or credit changes.
 *
 * Native keeps r4/r5 marker constants for shifted comparisons, splits the
 * end-pointer and displacement arithmetic, and branches to the loop test.
 * Both controls combine arithmetic and rotate the loop. Stock agscc also
 * predicates the stores; agbcc_arm uses lr and a conditional return.
 * This C study does not establish that the routine was compiled C.
 *
 * Natural trials (stock / agbcc_arm differ): separate scan pointer with
 * payload masks 60/59, bit-clear masks 58/60; u16 inputs 62/64;
 * reversed comparisons and top-of-loop break 58/60; index displacement
 * 59/61. Reusing data as the scan pointer gives the retained 59/56.
 * No FAKEMATCH devices, compiler options, or register bindings were added.
 */
#include "TYPES.H"

void Resource_PatchThumbBranch(u16 *data, u32 count)
{
    u16 *end;
    u16 *base;
    u32 lo;
    u32 hi;
    u32 offset;

    end = data + count;
    base = data + 1;
    data = base;
    while (data < end) {
        lo = *data++;
        if ((lo >> 11) != 31)
            continue;
        hi = data[-2];
        if ((hi >> 11) != 30)
            continue;
        offset = ((lo & ~0xf800) | ((hi & ~0xf800) << 11)) * 2;
        offset -= (u8 *)data - (u8 *)base;
        data[-2] = ((offset >> 12) & ~0xf800) | 0xf000;
        data[-1] = (offset >> 1) | 0xf800;
    }
}
