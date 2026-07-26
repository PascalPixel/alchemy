#include "types.h"

struct Object_0809280c {
    u8 padding_00[6];
    s16 angle;
    s32 y;
    u8 padding_0c[4];
    s32 x;
};

s32 Func_0808ba1c(u32 arg0);
s32 Func_080044d0(s32, s32);
void Func_0809163c(s32 arg0);

void Func_0809280c(s32 arg0, s32 arg1, s32 arg2)
{
    struct Object_0809280c *object1;
    struct Object_0809280c *object2;

    object1 = Func_0808ba1c(arg0);
    object2 = Func_0808ba1c(arg1);
    if (object1 != 0 && object2 != 0) {
        object1->angle = Func_080044d0(object2->x - object1->x,
                                      object2->y - object1->y);
        Func_0809163c(arg2);
    }
}
