typedef signed int s32;

/*
 * Resource 398 map-variant 0x31 setup at 0x020004b4.
 *
 * Complete owner: `push {lr}` / `sub sp,#8` at 0x020004b4 and the matching
 * `add sp,#8 / pop {r0} / bx r0` at 0x020004de.  52-byte row: 48 bytes of code
 * plus the single pool word 0x00000305 at 0x020004e4.  Control-flow walk: the
 * only branch is the `beq.n 0x020004de` guard, so nothing reaches 0x020004e4.
 * The return address is popped into r0, so the owner is `void`.
 *
 * Reached only from the root 0x0200046c, the entry-0 selector, when the
 * Data_02000240[224] variant reads 0x31.
 *
 * Cross-check.  The banked byte-exact assets/code/resource_398_c_02000424.c
 * places the same fixture with the same six arguments (31, 0, 1, 1, 8, 13) and
 * then *sets* flag 0x305; this owner *reads* flag 0x305 and repeats the
 * placement.  Resolving that file's own `bl` sites through the `+2` rule gives
 * veneer offsets 0x930 and 0x950, i.e. Func_080091c0 and Func_080770c8 — the
 * banked source names both imports for this one, backwards.
 *
 * Three call sites, matching the row's advertised count.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
s32 Func_080770c0();   /* read event flag */
void Func_080091c0();  /* place a fixture: (x, z, w, h, sx, sz) */
void Func_0808a100();  /* set actor visibility/state (slot, state) */

void Func_020004b4(void)
{
    if (Func_080770c0(0x305) != 0) {
        Func_080091c0(31, 0, 1, 1, 8, 13);
        Func_0808a100(8, 0);
    }
}
