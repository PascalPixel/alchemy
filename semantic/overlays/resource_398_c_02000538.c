typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 398 map-variant 0x2f setup at 0x02000538.
 *
 * Complete owner: `push {r5, lr}` at 0x02000538 and the matching
 * `add sp,#8 / pop {r5} / pop {r0} / bx r0` at 0x020007a2.  652-byte row: 626
 * bytes of code, an alignment halfword at 0x020007aa, and six pool words
 * (0x03001ebc, 0x00000883, 0xfffc0000, 0x00000302, 0x00000303, 0x023a0000)
 * filling 0x020007ac-0x020007c3.  The pool map is from a control-flow walk:
 * the body's branch targets are 0x020006e8, 0x02000704, 0x02000746,
 * 0x02000774, 0x0200078a and 0x020007a2, all below the pool, and the next
 * owner's prologue is at 0x020007c4.  The return address is popped into r0, so
 * the owner is `void`.
 *
 * Reached only from the root 0x0200046c, the entry-0 selector, when the
 * Data_02000240[224] variant reads 0x2f.  It is the largest of the three
 * variants and the only one that populates the full cast.
 *
 * Workspace.  0x03001ebc is a pointer CELL (`ldr r3,[pc] / ldr r3,[r3]`), as
 * the tracked byte-exact assets/code/resource_398_c_020007c4.c spells it.  The
 * opening store is the additive displacement/value pair: r2 is 448 while the
 * address is formed and only then becomes 516, so it writes 516 at
 * workspace + 448 — the same opening 0x020004e8 has.
 *
 * Func_02000904 is NOT an indirect call.  tools/overlay_call_targets.ts
 * classifies its fifteen sites as `call_via` because the two-byte body at
 * 0x02000904 is a bare `bx lr`, which collides with the thunk-bank shape; but
 * no site here loads r3 or r4, so it is a genuine standalone no-op leaf taking
 * one argument.  It is declared, not defined: the leaf has no prologue and is
 * not an inventory row.
 *
 * The nine `Func_080091e0(Func_0808a080(id), 0)` pairs and the fifteen
 * Func_0808a100 / Func_02000904 calls are written out straight-line because
 * that is what the assembly is.  Folding them into loops would deflate the
 * per-target multiset by exactly the number of iterations.
 *
 * Seventy-two call sites against the row's advertised 69; the manifest's
 * `calls` field is a floor.  Per-target, assembly = C: Func_02000904 15,
 * Func_080091c0 5, Func_080091e0 10, Func_080770c0 4, Func_0808a080 13,
 * Func_0808a0f0 3, Func_0808a100 21, Func_0808a1e0 1.
 */

extern u8 *Data_03001ebc;

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_02000904();  /* overlay-local no-op hook (slot) */
void Func_080091c0();  /* place a fixture: (x, z, w, h, sx, sz) */
void Func_080091e0();  /* attach an actor record to its display slot */
s32 Func_080770c0();   /* read event flag */
u8 *Func_0808a080();   /* actor record by slot id */
void Func_0808a0f0();  /* run script (id, a, b) */
void Func_0808a100();  /* set actor visibility/state (slot, state) */
void Func_0808a1e0();  /* set actor facing (slot, facing) */

void Func_02000538(void)
{
    u8 *actor;

    *(s32 *)(Data_03001ebc + 448) = 516;

    Func_080091e0(Func_0808a080(18), 0);
    Func_080091e0(Func_0808a080(19), 0);
    Func_080091e0(Func_0808a080(20), 0);
    Func_080091e0(Func_0808a080(21), 0);
    Func_080091e0(Func_0808a080(22), 0);
    Func_080091e0(Func_0808a080(23), 0);
    Func_080091e0(Func_0808a080(24), 0);
    Func_080091e0(Func_0808a080(25), 0);
    Func_080091e0(Func_0808a080(26), 0);

    Func_0808a100(18, 5);
    Func_0808a100(19, 5);
    Func_0808a100(20, 5);
    Func_0808a100(21, 5);
    Func_0808a100(22, 5);
    Func_0808a100(23, 3);
    Func_0808a100(24, 3);
    Func_0808a100(25, 3);
    Func_0808a100(26, 3);
    Func_0808a100(9, 2);
    Func_0808a100(10, 2);
    Func_0808a100(11, 2);
    Func_0808a100(12, 2);
    Func_0808a100(13, 2);
    Func_0808a100(14, 2);

    Func_02000904(18);
    Func_02000904(19);
    Func_02000904(20);
    Func_02000904(21);
    Func_02000904(22);
    Func_02000904(23);
    Func_02000904(24);
    Func_02000904(25);
    Func_02000904(26);
    Func_02000904(9);
    Func_02000904(10);
    Func_02000904(11);
    Func_02000904(12);
    Func_02000904(13);
    Func_02000904(14);

    if (Func_080770c0(0x883) != 0) {
        Func_0808a0f0(8, 0, 0);
        Func_0808a100(15, 5);

        actor = Func_0808a080(15);
        actor[0x55] = 0;

        actor = Func_0808a080(15);
        *(s32 *)(actor + 12) = (s32)0xfffc0000;

        actor = Func_0808a080(15);
        actor[0x23] |= 2;

        Func_0808a1e0(15, 2);
        Func_080091c0(0, 0, 1, 1, 18, 14);
    } else {
        Func_0808a100(8, 2);
        Func_080091e0(Func_0808a080(8), 0);
        Func_0808a100(15, 1);
    }

    Func_0808a100(16, 1);

    if (Func_080770c0(0x302) != 0) {
        Func_0808a100(17, 1);
        Func_080091c0(0, 1, 1, 1, 36, 22);
        Func_080091c0(0, 2, 1, 1, 36, 24);
    } else {
        Func_0808a100(17, 5);
        Func_080091c0(1, 1, 1, 1, 36, 22);
        Func_080091c0(1, 2, 1, 1, 36, 24);
    }

    if (Func_080770c0(0x303) != 0) {
        Func_0808a0f0(11, 0x023a0000, 188 << 17);
    }
    if (Func_080770c0(0x304) != 0) {
        Func_0808a0f0(12, 0x023a0000, 188 << 17);
    }
}
