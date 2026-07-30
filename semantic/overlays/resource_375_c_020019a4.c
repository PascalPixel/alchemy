#include "types.h"

/*
 * resource_375 owner at 0x020019a4, 68 bytes: code 0x020019a4-0x020019dd,
 * an alignment halfword at 0x020019de and the pool words 0x03001ebc /
 * 0x0000087d at 0x020019e0-0x020019e7.
 *
 * Conventions used here.  An overlay `bl` stores the target's image offset
 * minus two rather than a pc-relative displacement, so the disassembler's
 * printed targets are wrong; every call below was resolved with
 * `tools/overlay_call_targets.ts`.  The band from 0x02001a54 is this overlay's
 * 8-byte-per-entry import veneer table (`ldr r4,[pc,#0] / bx r4 /
 * .word <main-image address>`), so calls landing there are named by the
 * trailing word.  resource_375 is linked at 0x02008000 (witness: the word
 * 0x02008171 in the table at file offset 0x1efc is this overlay's own
 * Func_02000170 plus the Thumb bit).
 *
 * Return type: `pop {r0} / bx r0` pops the return address into r0, so nothing
 * survives as a result and the owner is void.
 *
 * Behaviour: run the shared entry Func_0808a018, then set two u32 fields of the
 * workspace `Data_03001ebc` points at (+448 = 512 and +456 = 64), select
 * variant 0x087d, run Func_0808a250(12, 0) and Func_080770c8(0x900), and
 * finish with Func_0808a020.
 *
 * 0x020019e8 is this owner's near-identical twin: it differs only in the two
 * values 0x087d -> 0x087e and 0 -> 1, which is the cheapest available proof
 * that both encode the same import set (HANDOVER §0).
 *
 * Uncertainty: 0x087d and 0x0900 are identifiers passed to the same import
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

void Func_020019a4(void)
{
    u8 *workspace;

    Func_0808a018();

    workspace = Data_03001ebc;
    *(u32 *)(workspace + 448) = 512;
    *(u32 *)(workspace + 456) = 64;

    Func_080770c8(0x87d);
    Func_0808a250(12, 0);
    Func_080770c8(0x900);
    Func_0808a020();
}
