#include "types.h"

/*
 * Resource 372 giant cutscene sheet at 0x020031ac (2716 bytes, 228 distinct
 * call targets across 245 call sites).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register saves and
 * `sub sp, #4` at 0x020031ac, matching `add sp, #4 / pop {r3, r5, r6, r7} /
 * mov r8..fp / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02003c1c.
 * `pop {r0} ; bx r0` — r0 is the popped return address, so the owner returns
 * nothing.  Three literal pools sit inside the span and are all branched over:
 * 0x02003392-0x020033c3 (`b.n` at 0x02003390), 0x020037ea-0x0200382b (`b.n` at
 * 0x020037e8) and 0x02003abc-0x02003adb (`b.n` at 0x02003aba); the trailing
 * pool is 0x02003c2e-0x02003c47.
 *
 * The third interior pool is the documented argument-carrying case: r0 and r3
 * are live across the `b.n` at 0x02003aba and are only finished at 0x02003adc
 * (`adds r0, #60` / `adds r3, #100`), so the store after it belongs to the
 * 0x020080ea result computed before the branch.
 *
 * `a prior working note` parked this row for the byte-exact
 * reconstruction on size and on those mid-function pools.  Neither is a semantic
 * obstacle; there are no jump tables and no DMA blocks in the span.
 *
 * The single stack word holds `obj19 + 85`, saved at 0x020035b0 and reused at
 * 0x020037bc — that is the only reason the frame exists.
 *
 * Two long-lived objects drive the whole sheet:
 *   obj19 = 0x020078ec(19), spr19 = *(void **)(obj19 + 0x50)
 *   obj27 = 0x020078f4(27), spr27 = *(void **)(obj27 + 0x50)
 * The +0x50 / +9 sprite-flag pair and the "+0x64 = query(...) + 60 with 90 as
 * the second argument" idiom are both proven by the byte-exact
 * `assets/code/resource_372_c_02000434.c`; the +0x66 field used as the
 * do-while guard and as a per-actor index is the same u16 that file writes.
 *
 * 17 imports are reached with two different argument counts at their two sites
 * (0x02007984, 0x020079ec, 0x02007a90, 0x02007a96, 0x02007aa6, 0x02007af2,
 * 0x02007b1c, 0x02007b5e, 0x02007b8a, 0x02007e98, 0x02007ebe, 0x02007eda,
 * 0x02007f1a, 0x02008018, 0x0200809a, 0x02008224, 0x02008234).  Old-style
 * declarations are therefore mandatory; 245 sites minus 228 distinct targets is
 * exactly those 17 second uses, which is the completeness check for this owner.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity for this overlay, not a place to
 * disassemble.  0x02003390, 0x02007412 and 0x0200755e are in that class —
 * 0x02003390 in particular is this owner's own `b.n` at the first interior
 * pool, which is what the identity reading predicts and a location reading
 * cannot explain.
 *
 * Uncertainties: handles, message ids and the 16.16-looking constants are
 * unresolved.  0x80000000 stored into +0x38/+0x3c/+0x40 reads like "unset".
 * r5 changes role three times (message-table pointer, then the mask -13, then
 * the merged sprite-flag byte); each use is written out explicitly below.
 */

