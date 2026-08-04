#include "types.h"

struct Obj {
    u8 pad00[8];
    s32 field08;
    s32 field0c;
    s32 field10;
    u8 pad14[0x22 - 0x14];
    u8 field22;
};

struct Arg {
    s32 field00;
    s32 field04;
    s32 field08;
    s32 field0c;
    s32 field10;
    s32 field14;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct Rect {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
};

extern struct Rect Data_0200adc0[];
extern u32 Data_0200ad68[];

extern struct Obj *Func_020007de(s32 *arg0, s32 *arg1, struct Arg *arg2);
extern s32 Func_02003140(struct Obj *arg0, s32 *arg1);

s32 Func_02000474(struct Arg *arg) {
    struct Vec v;
    s32 idx;
    struct Obj *obj;
    s32 count;
    s32 ny;
    u8 *flag;
    s32 nx;
    s32 i;
    s32 j;
    s32 t;
    s32 u;
    s32 yv;
    s32 off;
    u8 *tb;
    s32 k;

    arg->field14 = 0;
    obj = Func_020007de(&idx, &arg->field04, arg);
    if (obj == 0) return 0;
    flag = &obj->field22;
    *flag = 2;
    count = 0;

    k = arg->field00;
    t = Data_0200adc0[k].b;
    if (t < 0) t = -t;
    u = Data_0200adc0[k].d;
    if (u < 0) u = -u;
    ny = (t + u) >> 4;

    t = Data_0200adc0[k].a;
    if (t < 0) t = -t;
    u = Data_0200adc0[k].c;
    if (u < 0) u = -u;
    nx = (t + u) >> 4;

    v.x = obj->field08 + (Data_0200ad68[idx] & 0xffff0000);
    yv = obj->field0c;
    v.y = yv;
    v.z = obj->field10 + (Data_0200ad68[idx] << 16);
    arg->field0c = yv;

    for (;;) {
        arg->field10 = v.z + (Data_0200adc0[arg->field00].b << 16);
        for (i = 0; i < ny; i++) {
            arg->field08 = v.x + (Data_0200adc0[arg->field00].a << 16);
            for (j = 0; j < nx; j++) {
                if (Func_02003140(obj, &arg->field08) == 2) goto hit;
                arg->field08 += 0x100000;
            }
            arg->field10 += 0x100000;
        }
        count++;
        v.x += Data_0200ad68[idx] & 0xffff0000;
        v.z += Data_0200ad68[idx] << 16;
    }
hit:
    *flag = 0;
    if (count == 0) return 0;
    arg->field08 = obj->field08
        + (s32)(Data_0200ad68[idx] & 0xffff0000) * count;
    arg->field0c = obj->field0c;
    arg->field10 = obj->field10
        + count * (s32)(Data_0200ad68[idx] << 16);
    return 1;
}
