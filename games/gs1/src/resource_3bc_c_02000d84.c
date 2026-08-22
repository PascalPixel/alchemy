#include "types.h"

extern s32 Func_02005518(void);
extern void Func_020058c0(void);
extern void Func_02001312(void);

void Func_02000d84(void)
{
    if (Func_02005518() == 0) {
        Func_020058c0();
    } else {
        Func_02001312();
    }
}
