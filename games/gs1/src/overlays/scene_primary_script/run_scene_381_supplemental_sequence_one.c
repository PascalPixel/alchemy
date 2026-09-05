#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_020029a4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02005e66();
void Func_02005e92();
void Func_02005ea4();
s32 Func_02005ea6();
void Func_02005eae();
void Func_02005eb8();
void Func_02005ec0();
void Func_02005ec2();
void Func_02005ecc();
void Func_02005ed6();
void Func_02005ee0();
void Func_02005eea();
void Func_02005ef4();
void Func_02005efe();
void Func_02005f08();
void Func_02005f30();
void Func_02005f38();
void Func_02005f4e();
void Func_02005fcc();
void Func_02005fd6();
void Func_02005fe4();
void Func_0200601a();
void Func_02006048();
void Func_02006054();
void Func_020060e4();
void Func_02006150();
void Func_0200615e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

extern s32 Data_0200b684[];
extern s32 Data_0200bb10[];
extern s32 Data_0200bb40[];
extern u8 Data_0200aba1[];

struct Inner {
    u8 _0[9];
    u8 a : 2;
    u8 b : 2;
    u8 c : 4;
    u8 _10[28];
    u8 f38;
};

struct Record {
    u8 _0[80];
    struct Inner *inner;
    u8 _84;
    u8 f85;
};

void FieldScene_RunSupplementalSequenceOne(u32 a0)
{
    u32 i;
    s32 none;
    s32 one;
    s32 *p;
    s32 *pairs;
    u8 *flags;
    struct Record *record;
    s32 v1;
    s32 v2;
    s32 v3;

    for (i = 0; i <= 15; i++) {
        Func_02005eae(i + 16);
    }
    switch (a0) {
    case 0:
        Call2(Func_02005fcc, 0x4039d2, 1);
        break;
    case 1:
        Call2(Func_02005fcc, 0x4049d2, 1);
        break;
    case 2:
        Call2(Func_02005fcc, 0x404a4e, 1);
        break;
    case 3:
        Call2(Func_02005fd6, 0x403a52, 1);
        break;
    }
    Func_02005fe4(60);
    Func_0200601a(214);
    p = Data_0200b684;
    i = 0;
    none = i;
    pairs = p;
    for (; i <= 9; i++) {
        v1 = pairs[0];
        v3 = 0;
        v2 = pairs[1];
        switch (a0) {
        case 0:
            v1 += 0xe80000;
            v3 = 0x900000;
            break;
        case 1:
            v1 += 0xe80000;
            v3 = 0x1d00000;
            break;
        case 2:
            v1 += 0x02c70000;
            v3 = 0x900000;
            break;
        case 3:
            v1 += 0x02c70000;
            v3 = 0x1d00000;
            break;
        }
        Data_0200bb40[i] = none;
        record = (struct Record *)Value4(Func_02005ea6, 0x11c, v1, v2, v3);
        Data_0200bb10[i] = (s32)record;
        record->f85 = none;
        record->inner->f38 = none;
        record->inner->b = 1;
        Func_02005ec0(record, 6);
        Func_02005e66(6);
        pairs += 2;
    }
    if (a0 == 0) {
        Call3(Func_02006048, 0, 0x100, 0);
        Call3(Func_02006054, 1, 0x100, 0);
    }
    Func_02005e92(20);
    Call2(Func_02005ea4, (s32)Data_0200aba1, 0xc80);
    one = 1;
    Func_020060e4(246);
    Data_0200bb40[0] = one;
    Func_02005eae(6);
    Data_0200bb40[1] = one;
    Func_02005eb8(6);
    Data_0200bb40[2] = one;
    Func_02005ec2(6);
    Data_0200bb40[3] = one;
    Func_02005ecc(6);
    Data_0200bb40[4] = one;
    Func_02005ed6(6);
    Data_0200bb40[5] = one;
    Func_02005ee0(6);
    Data_0200bb40[6] = one;
    Func_02005eea(6);
    Data_0200bb40[7] = one;
    Func_02005ef4(6);
    Data_0200bb40[8] = one;
    Func_02005efe(6);
    Data_0200bb40[9] = one;
    Func_02005f08(6);
    for (;;) {
        flags = (u8 *)Data_0200bb40;
        for (i = 0; i <= 9; i++) {
            if (*(s32 *)(flags + (i << 2)) != 0) {
                i = 222 << 2;
                break;
            }
        }
        if (i != (222 << 2)) {
            break;
        }
        Func_02005f30(1);
    }
    Func_02005f38(40);
    Call1(Func_02005f4e, (s32)Data_0200aba1);
    Call2(Func_02006150, 0x10000, 1);
    Func_0200615e(40);
}
