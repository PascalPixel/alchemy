#include "types.h"

typedef struct {
    u8 filler0[12];
    s32 unk12;
} T;

extern s32 Data_02009810;
extern T *Func_020017b8(s32);

s32 Func_020003dc(void)
{
    s32 *d;
    T *p;

    d = &Data_02009810;
    p = Func_020017b8(1);
    *d = p->unk12;
    return 0;
}
