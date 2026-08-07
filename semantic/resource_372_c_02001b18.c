#include "types.h"

/*
 * Resource 372 guarded scene setup at 0x02001b18 (1412 bytes, 132 distinct
 * call targets across 142 call sites).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register saves at
 * 0x02001b18, matching `pop {r3, r5, r6, r7} / mov r8..fp / pop {r5, r6, r7} /
 * pop {r0} / bx r0` at 0x0200207e.  `pop {r0} ; bx r0` — r0 is the popped
 * return address, so the owner returns nothing.  There are two literal pools:
 * an interior one at 0x02001f18-0x02001f57, branched over by the `b.n` at
 * 0x02001f16, and a trailing one at 0x0200208e-0x0200209b.
 *
 * The interior pool is the documented trap: r0, r1 and r2 are set at
 * 0x02001f10-0x02001f14, the `b.n` hops the pool, and the first instruction
 * after it (`lsls r1, r1, #1`) finishes building the second argument before the
 * call at 0x02001f5a.  The arguments are carried across the branch, so
 * `Func_0808a1e8(29, 0x100, 0)` below is a three-argument call.
 *
 * Guarded by flag 0x838, which is set again on the way out.
 *
 * Two in-image words are written as state during the build:
 *   Data_0200d7fc  set to 0, 1, 0, 2, 0, 2, 1 across the seven sub-scenes.  The
 *                  byte-exact `assets/code/resource_372_c_020020fc.c` reads the
 *                  same word as a shift amount into the 0x03001e40 flag word
 *                  (`(*(u32 *)0x03001E40 >> Data_0200d7fc) & 3`), which is what
 *                  identifies it as a small selector rather than a pointer.
 *   Data_0200d7f8  cleared once and set to 1 near the end.
 * Two 3200-byte buffers (`Data_0200c56d`, `Data_0200c5a9`) are handed to
 * 0x02006350 / 0x0200635e and re-visited later by 0x02006632 / 0x02006638.  The
 * (buffer, 3200) shape matches `Func_02004cea(Data_0200c5b9, 0xC80)` in the
 * byte-exact `assets/code/resource_372_c_02000434.c`.
 *
 * A 40-iteration stepping loop at 0x02001cc2 advances four sub-objects (handles
 * 29, 30, 32, 33) one frame at a time; the counter is `bls`-tested against 39,
 * so it runs exactly 40 times.
 *
 * Many imports are reached with different argument counts at different sites
 * (0x0200643a, 0x02006472, 0x020064c6, 0x020064de, 0x02006638, 0x02006666,
 * 0x0200670a, 0x02006712, 0x020067ba).  Old-style declarations are mandatory.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity for this overlay, not a place to
 * disassemble.
 *
 * Uncertainties: handles, flag/message identifiers and the 16.16-looking
 * constants are unresolved.  0xffff0000 stored at +24 of the object returned by
 * 0x0200637e reads like -1.0 in 16.16 but is written as the literal it is.
 */

/* Imports, old-style: interfaces unknown, several with varying arity. */
s32 Func_080770c0();
void Func_0808a018();
void Func_0808a030();
void Func_02004550();
void Func_080000c0();
void Func_080f9010();
void Func_080091f0();
void Func_0808a010();
s32 *Func_0808a080();
void Func_0808a0f0();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0b0();
void Func_0808a0a0();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a100();
void Func_0808a1e0();
void Func_0808a188();
void Func_0808a170();
void Func_0808a1b8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_020042bc();
void Func_080000d0();
void Func_0808a0c0();
void Func_080000d8();
void Func_0808a128();
void Func_0808a138();
void Func_0808a180();
void Func_0808a158();
void Func_0808a5e8();
void Func_0808a148();
void Func_02004560();
void Func_0808a110();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a088();
void Func_080770c8();
void Func_0808a020();

/* In-image data. */
extern u8 Data_0200d4b0[];
extern u8 Data_0200ca00[];
extern u8 Data_0200ca3c[];
extern u8 Data_0200c56d[];
extern u8 Data_0200c5a9[];
extern s32 Data_0200d7f8;
extern s32 Data_0200d7fc;

