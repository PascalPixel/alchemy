#include "types.h"









/* Close through scene 8 when facing inward; otherwise select the story line. */
extern s32 Func_0200657c(void);
extern void Func_02006a06(s32 scene);
extern void Func_020068d4(void);
extern s32 Func_020068ba(s32 flag);
extern void Func_0200699c(s32 message);
extern s32 Func_020068cc(s32 flag);
extern void Func_020069ae(s32 message);
extern void Func_020069b6(s32 message);
extern void Func_020069ce(s32 actor, s32 mode);
extern void Func_02006912(void);
void Func_020032d0(void)
{
    if (Func_0200657c() != 0) {
        Func_02006a06(8);
        return;
    }

    Func_020068d4();
    if (Func_020068ba(0x87a) != 0)
        Func_0200699c(0x1bfc);
    else if (Func_020068cc(0x815) != 0)
        Func_020069ae(0x119d);
    else
        Func_020069b6(0x1035);
    Func_020069ce(8, 0);
    Func_02006912();
}
