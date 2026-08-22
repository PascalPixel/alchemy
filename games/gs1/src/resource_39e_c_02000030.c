#include "types.h"

struct S0030 {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
};

struct T0030 {
    u8 pad[0x50];
    struct S0030 *p;
};

void Func_02000030(struct T0030 *a, u32 v)
{
    a->p->f1 = v;
}
