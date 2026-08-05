#include "types.h"

/* Actor-8 presentation reset at 0x02001378, including alignment to 0x1390. */





extern void Func_02003028(void);
extern void Func_02003080(s32 actor, s32 mode);
extern void Func_0200303c(void);
void Func_02001378(void)
{
    Func_02003028();
    Func_02003080(8, 0);
    Func_0200303c();
}
