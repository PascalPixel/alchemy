#include "types.h"

/* resource_3bb owner at 0x02001f14, 164 bytes, instruction-for-instruction
 * twin of resource_3bc:29ac. Prologue through the sole return at
 * 0x02001fa4-0x02001fac, then alignment and two pool words through 0x02001fb7.
 * The party-count-selected interaction accounts for all seventeen calls. */
  
  
  
  
 

extern u8 * Func_02005e6a();
extern void Func_02005e5e();
extern s32 Func_02005e3a();
extern void Func_02005f04();
extern s32 Func_02005f24();
extern void Func_02005eae();
extern void Func_02005ebc();
extern void Func_02005ee0();
extern void Func_02005e9e();
extern void Func_02005ef8();
extern void Func_02005ff0();
extern void Func_02005ffc();
extern void Func_02005fc2();
extern void Func_02005f6a();
extern void Func_02005f82();
extern void Func_02005ee6();
void Func_02001f14(s32 selector)
{
    u8 *actor = Func_02005e6a(selector);
    s32 x = *(s16 *)(actor + 10);
    s32 z = *(s16 *)(actor + 18);
    Func_02005e5e();
    if (Func_02005e3a() <= 1) {
        Func_02005f04(0x20e5);
        if (Func_02005f24(selector, 0) == 0) {
            Func_02005eae(0, 0x10000, 0x8000);
            Func_02005ebc(selector, 0x10000, 0x8000);
            Func_02005ee0(selector, x, z + 64);
            Func_02005e9e(15);
            Func_02005ef8(0, x, z);
            Func_02005f04(0, x, z + 32);
            Func_02005ff0(); Func_02005ffc(); Func_02005fc2(11);
        }
    } else {
        Func_02005f6a(0x20e8); Func_02005f82(selector, 0);
    }
    Func_02005ee6();
}
