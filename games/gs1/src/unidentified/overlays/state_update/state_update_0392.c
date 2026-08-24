#include "types.h"

typedef struct Ent {
    u8 pad00[9];
    u8 b01:2;
    u8 f:2;
    u8 b45:4;
} Ent;

typedef struct Rec {
    u8 pad00[0x50];
    Ent *f50;
} Rec;

extern Rec *Func_02008cea(s32);
extern Rec *Func_02008cf2(s32);
extern Rec *Func_02008d0e(s32);

void Func_020045b8(void)
{
    Ent *src;
    Ent *dst;
    Ent *dst2;

    src = Func_02008cea(0)->f50;
    dst = Func_02008cf2(22)->f50;
    dst->f = src->f;
    dst2 = Func_02008d0e(8)->f50;
    dst2->f = src->f;
}
