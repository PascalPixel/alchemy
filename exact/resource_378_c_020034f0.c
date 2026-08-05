#include "types.h"








/* Close scene 8 when facing inward; otherwise emit its fixed story line. */
extern s32 Func_0200679c(void);
extern void Func_02006c26(s32 scene);
extern void Func_02006af4(void);
extern void Func_02006bb2(s32 message);
extern void Func_02006bca(s32 actor, s32 mode);
extern void Func_02006b0e(void);
void Func_020034f0(void)
{
    if (Func_0200679c() != 0) {
        Func_02006c26(8);
        return;
    }

    Func_02006af4();
    Func_02006bb2(0x1823);
    Func_02006bca(8, 0);
    Func_02006b0e();
}
