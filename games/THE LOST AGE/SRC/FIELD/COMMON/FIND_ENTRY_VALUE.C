#include "TYPES.H"

u8 *Func_080ed804(void);

s32 Func_080c9e1c(s32 id)
{
    u8 *p = Func_080ed804();

    while (*p != 0) {
        if (*p == id && p[1] != 0)
            return p[1];
        p += 20;
    }
    return 1;
}
