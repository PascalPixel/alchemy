#include "types.h"

typedef struct {
    u8 filler0[6];
    u16 unk06;
} Spr;

s32 Func_0200604c(void);
s32 Func_0200605a(void);

s32 Func_02000424(Spr *s)
{
    u16 *q = (u16 *)((u8 *)s + 0x66);
    s32 c = *q;
    s16 v = *(s16 *)q;

    if (v == 0) {
        {
            s32 t = ((u32)(Func_0200604c() << 15)) >> 16;
            s->unk06 = s->unk06 + t;
        }
        {
            s32 n = ((u32)(Func_0200605a() * 80)) >> 16;
            *q = n;
            if (n == 0) {
                goto out;
            }
            c = n;
        }
    }
    *q = c - 1;
out:
    return 1;
}
