#include "types.h"

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[14];
    u8 f22;
} Obj;

extern Obj *Func_020046cc(void);

void Func_02000a10(void)
{
    Obj *o;
    s32 x;
    s32 y;

    o = Func_020046cc();
    if (o != 0) {
        x = o->f08 >> 19;
        y = o->f10 >> 19;
        if (x >= 94 && x <= 95 && y > 23 && y <= 26) {
            o->f22 = 1;
        }
    }
}
