#include "types.h"

/*
 * resource_3a2 owner at 0x020010b8, 140 bytes.  Complete owner:
 * `push {r5, r6, lr}` / `sub sp,#8` prologue at 0x020010b8 and the interworking
 * return `add sp,#8 / pop {r5, r6} / pop {r0} / bx r0` at 0x02001136.  r0 holds
 * the popped return address, so the owner returns nothing; it takes no
 * arguments.  Straight-line body, no branches.
 *
 * A one-word literal pool sits at 0x02001140 (0x0200113e is the alignment
 * halfword after the `bx`), inside the row's 140-byte span but never reached by
 * a control-flow walk from the prologue:
 *   0x02001140 = 0x02009061
 *
 * LINK-BASE WITNESS: 0x02009061 is ODD, so by the parity rule it is a Thumb
 * function entry, and 0x02009061 - 0x8000 - 1 = 0x1060, which is exactly
 * Func_02001060 -- a function this overlay already has byte-exact in
 * assets/code/resource_3a2_c_02001060.c.  That confirms the 0x02008000 link
 * base for resource_3a2 against tracked material, and identifies the pool word
 * as an installed per-entity callback rather than data.  It is stored into the
 * entity record at +0x6c.
 *
 * The byte-exact assets/code/resource_3a2_c_02001060.c gives that callback's
 * own record layout: a `rank` s32 at +12 and a flags byte at +31 whose bit 1 it
 * sets or clears against the rank of Func_02002570(0) -- the printed name for
 * the same slot-record accessor this owner reaches as Func_0808a080.  So the
 * bit-1 clears below (`& 0xfd`) are the same flag that callback later drives.
 *
 * Calls resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a2 10b8
 * --json` (eleven sites, seven distinct veneer targets; the inventory's
 * `calls=11` counts sites here):
 *   0x10bc -> veneer 0x14f0 -> Func_0808a018
 *   0x10c2 -> veneer 0x1508 -> Func_0808a080
 *   0x10d4 -> veneer 0x1508 -> Func_0808a080
 *   0x10e2 -> veneer 0x1508 -> Func_0808a080
 *   0x10ee -> veneer 0x1508 -> Func_0808a080
 *   0x1106 -> veneer 0x14b8 -> Func_080091c0
 *   0x1118 -> veneer 0x14b8 -> Func_080091c0
 *   0x111e -> veneer 0x1498 -> Func_080000c0
 *   0x1126 -> veneer 0x14d8 -> Func_080770c8
 *   0x112e -> veneer 0x15b0 -> Func_0808a1e0
 *   0x1132 -> veneer 0x14f8 -> Func_0808a020
 *
 * Behaviour: prepare entity 14 -- clear bit 1 of its bytes at +35 and +89,
 * clear the byte at +85, install the Func_02001060 callback at +0x6c -- then
 * repaint two tile spans through the six-argument Func_080091c0, run one screen
 * step (Func_080000c0(1)), post cue 512 (built as `movs r0,#128 / lsls r0,#2`)
 * and hand entity 14 to Func_0808a1e0 with mode 2.
 *
 * Uncertainty: r5 holds the mask 253 across both bit clears and r6 holds 0 for
 * the +85 store; nothing else survives.  No register is set before
 * Func_0808a018, so it takes no asserted arguments.  Old-style declarations
 * keep every import's interface open.
 */

     /* installed callback, byte-exact in assets/code */









extern void Func_020025ae();
extern u8 * Func_020025cc();
extern u8 * Func_020025de();
extern u8 * Func_020025ec();
extern u8 * Func_020025f8();
extern void Func_020025c0();
extern void Func_020025d2();
extern void Func_020025b8();
extern void Func_02002600();
extern void Func_020026e0();
extern void Func_0200262c();
void Func_020010b8(void)
{
    u8 zero;

    zero = 0;
    Func_020025ae();

    Func_020025cc(14)[35] &= 0xfd;
    Func_020025de(14)[89] &= 0xfd;
    Func_020025ec(14)[85] = zero;
    *(void **)(Func_020025f8(14) + 108) = (void *)0x02009061;

    Func_020025c0(55, 16, 1, 1, 56, 18);
    Func_020025d2(55, 16, 1, 1, 20, 18);

    Func_020025b8(1);
    Func_02002600(512);
    Func_020026e0(14, 2);
    Func_0200262c();
}
