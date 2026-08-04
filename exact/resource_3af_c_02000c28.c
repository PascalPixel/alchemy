#include "types.h"

extern u8 *Data_03001ebc;
extern void Func_02004f1c(void);
extern void Func_02001804(void);
extern void Func_02001844(void);
extern void Func_02004f7e(s32, s32, s32);
extern void Func_02004fd2(s32, s32, s32);
extern void Func_02004f58(s32);
extern void Func_020050d8(s32);
extern void Func_020050ae(s32);

void Func_02000c28(void)
{
    u8 *b = Data_03001ebc;
    s32 f;

    Func_02004f1c();
    f = 0;
    switch (*(s16 *)(b + 0x16C)) {
    case 1:
        f = 1;
        Func_02001804();
        break;
    case 3:
        f = 1;
        Func_02001844();
        break;
    }
    if (f != 0) {
        Func_02004f7e(0, 0x9999, 0x4CCC);
        Func_02004fd2(0, 1, -10);
        Func_02004f58(10);
    } else {
        Func_020050d8(0x7B);
    }
    Func_020050ae(*(s16 *)(b + 0x16C));
}
