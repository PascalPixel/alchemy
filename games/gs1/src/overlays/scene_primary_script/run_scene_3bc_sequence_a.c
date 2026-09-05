#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000000[];

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};
extern struct ModeRecord Data_0200da50;
extern struct ModeRecord Data_0200cce2;
extern u8 Data_02000240[];
extern u8 Data_02002090[];
extern u8 Data_0200cbfc[];
extern u8 Data_0200cc28[];
extern u8 Data_0200cca4[];
extern u8 Data_03001ebc[];
extern u8 Data_02000240_t[][2];
void Func_0200640a();
void Func_02006436();
s32 Func_02006996();
s32 Func_020069c2();
void Func_02007e44();
void Func_02007fa2();
void Func_02006a56();
void Func_02006a5e();
void Func_02007fc2();
void Func_02007fcc();
void Func_02006a76();
void Func_02007fea();
void Func_02006aa6();
void Func_02008010();
u8 *Func_02006b14();
void Func_020080d0();
void Func_020080e4();
void Func_020080dc();
void Func_0200811c();
void Func_02006bba();
void Func_02008150();
void Func_02006be4();
void Func_02006bec();
void Func_0200817a();
void Func_0200817c();
void Func_02008186();
s32 Func_020081b0();
void Func_020081b2();
void Func_020081be();
void Func_02006c4e();
void Func_02006c5c();
void Func_02006c62();
void Func_02006c6a();
void Func_02006c80();
void Func_02006cb2();
u8 *Func_02006cde();
void Func_02006cea();
void Func_02006d00();
void Func_02006d36();
void Func_02006d42();
void Func_02006d62();
void Func_02006db2();

void Func_02002ba8();
void Func_02002d84();
/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bcSequenceA(s32 a0)
{
    s32 kind;

    Func_0200811c(247);
    Func_020080d0();
    Func_020080e4();
    Data_0200da50.span = a0 * 60;
    Data_0200cce2.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        Func_02007fa2(30);
        Func_02008150(86);
        Func_0200640a(8);
        Value2(Func_02006996, 3, 1);
        Func_02007fc2(-a0 * 60 + 60);
        kind = 0;
    } else {
        Func_02007fcc(30);
        Func_0200817c(a0 + 90);
        Func_02006436(4);
        Value2(Func_020069c2, 3, 0);
        Func_02007fea(a0 * 60 + 60);
        kind = 8;
    }
    Call3(Func_020080dc, kind, 0x105, 0);
    while (Value0(Func_020081b0) != 0) {
        Func_02007e44(1);
    }
    Func_020081b2(19);
    Func_02008010(30);
    Call1(Func_020081be, 0x121);
    Func_0200817a();
    Func_02008186();
}

