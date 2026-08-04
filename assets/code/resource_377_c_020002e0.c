#include "types.h"

extern u8 *Data_03001ebc;

extern s32 Func_02001bd4(s32);
extern void Func_02001d34(void);
extern void Func_02001d66(void);
extern void Func_02001d72(void);
extern void Func_02001d40(s32);

void Func_020002e0(s32 object)
{
    if (Func_02001bd4(0x834) != 0) {
        Func_02001d34();
    }
    *(s32 *)(Data_03001ebc + 448) = 521;
    Func_02001d66();
    Func_02001d72();
    Func_02001d40(object);
}
