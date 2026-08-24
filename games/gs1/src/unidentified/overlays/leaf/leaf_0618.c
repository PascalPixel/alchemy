#include "types.h"









/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */
extern s32 Func_02006754(void);
extern void Func_02006bde(s32 scene);
extern void Func_02006aac(void);
extern void Func_02006b6a(s32 message);
extern s32 Func_02006a98(s32 flag);
extern void Func_02006b7a(s32 message);
extern void Func_02006b92(s32 actor, s32 mode);
extern void Func_02006ad6(void);
void Func_020034a8(void)
{
    if (Func_02006754() != 0) {
        Func_02006bde(8);
        return;
    }

    Func_02006aac();
    Func_02006b6a(0x190a);
    if (Func_02006a98(0x909) != 0)
        Func_02006b7a(0x1951);
    Func_02006b92(8, 0);
    Func_02006ad6();
}
