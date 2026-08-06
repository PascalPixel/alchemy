#include "types.h"
  
extern void Func_0200219e();
extern s32 Func_0200200e(void);
extern void Func_020004d0();
s32 Func_02000344(u8 *actor)
{
    u8 params[40]; u32 tick=*(volatile u32 *)0x03001e40; s32 zero=0;
    if ((tick&7)==0) Func_0200219e(118);
    if ((tick&15)==0) {
        *(s32 *)(params+8)=0xcccc; *(s32 *)(params+12)=0xcccc;
        *(unsigned short *)(params+34)=(unsigned short)(((Func_0200200e()<<12)>>16)+0xf800);
        Func_020004d0(*(s32 *)(actor+8),*(s32 *)(actor+12),*(s32 *)(actor+16),
                      0,0,0,0x00880001,params);
    }
    return zero;
}
