/* Contiguous unnamed state-owner run for resource_3bd. */

#include "types.h"

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Ent;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec;

extern Ent *Func_02004a2c(s32);
extern s32 Func_020049ce(Ent *, Vec *);

void Func_02000d58(void)
{
    u8 *obj = *(u8 **)0x03001f30;
    Ent *p = Func_02004a2c(11);
    Vec v;

    v.x = p->unk8;
    v.y = p->unkC;
    v.z = p->unk10;

    if (Func_020049ce(p, &v) > 0) {
        obj[0x35] = 1;
    }
}

#include "types.h"

extern u8 *Func_02004a64(s32);
extern void Func_02004a06(s32, s32, s32, s32, s32, s32);
extern void Func_02004a66(s32);

void Func_02000d90(void)
{
    u8 *obj = *(u8 **)0x03001f30;
    u8 *p = Func_02004a64(11);
    s32 t;

    obj += 0x35;
    t = *obj;
    t = (s8)t;
    if (t == 0) {
        s32 a = 0x49;
        s32 b = 0x11;
        Func_02004a06(0x4c, 0x10, 1, 1, a, b);
        if (p != 0) {
            s32 c = 2;
            p[0x55] = c;
            p[0x23] = t;
        }
        Func_02004a66(0x211);
    }
}

