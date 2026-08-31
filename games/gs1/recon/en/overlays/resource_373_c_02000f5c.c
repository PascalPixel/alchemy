#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000f5c

/* Audited retained field-scene body at 0x02000f5c.
 * The complete production body preserves 60 decoded calls, 2 loop(s), and 23 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02006e98();
void Func_02006eb4();
void Func_02006ec8();
void Func_02006ef8();
void Func_02006f94();
void Func_02006fbc();
void Func_02006fc4();
void *Func_02006fd6();
void *Func_02006fde();
void Func_02006ff2();
void Func_02007022();
void Func_02007030();
void Func_02007064();
s32 Func_0200706e();
void Func_02007076();
void Func_0200707a();
void Func_0200707e();
s32 Func_02007082();
void Func_020070a6();
void Func_020070a8();
void Func_020070aa();
void *Func_020070ae();
void Func_020070b0();
void Func_020070be();
void Func_020070c2();
void Func_020070c4();
void Func_020070c8();
void Func_020070d4();
void Func_020070d6();
void Func_020070d8();
void Func_020070da();
void Func_020070de();
void Func_020070e8();
void Func_020070ee();
void Func_020070f0();
void Func_020070fe();
void Func_0200710a();
void Func_0200710e();
void Func_02007112();
void Func_02007114();
void Func_02007120();
void Func_0200712e();
void Func_02007142();
void Func_0200714e();
void Func_02007156();
void Func_02007174();
s32 Func_0200717a();
void Func_02007196();
void Func_0200719a();
void Func_020071a2();
void Func_020071bc();
void Func_020071c6();
void Func_020071d0();
void Func_020071d4();
void Func_020071e0();
void Func_020071f2();
void Func_0200724e();
void Func_02007280();
void Func_020072a2();
void Func_020072ae();

void Func_02000f5c(void)
{
    s32 base;
    void *p1;
    void *p13;
    void *p14;
    s32 i1;
    s32 i2;

    p1 = Func_020070ae(158);
    Func_02006e98(33617264, 49, 69);
    Func_02006f94(0, 326, 1126);
    Func_0200707a(8);
    Func_020070de(158);
    Func_02006ec8();
    Func_02006fc4(0, 374, 1238);
    Func_020070aa(9);
    Func_0200710e(158);
    Func_02006ef8(33617242, 35, 74);
    Func_02006ff2(0, 102, 1206);
    Func_020070d8(10);
    p13 = Func_02006fd6(0);
    p14 = Func_02006fde(5);
    Func_02006fbc();
    *(s32 *)(p14 + 8) = *(s32 *)(p13 + 8);
    *(s32 *)(p14 + 12) = *(s32 *)(p13 + 12);
    *(s32 *)(p14 + 16) = *(s32 *)(p13 + 16);
    *(s32 *)(p14 + 56) = *(s32 *)(p13 + 16);
    *(s32 *)(p14 + 60) = *(s32 *)(p13 + 16);
    *(s32 *)(p14 + 64) = *(s32 *)(p13 + 16);
    *(s32 *)(p14 + 36) = *(s32 *)(p13 + 16);
    *(s32 *)(p14 + 40) = *(s32 *)(p13 + 16);
    *(s32 *)(p14 + 44) = *(s32 *)(p13 + 16);
    *(s32 *)(p14 + 20) = *(s32 *)(p13 + 12);
    Func_02006eb4(1);
    Func_02007022(5, 65536, 32768);
    Func_02007064(5, 110, 283);
    Func_020070be(0, 5, 2);
    Func_020070d4(3897);
    Func_020070fe(40965, 0, 2);
    Func_0200710a(32773, 0, 2);
    Func_020070c2(0, 3);
    Func_02007030(2);
    Func_020070c8(5, 2);
    if (Func_0200706e(0) != 0) {
        Func_020070a8(5);
    }
    Func_020070d6(5);
    Func_020070e8(5, 0, 0);
    Func_020070da(0, 110, 303);
    Func_0200707e();
    if (Func_02007082(2056) == 0) {
        Func_020070b0();
        Func_020070ee(0, 65536, 32768);
        Func_0200714e(0, 1);
        Func_020070c4(2);
        base = 3917;
        Func_0200719a(base);
        Func_020071bc(15, 0, 2);
        Func_020071c6(16, 0, 2);
        Func_02007114(0);
        for (i1 = 0; i1 != 40; i1++) {
            *(s32 *)(p14 + 8) = *(s32 *)(p14 + 8);
            Func_02007112(1);
            Func_02007076();
        }
        Func_02007120(60);
        Func_020070f0(3919, 1);
        Func_0200712e(6);
        for (i2 = 0; i2 != 40; i2++) {
            *(s32 *)(p14 + 8) = *(s32 *)(p14 + 8);
            Func_02007142(1);
            Func_020070a6();
        }
        Func_02007156(60);
        Func_020071d0(0, 70, 741);
        Func_02007174();
    }
    if (Func_0200717a(2056) == 0) {
        Func_020071a2();
        Func_020071e0(0, 65536, 32768);
        base = 3917;
        Func_02007280();
        Func_020072a2(15, 0, 2);
        Func_020072ae(16, 0, 2);
        Func_02007196();
        Func_020071d4(6);
        Func_0200724e(0, 69);
        Func_020071f2();
    }
}
