#include "types.h"

typedef struct {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} S;

void Func_02000030(s32 obj, u32 v)
{
    S *p = *(S **)(obj + 0x50);
    p->f1 = v;
}
