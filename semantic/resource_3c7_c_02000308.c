typedef signed int s32;
typedef unsigned char u8;
typedef unsigned short u16;

/*
 * Resource 3c7 cutscene step at 0x02000308.
 *
 * Complete owner: `push {r5, lr}` at 0x02000308 through the interworking
 * return `pop {r5} / pop {r0} / bx r0` at 0x020003ea.  r0 receives the popped
 * return address, so the owner returns nothing (HANDOVER §0).  The entry value
 * of r0 is overwritten by `movs r0, #0` at 0x0200030a before any use, so the
 * owner takes no arguments.
 *
 * Two interior literal pools, derived from a control-flow walk from the
 * prologue: 0x02000358-0x0200035f (the 0xffffc000 mask and the message id
 * 0x26ec), hopped by the unconditional `b.n` at 0x02000356, and
 * 0x020003f0-0x020003f3 past the epilogue.  Neither is decoded as code.  Note
 * that the `b.n` here is a *lone* forward branch with no conditional structure
 * around it, and that the quadrant value in r5 is set BEFORE the pool and read
 * long after it — register state is carried across the pool, per HANDOVER §0.
 *
 * Link base 0x02008000, witnessed here for free: the pool word 0x02009314 is
 * even, so it is in-image data at file offset 0x1314, and it appears as the
 * second argument of Func_0808a098 — exactly the shape HANDOVER §0 records for
 * even in-image words in this family.
 *
 * Call accounting, per target, against `overlay_call_targets.ts`
 * (26 sites, 15 distinct); every count below was checked individually rather
 * than by totalling:
 *   Func_0808a010  8    Func_0808a180  4    Func_0808a110  2
 *   Func_080770c8  1    Func_0808a018  1    Func_0808a020  1
 *   Func_0808a080  1    Func_0808a098  1    Func_0808a138  1
 *   Func_0808a148  1    Func_0808a170  1    Func_0808a1b8  1
 *   Func_0808a1e8  1    Func_0808a460  1    Func_0808a580  1
 *
 * Uncertainties, all recorded rather than guessed:
 *  - r0 is not reloaded between 0x02000320, 0x02000324 and 0x02000328, so
 *    Func_080770c8(0x300)'s result feeds Func_0808a018 and its result in turn
 *    feeds Func_0808a460.  Whether either callee reads that register is not
 *    established; the dataflow is preserved as written, as the byte-exact
 *    resource_3b8 sources do for the same shape.
 *  - the repeated first argument 14 is a channel/actor id whose meaning is not
 *    established here.
 */

/* Import veneers.  Old-style declarations: arity is left open throughout,
 * because one import name can take different argument counts at different
 * sites in this overlay. */
s32 Func_080770c8();
s32 Func_0808a018();
s32 Func_0808a460();
u16 *Func_0808a080();
void Func_0808a010();
void Func_0808a020();
void Func_0808a098();
void Func_0808a110();
void Func_0808a138();
void Func_0808a148();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a580();

/* In-image data at file offset 0x1314 (0x02009314 under the 0x02008000 link
 * base).  The overlay image is writable EWRAM, so this is not const. */
extern u8 Data_02009314[];

void Func_02000308(void)
{
    u16 *scene;
    s32 quadrant;
    s32 chained;

    scene = Func_0808a080(0);

    /*
     * Heading at record offset 6, biased by 0x2000 and reduced to bits 14-15.
     * The `lsls #16 / asrs #16 / lsls #16` chain before the test at
     * 0x020003be only narrows the value to its low halfword and moves it into
     * the top half of the word, so the comparison is on the quadrant itself.
     */
    quadrant = ((s32)scene[3] + 0x2000) & 0xffffc000;

    chained = Func_080770c8(0x300);
    chained = Func_0808a018(chained);
    Func_0808a460(chained);

    Func_0808a170(0x26ec);
    Func_0808a010(50);
    Func_0808a1e8(14, 258, 50);
    Func_0808a148(14, 0, 20);
    Func_0808a180(14, 0);

    Func_0808a010(10);
    Func_0808a110(14, 4);
    Func_0808a010(30);
    Func_0808a180(14, 0);

    Func_0808a010(10);
    Func_0808a138(14, 2);
    Func_0808a010(30);
    Func_0808a180(14, 0);

    Func_0808a010(10);
    Func_0808a110(14, 3);
    Func_0808a010(20);
    Func_0808a180(14, 0);

    if ((quadrant & 0xffff) == 0x8000) {
        Func_0808a580(0, 0, 16);
        Func_0808a1b8(0, 0xc000, 0);
        Func_0808a010(20);
    }

    Func_0808a098(14, Data_02009314);
    Func_0808a020();
}