void Func_02002ba8(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p8b;
    u8 *p9;
    s32 p9b;
    u8 *rec;
    s32 record;
    s32 v5;
    s32 base6_4000208;
    s32 v1;
    s32 base4_2002090;
    s32 v4;
    s32 v0;
    s32 slot0;
    u8 *p6;

    p6 = *(volatile s32 *)0x03001e68;
    p11 = a0;
    p8 = a1;
    p10 = a2;
    rec = Func_02006b14();
    p6[6] = 1;
    p6[7] = 4;
    *(volatile s32 *)Data_0200cca4 = *(volatile s32 *)((s32)rec + 8);
    *(volatile s32 *)Data_0200cbfc = *(volatile s32 *)((s32)rec + 16);
    p9 = *(volatile s32 *)((s32)rec + 80);
    *(volatile s32 *)Data_0200cc28 = *(volatile u16 *)((s32)rec + 6);
    v5 = 1;
    Func_02006be4(p11, 2);
    {
        u8 value = *(volatile u8 *)&rec[35];
    
        rec[35] = (u8)(value | v5);
    }
    {
        s32 shown = 0x4000;
    
        *(volatile u16 *)((s32)rec + 6) = shown;
    }
    Func_02006aa6((s32)rec, 3);
    Func_02006a56((s32)rec, 0);
    Func_02006a5e((s32)rec, 1);
    p10b = ((s32)p10 << 16);
    p8b = ((s32)p8 << 16);
    Func_02006bba(p11, p8b, p10b);
    Func_02006bec(0, 0x4000, 0);
    base6_4000208 = 0x4000208;
    base4_2002090 = (s32)Data_02002090;
    v1 = *(volatile u16 *)base6_4000208;
    *(volatile u16 *)base6_4000208 = base6_4000208;
    if (*(volatile u16 *)base4_2002090 <= 31) {
        *(volatile u16 *)base4_2002090 += 1;
        *(volatile s32 *)((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4)) = 0xf00;
        *(volatile s32 *)(((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4) + 4)) = 0x4000050;
        *(volatile s32 *)(((((((*(volatile u16 *)base4_2002090 << 1) + *(volatile u16 *)base4_2002090) << 2) + base4_2002090) + 4) + 4) + 4) = 0x20000;
    }
    *(volatile u16 *)base6_4000208 = v1;
    p9[5] = ((s32)(-13 & p9[5]) | 4);
    p9[17] = ((s32)(-13 & p9[17]) | 4);
    slot0 = base4_2002090;
    v5 = 0;
    Func_02006d36(252);
    v4 = slot0;
    do {
        *(volatile s32 *)((s32)rec + 24) = ((v5 << 12) + 0x1000);
        *(volatile s32 *)((s32)rec + 28) = (0x1f000 - (v5 << 12));
        v0 = *(volatile u16 *)base6_4000208;
        *(volatile u16 *)base6_4000208 = base6_4000208;
        if (*(volatile u16 *)(v4) <= 31) {
            *(volatile u16 *)(v4) += 1;
            *(volatile s32 *)(((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4)) = (((15 - v5) << 8) | (v5 + 1));
            *(volatile s32 *)((((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4) + 4)) = 0x4000052;
            *(volatile s32 *)((((v4 + (((*(volatile u16 *)(v4) << 1) + *(volatile u16 *)(v4)) << 2)) + 4) + 4) + 4) = 0x20000;
        }
        *(volatile u16 *)base6_4000208 = v0;
        slot0 = v4;
        Func_02006a76(1);
        v5 = (v5 + 2);
        v4 = slot0;
    } while (v5 <= 15);
    v4 = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0x4000208;
    if (*(volatile u16 *)Data_02002090 <= 31) {
        *(volatile u16 *)Data_02002090 += 1;
        *(volatile s32 *)((((((*(volatile u16 *)Data_02002090 << 1) + *(volatile u16 *)Data_02002090) << 2) + 0x2002090) + 4)) = 16;
        *(volatile s32 *)(((((((*(volatile u16 *)Data_02002090 << 1) + *(volatile u16 *)Data_02002090) << 2) + 0x2002090) + 4) + 4)) = 0x4000052;
        *(volatile s32 *)(((((((*(volatile u16 *)Data_02002090 << 1) + *(volatile u16 *)Data_02002090) << 2) + 0x2002090) + 4) + 4) + 4) = 0x20000;
    }
    *(volatile u16 *)0x04000208 = v4;
    *(volatile s32 *)((s32)rec + 24) = 0x11000;
    *(volatile s32 *)((s32)rec + 28) = 0xf000;
    Func_02006c4e(1);
    *(volatile s32 *)((s32)rec + 24) = 0x10000;
    *(volatile s32 *)((s32)rec + 28) = 0x10000;
    Func_02006c5c(13);
    p9[5] &= -13;
    p9[17] &= -13;
    Func_02006cea(p11, 3);
    Func_02006c80(20);
    p9b = v5;
}

void Func_02002d84(s32 a0)
{
    u32 i;
    u8 *rec7;
    s32 record;
    u8 *p7;

    p7 = *(volatile s32 *)0x03001e68;
    rec7 = Func_02006cde();
    if (Data_02000240_t[249][0] == 1) {
        Data_02000240_t[249][0] = 0;
        Func_02006d42(a0, 1);
    } else {
        Call3(Func_02006db2, a0, 0x4000, 30);
        Func_02006d62(a0, 3);
        Func_02006d00(30);
    }
    p7[7] = 0;
    p7[6] = 15;
    *(volatile s32 *)((s32)rec7 + 8) = *(volatile s32 *)Data_0200cca4;
    *(volatile s32 *)((s32)rec7 + 16) = *(volatile s32 *)Data_0200cbfc;
    *(volatile u16 *)((s32)rec7 + 6) = *(volatile s32 *)Data_0200cc28;
    *(volatile s32 *)((s32)rec7 + 56) = -0x80000000;
    *(volatile s32 *)((s32)rec7 + 64) = -0x80000000;
    *(volatile s32 *)((s32)rec7 + 36) = 0;
    *(volatile s32 *)((s32)rec7 + 44) = 0;
    rec7[85] = 3;
    rec7[34] = (s32)Data_00000000;
    *(volatile s32 *)((s32)rec7 + 12) = 0;
    *(volatile s32 *)((s32)rec7 + 20) = 0;
    Func_02006cb2((s32)rec7, 1);
    Func_02006c62((s32)rec7, 0);
    Func_02006c6a((s32)rec7, 1);
    Func_02008150(1);
}
