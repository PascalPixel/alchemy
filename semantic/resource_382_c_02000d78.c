#include "types.h"
  
  
extern u8 * Func_0200275e(int);
extern void Func_02002744(void);
extern int Func_0200271a(int);
extern void Func_020027ec(int);
extern void Func_02002800(int);
extern void Func_020016ee(int);
extern void Func_0200277a(void);
extern u8 * Func_020027a0(int);
void Func_02000d78(void)
{
    u8 *actor = Func_0200275e(15); *(u16 *)(actor + 100) |= 2; Func_02002744();
    if (Func_0200271a(0x855) == 0) Func_020027ec(0x123d); else Func_02002800(0x134b);
    Func_020016ee(15); Func_0200277a(); actor = Func_020027a0(15); *(u16 *)(actor + 100) &= 1;
}
