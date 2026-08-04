/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `tools/overlay_call_targets.ts`.  The band
 * 0x02002468-0x0200261f is an 8-byte-per-entry import veneer table
 * (`ldr r4,[pc,#0] / bx r4 / .word <main-image address>`); calls landing there
 * are named by that trailing word, as the semantic resource_39f sources do.
 * Calls landing on an overlay prologue keep the overlay's `Func_0200xxxx` name.
 *
 * Link base: resource_3b4 is linked at 0x02008000.  The image is 0x2f84 bytes
 * and the table at 0x02002d40-0x02002f84 holds 59 words of the form
 * `<function offset> + 0x8000 + 1` (Thumb bit), e.g. 0x02008ec9 = Func_02000ec8.
 * So a pool word in 0x02008000-0x0200af83 is in-image data at value - 0x8000.
 * `Data_02000240` lies below the base and is referenced by byte-exact sources
 * in many other overlays, so it is a shared EWRAM global, not overlay data.
 *
 * Return type: `pop {r0} / bx r0` pops the return address into r0, so nothing
 * is returned and the owner is void.
 *
 * Declarations are old-style because imports here are reached with differing
 * argument counts from different call sites.
 */
#include "types.h"

/*
 * resource_3b4 owner at 0x02000b68, 356 bytes: code 0x02000b68-0x02000cb9,
 * two bytes of alignment, and the pool words 0x000009c8, 0x000009c9,
 * 0x0000e666 and 0x0200a6fc at 0x02000cbc.
 *
 * The follow-on cutscene to 0x02000ad0.  It runs only once scene flag 0x9c8
 * (the one 0x02000ad0 sets) is present and its own flag 0x9c9 is not; that
 * flag is then set immediately, so the body is once-only.
 *
 * r6 holds the second Func_080770c0 result, which the guard has already proved
 * to be zero, and it is reused later as the zero written into slot 15's
 * halfword at +6.  The assembly shares the register; the value is zero either
 * way.
 *
 * The body walks slot 15 through four stops -- (584, 664), (664, 664),
 * (744, 664) and (824, 664) -- retunes the camera twice, plays out the timed
 * waits, and finishes by writing 0xf800 into the halfword at +30 of the record
 * hanging off slot 15's pointer at +80, clearing slot 15's facing, and handing
 * it the descriptor `Data_0200a6fc` (in-image data at file offset 0x26fc under
 * this overlay's 0x02008000 link base).
 *
 * Every numeric constant is a `movs`/`lsls` pair in the assembly: 0x20000 and
 * 0x4000 for Func_0808a208, 258 for Func_0808a1f0, 0x10000/0x8000 and
 * 0x80000/0x4000 for the two Func_0808a090 calls, 0xa0000 for the +40 store,
 * (0x40000, 0x20000, 0x10000) for the first Func_080091f0, and
 * (0x3780000, 0x2980000) for Func_0808a0f0.
 */

/* Descriptor handed to Func_08009098; in-image at file offset 0x26fc. */
extern u8 Data_0200a6fc[];

/* Scene flag test: zero means not yet set. */
s32 Func_080770c0();
/* Scene flag set. */
void Func_080770c8();
void Func_0808a018();
void Func_0808a208();
void Func_0808a220();
void Func_0808a218();
void Func_0808a1b8();
void Func_0808a1f0();
void Func_0808a138();
void Func_0808a010();
void Func_0808a090();
void Func_080f9010();
/* Returns the record for a numbered slot. */
s32 *Func_0808a080();
void Func_0808a0d0();
void Func_080091f0();
void Func_0808a0f0();
void Func_08009088();
void Func_08009098();
void Func_0808a020();

void Func_02000b68(void)
{
    s32 clear;
    s32 *slot;

    if (Func_080770c0(0x9c8) == 0) return;

    clear = Func_080770c0(0x9c9);
    if (clear != 0) return;
    Func_080770c8(0x9c9);

    Func_0808a018();
    Func_0808a208(0x20000, 0x4000);
    Func_0808a220(15, 1);
    Func_0808a218();
    Func_0808a1b8(15, 0x4000, 20);
    Func_0808a1f0(15, 258);
    Func_0808a138(15, 2);
    Func_0808a010(20);
    Func_0808a090(15, 0x10000, 0x8000);
    Func_080f9010(152);

    Func_0808a080(15)[10] = 0xa0000;   /* +40 */

    Func_0808a0d0(15, 584, 664);
    Func_0808a1b8(15, 0x4000, 20);
    Func_0808a1f0(15, 258);
    Func_0808a010(30);
    Func_0808a090(15, 0x80000, 0x4000);
    Func_0808a0d0(15, 664, 664);
    Func_0808a0d0(15, 744, 664);
    Func_0808a0d0(15, 824, 664);
    Func_0808a010(10);
    Func_080f9010(208);

    Func_080091f0(0x40000, 0x20000, 0x10000);
    Func_0808a010(20);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(30);

    Func_0808a0f0(15, 0x3780000, 0x2980000);

    slot = Func_0808a080(15);
    *(u16 *)((u8 *)(s32 *)slot[20] + 30) = 0xf800;   /* [+80] -> +30 */
    *(u16 *)((u8 *)slot + 6) = (u16)clear;

    Func_08009088(slot, 0);
    Func_08009098(slot, Data_0200a6fc);
    Func_0808a020();
}
