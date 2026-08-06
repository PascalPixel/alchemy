#include "types.h"
extern s32 Func_020025d6(s32); extern s32 Func_020025e0(s32);
extern void Func_02002638(s32,s32,s32); extern u8 *Func_02002636(s32);
extern void Func_0200268a(s32,s32); extern void Func_02002676(s32,s32,s32);
extern void Func_0200262c(s32); extern void Func_02002712(s32);
void Func_02000c10(void)
{
    if (Func_020025d6(0x202) != 0 && Func_020025e0(0x201) == 0) {
        Func_02002638(0, 0x3333, 0x1999);
        Func_02002636(0)[0x55] = 0;
        Func_0200268a(0, 2);
        Func_02002676(0, 2, -8);
        Func_0200262c(13);
        Func_02002712(12);
    }
}
