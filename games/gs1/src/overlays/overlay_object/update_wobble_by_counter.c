#include "types.h"

struct Object {
    u8 filler00[24];
    s32 x;
    s32 z;
    u8 filler20[68];
    s16 counter;
    s16 mode;
};

extern s32 Func_02002222(void);
extern s32 Func_02002210(s32, s32);
extern s32 Func_02002230(void);
extern s32 Func_0200221e(s32, s32);

s32 OverlayObject_UpdateWobbleByCounter(struct Object *obj)
{
    switch (obj->counter) {
    case 6:
        obj->x += 0xffffc000;
        obj->z += 0x2000;
        break;
    case 4:
        obj->x += 0x2000;
        obj->z += 0xf868f001;
        break;
    case 2:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        break;
    case 0:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        if (obj->mode != 0) {
            obj->counter = Func_02002210(Func_02002222(), 40) + 40;
        } else {
            obj->counter = Func_0200221e(Func_02002230(), 20) + 20;
        }
        break;
    }
    obj->counter--;
    return 1;
}
