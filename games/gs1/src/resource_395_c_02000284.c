#include "types.h"

extern void Func_02001c20(void);
extern s32 Func_02001c0e(s32);
extern void Func_02001cb8(s32);
extern void Func_02001cc0(s32);
extern void Func_02001cd8(s32, s32);
extern void Func_02001c4c(void);

void Func_02000284(void)
{
    Func_02001c20();
    if (Func_02001c0e(0x845) != 0) {
        Func_02001cb8(0x151E);
    } else {
        Func_02001cc0(0x14EC);
    }
    Func_02001cd8(8, 0);
    Func_02001c4c();
}
