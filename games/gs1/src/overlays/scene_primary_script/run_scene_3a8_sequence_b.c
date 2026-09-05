#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
extern volatile s32 Data_0200bd84[];
void Func_02004aba();
void Func_02004c98();
s32 Func_02006376();
void Func_0200652a();
void Func_02006d4e();
void Func_02006d8e();
s32 Func_02006dd2();
void Func_02006dd4();
void Func_02006dde();
s32 Func_02006de4();
void Func_02006de6();
void Func_02006df4();
void Func_02006e94();
s32 Func_02006e96();
void Func_02006ea2();
void Func_02006ecc();
s32 Func_02006ece();
void Func_02006ed4();
s32 Func_02006ed6();
void Func_02006ef2();
s32 Func_02006ef6();
void Func_02006ef8();
void Func_02006f02();
void Func_02006f12();
void Func_02006f22();
void Func_02006f32();
void Func_02006f42();
void Func_02006f52();
void Func_02006f62();
void Func_02006f72();
void Func_02006f82();
s32 Func_02006f88();
void Func_02006f92();
void Func_02006fa2();
void Func_02006fb8();
void Func_02006fbc();
void Func_02006fc0();
void Func_02006fc8();
void Func_02006fd8();
void Func_02006fe8();
void Func_02006ff8();
void Func_02007008();
void Func_02007018();
void Func_02007028();
void Func_02007038();
void Func_02007048();
void Func_02007058();
void Func_02007072();
void Func_02007084();
void Func_020070a0();
void Func_020070ae();
void Func_020070bc();
void Func_020070ea();
void Func_020070ee();
s32 Func_020070f6();
void Func_02007118();
void Func_02007128();
void Func_0200712c();
void Func_0200713a();
void Func_02007140();
void Func_02007150();
void Func_02007160();
void Func_02007170();
void Func_0200717e();
void Func_02007180();
void Func_02007190();
void Func_020071a0();
void Func_020071b0();
void Func_020071c6();
void Func_020071d6();
void Func_020071e6();
void Func_020071f6();
void Func_02007206();
void Func_02007216();
void Func_02007226();
void Func_02007236();
void Func_02007250();
void Func_02007262();
void Func_0200727e();
void Func_0200728c();
void Func_020072cc();
void Func_020072e4();

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


void Func_020032a4(void)
{
    s32 rec8;
    s32 rec4;
    s32 rec7;
    s32 rec2;
    u32 i;
    s32 v5;
    s32 v6;
    s32 v7;

    rec8 = Value1(Func_02006ece, 8);
    rec4 = Value1(Func_02006ed6, 9);
    rec7 = Value1(Func_02006e96, 0x302);
    if (rec7 != 0) {
    } else if ((*(volatile s32 *)(rec8 + 8) >> 19) > 29) {
    } else {
        rec2 = Value1(Func_02006ef6, 11);
        Func_02006ed4();
        Func_02006e94(7, 44, 1, 1, rec7, 1);
        i = 67;
        v7 = 1;
        v6 = 5;
        do {
            Func_02006ea2(i, 58, 78, 41, v7, v6);
            Func_02006ef8(4);
            if (i == 70) {
                Func_02006ef2(0x302);
            }
            i++;
        } while (i <= 74);
        v5 = 2;
        Func_02006ecc(16, 109, 13, 109, 3, v5);
        Func_02006f22(40);
        *(s32 *)(rec2 + 24) = 0x1999;
        *(s32 *)(rec2 + 28) = 0x1999;
        Call3(Func_02006fc0, 11, 0x960000, 0x2d80000);
        Value2(Func_02006f88, 11, 0x200c268);
        v6 = 1;
        Func_02006f02(67, 64, 71, 44, v6, v5);
        Func_02006f12(67, 64, 72, 44, v6, v5);
        Func_02006f22(67, 68, 73, 43, v6, v5);
        Func_02006f32(67, 68, 74, 43, v6, v5);
        Func_02006f42(67, 64, 75, 44, v6, v5);
        Func_02006f52(67, 66, 76, 44, v6, v5);
        Func_02006f62(67, 64, 77, 44, v6, v5);
        Func_02006f72(67, 64, 78, 44, v6, v5);
        Func_02006f82(67, 64, 79, 44, v6, v5);
        Func_02006f92(67, 66, 80, 44, v6, v5);
        Func_02006fa2(2, 0, 9, 42, v5, v5);
        Func_02006ff8(40);
        Func_02006fb8(68, 64, 71, 44, v6, v5);
        Func_02006fc8(68, 64, 72, 44, v6, v5);
        Func_02006fd8(68, 68, 73, 43, v6, v5);
        Func_02006fe8(68, 68, 74, 43, v6, v5);
        Func_02006ff8(68, 64, 75, 44, v6, v5);
        Func_02007008(68, 66, 76, 44, v6, v5);
        Func_02007018(68, 64, 77, 44, v6, v5);
        Func_02007028(68, 64, 78, 44, v6, v5);
        Func_02007038(68, 64, 79, 44, v6, v5);
        Func_02007048(68, 66, 80, 44, v6, v5);
        Func_02007058(4, 0, 9, 42, v5, v5);
        Func_020070ae(40);
        v5 = 10;
        Func_02007072(7, 11, 7, 42, v5, 8);
        Func_02007084(71, 12, 71, 43, v5, 13);
        v5 = 44;
        Func_020070a0(6, 13, 12, 12, 6, v5);
        Func_020070ee(40);
        Func_02004aba();
        Func_020070bc(0, 1, 1, 1, 7, v5);
        Func_02007118();
    }
    if (Value1(Func_020070f6, 0x303) != 0) {
    } else if ((*(volatile s32 *)(rec4 + 8) >> 19) > 87) {
    } else {
        Func_0200712c();
        i = 67;
        v7 = 1;
        v6 = 5;
        do {
            Func_020070ea(i, 58, 107, 41, v7, v6);
            Func_02007140(4);
            if (i == 70) {
                Func_0200713a(0x303);
            }
            i++;
        } while (i <= 74);
        v6 = 2;
        Func_02007128(45, 109, 42, 109, 3, v6);
        Func_0200717e(40);
        v5 = 1;
        Func_02007140(67, 64, 102, 44, v5, v6);
        Func_02007150(67, 64, 103, 44, v5, v6);
        Func_02007160(67, 64, 104, 44, v5, v6);
        Func_02007170(67, 66, 105, 44, v5, v6);
        Func_02007180(67, 64, 106, 44, v5, v6);
        Func_02007190(67, 64, 107, 44, v5, v6);
        Func_020071a0(67, 64, 108, 44, v5, v6);
        Func_020071b0(67, 66, 109, 44, v5, v6);
        Func_02007206(40);
        Func_020071c6(68, 64, 102, 44, v5, v6);
        Func_020071d6(68, 64, 103, 44, v5, v6);
        Func_020071e6(68, 64, 104, 44, v5, v6);
        Func_020071f6(68, 66, 105, 44, v5, v6);
        Func_02007206(68, 64, 106, 44, v5, v6);
        Func_02007216(68, 64, 107, 44, v5, v6);
        Func_02007226(68, 64, 108, 44, v5, v6);
        Func_02007236(68, 66, 109, 44, v5, v6);
        Func_0200728c(40);
        v5 = 8;
        Func_02007250(38, 14, 38, 44, v5, 4);
        Func_02007262(102, 14, 102, 44, v5, 12);
        Call6(Func_0200727e, 37, 13, 10, 12, 37, 43);
        Func_020072cc(40);
        Func_02004c98();
        Func_020072e4();
    }
}
