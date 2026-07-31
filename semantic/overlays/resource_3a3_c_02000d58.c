typedef signed int s32;

/*
 * Resource 3a3 scripted sequence at 0x02000d58 (200 bytes,
 * 0x02000d58 .. 0x02000e1f, of which 0x02000e10 .. 0x02000e1f is the pool).
 * The overlay's import veneer table begins at 0x02000e20, immediately after
 * this owner, so this is the last executable row in the image.
 *
 * Complete owner: `push {lr}` at 0x02000d58 and the interworking return
 * `pop {r0} / bx r0` at 0x02000e0c, so the owner returns nothing.  The body is
 * straight-line -- there is no conditional branch in it at all.
 *
 * All 23 call sites are placed (row reports calls=23): Func_0808a018 x1,
 * Func_0808a208 x1, Func_0808a210 x1, Func_0808a218 x2, Func_0808a010 x3,
 * Func_0808a100 x1, Func_0808a5e0 x1, Func_080000d8 x1, Func_0808a148 x1,
 * Func_0808a1b8 x4, Func_080f9010 x1, Func_0808a138 x1, Func_020007b8 x1,
 * Func_0808a200 x1, Func_0808a110 x1, Func_080770c8 x1, Func_0808a020 x1.
 *
 * LINK-BASE WITNESS.  The pool word at 0x02000e18 is 0x02008d09 -- odd, in the
 * 0x0200_8xxx band -- which under the proven 0x02008000 link base is file
 * offset 0x0d09 = `Func_02000d08 + 1`.  It is the sole argument of
 * Func_080000d8, the task installer, so this sequence installs the
 * Func_02000d08 spawner as a task.  Third independent witness for the base in
 * this overlay, after 0x02008c45 (in Func_02000d08) and 0x020086f1 (in
 * Func_020007b8).
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
void Func_0808a018();       /* opens a scripted sequence */
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a010();       /* waits the given number of frames */
void Func_0808a100();
void Func_0808a5e0();
void Func_080000d8();       /* installs a task callback */
void Func_0808a148();
void Func_0808a1b8();
void Func_080f9010();
void Func_0808a138();
void Func_0808a200();
void Func_0808a110();
void Func_080770c8();       /* sets a flag id */
void Func_0808a020();       /* closes the scripted sequence */

/* Overlay-local: the actor-18 setup sequence at 0x020007b8. */
void Func_020007b8(void);

void Func_02000d58(void)
{
    Func_0808a018();

    Func_0808a208(0x6666, 0x0ccc);
    Func_0808a210(0x003f0000, -1, 0x01c20000, 1);   /* 252<<14, 225<<17 */
    Func_0808a218();
    Func_0808a010(30);

    Func_0808a100(18, 1);
    Func_0808a5e0(-1);

    /* Func_02000d08 + Thumb bit: installs the object spawner as a task. */
    Func_080000d8(0x02008d09);
    Func_0808a010(20);

    Func_0808a148(0, 18, 0);
    Func_0808a1b8(0, 0x4000, 0);                    /* 128<<7 */
    Func_0808a1b8(18, 0, 20);
    Func_0808a1b8(18, 0xd000, 40);                  /* 208<<8 */

    Func_080f9010(147);
    Func_0808a138(18, 2);
    Func_0808a010(20);

    Func_0808a1b8(18, 0xb000, 40);                  /* 176<<8 */

    Func_020007b8();

    Func_0808a200(0, 1);
    Func_0808a218();
    Func_0808a110(14, 4);

    Func_080770c8(0x8ff);
    Func_0808a020();
}
