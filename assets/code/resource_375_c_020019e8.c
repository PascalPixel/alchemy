#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_02003470(void);
/* The same import at two sites, so two names: the flag set at +0x1e and again
 * at +0x2e. */
extern void Func_02003474(s32);
extern void Func_02003484(s32);
extern void Func_020035a4(s32, s32);
extern void Func_020034a8(void);

void Func_020019e8(void)
{
    u8 *workspace;

    Func_02003470();

    workspace = Data_03001ebc;
    *(u32 *) (workspace + 448) = 512;
    *(u32 *) (workspace + 456) = 64;

    Func_02003474(0x87E);
    Func_020035a4(12, 1);
    Func_02003484(0x900);   /* 144 << 4 */
    Func_020034a8();
}
