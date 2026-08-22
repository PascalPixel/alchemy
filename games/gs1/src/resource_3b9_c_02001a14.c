#include "types.h"

extern u8 Data_02000240[];
extern u8 Value_0000008c;
extern u8 Value_0000008e;
extern void Func_02003476(void);
extern void Func_020036a2(void);

s32 Func_02001a14(void)
{
    s32 off = 448;
    s16 status = *(s16 *)(Data_02000240 + off);

    if (status == (s32)&Value_0000008c) {
        Func_02003476();
    } else if (status == (s32)&Value_0000008e) {
        Func_020036a2();
    }
    return 0;
}
