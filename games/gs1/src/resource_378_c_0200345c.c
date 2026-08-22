#include "types.h"









/* Close scene 8 when facing inward; otherwise choose its story line. */
extern s32 Func_02006708(void);
extern void Func_02006b92(s32 scene);
extern void Func_02006a60(void);
extern s32 Func_02006a46(s32 flag);
extern void Func_02006b28(s32 message);
extern void Func_02006b30(s32 message);
extern void Func_02006b48(s32 actor, s32 mode);
extern void Func_02006a8c(void);
void Func_0200345c(void)
{
    if (Func_02006708() != 0) {
        Func_02006b92(8);
        return;
    }

    Func_02006a60();
    if (Func_02006a46(0x845) != 0)
        Func_02006b28(0x171c);
    else
        Func_02006b30(0x1408);
    Func_02006b48(8, 0);
    Func_02006a8c();
}
