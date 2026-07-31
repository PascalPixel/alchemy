#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_0200342c(void);
/* The same import at two sites, so two names: the flag set at +0x1e and again
 * at +0x2e. */
extern void Func_02003430(s32);
extern void Func_02003440(s32);
extern void Func_02003560(s32, s32);
extern void Func_02003464(void);

void Func_020019a4(void)
{
    u8 *workspace;

    Func_0200342c();

    workspace = Data_03001ebc;
    *(u32 *) (workspace + 448) = 512;
    *(u32 *) (workspace + 456) = 64;

    Func_02003430(0x87D);
    Func_02003560(12, 0);
    Func_02003440(0x900);   /* 144 << 4 */
    Func_02003464();
}
