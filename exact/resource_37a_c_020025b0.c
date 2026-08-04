#include "types.h"

/* One import, four call sites, four distinct veneer addresses. */
s32 Func_02004f6c();
s32 Func_02004f78();
s32 Func_02004f84();
s32 Func_02004f90();

s32 Func_020025b0(void)
{
    s32 all_set = 1;

    if (Func_02004f6c(0x80b) == 0)
        all_set = 0;
    if (Func_02004f78(0x80c) == 0)
        all_set = 0;
    if (Func_02004f84(0x80d) == 0)
        all_set = 0;
    if (Func_02004f90(0x80e) == 0)
        all_set = 0;

    return all_set;
}
