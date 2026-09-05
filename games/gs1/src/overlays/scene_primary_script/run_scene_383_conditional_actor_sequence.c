#include "types.h"

#define FieldScene_RunConditionalActorSequence Func_020021c8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200689a();
void Func_02006900();
void Func_02006912();
void Func_02006948();
void Func_0200694c();
void Func_02006952();
void Func_02006972();
void Func_02006982();
void Func_020069b2();
void Func_020069ba();
void Func_02006a00();
void Func_02006a26();
void Func_02006a28();
void Func_02006a48();
void Func_02006a4a();
void Func_02006a6a();
void Func_02006a74();
void Func_02006a92();
void Func_02006aa4();
void Func_02006aae();
void Func_02006ab4();
void Func_02006ab6();
void Func_02006ad0();
void Func_02006ad6();
void Func_02006ade();
void Func_02006b24();
void Func_02006b28();
void Func_02006b4e();
u8 *Func_02006f56();
void Func_02006f86();
void Func_02006f88();
void Func_02006f90();
void Func_02006f9a();
s32 Func_02006fc2();
s32 Func_02006fda();
s32 Func_02006fec();
void Func_02006ff0();
void Func_02006ffe();
void Func_02007006();
void Func_02007008();
void Func_0200700c();
void Func_0200702c();
void Func_0200703a();
void Func_0200705c();
void Func_02007068();
void Func_02007076();
void Func_02007090();
void Func_020070a8();
void Func_020070c0();
void Func_020070c8();
void Func_020070d0();
void Func_020070ee();
void Func_020070f4();
void Func_02007108();
void Func_02007118();
void Func_0200711a();
s32 Func_0200711c();
void Func_0200712c();
void Func_02007136();
s32 Func_02007148();
void Func_02007158();
void Func_02007168();
void Func_02007172();
void Func_02007182();
s32 Func_02007186();
void Func_02007188();
void Func_02007196();
void Func_020071a2();
void Func_020071aa();
void Func_020071ae();
s32 Func_020071ba();
s32 Func_020071e8();
void Func_020071f4();
void Func_0200720e();
s32 Func_02007226();
void Func_02007242();
void Func_02007262();
void Func_020072a4();
void Func_020072bc();
void Func_020072ca();
void Func_020072d6();
void Func_020072de();
void Func_02007300();
void Func_02007302();
void Func_02007308();
void Func_0200730a();
void Func_0200732c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call0(void (*f)())
{
    f();
}

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Actor record +9 bits 2..3: mode = 1 emits movs #13 / negs, not a folded 243. */
struct Rec9 {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;
    u8 hi : 4;
};

struct Work80 {
    u8 pad00[80];
    struct Rec9 *rec;
};

void FieldScene_RunConditionalActorSequence(void)
{
    u32 i;
    u8 *record;

    Func_020070d0();
    Func_02007008(0, 1);
    {
        u8 *record = Func_02006f56(0);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 1);
    }
    Call4(Func_0200705c, 0x200000, 0x2400000, 0x1900000, 0x3a80000);
    Call3(Func_02006f86, 0, 0xcccc, 0x6666);
    Call3(Func_02006f90, 1, 0xcccc, 0x6666);
    Call3(Func_02006f9a, 2, 0xcccc, 0x6666);
    Call3(Func_02006ff0, 0, 0xf80000, 0x2d80000);
    Call3(Func_02006ffe, 2, 0x1080000, 0x2e80000);
    Call3(Func_0200700c, 1, 0xe80000, 0x2e80000);
    record = (u8 *)Value1(Func_02006fc2, 0);
    ((struct Work80 *)record)->rec->mode = 1;
    record = (u8 *)Value1(Func_02006fda, 1);
    ((struct Work80 *)record)->rec->mode = 1;
    record = (u8 *)Value1(Func_02006fec, 2);
    ((struct Work80 *)record)->rec->mode = 1;
    Func_02007090(0, 2, 0);
    Func_0200689a(1, 2, 30);
    Call3(Func_02007068, 24, 0x680000, 0x2b80000);
    Call3(Func_02007076, 25, 0x780000, 0x2b80000);
    Func_020070c0(24, 25, 0);
    Func_02007108(0, 0);
    Call0((void (*)())Func_0200711c);
    Func_02006f88();
    Func_02007006(30);
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c8)) = 24;
    *(s32 *)((*(u8 **)0x03001ebc + 0x1c0)) = 0x201;
    Func_02007182();
    Func_02007196();
    Func_0200702c(40);
    Func_020070f4(1, 1);
    Func_0200703a(10);
    Call1(Func_02007118, 0x12ae);
    Func_02006900(1, 20);
    Func_02006952(2, 3, 20);
    Func_02006912(2, 20);
    Func_0200694c(0, 1, 50);
    Func_02007136(0, 2, 0);
    Func_02006948(1, 2, 20);
    Func_02006982(2, 4, 20);
    Func_02007172(2, 0);
    Call3(Func_02007196, 0, 0x102, 0);
    Call3(Func_020071a2, 1, 0x102, 0);
    Func_020070a8(60);
    Func_020069b2(2, 3, 20);
    Func_02006972(2, 30);
    Func_02007182(1, 1);
    Func_020070c8(20);
    Func_020069ba(0, 1, 20);
    Value2(Func_020071ba, 1, 0);
    i = 0;
    if (Value2(Func_0200711c, 0, 0) != 0) {
        Func_020070ee(20);
        Func_020071ae(2, 2);
        Func_02006a00(1, 4, 30);
        Value2(Func_020071e8, 1, 0);
        if (Value2(Func_02007148, 0, 0) != 0) {
            Func_0200711a(20);
            Call3(Func_02007226, 2, 0x102, 0);
            Func_0200712c(60);
            Func_020071f4(2, 2);
            Func_02006a26(0, 2, 20);
            Value2(Func_02007226, 2, 0);
            if (Value2(Func_02007186, 2, 0) != 0) {
                Func_02007158(20);
                Call3(Func_02007262, 2, 0x105, 0);
                Func_02007168(60);
                Func_02006a28(2, 20);
                Func_02006a4a(1, 2, 10);
                Func_02007242(1, 1);
                Func_02007188(10);
                Func_02006a48(1, 10);
                Func_02006a6a(2, 1, 20);
                Call3(Func_020072a4, 2, 0x101, 0);
                Func_020071aa(60);
                Func_02006ab4(2, 4, 20);
                Func_02006a74(1, 10);
                Func_02006aae(1, 0, 20);
                Func_02006ad0(1, 3, 20);
                i = 1;
                Func_02006a92(1, 20);
            }
        }
    }
    if (i == 0) {
        Call1(Func_020072bc, 0x12bc);
        Func_02006aa4(1, 20);
        Func_02006ade(1, 0, 20);
        Func_02006ab6(1, 20);
    }
    Call3(Func_02007308, 0, 0x105, 0);
    Func_0200720e(60);
    Func_020072d6(1, 1);
    Func_02006ad6(1, 10);
    Func_02006b28(1, 3, 10);
    Func_02007302(1, 2, 0);
    Func_02006b24(0, 2, 10);
    Func_0200732c(1, 0);
    Func_02006b4e(2, 3, 10);
    Call3(Func_020072ca, 2, 248, 0x2d8);
    Call3(Func_020072de, 1, 248, 0x2d8);
    Func_02007300(1, 0, 0);
    Func_0200730a(2, 0, 0);
}
