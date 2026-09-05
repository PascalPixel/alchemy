#include "types.h"

#define FieldScene_RunEarlySequence Func_02000328

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020013e4();
void Func_020013e8();
s32 Func_0200140a();
s32 Func_02001460();
void Func_02001462();
void Func_02001496();
void Func_020014b0();
void Func_020014b4();
void Func_020014b8();
void Func_020014ba();
s32 Func_020014da();
s32 Func_020014e2();
void Func_020014e8();
void Func_0200151a();
s32 Func_02001522();
void Func_02001524();
void Func_0200152e();
void Func_02001538();
void Func_02001548();
void Func_0200154a();
void Func_02001552();
void Func_02001560();
void Func_02001568();
void Func_02001574();
void Func_0200157a();
void Func_0200157c();
void Func_02001582();
void Func_02001584();
void Func_02001598();
void Func_020015ac();
void Func_020015b6();
u8 *Func_020015c0();
void Func_020015c8();
void Func_020015da();
void Func_020015dc();
void Func_020015e6();
void Func_020015ee();
void Func_020015f6();
void Func_0200160e();
void Func_02001612();
void Func_0200163e();
void Func_02001666();
void Func_0200166a();
void Func_02001676();
void Func_0200167a();
void Func_020016ca();
void Func_020016ea();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunEarlySequence(void)
{
    u32 i;
    u8 *record;
    s32 v5;
    u8 *tbl;
    u8 *tbl2;
    s32 off;
    s32 off2;
    s32 a1;
    s32 a2;
    u8 *p7;

    p7 = *(u8 *volatile *)Data_03001ebc;
    Func_020013e8();
    for (i = 8; i < 66; i++) {
        record = (u8 *)Value1(Func_0200140a, i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    v5 = (s32)((s32)(*(volatile u16 *)(p7 + 0x16c) - 3) << 16) >> 16;
    if (v5 == 6) {
        Func_0200151a(188);
    } else {
        Call1((void (*)())Func_02001522, 158);
    }
    off = v5 << 2;
    tbl = (u8 *)0x02009d0c;
    a1 = *(s16 *)(tbl + off);
    off2 = off + 2;
    a2 = *(s16 *)(tbl + off2);
    tbl2 = (u8 *)0x02009cf0;
    Value3(Func_020013e4, *(s32 *)(tbl2 + off), a1, a2);
    Call3(Func_02001462, 0, 0x8000, 0x4000);
    *(u8 *)(Func_02001460(0) + 85) = 0;
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    if (v5 == 6) {
        Func_020014b8(0, 2);
        Call3(Func_020014b4, 0, 0, -4);
    } else {
        Call3(Func_020014ba, 0, 3, -16);
    }
    if (v5 == 4) {
        Func_0200152e(0, 3);
    } else {
        Func_02001538(0, 2);
    }
    Func_02001496(16);
    Func_02001574(v5 + 3);
    Func_020014b0();
}

