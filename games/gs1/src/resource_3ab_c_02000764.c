#include "types.h"
extern u8 *Data_03001ebc;

extern void Func_02002148(void);
extern s32 Func_0200212e(s32);
extern void Func_020021e8(s32);
extern void Func_02002200(s32, s32);
extern void Func_020021f8(s32);
extern s32 Func_02002208(s32, s32);
extern s32 Func_02002190(s32, s32);
extern s32 Func_0200222c(s32, s32);
extern s32 Func_020021b4(s32, s32);
extern void Func_02002256(s32, s32);
extern void Func_020021c2(void);
void Func_02000764(void)
{
    Func_02002148();
    if (Func_0200212e(0x85a) == 0) {
        Func_020021e8(0x1be1);
        Func_02002200(18, 0);
    } else {
        Func_020021f8(0x1b9f);
        Func_02002208(18, 0);
        if (Func_02002190(0, 0) == 0) {
            u8 **base = &Data_03001ebc;
            (*(u16 *)(*base + 472))++;
            Func_0200222c(18, 0);
            if (Func_020021b4(0, 0) == 1)
                (*(u16 *)(*base + 472))++;
        }
        Func_02002256(18, 0);
    }
    Func_020021c2();
}
