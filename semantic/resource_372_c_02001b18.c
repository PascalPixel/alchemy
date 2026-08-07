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
s32 Func_02006206();
void Func_02006230();
void Func_02006246();
void Func_0200608e();
void Func_02006180();
void Func_020063fe();
void Func_02006216();
void Func_02006254();
void Func_0200622c();
void Func_0200642a();
void Func_02006270();
s32 *Func_020062ae();
void Func_0200631c();
void Func_020062da();
void Func_020062e8();
void Func_020062f8();
void Func_02006318();
void Func_0200630e();
void Func_020063da();
void Func_020063e6();
void Func_020062a6();
void Func_020064a4();
void Func_020062ea();
void Func_020062c2();
void Func_020064c0();
void Func_02006306();
void Func_02006430();
void Func_0200643a();
void Func_02006320();
void Func_020063c8();
void Func_020063d0();
void Func_020063d8();
void Func_020063e0();
s32 *Func_0200637e();
void Func_02006462();
void Func_0200646a();
void Func_02006472();
void Func_0200647a();
void Func_02006450();
void Func_0200648e();
void Func_0200649a();
void Func_020064c6();
void Func_020064de();
void Func_020064ea();
s32 *Func_020063f2();
void Func_02005f86();
s32 *Func_020063fc();
void Func_02005f90();
s32 *Func_02006406();
void Func_02005f9a();
s32 *Func_02006410();
void Func_02005fa4();
void Func_0200632c();
void Func_02006350();
void Func_0200635e();
void Func_02006414();
void Func_02006420();
s32 *Func_0200646c();
void Func_02006492();
void Func_02006610();
void Func_020064d2();
void Func_02006502();
void Func_02006638();
void Func_020064cc();
void Func_02006508();
void Func_02006538();
void Func_02006470();
void Func_0200666e();
void Func_02006506();
void Func_02006540();
void Func_02006570();
void Func_020064a8();
void Func_020066a6();
void Func_0200653a();
void Func_02006576();
void Func_020065a6();
void Func_020066dc();
void Func_02006572();
void Func_020065ac();
void Func_020065dc();
void Func_02006514();
void Func_02006712();
void Func_0200655c();
void Func_02006686();
void Func_0200662e();
void Func_02006666();
void Func_0200669a();
void Func_0200664a();
void Func_0200668c();
void Func_020066be();
void Func_0200669e();
void Func_0200665c();
void Func_0200650a();
void Func_020066a2();
void Func_020066ec();
void Func_020066e8();
void Func_020066f2();
void Func_020066fe();
void Func_0200670a();
void Func_02006768();
void Func_02006718();
void Func_02006670();
void Func_02006772();
void Func_02006840();
void Func_02006656();
void Func_020067c2();
void Func_020067dc();
void Func_020067e8();
void Func_0200686c();
void Func_02006786();
void Func_020066c4();
void Func_020067ee();
void Func_020066d4();
void Func_02006632();
void Func_020067d8();
void Func_020067ba();
void Func_020066f8();
void Func_02006568();
void Func_020067a4();
void Func_020067b4();
s32 *Func_02006758();
void Func_020067a2();
void Func_020067c8();
void Func_020067da();
void Func_02006788();
void Func_0200678e();
void Func_02006794();
void Func_0200679a();
void Func_020067a0();
void Func_020067a6();
void Func_0200675c();
void Func_02006780();

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

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

    if (Func_02006206(0x838) != 0) {
        return;
    }

    Func_02006230();
    Func_02006246(Data_0200d4b0);
    Func_0200608e();
    Func_02006180(1);
    Func_020063fe(141);
    Func_02006216(0x20000, 0x20000, 0x10000);   /* 128<<10, 128<<9 */
    Func_02006254(30);
    Func_0200622c(0x30000, 0x30000, 0x10000);   /* 192<<10 */
    Func_0200642a(145);
    Func_02006270(30);

    desc = Func_020062ae(0);
    if (desc != 0) {
        Func_0200631c(22, desc[2], desc[4]);    /* +8 and +16 */
    }

    Func_020062da(0, 0x20000, 0x10000);
    Func_020062e8(22, 0x20000, 0x10000);
    Func_020062f8(0, Data_0200ca00);
    Func_02006318(22, Data_0200ca3c);
    Func_0200630e(0);
    Func_020063da(0, 0x100, 0);                 /* 128<<1 */
    Func_020063e6(22, 0x100, 30);
    Func_020062a6(0x40000, 0x40000, 0x10000);   /* 128<<11 */
    Func_020064a4(145);
    Func_020062ea(40);
    Func_020062c2(0x50000, 0x50000, 0x10000);   /* 160<<11 */
    Func_020064c0(145);
    Func_02006306(20);
    Func_02006430(0, 0x102);                    /* 129<<1 */
    Func_0200643a(22, 0x102);
    Func_02006320(40);
    Func_020063c8(32, 5);
    Func_020063d0(33, 5);
    Func_020063d8(30, 8);
    Func_020063e0(29, 8);

    *(s32 *)(obj + 24) = (s32)0xffff0000;
    obj = Func_0200637e(30);

    Func_02006462(32, 2);
    Func_0200646a(33, 2);
    Func_02006472(30, 3);
    Func_0200647a(29, 3);
    Func_02006450(0xe7f);
    Func_02006472(28, 0, 20);                   /* three arguments here */
    Func_0200648e(0, 0xc000, 0);                /* 192<<8 */
    Func_0200649a(22, 0xc000, 20);
    Func_020064c6(0x40000, 0x8000);             /* 128<<11, 128<<8 */
    Func_020064de(0x700000, -1, 0x14b0000, 1);  /* 224<<15 */
    Func_020064ea();

    /* Exactly 40 iterations: counter starts at 0, is incremented, then
     * `cmp #39 / bls` loops back. */
    do {
        Func_020063f2(32);
        Func_02005f86();
        Func_020063fc(33);
        Func_02005f90();
        Func_02006406(30);
        Func_02005f9a();
        Func_02006410(29);
        Func_02005fa4();
        i++;
        Func_0200632c(1);
    } while ((unsigned int)i <= 39);
    i = 0;

    Data_0200d7f8 = 0;
    Data_0200d7fc = 0;
    Func_02006350(Data_0200c56d, 3200);         /* 200 << 4 */
    Func_0200635e(Data_0200c5a9, 3200);
    Func_02006414(40);

    Func_02006420(30);
    Data_0200d7fc = 1;
    Func_020064c6(19, 0x720000, 0x1220000);     /* 228<<15, 145<<17 */

    obj = Func_0200646c(19);
    *(s32 *)(obj + 12) += 0x400000;             /* 128 << 15 */
    *(s32 *)(obj + 60) = *(s32 *)(obj + 12);

    Func_02006492(19, 0xcccc, 0x6666);
    Func_02006610(145);
    Func_020064d2(19, 114, 0x14d);
    Func_02006502(19, 2);
    Func_0200643a(0x20000, 0x20000, 0x10000);   /* three arguments here */
    Func_02006638(145);

    Data_0200d7fc = 0;
    Func_020064cc(19, 0x6666, 0x3333);
    Func_02006508(19, 114, 0x12c);              /* 150<<1 */
    Func_02006538(19, 2);
    Func_02006470(0x50000, 0x50000, 0x10000);
    Func_0200666e(145);

    Func_02006506(19, 0xcccc, 0x6666);
    Func_02006540(19, 114, 0x14d);
    Data_0200d7fc = 2;
    Func_02006570(19, 2);
    Func_020064a8(0x20000, 0x20000, 0x10000);
    Func_020066a6(145);

    Data_0200d7fc = 0;
    Func_0200653a(19, 0x6666, 0x3333);
    Func_02006576(19, 114, 0x12c);
    Func_020065a6(19, 2);
    Func_020064de(0x40000, 0x40000, 0x10000);   /* three arguments here */
    Func_020066dc(145);

    Data_0200d7fc = 2;
    Func_02006572(19, 0xcccc, 0x6666);
    Func_020065ac(19, 114, 0x14d);
    Func_020065dc(19, 2);
    Func_02006514(0x20000, 0x20000, 0x10000);
    Func_02006712(145);                         /* one argument here */

    Func_0200655c(20);
    Data_0200d7fc = 1;
    Func_02006686(32, 0x102);
    Func_0200662e(32, 2);
    Func_02006666(31, 0);
    Func_0200669a(33, 0x100, 0);
    Func_0200664a(33, 2);
    Func_0200668c(28, 0, 40);
    Func_020066be(30, 0x102);
    Func_02006666(30, 2);
    Func_0200669e(30, 0);

    Func_0200665c(29, 1);
    Func_0200650a(1);
    Func_020066a2(29, 0);
    Data_0200d7f8 = 1;
    Func_020066ec(29, 0x105, 20);
    Func_020066e8(29, 0x8000, 40);              /* 128<<8 */
    Func_020066f2(29, 0, 20);
    Func_020066fe(29, 0x8000, 20);
    Func_0200670a(29, 0x4000, 40);              /* 128<<7 */

    /* Arguments built before the `b.n` that hops the interior pool. */
    Func_02006768(29, 0x100, 0);
    Func_02006718(29, 2);
    Func_02006712(29, 4, 40);                   /* three arguments here */
    Func_0200670a(29, 9);                       /* two arguments here */
    Func_02006670(10);
    Func_02006772(29, 0, 20);
    Func_02006840(0x121);
    Func_02006656(-1, -1, 0xe666);
    Func_020067c2(0x60000, 0xc000);             /* 192<<11, 192<<8 */
    Func_020067dc(0x540000, -1, 0x2340000, 1);  /* 168<<15, 141<<18 */
    Func_020067e8();
    Func_0200686c();
    Func_02006786(22, 0, 0);
    Func_020066c4(20);
    Func_020067ee(22, 0x102);
    Func_020066d4(30);
    Func_02006632(Data_0200c56d);
    Func_02006638(Data_0200c5a9);               /* one argument here */
    Func_020067d8(22, 0);
    Func_020067ba(0, 22, 0);
    Func_020066f8(20);
    Func_02006568();
    Func_020067a4(0, 3);
    Func_020067b4(22, 3);
    Func_02006712(20);                          /* one argument here */
    Func_020067ba(22, 2);                       /* two arguments here */

    pos = Func_02006758(0);
    if (pos != 0) {
        Func_020067a2(22, pos[5], pos[9]);      /* +10 and +18, signed */
    }

    Func_020067c8(22);
    Func_020067da(22, 0, 0);
    Func_02006788(31);
    Func_0200678e(28);
    Func_02006794(30);
    Func_0200679a(29);
    Func_020067a0(32);
    Func_020067a6(33);
    Func_0200675c(0x838);
    Func_02006780();
}
