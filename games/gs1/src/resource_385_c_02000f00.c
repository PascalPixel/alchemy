#include "types.h"

/* Complete paired scene-resource enable wrapper through its two-word pool. */




extern void Func_02001fec(void);
extern void Func_02001fbc(s32 resource, s32 enabled);
extern void Func_02001fc4(s32 resource, s32 enabled);
extern void Func_02002008(void);
void Func_02000f00(void)
{
    Func_02001fec();
    Func_02001fbc(0x947, 1);
    Func_02001fc4(0x29dc, 1);
    Func_02002008();
}
