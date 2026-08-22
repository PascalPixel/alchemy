#include "types.h"

typedef struct Obj {
    u8 pad0[6];
    u16 kind;       /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
    u8 pad14[0x10];
    s32 vx;         /* 0x24 */
    u8 pad28[4];
    s32 vy;         /* 0x2c */
    u8 pad30[8];
    s32 a;          /* 0x38 */
    u8 pad3c[4];
    s32 b;          /* 0x40 */
} Obj;

typedef struct Other {
    u8 pad0[0x50];
    s32 *info;      /* 0x50 */
} Other;

extern s32 Data_0200ded8[];
extern s32 Data_0200df18[];

Other *Func_02000342(s32 *vec, Obj *o);
s32 Func_02005902(Obj *o, s32 *vec);

s32 Func_020002a8(Obj *o)
{
    s32 vec[3];
    u32 idx = o->kind >> 12;
    s32 t = Data_0200ded8[idx];
    Other *r;

    vec[0] = o->x + (t & 0xffff0000);
    vec[1] = o->y;
    t <<= 16;
    vec[2] = o->z + t;
    r = Func_02000342(vec, o);
    if (r != 0) {
        u32 i = 0;
        s32 v = ((s16 *)*(s32 *)((u8 *)r->info + 0x28))[i];

        {
            s32 *p = Data_0200df18;

            do {
                if (v == *p++) {
                    goto done;
                }
                i++;
            } while (i <= 5);
        }
        o->vx = 0;
        o->vy = 0;
        o->a = 0x80000000;
        o->b = 0x80000000;
    }
    t = Data_0200ded8[idx];
    vec[0] = o->x + (t & 0xffff0000);
    vec[1] = o->y;
    t <<= 16;
    vec[2] = o->z + t;
    if (Func_02005902(o, vec) > 0) {
        o->vx = 0;
        o->vy = 0;
        o->a = 0x80000000;
        o->b = 0x80000000;
    }
done:
    return 0;
}
