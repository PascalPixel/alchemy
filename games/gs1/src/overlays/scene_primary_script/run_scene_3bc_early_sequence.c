#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00003333[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
s32 Func_02000f3a();
s32 Func_02000f68();
void Func_02000fd0();
void Func_02005014();
void Func_02005036();
s32 Func_02005154();
s32 Func_0200519a();
s32 Func_020051a0();
s32 Func_020051a8();
void Func_020051c0();
void Func_020051d4();
void Func_020051d6();
void Func_020051e2();
void Func_020051e4();
s32 Func_02005208();
void Func_0200522a();
void Func_02005238();
void Func_0200523c();
void Func_02005246();
void Func_0200527a();
void Func_02005288();
void Func_02005298();
void Func_0200529e();
void Func_020052be();
void Func_020052c4();
void Func_020052d0();
void Func_020052f4();
void Func_0200530a();
void Func_02005310();
void Func_0200531c();
void Func_0200531e();
void Func_02005344();
void Func_0200534c();
void Func_020053a6();
void Func_020053b4();
void Func_020053da();
void Func_020053ee();
void Func_0200549c();
void Func_02005538();
void Func_0200553e();
void Func_02005550();
void Func_02005556();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunEarlySequence(void)
{
    s32 rec4;
    s32 rec2;
    s32 base5_2000434;
    s32 tile;
    s32 v3;
    s32 v7;
    s32 v9;
    s32 v5;
    s32 base5_3333;
    s32 slot16;
    s32 slot12;
    s32 record;
    s32 p5;
    s32 p6;
    s32 *q;
    u8 *base0;
    u8 *tbl;
    s32 off;
    s32 t;
    s32 a8;
    volatile s32 *fl;
    s32 slot20[3];

    base0 = Data_02000240;
    base5_2000434 = (s32)(base0 + 500);
    rec4 = Value1(Func_020051a0, *(volatile s32 *)base5_2000434);
    rec2 = Value1(Func_020051a8, 31);
    tbl = (u8 *)0x0200cc38;
    v9 = 0;
    off = (s32)(((u32)*(volatile u16 *)(rec4 + 6) >> 13) << 1);
    tile = *(volatile u16 *)(tbl + off);
    a8 = *(volatile s32 *)(rec4 + 8);
    p5 = *(volatile s32 *)base5_2000434;
    q = slot20;
    q[0] = ((a8 & -0x100000) + 0x80000);
    q[1] = *(volatile s32 *)(rec4 + 12);
    q[2] = ((*(volatile s32 *)(rec4 + 16) & -0x100000) + 0x80000);
    Call3(Func_02005036, 0x100000, tile, (s32)q);
    v7 = *(volatile s32 *)(rec2 + 8);
    v3 = q[0] - v7;
    p6 = *(volatile s32 *)(rec2 + 16);
    if (v3 < 0) {
        v3 = v7 - q[0];
        if (v3 > 0x80000) {
            goto L_020009ea;
        }
        t = q[2];
    } else {
        if (v3 > 0x80000) {
            goto L_020009ea;
        }
        t = q[2];
    }
    {
        if ((t - p6) >= 0) {
            if ((t - p6) > 0x200000) {
                goto L_020009ea;
            }
            fl = (volatile s32 *)0x03001ae8;
        } else {
            if ((p6 - t) > 0x200000) {
                goto L_020009ea;
            }
            fl = (volatile s32 *)0x03001ae8;
        }
        if ((*fl & 32) != 0) {
            slot16 = 2;
            slot12 = -8;
            L_0200081c:;
            v5 = (v7 + -0x100000);
            if (Value2(Func_02000f3a, v5, p6) != 0) {
                goto L_02000862;
            }
            v9 = (v9 + 1);
            v7 = v5;
            goto L_0200081c;
        }
        if ((*fl & 16) == 0) {
            goto L_020009f2;
        }
        slot16 = 3;
        slot12 = 8;
        L_02000848:;
        v5 = (v7 + 0x100000);
        if (Value2(Func_02000f68, v5, p6) == 0) {
            v9 = (v9 + 1);
            v7 = v5;
            goto L_02000848;
        }
        L_02000862:;
        if (v9 == 0) {
            goto L_020009f2;
        }
        Call6(Func_020051c0, 74, 8, 1, 4, (*(volatile s32 *)(rec2 + 8) >> 20), 9);
        Call6(Func_020051d4, 120, 60, 8, 5, 74, 60);
        Func_02005298();
        Func_02005310(p5, 8);
        Func_0200529e(6);
        *(volatile s32 *)(rec2 + 48) = 0x8000;
        base5_3333 = (s32)Data_00003333;
        *(volatile s32 *)(rec2 + 52) = base5_3333;
        Value2(Func_0200519a, rec2, slot16);
        Func_020051d6(rec2, v7, 0, p6);
        Func_020052c4(6);
        Func_02005344(p5, 2);
        record = Value2(Func_02005154, 27, 0xccc);
        Func_020051e2(*(volatile s32 *)((record + 0x1e0)), rec2);
        Call3(Func_0200531e, p5, 0x8000, base5_3333);
        Func_0200549c(239);
        Func_020051e4(rec4, 2);
        Func_0200522a(rec4, (((v9 * slot12) << 16) + *(volatile s32 *)(rec4 + 8)), 0, *(volatile s32 *)(rec4 + 16));
        Func_02005238(rec4);
        Value2(Func_02005208, rec4, 1);
        Func_02005246(rec2);
        if (v7 >= 0x5300000) {
            Call1(Func_020052f4, 0x369);
            Func_020053b4(31, 3);
            Func_020053a6(31, 18, 6);
            Func_0200534c(30);
            Func_0200523c(rec2, 8);
            Func_0200527a(rec2);
            *(u8 *)(rec2 + 35) = 2;
            v5 = 84;
            Call6(Func_020052be, 86, 10, 1, 2, v5, 10);
            Call6(Func_020052d0, 86, 9, 1, 1, v5, 12);
            Call1(Func_02005538, 0x120);
            Func_0200553e(240);
        } else {
            Func_02005288(rec2, 1);
            Call1(Func_02005550, 0x120);
            Func_02005556(213);
            v5 = (v7 >> 20);
            Call6(Func_0200530a, 85, 9, 1, 4, v5, 9);
            Call6(Func_0200531c, 85, 9, 1, 4, v5, 61);
        }
        Func_020053da(15);
        Func_020053ee();
        goto L_020009f2;
    }
    L_020009ea:;
    Func_02005014();
    Func_02000fd0();
    L_020009f2:;
}
