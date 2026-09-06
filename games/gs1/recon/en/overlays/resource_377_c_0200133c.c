#include "types.h"

/* Boundary-preserved draft for the distinct owner beginning at 0x0200133c. */

void Func_02002c4c();
s32 Func_02002c32();
void Func_02002d12();
void Func_02002c60();
void Func_02002d26();
void Func_02002d3e();
void Func_02002d10();
void Func_02002c7e();
void Func_02002d46();
void Func_02002d6a();
void Func_02002c6a();
void Func_02002cae();

void Func_0200133c(void)
{
    Func_02002c4c();
    if (Func_02002c32() != 0) {
        Func_02002d12(8, 65536);
        Func_02002c60(20);
        Func_02002d26();
        Func_02002d3e(8, 0);
    } else {
        Func_02002d10(8, 2);
        Func_02002c7e(40);
        Func_02002d46();
        Func_02002d6a(8, 0, 40);
        Func_02002c6a();
    }
    Func_02002cae();
}