void Func_02001b18(void)
{
    s32 *desc;
    s16 *pos;
    u8 *obj;
    s32 i;

    if (Func_080770c0(0x838) != 0) {
        return;
    }

    Func_0808a018();
    Func_0808a030(Data_0200d4b0);
    Func_02004550();
    Func_080000c0(1);
    Func_080f9010(141);
    Func_080091f0(0x20000, 0x20000, 0x10000);   /* 128<<10, 128<<9 */
    Func_0808a010(30);
    Func_080091f0(0x30000, 0x30000, 0x10000);   /* 192<<10 */
    Func_080f9010(145);
    Func_0808a010(30);

    desc = Func_0808a080(0);
    if (desc != 0) {
        Func_0808a0f0(22, desc[2], desc[4]);    /* +8 and +16 */
    }

    Func_0808a090(0, 0x20000, 0x10000);
    Func_0808a090(22, 0x20000, 0x10000);
    Func_0808a098(0, Data_0200ca00);
    Func_0808a0b0(22, Data_0200ca3c);
    Func_0808a0a0(0);
    Func_0808a1e8(0, 0x100, 0);                 /* 128<<1 */
    Func_0808a1e8(22, 0x100, 30);
    Func_080091f0(0x40000, 0x40000, 0x10000);   /* 128<<11 */
    Func_080f9010(145);
    Func_0808a010(40);
    Func_080091f0(0x50000, 0x50000, 0x10000);   /* 160<<11 */
    Func_080f9010(145);
    Func_0808a010(20);
    Func_0808a1f0(0, 0x102);                    /* 129<<1 */
    Func_0808a1f0(22, 0x102);
    Func_0808a010(40);
    Func_0808a100(32, 5);
    Func_0808a100(33, 5);
    Func_0808a100(30, 8);
    Func_0808a100(29, 8);

    *(s32 *)(obj + 24) = (s32)0xffff0000;
    obj = Func_0808a080(30);

    Func_0808a1e0(32, 2);
    Func_0808a1e0(33, 2);
    Func_0808a1e0(30, 3);
    Func_0808a1e0(29, 3);
    Func_0808a170(0xe7f);
    Func_0808a188(28, 0, 20);                   /* three arguments here */
    Func_0808a1b8(0, 0xc000, 0);                /* 192<<8 */
    Func_0808a1b8(22, 0xc000, 20);
    Func_0808a208(0x40000, 0x8000);             /* 128<<11, 128<<8 */
    Func_0808a210(0x700000, -1, 0x14b0000, 1);  /* 224<<15 */
    Func_0808a218();

    /* Exactly 40 iterations: counter starts at 0, is incremented, then
     * `cmp #39 / bls` loops back. */
    do {
        Func_0808a080(32);
        Func_020042bc();
        Func_0808a080(33);
        Func_020042bc();
        Func_0808a080(30);
        Func_020042bc();
        Func_0808a080(29);
        Func_020042bc();
        i++;
        Func_080000c0(1);
    } while ((unsigned int)i <= 39);
    i = 0;

    Data_0200d7f8 = 0;
    Data_0200d7fc = 0;
    Func_080000d0(Data_0200c56d, 3200);         /* 200 << 4 */
    Func_080000d0(Data_0200c5a9, 3200);
    Func_0808a010(40);

    Func_0808a010(30);
    Data_0200d7fc = 1;
    Func_0808a0f0(19, 0x720000, 0x1220000);     /* 228<<15, 145<<17 */

    obj = Func_0808a080(19);
    *(s32 *)(obj + 12) += 0x400000;             /* 128 << 15 */
    *(s32 *)(obj + 60) = *(s32 *)(obj + 12);

    Func_0808a090(19, 0xcccc, 0x6666);
    Func_080f9010(145);
    Func_0808a0c0(19, 114, 0x14d);
    Func_0808a100(19, 2);
    Func_080091f0(0x20000, 0x20000, 0x10000);   /* three arguments here */
    Func_080f9010(145);

    Data_0200d7fc = 0;
    Func_0808a090(19, 0x6666, 0x3333);
    Func_0808a0c0(19, 114, 0x12c);              /* 150<<1 */
    Func_0808a100(19, 2);
    Func_080091f0(0x50000, 0x50000, 0x10000);
    Func_080f9010(145);

    Func_0808a090(19, 0xcccc, 0x6666);
    Func_0808a0c0(19, 114, 0x14d);
    Data_0200d7fc = 2;
    Func_0808a100(19, 2);
    Func_080091f0(0x20000, 0x20000, 0x10000);
    Func_080f9010(145);

    Data_0200d7fc = 0;
    Func_0808a090(19, 0x6666, 0x3333);
    Func_0808a0c0(19, 114, 0x12c);
    Func_0808a100(19, 2);
    Func_080091f0(0x40000, 0x40000, 0x10000);   /* three arguments here */
    Func_080f9010(145);

    Data_0200d7fc = 2;
    Func_0808a090(19, 0xcccc, 0x6666);
    Func_0808a0c0(19, 114, 0x14d);
    Func_0808a100(19, 2);
    Func_080091f0(0x20000, 0x20000, 0x10000);
    Func_080f9010(145);                         /* one argument here */

    Func_0808a010(20);
    Data_0200d7fc = 1;
    Func_0808a1f0(32, 0x102);
    Func_0808a138(32, 2);
    Func_0808a180(31, 0);
    Func_0808a1e8(33, 0x100, 0);
    Func_0808a138(33, 2);
    Func_0808a188(28, 0, 40);
    Func_0808a1f0(30, 0x102);
    Func_0808a138(30, 2);
    Func_0808a180(30, 0);

    Func_0808a100(29, 1);
    Func_080000c0(1);
    Func_0808a158(29, 0);
    Data_0200d7f8 = 1;
    Func_0808a1e8(29, 0x105, 20);
    Func_0808a1b8(29, 0x8000, 40);              /* 128<<8 */
    Func_0808a1b8(29, 0, 20);
    Func_0808a1b8(29, 0x8000, 20);
    Func_0808a1b8(29, 0x4000, 40);              /* 128<<7 */

    /* Arguments built before the `b.n` that hops the interior pool. */
    Func_0808a1e8(29, 0x100, 0);
    Func_0808a138(29, 2);
    Func_0808a128(29, 4, 40);                   /* three arguments here */
    Func_0808a100(29, 9);                       /* two arguments here */
    Func_0808a010(10);
    Func_0808a188(29, 0, 20);
    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a208(0x60000, 0xc000);             /* 192<<11, 192<<8 */
    Func_0808a210(0x540000, -1, 0x2340000, 1);  /* 168<<15, 141<<18 */
    Func_0808a218();
    Func_0808a5e8();
    Func_0808a148(22, 0, 0);
    Func_0808a010(20);
    Func_0808a1f0(22, 0x102);
    Func_0808a010(30);
    Func_080000d8(Data_0200c56d);
    Func_080000d8(Data_0200c5a9);               /* one argument here */
    Func_0808a180(22, 0);
    Func_0808a148(0, 22, 0);
    Func_0808a010(20);
    Func_02004560();
    Func_0808a100(0, 3);
    Func_0808a110(22, 3);
    Func_0808a010(20);                          /* one argument here */
    Func_0808a100(22, 2);                       /* two arguments here */

    pos = Func_0808a080(0);
    if (pos != 0) {
        Func_0808a0b8(22, pos[5], pos[9]);      /* +10 and +18, signed */
    }

    Func_0808a0e8(22);
    Func_0808a0f0(22, 0, 0);
    Func_0808a088(31);
    Func_0808a088(28);
    Func_0808a088(30);
    Func_0808a088(29);
    Func_0808a088(32);
    Func_0808a088(33);
    Func_080770c8(0x838);
    Func_0808a020();
}
