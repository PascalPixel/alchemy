#include "types.h"

/*
 * Resource 372 scene setup at 0x02000a10 (844 bytes, 78 distinct call targets
 * across 82 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x02000a10, matching
 * `pop {r5} / pop {r0} / bx r0` at 0x02000d08.  `pop {r0} ; bx r0` — r0 is the
 * popped return address, so the owner returns nothing.
 * 0x02000d0e-0x02000d5b is alignment plus the literal pool, reached only by
 * `ldr rN, [pc, #imm]`.
 *
 * Straight-line scene construction: install the backdrop and two actors
 * (handles 9 and 10), poke the two global fields at +0x1c0 and +0x1c8 of the
 * record behind 0x03001ebc, then run a short wait loop and finish the layout.
 * The two global offsets are the same pair the byte-exact
 * `assets/code/resource_372_c_02000434.c` writes (there 0x100 and 24, here
 * 0x100 and 16), which is what fixes their identity.
 *
 * r5 carries a message identifier 0xe5c that is bumped by 8 to 0xe64 before its
 * second use — the same "held message id, advanced in place" idiom as
 * `..._0200150c.c` and `..._02001828.c`.
 *
 * Three imports are reached with different argument counts at different sites,
 * so old-style declarations are mandatory:
 *   0x02005268  four arguments at 0x02000a42, three at 0x02000af2
 *   0x020053f6  three at 0x02000bf8, two at 0x02000c10
 *   0x020053e6  two at 0x02000c00, three at 0x02000ca8, one at 0x02000d00
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity for this overlay, not a place to
 * disassemble.
 *
 * Uncertainties: the handles (9, 10, 21, 22), the flag/message identifiers and
 * the 16.16-looking constants are not resolved.  The wait loop's exit test is
 * `== 1` exactly, not "non-zero"; that is faithful to `cmp r0, #1 / beq`.
 */

/* Imports, old-style: interfaces unknown. */
void Func_0808a018();
void Func_0808a2c0();
void Func_0808a2c8();
void Func_0808a2d8();
void Func_080000c0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a0c8();
void Func_0808a0f0();
void Func_0808a360();
void Func_0808a370();
void Func_0808a2e0();
void Func_080f9010();
void Func_08009178();
void Func_0808a1f0();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a5e8();
void Func_0808a100();
void Func_0808a138();
void Func_0808a1b8();
void Func_0808a170();
void Func_0808a180();
void Func_0808a010();
void Func_0808a110();
void Func_0808a150();
void Func_0808a188();
void Func_0808a1e8();
void Func_080770c8();
void Func_0808a178();
s32 Func_0808a070();
void Func_080770d0();
void Func_0808a020();

/* In-image data block (pool word 0x0200d78a; the byte-exact
 * assets/code/resource_372_c_020002e4.c names the same block). */
extern u8 Data_0200d78a[];

void Func_02000a10(void)
{
    u8 *state;
    s32 msg = 0xe5c;    /* r5 */

    Func_0808a018();
    Func_0808a2c0();
    Func_0808a2c8();
    Func_0808a2d8();
    Func_080000c0(60);
    Func_0808a208(0x4000, 0x800);                      /* 128<<7, 128<<4 */
    Func_0808a210(0x13c0000, 0xa00000, 0x3700000, 1);  /* 158<<17, 160<<16, 220<<18 */
    Func_0808a0f0(10, 0x1260000, 0x3640000);           /* 147<<17, 217<<18 */
    Func_0808a0f0(0, 0, 0);

    *(s32 *)(state + 0x1c0) = 0x100;
    *(s32 *)(state + 0x1c8) = 16;
    state = *(u8 **)0x03001ebc;

    Func_0808a360();
    Func_0808a370();
    Func_0808a2e0();
    Func_080f9010(158);
    Func_08009178(Data_0200d78a, 50, 44);
    Func_0808a1f0(22, 0x101);
    Func_0808a090(9, 0xcccc, 0x6666);
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(10, 0xcccc, 0x6666);
    Func_0808a0f0(9, 0x1560000, 0x37a0000);            /* 171<<17 */
    Func_0808a0d0(9, 0x156, 0x389);                    /* 171<<1 */
    Func_0808a5e8();
    Func_0808a0c8(9, 0x128, 0x389);                    /* 148<<1 */
    Func_0808a0f0(0, 0x1560000, 0x37a0000);
    Func_0808a0c8(0, 0x156, 0x37a);                    /* three arguments here */
    Func_0808a0d0(0, 0x156, 0x389);
    Func_0808a0d0(0, 0x13e, 0x389);                    /* 159<<1 */
    Func_0808a100(9, 1);
    Func_0808a138(9, 1);
    Func_0808a1b8(9, 0xc000, 60);                      /* 192<<8 */
    Func_0808a170(msg);
    Func_0808a180(9, 0);
    Func_0808a0d0(10, 0x126, 0x346);                   /* 147<<1 */
    Func_0808a010(40);
    Func_0808a110(10, 4);
    Func_0808a180(10, 0);
    Func_0808a150(0, 9, 0);
    Func_0808a010(40);
    Func_0808a1b8(10, 0x4000, 0);                      /* 128<<7 */
    Func_0808a188(10, 0, 20);
    Func_0808a1e8(9, 0x101, 20);
    Func_0808a1b8(9, 0xc000, 10);
    Func_0808a188(9, 0, 10);
    Func_0808a110(10, 4);
    Func_0808a180(10, 0);
    Func_0808a1f0(9, 0x102);                           /* 129<<1 */
    Func_0808a010(30);
    Func_0808a1b8(9, 0, 50);
    msg += 8;
    Func_0808a1b8(9, 0xc000, 10);
    Func_0808a090(9, 0x18000, 0xc000);                 /* 192<<9, 192<<8 */
    Func_0808a0d0(9, 0x121, 0x373);
    Func_0808a1b8(9, 0xe000, 0);                       /* 224<<8 */
    Func_0808a180(9, 0);
    Func_0808a138(10, 2);
    Func_0808a180(10, 0);                              /* two arguments here */
    Func_0808a110(9, 4);
    Func_0808a188(9, 0, 10);
    Func_0808a1b8(9, 0x2000, 10);                      /* 128<<6 */
    Func_0808a170(msg);
    Func_0808a178(9, 0);
    Func_0808a0d0(0, 0x12e, 0x389);                    /* 151<<1 */
    Func_0808a1b8(0, 0xc000, 0);

    do {
        Func_0808a138(9, 1);
        Func_0808a170(0xe65);
        Func_0808a178(9, 0);
    } while (Func_0808a070(0, 0) == 1);

    Func_0808a110(9, 3);
    Func_0808a170(0xe66);
    Func_0808a188(9, 0, 10);
    Func_0808a110(0, 3);
    Func_0808a090(10, 0x18000, 0xc000);                /* three arguments here */
    Func_0808a0c8(10, 0x129, 0x2ee);
    Func_0808a010(10);
    Func_0808a0d0(9, 0x129, 0x2ee);
    Func_0808a0f0(9, 0, 0);
    Func_0808a0f0(10, 0, 0);
    Func_0808a100(10, 1);
    Func_0808a100(21, 2);
    Func_0808a100(22, 5);
    Func_080770d0(0x12f);
    Func_080770c8(0x87b);
    Func_080770c8(0x205);                              /* one argument here */
    Func_0808a020();
}
