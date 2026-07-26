#include "types.h"

struct Fields_080a23f4 {
    u8 filler[8];
    u16 field8;
    u16 field10;
    u16 field12;
    u16 field14;
};

void Func_080a23f4(struct Fields_080a23f4 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (arg0 != NULL) {
        arg0->field8 = arg3;
        arg0->field12 = arg1;
        arg0->field10 = arg4;
        arg0->field14 = arg2;
    }
}
