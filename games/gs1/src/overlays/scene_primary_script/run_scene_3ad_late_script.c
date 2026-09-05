#include "types.h"

#define FieldScene_RunLateScript Func_02001448

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000254e[];
extern u8 Data_03001ebc[];
void Func_02002fae();
void Func_02002ff0();
void Func_02003000();
void Func_02003006();
void Func_02003008();
void Func_0200301e();
void Func_02003020();
void Func_02003034();
void Func_02003036();
void Func_0200303c();
void Func_0200303e();
void Func_02003046();
void Func_02003054();
void Func_0200305e();
void Func_0200306e();
void Func_02003074();
void Func_02003076();
void Func_0200307e();
void Func_02003080();
void Func_02003086();
void Func_0200308c();
void Func_0200308e();
void Func_020030a0();
void Func_020030a6();
void Func_020030bc();
void Func_020030ce();
void Func_020030d2();
void Func_020030d6();
void Func_020030de();
void Func_020030e4();
void Func_020030e6();
void Func_020030f6();
void Func_020030fa();
void Func_02003106();
void Func_02003108();
void Func_0200310c();
void Func_0200313c();
void Func_02003156();
void Func_02003172();
void Func_0200317c();
void Func_02003188();
void Func_02003194();
void Func_02003196();
void Func_0200319c();
void Func_020031a0();
void Func_020031b2();
void Func_020031be();
void Func_020031ca();
void Func_020031cc();
void Func_020031d6();
s32 Func_020031e4();
void Func_020031e8();
void Func_020031fc();
void Func_02003200();
void Func_02003206();
s32 Func_02003214();
void Func_02003216();
void Func_02003218();
void Func_02003220();
void Func_02003224();
void Func_0200322e();
void Func_02003236();
s32 Func_02003244();
void Func_02003246();
void Func_0200324c();
void Func_0200324e();
void Func_02003254();
void Func_02003256();
void Func_02003266();
void Func_02003276();
void Func_02003284();
void Func_02003296();
void Func_02003320();
void Func_02003330();
void Func_02003334();

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

void FieldScene_RunLateScript(void)
{
    u32 i;
    s32 record;
    s32 base5_254e;

    Call3(Func_0200301e, 1, 0x105, 60);
    Func_02002ff0(1, 0, 0);
    base5_254e = (s32)Data_0000254e;
    Func_02003008(base5_254e);
    Func_02003020(1, 0);
    Func_02003000(3, 1);
    Func_0200301e((base5_254e + 1));
    Func_02003036(3, 0);
    Func_02003020(2, 13, 0);
    Func_02002fae(60);
    Func_0200303c((base5_254e + 2));
    Func_02003054(2, 0);
    Func_0200303e(13, 2, 0);
    Call3(Func_02003080, 13, 0x105, 70);
    Func_0200305e((base5_254e + 3));
    Func_02003076(13, 0);
    Func_02003046(12, 4);
    Func_02003074((base5_254e + 4));
    Func_0200308c(12, 0);
    Call3(Func_020030a0, 12, 0x3000, 0);
    Func_02003006(20);
    Func_0200306e(12, 3);
    Func_0200306e(0, 3);
    Func_02003076(1, 3);
    Func_0200307e(2, 3);
    Func_02003086(3, 3);
    Func_02003034(60);
    Call3(Func_02003080, 13, -16, 0);
    Func_0200308e(13);
    Func_020030a6(13, 1);
    Func_02003054(40);
    Func_020030bc(13, 3);
    Call3(Func_02003108, 13, 0x5000, 0);
    Func_0200306e(30);
    Func_020030ce(0, 3);
    Func_020030d6(1, 3);
    Func_020030de(2, 3);
    Func_020030e6(3, 3);
    Call3(Func_020030d2, 12, 156, 0x210);
    Func_020030a0(20);
    Call3(Func_020030e4, 13, 164, 0x210);
    Func_020030fa(12);
    Call3(Func_020030f6, 12, 168, 0x280);
    Func_0200310c(13);
    Call3(Func_02003108, 13, 168, 0x280);
    Call3(Func_0200317c, 0, 0x5000, 0);
    Call3(Func_02003188, 2, 0x5000, 0);
    Call3(Func_02003194, 3, 0x5000, 0);
    Call3(Func_020031a0, 1, 0x5000, 0);
    Func_02003106(20);
    Call3(Func_020031b2, 0, 0x4000, 0);
    Call3(Func_020031be, 2, 0x4000, 0);
    Call3(Func_020031ca, 3, 0x4000, 0);
    Call3(Func_020031d6, 1, 0x4000, 0);
    Func_0200313c(60);
    Func_02003196(13, 0, 0);
    Func_020031a0(12, 0, 0);
    Func_02003156(110);
    ((void (*)())Func_020031e4)((base5_254e + 5));
    Func_020031fc(1, 0);
    Func_020031cc(3, 3);
    Func_02003172(30);
    Func_02003200((base5_254e + 6));
    Func_02003218(3, 0);
    Func_020031e8(2, 3);
    Func_02003216((base5_254e + 7));
    Func_0200322e(2, 0);
    Func_0200319c(140);
    Func_02003220(1, 0, 0);
    Func_02003236((base5_254e + 8));
    Func_0200324e(1, 0);
    Func_02003216(1, 2);
    record = Value1(Func_020031e4, 0);
    if (record != 0) {
        Func_02003206(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02003224(1);
    Func_02003236(1, 0, 0);
    Func_02003246(2, 2);
    record = Value1(Func_02003214, 0);
    if (record != 0) {
        Func_02003236(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02003254(2);
    Func_02003266(2, 0, 0);
    Func_02003276(3, 2);
    record = Value1(Func_02003244, 0);
    if (record != 0) {
        Func_02003266(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02003284(3);
    Func_02003296(3, 0, 0);
    Func_0200324c(30);
    Func_02003334(0, 1);
    Func_02003330();
    Func_02003320(0, 0);
    Call1(Func_02003256, 0x94f);
}
