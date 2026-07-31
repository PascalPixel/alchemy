/*
 * Resource 3b6 scripted cutscene sequence at 0x02000328.
 *
 * Complete owner: `push {lr}` at 0x02000328 and the matching
 * `pop {r0} / bx r0` interworking return at 0x020003d4.  The return address is
 * popped into r0, so the owner is `void`, and it takes no argument: r0 is
 * written before every call and never read on entry.  A one-word literal pool
 * holds the only non-immediate constant at 0x020003d8.
 *
 * Straight-line: no branches at all between the prologue and the epilogue.
 * Twenty-four `bl` instructions, all placed below in source order; the
 * inventory advertises `calls=23`, an undercount of one.
 *
 * All sites resolve with tools/overlay_call_targets.ts — an overlay `bl`
 * stores `target_offset - 2` — and are named for the main-image address in
 * each veneer entry's trailing word.  Ten of the twenty-four are the delay
 * import 0x0808a010, which is what gives the row its high call count for its
 * size.  The begin/message/act/end quartet is the one banked byte-exact source
 * assets/code/resource_3b6_c_0200073c.c uses under its printed names.
 *
 * This owner is one of the two overlay-internal prologue targets reached from
 * 0x020003dc; it is a leaf of that dispatcher's call graph.
 *
 * Subjects are the fixed ids 31, 32 and 33 rather than a parameter, so this is
 * a scene with a fixed cast.  The repeated `act on subject / delay` blocks are
 * kept as written: each is a separate call site and none is a loop.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_0808a018();  /* begin step */
void Func_0808a170();  /* present message by id */
void Func_0808a010();  /* wait / delay by count */
void Func_0808a128();
void Func_0808a180();  /* act on the subject */
void Func_0808a1e8();
void Func_0808a110();
void Func_0808a138();
void Func_0808a020();  /* end step */

void Func_02000328(void)
{
    Func_0808a018();
    Func_0808a170(0x23a4);
    Func_0808a010(30);

    /* Same import, same first two arguments, differing only in the third.
     * Two call sites, not a loop. */
    Func_0808a128(31, 4, 13);
    Func_0808a128(31, 4, 30);

    Func_0808a180(31, 0);
    Func_0808a010(10);

    /* r1 = 129 << 1 = 0x102.  Argument registers are set r1, r2, r0. */
    Func_0808a1e8(32, 0x102, 50);

    Func_0808a010(10);
    Func_0808a110(32, 3);
    Func_0808a010(30);
    Func_0808a180(32, 0);
    Func_0808a010(10);
    Func_0808a110(33, 4);
    Func_0808a010(20);
    Func_0808a180(33, 0);
    Func_0808a010(10);
    Func_0808a138(31, 2);
    Func_0808a010(20);
    Func_0808a180(31, 0);
    Func_0808a010(10);

    /* Repeats the (32, 3) call made above; a second site, deliberately not
     * folded with the first. */
    Func_0808a110(32, 3);
    Func_0808a010(30);

    Func_0808a020();
}
