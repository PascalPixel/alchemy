#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_020017a8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001998();
void Func_02001aa6();
s32 Func_02002122();
void Func_020022e0();
void Func_020023fc();
void Func_02002424();
void Func_0200248c();
void Func_02002e8e();
void Func_02002ea4();
void Func_0200326a();
void Func_02003316();
void Func_02003324();
void Func_02005264();
void Func_020053a2();
s32 Func_020071a6();
s32 Func_020071be();
void Func_020071ce();
void Func_020071de();
s32 Func_02007258();
void Func_020072a4();
s32 Func_020072ce();
void Func_02007310();
void Func_02007316();
void Func_0200731c();
void Func_02007326();
void Func_02007366();
void Func_02007394();
void Func_0200741c();
void Func_0200741e();
void Func_02007430();
void Func_02007442();
void Func_02007452();
void Func_02007462();
void Func_02007472();
void Func_02007476();
void Func_02007490();
void Func_020074a0();
void Func_020074a6();
void Func_020074b0();
void Func_020074c0();
void Func_020074d0();
void Func_020074e0();
void Func_020074f0();
void Func_02007500();
void Func_0200752e();
void Func_020075aa();
s32 Func_020075c2();
void Func_020075f4();
void Func_0200760e();
void Func_02007620();
void Func_02007640();
void Func_02007664();
void Func_0200766c();
void Func_020076fa();
void Func_02007712();
void Func_0200771e();

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

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    s32 *rec;
    s32 outer;
    s32 shift4;
    s32 v8;
    s32 inner;
    s32 neg;
    s32 nsh;
    s32 va0;
    s32 vb0;
    s32 va;
    s32 vb;
    s32 next;
    s32 slot20[10];

    Call6(Func_020071ce, 78, 59, 110, 36, 1, 1);
    Call6(Func_020071de, 76, 59, 109, 36, 1, 1);
    rec = slot20;
    rec[1] = 7;
    rec[2] = 0x8000;
    rec[3] = 0x8000;
    outer = 0;
    do {
        shift4 = (outer << 4);
        inner = 0;
        nsh = -(outer << 20);
        v8 = (0x2d80000 + nsh);
        do {
            if ((inner & 1) != 0) {
                va0 = Func_020071a6();
                va = (((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) + ((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                neg = -inner;
                vb0 = Func_020071be();
                vb = (((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) + ((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                Func_02001998(v8, 0, 0x2480000, va, 0, vb, 0x90000, (s32)rec);
                Func_02007310(1);
            } else {
                neg = -inner;
            }
            Func_020053a2(((s32)((s32)(neg - shift4) << 16) + 0x2d80000), 0, 0x2480000);
            inner = (inner + 1);
            v8 = (v8 + -0x10000);
        } while ((u32)inner <= 7);
        Call6(Func_020072a4, 76, 59, (108 - outer), 36, 2, 1);
        next = outer + 1;
        Func_02002e8e(a0, outer, next);
        outer = next;
    } while ((u32)next <= 1);
    Func_02007366(a0);
    Func_02002ea4(0, next, (next + 1));
    Func_0200752e(211);
    Value2(Func_02007258, 0x2009791, 0xc80);
    Func_0200731c();
}


typedef struct {
    unsigned lo : 24;
    unsigned mid : 24;
    unsigned hi : 16;
} SceneTail;

typedef struct {
    s32 f0;
    s32 f1;
    s32 f2;
    s32 f3;
    SceneTail tail;
} SceneEvent;

