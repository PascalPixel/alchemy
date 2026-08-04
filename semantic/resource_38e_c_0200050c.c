typedef signed int s32;

/*
 * resource_38e owner at 0x0200050c, 208 bytes: the scene-0x22 body - decide
 * from six event flags which cameras and which entities this visit gets.
 *
 * Reached only from the overlay's entry-0 root
 * (semantic/overlays/resource_38e_c_020004bc.c, call site 0x020004f2), which is
 * why it was taken second in a root-down order.
 *
 * Complete owner: `push {lr}` at 0x0200050c through the single epilogue at
 * 0x020005b8, ending `pop {r0} / bx r0` - void.
 *
 * Pool map from a control-flow walk: 0x0200050c-0x020005bb is reached as
 * instructions, 0x020005bc-0x020005db is not (eight words: 0x00000109,
 * 0x00000fd2, 0x0000084a, 0x01070000, 0x0000084f, 0x00000845, 0x02008bf4,
 * 0x0000085e).  176 code + 32 pool = 208, exactly the inventory row's figures.
 *
 * Calls: 17 sites, matching the advertised 17.  Per-target multiset from
 * `bun tools/overlay_call_targets.ts resource_38e 50c`:
 *   Func_080770c0  x7   0x510 0x522 0x532 0x554 0x55e 0x57e 0x5ac
 *   Func_0808a0f0  x4   0x542 0x54e 0x56c 0x590
 *   Func_0808a1b8  x2   0x59c 0x5a6
 *   Func_080770d0  x1   0x51c
 *   Func_0808a168  x1   0x578
 *   Func_02000974  x1   0x52c   (this overlay, prologue site)
 *   Func_020005dc  x1   0x5b4   (this overlay, prologue site)
 *
 * Flag 0x845 is queried at TWO separate sites, 0x0200055e and 0x0200057e, and
 * they are kept as two.  Caching it in a local would read as tidier C and would
 * deflate the Func_080770c0 multiset from 7 to 6 - exactly the normalisation
 * the handover warns against.  The two queries are also used with opposite
 * polarity, so the flag can legitimately be set between them by nothing at all
 * and still take different branches only because the tests differ.
 *
 * The two nested tests inside the 0x84a block are written with `&&`.  That is
 * faithful rather than convenient: the assembly branches to the common
 * continuation on the first non-zero result, so the second query really is
 * conditional on the first, which is what short-circuit evaluation means.
 *
 * 0x02008bf4 is even, so under the proven 0x02008000 link base it is in-image
 * DATA at file offset 0x0bf4, not a function pointer - the odd/even split is
 * the whole test.
 *
 * The camera arguments are 16.16: 0x01340000 = 308.0, 0x015a0000 = 346.0,
 * 0x01070000 = 263.0, 0x00e00000 = 224.0, 0x01240000 = 292.0.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
s32 Func_080770c0();                 /* read an event flag */
void Func_080770d0();                /* clear an event flag */
void Func_0808a0f0();                /* place/aim the camera at a point */
void Func_0808a168();                /* three-argument presentation call */
void Func_0808a1b8();                /* three-argument presentation call */

void Func_02000974(s32 selector);    /* this overlay, 0x02000974 */
void Func_020005dc(void);            /* this overlay, 0x020005dc */

void Func_0200050c(void)
{
    if (Func_080770c0(0x109) != 0) {
        Func_080770d0(512);                             /* 128 << 2 */
    }

    if (Func_080770c0(0xfd2) == 0) {
        Func_02000974(13);
    }

    if (Func_080770c0(0x84a) != 0) {
        Func_0808a0f0(11, 0x01340000, 0x01070000);      /* 154 << 17 */
        Func_0808a0f0(12, 0x015a0000, 0x01070000);      /* 173 << 17 */

        if (Func_080770c0(0x84f) == 0 && Func_080770c0(0x845) == 0) {
            Func_0808a0f0(11, 0, 0);
            /* 0x02008bf4 = in-image data at file offset 0x0bf4. */
            Func_0808a168(12, 0x00010000, (void *)0x02008bf4);
        }
    }

    /* A second, independent query of flag 0x845. */
    if (Func_080770c0(0x845) != 0) {
        Func_0808a0f0(10, 0x00e00000, 0x01240000);      /* 224 << 16, 146 << 17 */
        Func_0808a1b8(10, 0x00004000, 0);               /* 128 << 7 */
        Func_0808a1b8(8, 0, 0);

        if (Func_080770c0(0x85e) == 0) {
            Func_020005dc();
        }
    }
}
