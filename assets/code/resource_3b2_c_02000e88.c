#include "types.h"

extern u8 Data_02000240[];
extern void Func_02003efc(void);
extern void Func_02003f86(s32 arg0, s32 arg1);
extern void Func_02003f8e(s32 arg0, s32 arg1);
extern void Func_02003f2a(void);

void Func_02000e88(void) {
    u8 *base;

    Func_02003efc();
    base = Data_02000240;
    Func_02003f86(*(s16 *)(base + 448), 5);
    base[555] = 3;
    Func_02003f8e(84, 5);
    Func_02003f2a();
}
