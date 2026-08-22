#include "types.h"

extern u8 Data_0807b490[];
extern s32 Func_08078aa0(s32, s32);

s32 Func_08078ad0(s32 no, s32 value)
{
    s32 mask = 0x1ff;
    u8 entry;
    s32 result = 0;

    entry = Data_0807b490[no & mask];
    if (entry != 0) {
        result = Func_08078aa0(entry - 1, value);
    }
    return result;
}
