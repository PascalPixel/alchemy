#include "types.h"

extern s32 Func_02002704(void);
extern s32 Func_02002b14(s32);
extern void Func_02002b3c();
extern void Func_02002c2e(s32, s32);
extern void Func_02002c44(s32);
extern void Func_02002b4a(s32);
extern void Func_02002b50(s32);
extern void Func_02002b6c(void);
extern s32 Func_02002b4c(s32);
extern void Func_02002b74(void);
extern void Func_02002c64(s32, s32);
extern void Func_02002c7a(s32);
extern void Func_02002b70(s32);
extern void Func_02002b80(s32);
extern void Func_02002b86(s32);
extern s32 Func_02002b7c(s32);
extern void Func_02000bc4(void);
extern void Func_02002bb0(void);

void Func_02000150(void)
{
    if (Func_02002704() != 0) {
        if (Func_02002b14(0x200) == 0) {
            Func_02002b3c();
            Func_02002c2e(0x10000, 1);
            Func_02002c44(20);
            Func_02002b3c(0x200);
            Func_02002b4a(0x201);
            Func_02002b50(0x202);
            Func_02002b6c();
        }
    } else {
        if (Func_02002b4c(0x201) == 0) {
            Func_02002b74();
            Func_02002c64(0x2051cc, 1);
            Func_02002c7a(20);
            Func_02002b70(0x201);
            Func_02002b80(0x200);
            Func_02002b86(0x202);
            if (Func_02002b7c(0x80a) == 0) {
                Func_02000bc4();
            }
            Func_02002bb0();
        }
    }
}
