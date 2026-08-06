#include "types.h"
extern u8 *Data_03001ebc;  
 
 
 
   
extern void Func_02002554(void);
extern u8 * Func_0200257e(s32);
extern void Func_020026c4(s32);
extern void Func_02002532(s32,u16,u16);
extern void Func_020025c8(s32,s32,s32);
extern u8 * Func_020025c6(s32);
extern void Func_0200261c(s32,s32);
extern void Func_0200260c(s32,s32,s32);
extern void Func_020025c2(s32);
extern void Func_020026aa(s32);
extern void Func_020026c6(void);
extern void Func_020026d2(void);
extern void Func_020025e6(void);
void Func_02000b6c(void)
{
    u8 *workspace = *(u8 **)Data_03001ebc;
    s32 slot;
    s32 index;
    const u8 *entry;

    Func_02002554();
    for (slot = 8; slot <= 65; slot++) {
        u8 *actor = Func_0200257e(slot);
        if (actor != 0)
            actor[0x55] = 0;
    }
    index = *(s16 *)(workspace + 364) - 4;
    Func_020026c4(158);
    entry = (const u8 *)0x0200a50c + index * 8;
    Func_02002532(*(s32 *)entry, *(u16 *)(entry + 4), *(u16 *)(entry + 6));
    Func_020025c8(0, 0x8000, 0x4000);
    Func_020025c6(0)[0x55] = 0;
    Func_0200261c(0, 2);
    if (index != 6) {
        Func_0200260c(0, 2, -8);
        Func_020025c2(10);
    }
    Func_020026aa(*(s16 *)(workspace + 364));
    Func_020026c6();
    Func_020026d2();
    Func_020025e6();
}
