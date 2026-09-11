#include "types.h"
#include "scene.h"

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[20];
    s32 f28;
    u8 f2c[41];
    u8 f55;
} Obj;

extern u8 gOv[];

extern Obj *State_Run(s32, s32, s32, s32);

void State_unk2_4(Obj *a)
{
    s32 t[3];
    u32 n;

    if (a->f28 >= -255 && a->f28 <= 255) {
        a->f55 = 0;
    }
    n = State_unk3_4();
    if (n * 100 >> 16 <= 9) {
        Obj *o;
        s32 u;
        s32 w;

        t[0] = a->f08;
        t[1] = a->f0c;
        t[2] = a->f10;
        u = State_Check();
        w = State_unk2();
        State_Place(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = State_Run(285, x, y, z);
        }
        if (o != 0) {
            o->f55 = 0;
            State_Apply(o, 0);
            State_Apply2(o, (s32)gOv);
            State_Apply3(o, 1);
            State_Apply4(o, 0);
        }
    }
}
