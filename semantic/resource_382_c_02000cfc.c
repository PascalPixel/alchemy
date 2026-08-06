#include "types.h"
  
  
extern u8 * Func_020026e2(int);
extern void Func_020026c8(void);
extern int Func_0200269e(int);
extern void Func_02002770(int);
extern void Func_02002784(int);
extern int Func_020026c2(int);
extern void Func_0200168c(int);
extern void Func_02002718(void);
extern u8 * Func_0200273e(int);
void Func_02000cfc(void)
{
    u8 *actor = Func_020026e2(14); u16 *state = (u16 *)(actor + 100); *state |= 2;
    Func_020026c8();
    if (Func_0200269e(0x855) == 0) {
        Func_02002770(0x123c);
    } else {
        Func_02002784(0x1349);
        if (Func_020026c2(2) != 0) { u8 *workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    }
    Func_0200168c(14); Func_02002718(); actor = Func_0200273e(14); *(u16 *)(actor + 100) &= 1;
}
