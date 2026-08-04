#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern s32 Func_08019d2c();
extern s32 Func_080162d4();
extern s32 Func_0801ec6c();

s32 Func_08019da8(s32 arg0, s32 arg1, s32 x, s32 y)
{
    s32 object;
    s32 minus_four;
    s32 offset;
    void *work;

    work = *(void **)0x03001e8c;
    if (Func_08019d2c(arg0) == -1) {
        return 0;
    }
    minus_four = -4;
    offset = minus_four;
    if (M2C_FIELD(work, u8, 0xea4) != 0) {
        object = Func_080162d4(x, y, 6, 5, 2);
        offset = 0;
    } else {
        object = Func_080162d4(x, y, 5, 5, 2);
    }
    if (object != 0) {
        Func_0801ec6c(arg0, arg1, -1, object, offset, minus_four);
    }
    return object;
}
