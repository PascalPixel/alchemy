#include "types.h"

/*
 * resource_375 owner at 0x020019e8, 68 bytes: code 0x020019e8-0x02001a21,
 * an alignment halfword at 0x02001a22 and the pool words 0x03001ebc /
 * 0x0000087e at 0x02001a24-0x02001a2b.
 *
 * Conventions used here.  An overlay `bl` stores the target's image offset
 * minus two rather than a pc-relative displacement, so the disassembler's
 * printed targets are wrong; every call below was resolved with
 * `tools/overlay_call_targets.ts`.  The band from 0x02001a54 is this overlay's
 * 8-byte-per-entry import veneer table, so calls landing there are named by the
 * trailing word.  resource_375 is linked at 0x02008000.
 *
 * Return type: `pop {r0} / bx r0` pops the return address into r0, so nothing
 * survives as a result and the owner is void.
 *
 * This is the twin of 0x020019a4: the two bodies are identical except for the
 * variant id (0x087d there, 0x087e here) and the second argument of
 * Func_0808a250 (0 there, 1 here).  Both resolve to exactly the same four
 * imports over five call sites, which is the cheapest proof available that the
 * bit-identical `bl` encodings denote the same callees (HANDOVER §0).
 *
 * Uncertainty: 0x087e and 0x0900 are identifiers passed to the same import
 * Func_080770c8 and are left numeric; the two workspace fields are written but
 * never read here, so only their offsets and widths are asserted.
 */

extern u8 *Data_03001ebc;

/* Old-style declarations: arities are not established and Func_080770c8 is
 * reached twice from this owner with a single argument each time. */
void Func_0808a018();
void Func_080770c8();
void Func_0808a250();
void Func_0808a020();

void Func_020019e8(void)
{
    u8 *workspace;

    Func_0808a018();

    workspace = Data_03001ebc;
    *(u32 *)(workspace + 448) = 512;
    *(u32 *)(workspace + 456) = 64;

    Func_080770c8(0x87e);
    Func_0808a250(12, 1);
    Func_080770c8(0x900);
    Func_0808a020();
}