/* Imports, old-style: interfaces unknown, 17 of them with varying arity. */
u8 *Func_020078ec();
u8 *Func_020078f4();
void Func_020079fa();
void Func_02007a12();
void Func_02007934();
void Func_0200793e();
void Func_02007948();
void Func_02007952();
void Func_02007964();
void Func_0200791a();
void Func_02007972();
void Func_02007a96();
void Func_0200792c();
void Func_02007984();
void Func_0200793a();
void Func_02007aa6();
void Func_02007996();
void Func_0200794c();
void Func_02003390();
void Func_02007a90();
void Func_02007966();
void Func_02007a0a();
void Func_020079ec();
void Func_020079f4();
void Func_020079fc();
void Func_02007a1c();
void Func_0200798c();
void Func_02007b8a();
void Func_020079d0();
void Func_020079a8();
void Func_020079e6();
void Func_02007b08();
void Func_02007b12();
void Func_02007b1c();
void Func_02007b26();
void Func_02007b30();
void Func_02007af2();
void Func_02007afc();
void Func_02007a32();
u8 *Func_02007a70();
s32 Func_0200799e();
s32 Func_02007984_b();
u8 *Func_02007a90_b();
s32 Func_020079be();
s32 Func_020079a4();
u8 *Func_02007aa8();
s32 Func_020079d6();
s32 Func_020079bc();
u8 *Func_02007af4();
s32 Func_02007a22();
s32 Func_02007a08();
u8 *Func_02007b0c();
s32 Func_02007a3a();
s32 Func_02007a20();
void Func_02007b40();
void Func_02007af6();
void Func_02007b4e();
void Func_02007b56();
void Func_02007b5e();
void Func_02007b66();
void Func_02007ce2();
void Func_02007afa();
void Func_02007cf8();
void Func_02007b3e();
void Func_02007b16();
void Func_02007b54();
void Func_02007b2c();
void Func_02007d2a();
void Func_02007b70();
void Func_02007b48();
void Func_02007b86();
void Func_02007d5c();
void Func_02007ba2();
void Func_02007b7a();
void Func_02007bb8();
void Func_02007b90();
void Func_02007d8e();
void Func_02007bd4();
void Func_02007bac();
void Func_02007bea();
void Func_02007d56();
void Func_02007bc6();
void Func_02007c04();
void Func_02007bda();
void Func_02007d46();
void Func_02007d5e();
void Func_02007db6();
void Func_02007dc4();
void Func_02007d22();
u8 *Func_02007c88();
void Func_02007c16();
u8 *Func_02007c94();
void Func_02007c22();
u8 *Func_02007dfc();
u8 *Func_02007e06();
u8 *Func_02007e0c();
u8 *Func_02007e12();
u8 *Func_02007e1a();
u8 *Func_02007e22();
void Func_02007c34();
void Func_02007e2c();
void Func_02007c90();
void Func_02007c4e();
void Func_02007e8e();
void Func_02007e98();
void Func_02007ea6();
void Func_02007c6c();
void Func_02007c7e();
void Func_02007d86();
void Func_02007e68();
void Func_02007e80();
void Func_02007ca2();
void Func_02007eea();
void Func_02007ef8();
void Func_02007cbe();
void Func_02007d4a();
u8 *Func_02007ebe();
u8 *Func_02007eca();
u8 *Func_02007ed2();
u8 *Func_02007eda();
u8 *Func_02007ee2();
u8 *Func_02007ee8();
void Func_02007d08();
void Func_02007e16();
void Func_02007d04();
void Func_02007e64();
void Func_02007d38();
void Func_02007e9e();
void Func_02007f1a();
void Func_02007eba();
void Func_02007ebe_b();
void Func_02007f3a();
void Func_02007eda_b();
void Func_02007ede();
void Func_02007f5a();
void Func_02007efa();
void Func_02007efe();
void Func_02007f7a();
void Func_02007f20();
void Func_02007f9c();
void Func_02007f3c();
void Func_02007fda();
void Func_02007e3e();
void Func_02007412();
void Func_02007f64();
void Func_0200800a();
void Func_02007eb2();
void Func_02007e82();
void Func_02007f98();
void Func_02007f9e();
void Func_02007fa4();
void Func_02007e92();
u8 *Func_02007f88();
void Func_02008026();
u8 *Func_02007fc4();
void Func_02008064();
u8 *Func_02008002();
void Func_0200809a();
u8 *Func_02008038();
void Func_0200755e();
void Func_020080ce();
void Func_0200814a();
void Func_02008156();
void Func_02008046();
void Func_0200800c();
void Func_02008012();
void Func_02008018();
void Func_0200801e();
void Func_02008024();
void Func_0200802a();
void Func_02008202();
void Func_0200820c();
void Func_0200821a();
void Func_02007fe0();
void Func_02008220();
void Func_02008228();
void Func_02008236();
void Func_020080b4();
void Func_020080ba();
void Func_020080c6();
void Func_0200807c();
void Func_02008082();
void Func_02008088();
void Func_0200808e();
void Func_02008094();
void Func_0200819a();
void Func_020081aa();
u8 *Func_02008148();
s32 Func_02008080();
s32 Func_02008066();
void Func_02008190();
void Func_020081e4();
void Func_020081f4();
u8 *Func_02008192();
s32 Func_020080c4();
s32 Func_020080aa();
void Func_020081d0();
void Func_02008224();
void Func_02008234();
u8 *Func_020081d2();
s32 Func_02008104();
s32 Func_020080ea();
void Func_02008288();
u8 *Func_0200822e();
s32 Func_02008160();
s32 Func_02008146();
void Func_0200826e();
void Func_020082c6();
u8 *Func_02008264();
u8 *Func_02008274();
void Func_020081a2();
void Func_020082f6();
u8 *Func_0200829c();
void Func_02008310();
void Func_020083ae();
void Func_02008212();
void Func_02008324();
void Func_0200832e();
void Func_02008338();
void Func_02008342();
void Func_0200834c();
void Func_02008356();
void Func_02008362();
void Func_0200836e();
void Func_02008464();
void Func_0200846e();
void Func_0200847c();
void Func_020082fa();
void Func_020084b6();
void Func_02008304();
void Func_02008332();
void Func_02008486();

                    

                     

                    

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

