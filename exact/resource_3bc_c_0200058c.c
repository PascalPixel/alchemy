#include "types.h"

struct Object {
    u8 filler00[8];
    s32 x;
};

extern struct Object *Func_02004fac(s32);
extern void Func_02004f72(s32, s32);
extern void Func_02004ef6(s32, s32, s32, s32, s32, s32);
extern void Func_02004f06(s32, s32, s32, s32, s32, s32);

void Func_0200058c(void)
{
    struct Object *object;
    s32 x;

    object = Func_02004fac(13);
    x = object->x >> 20;
    Func_02004f72(880, x);
    Func_02004ef6(18, 10, 3, 1, 18, 11);
    Func_02004f06(17, 11, 1, 1, x, 11);
}
