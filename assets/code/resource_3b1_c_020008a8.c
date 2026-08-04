#include "types.h"

void Func_02006d74(void);
s32 Func_02006d52(s32 n);
void Func_02006e64(s32 n);
void Func_02006e74(s32 n);
void Func_02006e8c(s32 a, s32 b);
void Func_02006da8(void);

extern u8 Value_00000925;
extern u8 Value_00001e19;
extern u8 Value_00001d50;

void Func_020008a8(void) {
    Func_02006d74();
    if (Func_02006d52((s32)&Value_00000925) != 0) {
        Func_02006e64((s32)&Value_00001e19);
        Func_02006e8c(10, 0);
    } else {
        Func_02006e74((s32)&Value_00001d50);
        Func_02006e8c(10, 0);
    }
    Func_02006da8();
}