/* In-image data blocks. */
extern u8 Data_0200cd6c[];
extern u8 Data_0200ce04[];
extern u8 Data_0200ce30[];
extern u8 Data_0200ce5c[];
extern u8 Data_0200ce88[];
extern u8 Data_0200ceb4[];
extern u8 Data_0200bce5[];
extern u8 Data_0200cedc[];
extern u8 Data_0200be19[];
extern u8 Data_0200cec8[];
extern u8 Data_0200c5b9[];

void Func_020031ac(void)
{
    u8 *obj19;      /* r7 */
    u8 *obj27;      /* r6, later reused for the four sub-actors */
    u8 *spr27;      /* fp = *(void **)(obj27 + 0x50) */
    u8 *spr19;      /* sl = *(void **)(obj19 + 0x50), later reused as a value */
    u8 *o;
    u8 *guard;      /* obj19 + 102 */
    u8 *saved;      /* the stack word: obj19 + 85 */
    s32 mask;       /* r5 in its second role: -13 */
    s32 hidden;     /* r8: 0, then 0x80000000, then 0xe000 */
    s32 keep;       /* r9: 0, then 0xb000 */
    s32 lift;       /* sl in its second role: 0x20000 */
    s32 clear;      /* sl in its third role: the 254 mask */

    obj19 = Func_020078ec(19);
    spr19 = *(u8 **)(obj19 + 80);
    obj27 = Func_020078f4(27);
    spr27 = *(u8 **)(obj27 + 80);

    Func_020079fa(0x10000, 0x2000);                 /* 128<<9, 128<<6 */
    Func_02007a12(0x6e0000, -1, 0x58b0000, 1);      /* 220<<15 */
    Func_02007934(8, 0x13333, 0x9999);
    Func_0200793e(26, 0x13333, 0x9999);
    Func_02007948(0, 0x13333, 0x9999);
    Func_02007952(22, 0x13333, 0x9999);
    Func_02007964(8, Data_0200cd6c);
    Func_0200791a(10);
    Func_02007972(26, Data_0200cd6c);
    Func_02007a96();
    Func_0200792c(10);
    Func_02007984(0, Data_0200cd6c);
    Func_0200793a(10);
    Func_02007aa6();
    Func_02007996(22, Data_0200cd6c);
    Func_0200794c(128);
    Func_02003390();
    Func_02007a90(0xae0000, -1, 0x5940000, 1);      /* 174<<16 */
    Func_02007966(104);
    Func_02007aa6(0x990000, -1, 0x52d0000, 1);      /* 153<<16, four arguments */
    Func_02007a0a(9, 158, 0x4f8);                   /* 159<<3 */
    Func_02007a96(9, 0x2000, 0);                    /* three arguments */
    Func_020079ec(8);
    Func_020079ec(8, Data_0200ce04);                /* two arguments */
    Func_020079f4(26, Data_0200ce30);
    Func_020079fc(0, Data_0200ce5c);
    Func_02007a1c(22, Data_0200ce88);
    Func_0200798c(0x40000, 0x40000, 0x10000);       /* 128<<11 */
    Func_02007b8a(145);
    Func_020079d0(20);
    Func_020079a8(0x10000, 0x10000, 0x10000);
    Func_020079e6(60);
    Func_02007b08(0, 0x101, 0);
    Func_02007b12(26, 0x101, 0);
    Func_02007b1c(22, 0x101, 0);
    Func_02007b26(8, 0x101, 0);
    Func_02007b30(9, 0x101, 60);
    Func_02007af2(26, 8, 0);
    Func_02007afc(22, 0, 0);
    Func_02007a32(20);

    /* Five copies of the "+0x64 = query(...) + 20" frame-counter idiom. */
    o = Func_02007a70(0);
    *(u16 *)(o + 100) = (u16)(Func_02007984_b(Func_0200799e(), 20) + 20);
    hidden = 0;
    keep = 0;
    o = Func_02007a90_b(22);                          /* one argument */
    *(u16 *)(o + 100) = (u16)(Func_020079a4(Func_020079be(), 20) + 20);
    o = Func_02007aa8(26);
    *(u16 *)(o + 100) = (u16)(Func_020079bc(Func_020079d6(), 20) + 20);
    o = Func_02007af4(8);
    *(u16 *)(o + 100) = (u16)(Func_02007a08(Func_02007a22(), 20) + 20);
    o = Func_02007b0c(9);
    *(u16 *)(o + 100) = (u16)(Func_02007a20(Func_02007a3a(), 20) + 20);

    Func_02007b40(9, Data_0200ceb4);
    Func_02007af6(30);
    Func_02007b4e(0, Data_0200ceb4);
    Func_02007b56(26, Data_0200ceb4);
    Func_02007b5e(22, Data_0200ceb4);
    Func_02007b66(8, Data_0200ceb4);
    Func_02007b1c(10);                              /* one argument */
    Func_02007ce2(17);
    Func_02007afa(0x30000, 0x30000, 0x10000);       /* 192<<10 */
    Func_02007cf8(145);
    Func_02007b3e(30);
    Func_02007b16(0x10000, 0x10000, 0x10000);
    Func_02007b54(120);
    Func_02007b2c(0x30000, 0x30000, 0x10000);
    Func_02007d2a(145);
    Func_02007b70(40);
    Func_02007b48(0x20000, 0x20000, 0x10000);       /* 128<<10 */
    Func_02007b86(60);
    Func_02007b5e(0x40000, 0x40000, 0x10000);       /* three arguments */
    Func_02007d5c(145);
    Func_02007ba2(20);
    Func_02007b7a(0x10000, 0x10000, 0x10000);
    Func_02007bb8(60);
    Func_02007b90(0x30000, 0x30000, 0x10000);
    Func_02007d8e(145);
    Func_02007bd4(40);
    Func_02007bac(0x10000, 0x10000, 0x10000);
    Func_02007bea(60);
    Func_02007d56();
    Func_02007bc6(0x10000, 0x10000, 0x10000);
    Func_02007c04(1);
    Func_02007bda(-1, -1, 0xe666);
    Func_02007d46(0x80000, 0x80000);                /* 128<<12 */
    Func_02007d5e(0xd90000, -1, 0x43c0000, 1);      /* 217<<16 */
    Func_02007db6(0, 0);
    Func_02007dc4(40);
    Func_02007b8a(40);                              /* one argument */
    Func_02007d22(19, 0);
    Func_02007c16(Func_02007c88(19), 0);
    Func_02007c22(Func_02007c94(27), 0);

    *(s32 *)(obj27 + 24) = 0xcccc;
    *(s32 *)(obj27 + 28) = 0xcccc;
    obj27[35] = (u8)(obj27[35] & 254);
    spr27[9] = (u8)((spr27[9] & ~12) | 4);

    *(s32 *)(obj19 + 8) = 0xc80000;                 /* 200<<16 */
    *(s32 *)(obj19 + 12) = 0xc80000;
    *(s32 *)(obj19 + 56) = 0xc80000;
    *(s32 *)(obj19 + 60) = 0xc80000;
    *(s32 *)(obj19 + 16) = 0x3820000;
    *(s32 *)(obj19 + 64) = 0x3820000;
    saved = obj19 + 85;
    *saved = (u8)hidden;                            /* 0 */
    obj19[35] = (u8)(obj19[35] & 254);
    spr19[9] = (u8)(spr19[9] & ~12);

    o = Func_02007dfc(); *(s32 *)(o + 56) = (s32)0x80000000;
    o = Func_02007e06(); *(s32 *)(o + 60) = (s32)0x80000000;
    o = Func_02007e0c(); *(s32 *)(o + 64) = (s32)0x80000000;
    o = Func_02007e12(); *(s32 *)(o + 36) = keep;   /* 0 */
    o = Func_02007e1a(); *(s32 *)(o + 40) = keep;
    o = Func_02007e22(); *(s32 *)(o + 44) = keep;

    Func_02007c34(1);
    Func_02007e2c(0xf70000, 0x800000, 0x3950000, 0);  /* 247<<16, 128<<16 */
    Func_02007c90();
    Func_02007c4e(1);
    Func_02007e8e(0x10003, 1);
    Func_02007e98(0x10000, 2);                      /* 128<<9 */
    Func_02007ea6(30);
    Func_02007c6c(30);
    Func_02007c7e(Data_0200bce5, 3200);             /* 200<<4 */
    Func_02007d86(19, Data_0200cedc);
    Func_02007e68(0x20000, 0x7ae);                  /* 128<<10 */
    Func_02007e80(0xaf0000, 0x600000, 0x43e0000, 1); /* 175<<16, 192<<15 */

    /* Spin until obj19's +0x66 counter reaches 8. */
    guard = obj19 + 102;
    do {
        Func_02007ca2(1);
    } while (*(s16 *)guard != 8);

    Func_02007eea(0, 0);
    Func_02007ef8(60);
    Func_02007cbe(60);
    Func_02007d4a();

    hidden = (s32)0x80000000;
    o = Func_02007ebe(); *(s32 *)(o + 56) = hidden;
    o = Func_02007eca(); *(s32 *)(o + 60) = hidden;
    o = Func_02007ed2(); *(s32 *)(o + 64) = hidden;
    o = Func_02007eda(); *(s32 *)(o + 36) = 0;
    o = Func_02007ee2(); *(s32 *)(o + 40) = 0;
    o = Func_02007ee8(); *(s32 *)(o + 44) = 0;

    Func_02007d08(Data_0200bce5);
    Func_02007e16(19);
    Func_02007d04(1);
    Func_02007e64(19, 0);

    *(s32 *)(obj27 + 24) = 0x14000;                 /* 160<<9 */
    *(s32 *)(obj27 + 28) = 0x14000;
    spr27[35] = 2;
    *(s32 *)(spr27 + 24) = 0x14000;
    lift = 0x20000;                                 /* 128<<10 */
    *(s32 *)(obj19 + 24) = lift;
    *(s32 *)(obj19 + 28) = lift;
    *(s32 *)(obj19 + 8) = 0;
    *(s32 *)(obj19 + 16) = 0;
    *(s32 *)(obj19 + 56) = 0;
    *(s32 *)(obj19 + 64) = 0;

    Func_02007d38(1);
    Func_02007e98(23, 8);                           /* two arguments */
    Func_02007e9e(9, 0xa90000, 0x4f00000);          /* 169<<16, 158<<19 */
    Func_02007f1a(9, 0xc000, 0);                    /* 192<<8 */
    Func_02007eba(9, 9);
    Func_02007ebe_b(26, 0x970000, 0x50c0000);         /* three arguments, 151<<16 */
    Func_02007f3a(26, 0x8000, 0);                   /* 128<<8 */
    Func_02007eda_b(26, 5);                           /* two arguments */
    Func_02007ede(8, 0xaa0000, 0x5210000);          /* 170<<16 */
    Func_02007f5a(8, 0x6000, 0);                    /* 192<<7 */
    Func_02007efa(8, 5);
    Func_02007efe(0, 0xb90000, 0x5350000);          /* 185<<16 */
    Func_02007f7a(0, 0x2000, 0);                    /* 128<<6 */
    Func_02007f1a(0, 17);                           /* two arguments */
    Func_02007f20(22, 0xa90000, 0x5680000);         /* 169<<16, 173<<19 */
    Func_02007f9c(22, 0x4000, 0);                   /* 128<<7 */
    Func_02007f3c(22, 0);
    Func_02007fda(0xa60000, 0, 0x5390000, 0);       /* 166<<16 */
    Func_02007e3e();

    *saved = 0;
    *(s32 *)(obj19 + 56) = hidden;
    *(s32 *)(obj19 + 60) = hidden;
    *(s32 *)(obj19 + 64) = hidden;

    Func_02007412();
    Func_02007f64(27, 0xda0000, 0x4980000);         /* 218<<16, 147<<19 */
    Func_0200800a(0xd20000, 0, 0x4ac0000, 0);       /* 210<<16 */
    Func_02007eb2();

    *(s32 *)(obj27 + 24) = lift;
    *(s32 *)(obj27 + 28) = lift;

    Func_02007e82(Data_0200be19, 3200);
    Func_02007f98(10);
    Func_02007f9e(24);
    Func_02007fa4(25);
    Func_02007e92(1);

    /* Four sub-actors, each: clear bit 0 of +0x23, set +0x18/+0x1c, set the
     * +0x06 halfword and clear bits 2-3 of the sprite's +9 flags. */
    clear = 254;
    mask = -13;
    obj27 = Func_02007f88(10);
    spr27 = *(u8 **)(obj27 + 80);
    obj27[35] = (u8)(obj27[35] & clear);
    *(s32 *)(obj27 + 24) = 0x10000;                 /* 128<<9 */
    *(s32 *)(obj27 + 28) = 0x10000;
    *(u16 *)(obj27 + 6) = 0xd000;                   /* 208<<8 */
    spr27[9] = (u8)(spr27[9] & mask);
    Func_02008026(10, 0);

    obj27 = Func_02007fc4(24);
    spr27 = *(u8 **)(obj27 + 80);
    obj27[35] = (u8)(obj27[35] & clear);
    *(s32 *)(obj27 + 24) = 0x10000;
    *(s32 *)(obj27 + 28) = 0x10000;
    keep = 0xb000;                                  /* 176<<8 */
    spr27[9] = (u8)(spr27[9] & mask);
    *(u16 *)(obj27 + 6) = (u16)keep;
    Func_02008064(24, 5);

    obj27 = Func_02008002(25);
    spr27 = *(u8 **)(obj27 + 80);
    obj27[35] = (u8)(obj27[35] & clear);
    *(s32 *)(obj27 + 24) = 0x10000;
    *(s32 *)(obj27 + 28) = 0x10000;
    *(u16 *)(obj27 + 6) = (u16)keep;
    spr27[9] = (u8)(spr27[9] & mask);
    Func_0200809a(25, 5);

    obj27 = Func_02008038(27);
    spr27 = *(u8 **)(obj27 + 80);
    Func_0200755e();
    *(s32 *)(obj19 + 12) = 0xc00000;                /* 192<<14 */
    *(s32 *)(obj19 + 8) = 0xd60000;                 /* 214<<16 */
    *(s32 *)(obj19 + 16) = 0x4c00000;               /* 152<<19 */
    *(s32 *)(obj19 + 56) = hidden;
    *(s32 *)(obj19 + 60) = hidden;
    *(s32 *)(obj19 + 64) = hidden;
    /* r5 stops being the -13 mask here: it becomes the merged flag byte. */
    mask = (spr27[9] & mask) | 4;
    spr27[9] = (u8)mask;

    Func_020080ce(27);
    Func_0200814a(24, 0xc000, 0);                   /* 192<<8 */
    Func_02008156(25, 0xc000, 20);
    Func_02008046(0x166);                           /* 179<<1 */
    Func_0200800c(0);
    Func_02008012(1);
    Func_02008018(2);
    Func_0200801e(3);
    Func_02008024(4);
    Func_0200802a(5);
    Func_02008202(0x10003, 1);
    Func_0200820c(0x10000, 2);
    Func_0200821a(120);
    Func_02007fe0(160);
    Func_02008220(0x7fff, 1);
    Func_02008228(0x7fff, 2);
    Func_02008236(80);
    Func_020080b4(80);
    Func_020080ba(100);
    Func_02008018(Data_0200be19);                   /* one argument */

    *(s32 *)(spr27 + 24) = *(s32 *)(obj27 + 24);

    Func_020080c6(0x166);
    Func_0200807c(0);
    Func_02008082(1);
    Func_02008088(2);
    Func_0200808e(3);
    Func_02008094(4);
    Func_0200809a(5);                               /* one argument */
    Func_02007af2();                                /* no arguments */

    /* Four billboards: set +0x06, then "+0x64 = query(...) + 60" with 90, then
     * a per-actor index into +0x66. */
    Func_0200819a(9, 0xa50000, 0x4cd0000);          /* 165<<16 */
    Func_020081aa(9, 1);
    o = Func_02008148(9);
    hidden = 0xe000;                                /* 224<<8 */
    *(u16 *)(o + 6) = (u16)hidden;
    *(u16 *)(o + 100) = (u16)(Func_02008066(Func_02008080(), 90) + 60);
    *(u16 *)(o + 102) = 1;
    Func_02008190(9, Data_0200cec8);

    Func_020081e4(26, 0xa50000, 0x4e60000);
    Func_020081f4(26, 1);
    o = Func_02008192(26);
    *(u16 *)(o + 6) = (u16)hidden;
    *(u16 *)(o + 100) = (u16)(Func_020080aa(Func_020080c4(), 90) + 60);
    *(u16 *)(o + 102) = 2;
    Func_020081d0(26, Data_0200cec8);

    Func_02008224(22, 0x980000, 0x5050000);         /* 152<<16 */
    Func_02008234(22, 1);
    o = Func_020081d2(22);
    *(u16 *)(o + 6) = (u16)hidden;
    /* r0 and r3 are carried across the `b.n` that hops the third pool. */
    *(u16 *)(o + 100) = (u16)(Func_020080ea(Func_02008104(), 90) + 60);
    *(u16 *)(o + 102) = 3;
    Func_02008234(22, Data_0200cec8);               /* two arguments */

    Func_02008288(8, 0xb40000, 0x51f0000);          /* 180<<16 */
    o = Func_0200822e(8);
    *(u16 *)(o + 6) = (u16)hidden;
    *(u16 *)(o + 100) = (u16)(Func_02008146(Func_02008160(), 90) + 60);
    *(u16 *)(o + 102) = 4;
    Func_0200826e(8, Data_0200cec8);
    Func_020082c6(8, 6);

    o = Func_02008264(22);
    o[35] = (u8)(o[35] & clear);
    o = Func_02008274(8);
    clear = o[35] & clear;
    o[35] = (u8)clear;

    Func_020081a2(Data_0200c5b9, 3200);
    Func_020082f6(0, 0xb50000, 0x4f90000);          /* 181<<16 */
    o = Func_0200829c(0);
    *(u16 *)(o + 6) = (u16)hidden;
    Func_02008310(0, 1);
    Func_020083ae(0xb50000, 0, 0x4f90000, 0);
    Func_02008212();
    Func_02008324(10, 0, 0);
    Func_0200832e(19, 0, 0);
    Func_02008338(24, 0, 0);
    Func_02008342(25, 0, 0);
    Func_0200834c(23, 0, 0);
    Func_02008356(27, 0, 0);
    Func_02008362(17, 0x900000, 0x42e0000);         /* 144<<16 */
    Func_0200836e(18, 0x1140000, 0x4f60000);        /* 138<<17 */
    Func_02008224(60);                              /* one argument */
    Func_02008464(0x10003, 1);
    Func_0200846e(0x10000, 2);
    Func_0200847c(80);
    Func_020082fa(60);
    Func_020084b6();
    Func_02008304(60);
    Func_02008332(1);
    Func_02008486();
}
