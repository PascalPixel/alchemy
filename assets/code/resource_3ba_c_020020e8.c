#include "types.h"

typedef struct Rec {
    u8 pad00[216];
    u16 fd8[15];
} Rec;

extern Rec *Func_02005d34(s32);
extern void Func_02005d46(s32, s32);
extern void Func_02005d62(s32, s32);

void Func_020020e8(s32 a, s32 b)
{
    Rec *t = Func_02005d34(a);
    s32 i;

    Func_02005d46(a, b);
    for (i = 0; i <= 14; i++) {
        if (t->fd8[i] == b) {
            Func_02005d62(a, i);
        }
    }
}
