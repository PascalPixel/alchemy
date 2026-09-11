#include "types.h"
#include "scene.h"

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[65];
    u8 f55;
    u8 f56[14];
    s16 f64;
} Obj;

extern u8 gOv[];

extern Obj *State_Run(s16);

    State_SetMode(o, a->f08, a->f0c + 0x240000, a->f10);
    o->f55 = 0;
    State_Apply(o, (s32)gOv);
    State_Do(83);
    a->f64 = 0;
    return 0;
}
