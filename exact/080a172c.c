#include "types.h"

extern s32 Func_08004080(void);
extern s32 Func_08003fa4();
extern s32 Func_080150c8();
s32 Func_080a172c(s32 first, s32 unused, s32 second, s32 third)
{
    s32 entry_no;
    s32 result;
    result = 0;
    entry_no = Func_08004080();
    if (entry_no != 0) {
        Func_08003fa4(entry_no, 0x80, 0x080AEA4C);
        result = Func_080150c8(entry_no, 0x40000000, first, second, third);
    }
    return result;
}
