#include "types.h"

#define RunEventScript01 Func_020028a4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000e9b[];
extern u8 Data_00000ea1[];
extern u8 Data_0200cec8[];
void Func_02006fea();
void Func_02007002();
s32 Func_02007008();
s32 Func_02007020();
s32 Func_02007022();
s32 Func_02007038();
s32 Func_0200703a();
s32 Func_02007052();
void Func_02007058();
void Func_02007070();
void Func_0200707a();
void Func_0200707c();
void Func_02007084();
s32 Func_0200708a();
void Func_0200708c();
void Func_02007094();
void Func_020070a4();
void Func_020070aa();
void Func_020070ac();
void Func_020070b8();
void Func_020070c4();
void Func_020070cc();
void Func_020070d0();
void Func_020070dc();
void Func_020070ea();
s32 Func_020070f4();
void Func_020070f6();
void Func_020070fe();
void Func_02007104();
s32 Func_0200710c();
s32 Func_02007124();
void Func_0200712c();
void Func_02007136();
void Func_0200713a();
void Func_02007146();
void Func_0200714e();
void Func_02007156();
void Func_02007158();
s32 Func_02007160();
void Func_02007164();
void Func_02007168();
void Func_02007176();
void Func_02007184();
void Func_0200718e();
void Func_0200719a();
void Func_020071a4();
void Func_020071a6();
void Func_020071c0();
void Func_020071ce();
void Func_020071de();
void Func_020071f2();
void Func_020071f4();
void Func_02007208();
void Func_02007232();
s32 Func_02007236();
void Func_0200723e();
void Func_02007266();
void Func_0200726e();
void Func_02007270();
void Func_0200727e();
void Func_02007280();
void Func_0200728c();
s32 Func_020072aa();
void Func_020072ae();
void Func_020072c0();
void Func_020072e0();
void Func_020072ea();
void Func_020072f6();
s32 Func_02007304();
void Func_02007314();
void Func_0200731a();
void Func_02007320();
void Func_02007328();
void Func_0200732e();
void Func_02007340();
void Func_02007344();
void Func_0200734a();
s32 Func_02007350();
void Func_02007354();
void Func_02007356();
void Func_02007364();
void Func_0200736c();
void Func_0200737a();
void Func_0200737c();
void Func_02007388();
void Func_02007394();
void Func_0200739e();
void Func_020073a4();
void Func_020073ac();
s32 Func_020073b4();
void Func_020073b8();
void Func_020073bc();
s32 Func_020073c0();
void Func_020073c6();
void Func_020073d4();
void Func_020073d8();
void Func_020073e8();
void Func_02007404();
void Func_0200740a();
void Func_0200740c();
void Func_0200741c();
void Func_0200741e();
void Func_02007422();
void Func_0200742e();
void Func_02007430();
void Func_02007436();
void Func_0200743e();
void Func_02007442();
void Func_02007448();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void RunEventScript01(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base5_200cec8;
    s32 base5_e9b;
    s32 base5_ea1;

    Call3(Func_020070ac, 26, 0x3000, 0);
    Call3(Func_020070b8, 24, 0xd000, 0);
    Call3(Func_020070c4, 25, 0xb000, 0);
    Call3(Func_020070d0, 9, 0x3000, 0);
    Call3(Func_020070dc, 10, 0xd000, 20);
    Func_0200707c(26, 3);
    Func_02007084(24, 3);
    Func_0200708c(25, 3);
    Func_02007094(9, 3);
    Func_020070a4(25, 3);
    Func_02007002(20);
    Call2(Func_02007136, 0x10000, 0x2000);
    Call4(Func_0200714e, 0x860000, -1, 0x4ab0000, 1);
    Call3(Func_02007070, 26, 0x19999, 0xcccc);
    Call3(Func_0200707a, 9, 0x19999, 0xcccc);
    Value2(Func_0200708a, 26, 0x200cab4);
    Call2(Func_020070aa, 9, 0x200ca78);
    Func_02007208(158);
    Call3(Func_02006fea, 0x200d7a0, 38, 72);
    Func_02007058(10);
    Call3(Func_020070ea, 9, 149, 0x497);
    Func_02007104(9, 0, 0);
    Call3(Func_020070fe, 25, 250, 0x4be);
    Func_02007232();
    Call3(Func_0200718e, 10, 0x3000, 0);
    Call3(Func_0200719a, 24, 0x3000, 0);
    Call3(Func_020071a6, 25, 0x3000, 0);
    Func_02007146(10, 5);
    Func_0200714e(24, 6);
    Func_02007156(25, 6);
    rec7 = Value1(Func_020070f4, 10);
    record = Func_02007022();
    *(u16 *)(rec7 + 100) = (Func_02007008(record, 90) + 60);
    rec7 = Value1(Func_0200710c, 24);
    record = Func_0200703a();
    *(u16 *)(rec7 + 100) = (Func_02007020(record, 90) + 60);
    rec7 = Value1(Func_02007124, 25);
    record = Func_02007052();
    *(u16 *)(rec7 + 100) = (Func_02007038(record, 90) + 60);
    base5_200cec8 = (s32)Data_0200cec8;
    Func_02007158(10, base5_200cec8);
    Value2(Func_02007160, 24, base5_200cec8);
    Func_02007168(25, base5_200cec8);
    Func_02007176(26);
    ((void (*)())Func_02007124)(10);
    Func_020072ea(159);
    Call3(Func_020070cc, 0x200d7e2, 38, 72);
    Func_0200713a(30);
    Func_020072f6();
    Call4(Func_0200727e, 0x700000, -1, 0x4c90000, 1);
    Func_02007314(158);
    Call3(Func_020070f6, 0x200d78a, 35, 73);
    Func_02007164(20);
    Func_02007320();
    Call2(Func_020071c0, 9, 0x200cb28);
    Func_02007176(20);
    Call2(Func_020071ce, 26, 0x200cb9c);
    Func_02007184(40);
    Func_0200734a(159);
    Call3(Func_0200712c, 0x200d7cc, 35, 73);
    Func_020071f2(26);
    Func_02007356();
    Func_020071a4(40);
    base5_e9b = (s32)Data_00000e9b;
    Func_0200728c(base5_e9b);
    Func_020072ae(9, 0, 20);
    Func_02007266(26, 3);
    Call3(Func_020072c0, 0x201a, 0, 40);
    Func_02007270(9, 3);
    Func_02007280(26, 3);
    Func_020071de(30);
    Value2(Func_02007236, 9, 0x200cc0c);
    Call2(Func_0200723e, 26, 0x200cc5c);
    Func_020071f4(40);
    Call2(Func_02007328, 0x20000, 0x4000);
    Call4(Func_02007340, 0x690000, -1, 0x43e0000, 1);
    Func_0200726e(9);
    Func_02007328(9, 0, 0);
    Call3(Func_02007344, 9, 0x100, 40);
    Func_0200732e(9, 0, 10);
    Call3(Func_0200734a, 0, 0x8000, 0);
    Call3(Func_02007356, 22, 0x8000, 10);
    Call3(Func_020072e0, 9, 105, 0x43e);
    Func_02007320(9, 2);
    Value2(Func_02007350, 0x8009, 0);
    Func_0200737a(22, 0, 0);
    if (Value2(Func_020072aa, 0, 0) == 0) {
        Func_0200732e(9, 3);
        Func_0200736c((base5_e9b + 4));
    } else {
        Func_02007356(9, 2);
        Func_0200737c((base5_e9b + 5));
    }
    Call2(Func_02007394, 0x8009, 0);
    Call3(Func_020073b8, 22, 0x8000, 40);
    Call3(Func_020073d4, 9, 0x100, 30);
    base5_ea1 = (s32)Data_00000ea1;
    Func_020073a4(base5_ea1);
    Value2(Func_020073b4, 0x8009, 0);
    if (Value2(Func_02007304, 0, 0) == 0) {
        Func_02007388(9, 3);
        Func_020073c6((base5_ea1 + 1));
        Call3(Func_020073e8, 0x8009, 0, 30);
        Call3(Func_02007404, 22, 0x8000, 20);
        Func_020073a4(0, 3);
        Func_020073ac(22, 3);
        Func_020073bc(9, 3);
        Func_0200731a(40);
    } else {
        Call3(Func_0200743e, 9, 0x105, 90);
        Call3(Func_02007448, 9, 0x103, 40);
        Func_020073d8(9, 4);
        Func_0200741e((base5_ea1 + 2));
        Call2(Func_02007436, 0x8009, 0);
    }
    Call2(Func_0200739e, 9, 0x200cca8);
    Func_02007354(90);
    Func_0200742e(0, 22, 0);
    Func_02007364(40);
    Func_0200740c(0, 3);
    Func_0200741c(22, 3);
    Func_0200737a(20);
    Func_02007422(22, 2);
    record = Value1(Func_020073c0, 0);
    if (record != 0) {
        Func_0200740a(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02007430(22);
    Func_02007442(22, 0, 0);
}
