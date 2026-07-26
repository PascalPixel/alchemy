#include "types.h"

struct Fields_080a4e68 {
    u8 filler[0x10c];
    void *object;
};

extern struct Fields_080a4e68 *Data_03001f2c;

void Func_080a23f4(struct Fields_080a23f4 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void Func_080a4e68(void) {
    Func_080a23f4(Data_03001f2c->object, 13, 0, 17, 5);
}
