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
