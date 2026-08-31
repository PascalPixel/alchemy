#include "types.h"

#define FieldScene_RunLateSequence Func_020031b8

/* Audited retained field-scene body at 0x020031b8.
 * This reconstruction preserves 89 decoded calls, 2 loop(s), and 19 explicit memory operation(s)
 * across the complete production body. The approved compiler changes its
 * scheduling, allocation, control-flow lowering, and literal placement. */

void Func_020032d6();
void Func_02003536();
void Func_02004aba();
void Func_02004c98();
void *Func_02006376();
void Func_0200652a();
void Func_02006d4e();
void Func_02006d8e();
s32 Func_02006dd2();
void Func_02006dd4();
void Func_02006dde();
void *Func_02006de4();
void Func_02006de6();
void Func_02006df4();
void Func_02006e94();
void Func_02006e96();
void Func_02006ea2();
void Func_02006ecc();
void Func_02006ece();
void Func_02006ed4();
void Func_02006ed6();
void Func_02006ef2();
void *Func_02006ef6();
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
void Func_02006f88();
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

void Func_020031b8(void)
{
    s32 base;
    void *p1;
    void *p17;
    void *p2;
    s32 i1;
    s32 i2;

    p1 = Func_02006de4(0);
    p2 = Func_02006376();
    *(u8 *)((u8 *)(p2) + 34) = 2;
    if (Func_02006dd2() <= 0) {
        Func_02006d8e();
        base = 13107;
        Func_02006d4e(15);
        Func_02006fbc(185);
        *(s32 *)(p2 + 48) = base;
        *(s32 *)(p2 + 52) = base;
        Func_02006dd4();
        *(s32 *)(p1 + 48) = base;
        *(s32 *)(p1 + 52) = base;
        Func_02006de6();
        Func_02006df4();
        Func_02006fd8();
        *(s32 *)(p2 + 8) = 0;
        *(s32 *)(p2 + 16) = 0;
        *(s32 *)(p2 + 36) = 0;
        *(s32 *)(p2 + 44) = 0;
        Func_02006dde();
        Func_0200652a();
    }
    Func_02006ece(8);
    Func_02006ed6(9);
    Func_02006e96(770);
    Func_020032d6();
    p17 = Func_02006ef6(11);
    Func_02006ed4();
    Func_02006e94(7, 44, 1, 1, 1);
    for (i1 = 67; i1 < 74; i1++) {
        Func_02006ea2(67, 58, 78, 41, 1, 5);
        Func_02006ef8(4);
        Func_02006ef2(770);
    }
    Func_02006ecc(16, 109, 13, 109, 3, 2);
    Func_02006f22(40);
    *(s32 *)(p17 + 24) = 6553;
    *(s32 *)(p17 + 28) = 6553;
    Func_02006fc0(11, 9830400, 47710208, 6553);
    Func_02006f88(11, 33604200);
    Func_02006f02(67, 64, 71, 44, 1, 2);
    Func_02006f12(67, 64, 72, 44, 1, 2);
    Func_02006f22(67, 68, 73, 43, 1, 2);
    Func_02006f32(67, 68, 74, 43, 1, 2);
    Func_02006f42(67, 64, 75, 44, 1, 2);
    Func_02006f52(67, 66, 76, 44, 1, 2);
    Func_02006f62(67, 64, 77, 44, 1, 2);
    Func_02006f72(67, 64, 78, 44, 1, 2);
    Func_02006f82(67, 64, 79, 44, 1, 2);
    Func_02006f92(67, 66, 80, 44, 1, 2);
    Func_02006fa2(2, 0, 9, 42, 2, 2);
    Func_02006ff8(40);
    Func_02006fb8(68, 64, 71, 44, 1, 2);
    Func_02006fc8(68, 64, 72, 44, 1, 2);
    Func_02006fd8(68, 68, 73, 43, 1, 2);
    Func_02006fe8(68, 68, 74, 43, 1, 2);
    Func_02006ff8(68, 64, 75, 44, 1, 2);
    Func_02007008(68, 66, 76, 44, 1, 2);
    Func_02007018(68, 64, 77, 44, 1, 2);
    Func_02007028(68, 64, 78, 44, 1, 2);
    Func_02007038(68, 64, 79, 44, 1, 2);
    Func_02007048(68, 66, 80, 44, 1, 2);
    Func_02007058(4, 0, 9, 42, 2, 2);
    Func_020070ae(40);
    Func_02007072(7, 11, 7, 42, 10, 8);
    Func_02007084(71, 12, 71, 43, 10, 13);
    Func_020070a0(6, 13, 12, 12, 6, 44);
    Func_020070ee(40);
    Func_02004aba();
    Func_020070bc(0, 1, 1, 1, 7, 44);
    Func_02007118();
    if (Func_020070f6(771) != 0) {
    } else {
        Func_02003536();
        Func_0200712c();
        for (i2 = 67; i2 < 74; i2++) {
            Func_020070ea(67, 58, 107, 41, 1, 5);
            Func_02007140(4);
            Func_0200713a(771);
        }
        Func_02007128(45, 109, 42, 109, 3, 2);
        Func_0200717e(40);
        Func_02007140(67, 64, 102, 44, 1, 2);
        Func_02007150(67, 64, 103, 44, 1, 2);
        Func_02007160(67, 64, 104, 44, 1, 2);
        Func_02007170(67, 66, 105, 44, 1, 2);
        Func_02007180(67, 64, 106, 44, 1, 2);
        Func_02007190(67, 64, 107, 44, 1, 2);
        Func_020071a0(67, 64, 108, 44, 1, 2);
        Func_020071b0(67, 66, 109, 44, 1, 2);
        Func_02007206(40);
        Func_020071c6(68, 64, 102, 44, 1, 2);
        Func_020071d6(68, 64, 103, 44, 1, 2);
        Func_020071e6(68, 64, 104, 44, 1, 2);
        Func_020071f6(68, 66, 105, 44, 1, 2);
        Func_02007206(68, 64, 106, 44, 1, 2);
        Func_02007216(68, 64, 107, 44, 1, 2);
        Func_02007226(68, 64, 108, 44, 1, 2);
        Func_02007236(68, 66, 109, 44, 1, 2);
        Func_0200728c(40);
        Func_02007250(38, 14, 38, 44, 8, 4);
        Func_02007262(102, 14, 102, 44, 8, 12);
        Func_0200727e(37, 13, 10, 12, 37, 43);
        Func_020072cc(40);
        Func_02004c98();
        Func_020072e4();
    }
}
