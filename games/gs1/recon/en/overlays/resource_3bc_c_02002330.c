#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02002330

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 36 calls, 1 loop(s), and 2 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_0200242c();
void Func_0200469e();
void Func_020046b8();
void Func_02004710();
void Func_02004bd6();
s32 Func_02004dec();
void Func_02004fcc();
void Func_02005048();
void Func_020059ea();
void Func_02005c88();
void Func_02005cd0();
void Func_02005d36();
void Func_02006c72();
void *Func_02006d50();
void Func_02006d86();
void Func_02006dd8();
void Func_02006de0();
void Func_02006de6();
void Func_02006df6();
void Func_02006e16();
void *Func_02006e2e();
void Func_02006e56();
void Func_02006e5e();
void Func_02006e6a();
void Func_02006e6c();
void Func_02006e84();
void Func_02006e90();
void Func_02006ea8();
void Func_02006ed0();
void Func_02006eee();
void Func_02006ef0();
void Func_02006f14();
void Func_02006f20();
void Func_02006f3a();
void Func_02006f52();
void Func_02006f64();

void Func_02002330(void)
{
    void *p2;
    void *p23;
    s32 i1;

    Func_02004bd6();
    p2 = Func_02006d50();
    if (Func_02004dec() != 0) {
    } else {
        Func_02006e16(8387);
        Func_02006e6a(196608, 24576);
        Func_02006e84(70778880, -1, 11010048, 1);
        Func_02006e90();
        Func_02006d86(30);
        Func_02006e56();
        Func_02006e5e();
        Func_020059ea(0, 984, 184);
        Func_02006de0(0, 98304, 49152);
        Func_020046b8(0, 992, 184);
        Func_02006df6(0, 19660, 9830);
        Func_0200469e(0, 1120, 184);
        Func_02006dd8(120);
        Func_02006ed0(0, 257);
        Func_02006de6(120);
        Func_02005cd0(0);
        Func_02006e6c(0, 1);
        Func_02006eee(0, 256);
        Func_02006ef0(0, 261, 0);
        p23 = Func_02006e2e(0);
        for (i1 = 119; i1 >= 0; i1--) {
            Func_0200242c();
            *(s32 *)(p23 + 8) = 0;
            Func_02006c72(1);
        }
        Func_02006f20(0, 259, 60);
        Func_02004710(0, 1120, 184);
        Func_02006f14();
        Func_02005d36(0);
        Func_02005c88(0, 498, 33555506, 1);
        Func_02006f64(0, 0);
        Func_02004fcc();
    }
    Func_02006f3a();
    Func_02006f52();
    Func_02005048();
    Func_02006ea8();
}
