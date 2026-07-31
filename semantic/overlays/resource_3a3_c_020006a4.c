typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

/*
 * Resource 3a3 scripted sequence at 0x020006a4 (76 bytes,
 * 0x020006a4 .. 0x020006ef, of which the word at 0x020006ec is the pool).
 *
 * Complete owner: `push {r5, lr}` at 0x020006a4 and the interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x020006e6.  r0 is the popped return
 * address, so the owner returns nothing.
 *
 * All 8 call sites are placed (row reports calls=8): Func_0808a018,
 * Func_0808a080, Func_080f9010, Func_0808a0d8, Func_0808a248, Func_0808a368,
 * Func_0808a370, Func_0808a020, one each.
 *
 * `0x03001ebc` is a pointer CELL, not the workspace itself: the code does
 * `ldr r3,[pc] / ldr r5,[r3]`.  The byte-exact `resource_3c7_c_0200048c.c`
 * spells the same idiom.  The halfword the sequence reads sits at
 * workspace + 364 (`movs r3,#182 / lsls r3,#1`) and is loaded with `ldrsh`,
 * so it is signed.
 *
 * Uncertainty: at 0x020006d4 the `ldrsh r0,[r5,r3]` offset register r3 is
 * still 0 when Func_0808a248 is entered.  That is the documented offset
 * register artefact, not a second argument, so only r0 is passed here.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
void Func_0808a018();       /* opens a scripted sequence */
u8 *Func_0808a080();        /* scene/actor record accessor */
void Func_080f9010();
void Func_0808a0d8();
void Func_0808a248();
void Func_0808a368();
void Func_0808a370();
void Func_0808a020();       /* closes the scripted sequence */

extern u8 *Data_03001ebc;   /* pointer cell holding the overlay workspace */

void Func_020006a4(void)
{
    u8 *workspace;
    u8 *record;

    workspace = Data_03001ebc;

    Func_0808a018();

    record = Func_0808a080(0);
    record[0x55] = 0;

    Func_080f9010(123);
    Func_0808a0d8(0, 2, -16);

    Func_0808a248((s32)*(s16 *)(workspace + 364));

    Func_0808a368();
    Func_0808a370();
    Func_0808a020();
}
