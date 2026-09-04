#include "types.h"

#define RunEventScript01 Func_02001450

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020056e4();
void Func_02005704();
void Func_0200574c();
void Func_020057c4();
void Func_0200593e();
void Func_0200595e();
void Func_02005974();
void Func_020059a4();
void Func_02005e18();
void Func_02005e48();
void Func_02005e68();
void Func_02005e74();
void Func_02005eb0();
void Func_02005ec0();
void Func_02005ed0();
void Func_02005ed2();
void Func_02005ed8();
void Func_02005edc();
void Func_02005ee4();
void Func_02005ef0();
void Func_02005efa();
void Func_02005efc();
void Func_02005f04();
void Func_02005f08();
void Func_02005f0e();
void Func_02005f14();
void Func_02005f2a();
void Func_02005f34();
void Func_02005f42();
void Func_02005f5e();
void Func_02005f6e();
void Func_02005f70();
void Func_02005f88();
void Func_02005f8a();
void Func_02005f94();
void Func_02005fa2();
void Func_02005fa6();
void Func_02005fae();
void Func_02005fb2();
void Func_02005fc6();
void Func_02005fd0();
void Func_02005fd4();
void Func_02005fdc();
void Func_02005fe2();
void Func_02005fe8();
void Func_02005fee();
void Func_02005ff0();
void Func_02006014();
void Func_02006020();
void Func_02006026();
void Func_02006044();
void Func_02006050();
void Func_0200605c();
void Func_0200605e();
void Func_02006068();
void Func_02006076();
void Func_0200609a();
void Func_020060a2();
void Func_020060a6();
void Func_020060ae();
void Func_020060b2();
void Func_020060b6();
void Func_020060b8();
void Func_020060c4();
void Func_020060ca();
void Func_020060ce();
void Func_020060d0();
void Func_020060ec();
void Func_020060f2();
void Func_02006100();
void Func_02006108();
void Func_02006114();
void Func_0200611c();
void Func_02006120();
void Func_0200614a();
void Func_02006160();
void Func_02006168();
void Func_0200616a();
void Func_0200617c();
void Func_02006188();
void Func_02006194();
void Func_02006198();
void Func_020061a0();
void Func_020061ae();
void Func_020061bc();
void Func_020061c2();
void Func_020061c8();
void Func_020061f4();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;

    Call3(Func_02005ed8, 5, 0x6000, 0);
    Call3(Func_02005ee4, 9, 0x5000, 0);
    Call3(Func_02005ef0, 10, 0x8000, 0);
    Call3(Func_02005efc, 12, 0x5000, 40);
    Call3(Func_02005f08, 11, 0xd000, 40);
    Func_02005ec0(11, 2);
    Func_020056e4(11, 20);
    Func_02005ed0(12, 2);
    Call2(Func_02005f42, 12, 0x102);
    Func_02005e48(60);
    Func_02005704(12, 10);
    Func_02005efa(10, 12, 0);
    Func_02005f04(5, 12, 0);
    Func_02005f0e(9, 12, 0);
    Func_02005e74(40);
    Func_02005f14(10, 1);
    Call3(Func_02005f70, 10, 0x8000, 10);
    Func_02005f08(10, 3);
    Func_0200574c(10, 10);
    Func_02005f8a(11, 0, 10);
    Func_02005f2a(11, 3);
    Func_02005eb0(10);
    Func_02005fa2(5, 0, 0);
    Call3(Func_02005fae, 9, 0x8000, 20);
    Func_02005f5e(5, 2);
    Func_02005f6e(9, 2);
    Func_02005edc(10);
    Call3(Func_02005fd0, 5, 0xc000, 0);
    Call3(Func_02005fdc, 9, 0xb000, 10);
    Call3(Func_02005fe8, 10, 0xb000, 40);
    Func_02005f88(10, 3);
    Func_020057c4(10, 10);
    Call3(Func_02006014, 5, 0x100, 20);
    Call2(Func_02006026, 12, 0x102);
    Func_02005fc6(12, 3);
    Func_02005f34(40);
    Call3(Func_02005fb2, 1, 0x15a0000, 0x1b80000);
    Func_02005e18(1);
    Func_02006020(1, 0);
    Call3(Func_02006044, 0, 0x2000, 0);
    Call3(Func_02006050, 1, 0xa000, 20);
    Func_020060c4();
    Func_020060d0();
    Call3(Func_02005fee, 1, 0x1180000, 0x1d60000);
    Call4(Func_020060a6, 0x1050000, -1, 0x1d20000, 0);
    Func_02005ed2();
    Func_02005e68(1);
    Func_020060ec();
    Func_02006100();
    Func_02005fa6(20);
    Call3(Func_0200609a, 0, 0x3000, 10);
    Call3(Func_020060b6, 1, 0x102, 0);
    Func_0200605e(1, 3);
    Func_020060ae(1, 0);
    Func_02005fd4(10);
    Func_0200605c(1, 4);
    Func_02005fe2(10);
    Func_020060ca(1, 0);
    Func_02005ff0(10);
    Call1(Func_020060b6, 0x109b);
    Func_020060ce(11, 0);
    Call3(Func_020060f2, 11, 0xd000, 10);
    Func_020060a2(0, 2);
    Func_020060b2(0, 2);
    Func_02006020(10);
    Call3(Func_02006114, 0, 0xe000, 0);
    Call3(Func_02006120, 1, 0xe000, 20);
    Func_02006194();
    Func_020061a0();
    Call4(Func_02006168, 0x1dd0000, -1, 0x14e0000, 0);
    Func_02005f94();
    Func_02005f2a(1);
    Func_020061ae();
    Func_020061c2();
    Func_02006068(20);
    Func_02006108(9, 2);
    Func_02006076(10);
    Call3(Func_0200616a, 9, 0x8000, 30);
    Func_0200593e(9, 20);
    Func_0200617c(5, 0, 40);
    Func_0200611c(5, 4);
    Func_020060a2(20);
    Func_0200595e(5, 10);
    Func_0200614a(12, 2);
    Func_020060b8(80);
    Func_02005974(12, 20);
    Func_02006160(5, 2);
    Call3(Func_020061bc, 5, 0xc000, 0);
    Call3(Func_020061c8, 9, 0xb000, 20);
    Func_02006168(12, 3);
    Func_020059a4(12, 20);
    Func_02006188(5, 2);
    Func_02006198(9, 2);
    Call3(Func_020061f4, 12, 0xb000, 40);
}
