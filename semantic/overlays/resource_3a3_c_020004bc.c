typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * Resource 3a3 interaction handler at 0x020004bc (104 bytes,
 * 0x020004bc .. 0x02000523, of which 0x02000510 .. 0x02000523 is the pool).
 *
 * Complete owner: `push {lr}` at 0x020004bc and the interworking return
 * `pop {r0} / bx r0` at 0x0200050c, so the owner returns nothing.
 *
 * All 9 call sites are placed (row reports calls=9): Func_0808a080,
 * Func_080b0018, Func_0808a018, Func_080770c0, Func_0808a170 x2,
 * Func_0808a180, Func_0808a190, Func_0808a020.
 *
 * The head is the same facing test that the byte-exact
 * `assets/code/resource_3a3_c_02000338.c` factors out as its own function --
 * here it is inlined.  The record's halfword at +6 is biased by 0x5fff,
 * shifted left 16 and compared unsigned against 0x3ffe0000, so the guarded
 * side is an arc of the actor's angle.  The `bhi` at 0x020004d0 means the
 * "inside the arc" case is the one that takes the Func_080b0018 exit.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
u8 *Func_0808a080();        /* scene/actor record accessor */
void Func_080b0018();
void Func_0808a018();       /* opens a scripted sequence */
s32 Func_080770c0();        /* tests a flag id; nonzero when set */
void Func_0808a170();       /* queues a cue / script id */
void Func_0808a180();
void Func_0808a190();
void Func_0808a020();       /* closes the scripted sequence */

void Func_020004bc(void)
{
    u8 *record;

    record = Func_0808a080(0);

    if ((u32)((*(u16 *)(record + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Func_080b0018(6, 18);
        return;
    }

    Func_0808a018();

    if (Func_080770c0(0x909) != 0) {
        Func_0808a170(0x1947);
        Func_0808a180(18, 0);
    } else {
        Func_0808a170(0x18f5);
        Func_0808a190(18, 0);
    }

    Func_0808a020();
}
