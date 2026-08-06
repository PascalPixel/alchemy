#include "types.h"

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
       /* opens a scripted sequence */



       /* waits the given number of frames */


       /* installs a task callback */






       /* sets a flag id */
       /* closes the scripted sequence */

/* Overlay-local: the actor-18 setup sequence at 0x020007b8. */


extern void Func_02001c04();
extern void Func_02001cd4();
extern void Func_02001cee();
extern void Func_02001cfa();
extern void Func_02001c20();
extern void Func_02001c88();
extern void Func_02001d38();
extern void Func_02001bc6();
extern void Func_02001c3c();
extern void Func_02001cbe();
extern void Func_02001d0a();
extern void Func_02001d14();
extern void Func_02001d20();
extern void Func_02001d8e();
extern void Func_02001ce6();
extern void Func_02001c7c();
extern void Func_02001d40();
extern void Func_020015a4(void);
extern void Func_02001d5c();
extern void Func_02001d78();
extern void Func_02001d08();
extern void Func_02001c96();
extern void Func_02001cba();
void Func_02000d58(void)
{
    Func_02001c04();

    Func_02001cd4(0x6666, 0x0ccc);
    Func_02001cee(0x003f0000, -1, 0x01c20000, 1);   /* 252<<14, 225<<17 */
    Func_02001cfa();
    Func_02001c20(30);

    Func_02001c88(18, 1);
    Func_02001d38(-1);

    /* Func_02000d08 + Thumb bit: installs the object spawner as a task. */
    Func_02001bc6(0x02008d09);
    Func_02001c3c(20);

    Func_02001cbe(0, 18, 0);
    Func_02001d0a(0, 0x4000, 0);                    /* 128<<7 */
    Func_02001d14(18, 0, 20);
    Func_02001d20(18, 0xd000, 40);                  /* 208<<8 */

    Func_02001d8e(147);
    Func_02001ce6(18, 2);
    Func_02001c7c(20);

    Func_02001d40(18, 0xb000, 40);                  /* 176<<8 */

    Func_020015a4();

    Func_02001d5c(0, 1);
    Func_02001d78();
    Func_02001d08(14, 4);

    Func_02001c96(0x8ff);
    Func_02001cba();
}
