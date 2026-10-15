#include "local.h"
#include "call.h"

extern const u8 gData_02002272[];

void Func_02001200(void);

void Func_02001304(void)
{
    struct ObjectRuntime *object = Object_GetById(10);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;

    Func_02001200();
    Func_02002190(gData_02002272);
    if (x == 12 && z == 15) {
        Call6((void (*)())Func_02002030, 42, 3, 7, 3, 42, 14);
        Call6((void (*)())Func_02002030, 42, 9, 10, 10, 11, 73);
    } else if (x == 13 && z == 15) {
        Call6((void (*)())Func_02002030, 42, 0, 7, 3, 42, 14);
        Call6((void (*)())Func_02002030, 42, 9, 10, 10, 11, 73);
    } else if (x == 16 && z == 15) {
        Call6((void (*)())Func_02002030, 42, 0, 7, 3, 42, 14);
        Call6((void (*)())Func_02002030, 42, 9, 10, 10, 11, 73);
    } else {
        Call6((void (*)())Func_02002030, 42, 9, 10, 10, 11, 73);
    }
}
