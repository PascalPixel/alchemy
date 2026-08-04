#include "types.h"

extern s32 Data_03001e40;
extern s32 Func_02005d48(void);
extern u32 Func_02005d2e(s32, s32);
extern void Func_020051a8(s32);
extern void Func_020051b0(s32);

void Func_0200293c(void)
{
    if ((Data_03001e40 & 1) == 0) {
        if (Func_02005d2e(Func_02005d48(), 100) > 50) {
            Func_020051a8(1);
        } else {
            Func_020051b0(0);
        }
    }
}
