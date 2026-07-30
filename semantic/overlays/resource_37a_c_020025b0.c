typedef signed int s32;

/*
 * Resource 37a owner at 0x020025b0 (76 bytes, 0x020025b0-0x020025fb).
 *
 * Complete owner: `push {r5, lr}` prologue at 0x020025b0 and the matching
 * interworking return `pop {r5} / pop {r1} / bx r1` at 0x020025e6.  The popped
 * return address goes to r1, so r0 survives and IS the result (r0 is loaded
 * from r5 immediately before the pop).
 *
 * The sixteen bytes at 0x020025ec-0x020025fb are this owner's literal pool
 * (0x0000080b, 0x0000080c, 0x0000080d, 0x0000080e).  Execution reaches `bx r1`
 * at 0x020025ea and never falls into them; the `lsrs` pairs the disassembler
 * prints there are flag ids, not code.  The pool map was derived by walking the
 * owner's control flow from the prologue: every branch here is a forward
 * `bne.n` over a single `movs r5, #0`, and nothing reaches 0x020025ec.
 *
 * All four call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the disassembler's own `bl`
 * annotations (0x02004f6c/0x02004f78/0x02004f84/0x02004f90) are the usual
 * overlay artefact and are NOT the callees.  Every site resolves to the same
 * import:
 *   0x020025b6 -> veneer 0x020029b4 -> Func_080770c0
 *   0x020025c2 -> veneer 0x020029b4 -> Func_080770c0
 *   0x020025ce -> veneer 0x020029b4 -> Func_080770c0
 *   0x020025da -> veneer 0x020029b4 -> Func_080770c0
 * sites=4, distinct_targets=1, matching the inventory's calls=4.
 *
 * Func_080770c0 is the global story-flag test (the same import that
 * semantic/overlays/resource_3cb_c_020010e8.c resolves and uses as a test,
 * paired there with Func_080770c8 = set and Func_080770d0 = clear).
 *
 * Behaviour: the owner reports whether all four of the flags 0x080b..0x080e are
 * set.  r5 is preloaded with 1 and cleared by any flag that tests zero; the
 * tests are not short-circuited, so all four calls always happen.
 *
 * Uncertainties: the result is used as a boolean by the callers, so the exact
 * width Func_080770c0 returns is not pinned here beyond "zero / non-zero".
 */

/* Old-style declaration: this import's arity varies across the overlay.
   Declared s32 because the result is tested. */
s32 Func_080770c0();

s32 Func_020025b0(void)
{
    s32 all_set = 1;

    if (Func_080770c0(0x80b) == 0)
        all_set = 0;
    if (Func_080770c0(0x80c) == 0)
        all_set = 0;
    if (Func_080770c0(0x80d) == 0)
        all_set = 0;
    if (Func_080770c0(0x80e) == 0)
        all_set = 0;

    return all_set;
}
