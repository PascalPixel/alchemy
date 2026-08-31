#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_020015e0

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 19 calls, 1 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_0200702a();
void Func_02007040();
void Func_0200705a();
void Func_0200706e();
void Func_020070ba();
void Func_020070c8();
void Func_02007100();
void Func_02007114();
void Func_0200712a();
void Func_0200713e();
void Func_02007176();
void Func_02007184();
void Func_02007192();
void Func_020071be();
void Func_020071fa();
void Func_02007262();
void Func_0200733a();
void Func_020073c6();

void Func_020015e0(void)
{
    s32 i1;

    Func_02007262(219);
    Func_0200702a(6);
    Func_02007040(44, 51, 1, 6);
    Func_0200705a(109, 32, 108, 32, 1, 4);
    Func_0200706e(109, 51, 109, 32, 1, 4);
    Func_020070ba(327680, 327680, 65536);
    Func_020070c8(-1, -1, 58982);
    Func_0200713e();
    Func_020070c8(42, 52, 4, 5, 42, 33);
    Func_0200733a(219);
    for (i1 = 0; i1 < 2; i1++) {
        Func_02007100(40, 32, 41, 32, 3, 6);
        Func_02007114(39, 51, 40, 32, 1, 6);
        Func_0200712a(105, 51, 106, 32, 2, 4);
        Func_02007176(327680, 327680, 65536);
        Func_02007184(-1, -1);
        Func_020071fa();
    }
    Func_020073c6(288);
    Func_02007192(106, 33, 4, 5, 42, 33);
    Func_020071be();
}
