#include "types.h"

struct Object {
    u8 filler00[8];
    s32 x;
};

extern void Func_02004e46(void);
extern void Func_02004e46_b(s32);
extern struct Object *Func_02004e84(s32);
extern void Func_02004e08(s32, s32, s32, s32, s32, s32);
extern void Func_02004e7a(void);

void Func_020000fc(void)
{
    struct Object *object;
    s32 x;
    s32 z;

    Func_02004e46();
    object = Func_02004e84(26);
    if ((object->x >> 20) == 42) {
        x = 41;
        z = 24;
        Func_02004e08(101, 24, 3, 4, x, z);
        Func_02004e46_b(0x859);
    }
    Func_02004e7a();
}
