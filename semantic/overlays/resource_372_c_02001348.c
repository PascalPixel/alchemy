typedef signed int s32;

/*
 * Resource 372 guarded cutscene step at 0x02001348 (336 bytes, 27 distinct
 * call targets across 28 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x02001348, matching
 * `pop {r5} / pop {r0} / bx r0` at 0x02001458.  `pop {r0} ; bx r0` — r0 is the
 * popped return address, so the owner returns nothing.  0x0200145e-0x02001497
 * is alignment plus the literal pool, reached only by `ldr rN, [pc, #imm]`.
 *
 * Shape: an outer flag guard (0x313) that skips the whole body, an inner
 * one-shot guard (0x833) around the object setup, and then a second guarded
 * section that picks one of two script branches on a signed comparison of the
 * word at +16 of a queried record against 0x0479ffff.
 *
 * Note `Func_0808a080` is reached twice with different argument counts: once
 * with one argument, where its result is the object pointer used below, and
 * once with three.  That is the documented overlay idiom, hence the old-style
 * declarations.
 *
 * `work/claude/notes/resource_372-1348.md` parks this row for the byte-exact
 * lane over constant re-materialisation (the pool words 0x313, 0x833 and 0x30c
 * are each used twice and get CSE'd into registers).  That is a codegen
 * obstacle, not a semantic one.
 *
 * Uncertainties: the small pool constants are event/flag identifiers whose
 * meanings are not established; 0xe666 recurs across this overlay as the third
 * argument of a (-1, -1, k) call and reads like 0.9 in 16.16, but is only
 * asserted here as the literal it is.  The three intra-overlay targets
 * 0x02002882, 0x02002e8e and 0x02002ea4 land inside other owners' bodies and
 * are treated, per this overlay's established convention, as import identities
 * rather than as locations.
 */

/* Imports, old-style: interfaces unknown; 0x02005a94 takes one argument at one
 * site and three at another. */
s32 Func_080770c0();
void Func_0808a018();
s32 *Func_0808a080();
s32 *Func_080091f0();

void Func_080f9010();
void Func_080000c0();
void Func_0808a0f0();
void Func_0808a0d0();
void Func_0808a010();
void Func_080091f8();
void Func_0808a5e8();
void Func_080770c8();
void Func_02001498();
void Func_02001a64();
void Func_0808a020();

void Func_02001348(void)
{
    s32 *obj;
    s32 *rec;

    if (Func_080770c0(0x313) != 0) {
        return;
    }

    Func_0808a018();

    if (Func_080770c0(0x833) == 0) {
        obj = Func_0808a080(14);
        Func_080091f0(0x40000, 0x40000, 0x10000);  /* 128<<11, 128<<11, 128<<9 */
        Func_080f9010(141);
        Func_080000c0(40);
        Func_080f9010(145);
        Func_0808a0f0(14, 0x1da0000, 0x47b0000);   /* 237<<17 */

        obj[12] = 0x10000;                         /* +48, 128<<9 */
        obj[13] = 0x10000;                         /* +52 */
        obj[3] += 0x480000;                        /* +12, 144<<15 */
        obj[15] = obj[3];                          /* +60 gets the new value */
        obj[17] = 0x8000;                          /* +68, 128<<8 */

        Func_0808a0d0(14, 0x1b0, 0x47b);           /* 216<<1 */
        Func_0808a010(40);
        Func_080f9010(0x121);
        Func_080091f0(-1, -1, 0xe666);             /* three arguments here */
        Func_080091f8();
        Func_0808a5e8();
        Func_080770c8(0x833);
    }

    Func_02001498();
    Func_080770c8(0x313);

    if (Func_080770c0(0x837) != 0 && Func_080770c0(0x841) == 0 &&
        Func_080770c0(0x30c) == 0) {               /* 195<<2 */
        rec = Func_0808a080(0);
        if (rec[4] > 0x0479ffff) {                 /* signed word at +16 */
            Func_02001a64(0x1bd, 0x494);
            Func_0808a0d0(0, 0x1bf, 0x4cb);
        } else {
            Func_02001a64(0x19c, 0x460);           /* 206<<1, 140<<3 */
            Func_0808a0d0(0, 0x19e, 0x42c);        /* 207<<1 */
        }
        Func_080770c8(0x30c);
    }

    Func_0808a020();
}
