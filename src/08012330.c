#include "types.h"

struct Work_08012330 {
    s32 unknown_00;
    s32 value_04;
    s32 value_08;
    s32 value_0c;
};

void Func_08012330(s32 value0, s32 value1, s32 value2)
{
    struct Work_08012330 *work;

    work = *(struct Work_08012330 **)0x03001E70;
    if (value0 >= 0) {
        work->value_04 = value0;
    }
    if (value1 >= 0) {
        work->value_08 = value1;
    }
    if (value2 >= 0) {
        work->value_0c = value2;
    }
}
