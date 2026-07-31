typedef signed int s32;

/*
 * Resource 398 room-layout entry at 0x020001bc.
 *
 * Complete owner: `push {lr}` / `sub sp,#8` at 0x020001bc and the matching
 * `add sp,#8 / pop {r0} / bx r0` at 0x020001e8.  50 bytes, no literal pool.
 * The return address is popped into r0, so the owner is `void`.
 *
 * Role.  The 12-byte interaction records at file offset 0x4e0 name this
 * address as a handler through the pool word 0x020081bd (= 0x0200_01bc + the
 * Thumb bit under the proven 0x02008000 link base), keyed 0x40009085 — the
 * sibling key of the 0x50009085 entry that names 0x02000188.
 *
 * PROVEN TWIN of 0x02000188.  The two rows are 50 bytes each, every `bl`
 * halfword is bit-identical, and they differ in exactly four immediates:
 * (95, 21) here against (111, 37) there for the first placement, and (1, 25)
 * against (2, 24) for the second.  That diff is this file's correctness proof.
 *
 * Imports resolved with tools/overlay_call_targets.ts (an overlay `bl` stores
 * `target_offset - 2`); both are eight-byte veneers whose trailing word gives
 * the main-image address.  The six-argument spelling — four register
 * arguments plus two pushed at sp+0 and sp+4 — is the one banked byte-exact
 * source assets/code/resource_398_c_02000148.c already uses for
 * Func_080091c0 under its printed name Func_02000a8e.
 *
 * Two call sites, matching the row's advertised count.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_08009180(); /* place a fixture: (x, z, w, h, sx, sz) */
void Func_080091c0(); /* place a fixture, second bank */

void Func_020001bc(void)
{
    Func_08009180(95, 21, 97, 21, 1, 3);
    Func_080091c0(46, 38, 3, 1, 32, 25);
}
