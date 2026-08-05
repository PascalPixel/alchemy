#include "types.h"
extern u8 *Func_020027f2(int); extern void Func_020027d6(void); extern int Func_020027cc(int);
extern void Func_020028a6(int); extern void Func_02002856(int,int); extern void Func_020027ec(int);
extern void Func_020028be(int,int); extern void Func_02002822(void);
void Func_02000e0c(void)
{
    u8 *actor = Func_020027f2(19); actor[91] = 1; Func_020027d6();
    if (Func_020027ac(0x855) == 0) {
        Func_0200287e(0x1241); Func_02002856(19, 0); Func_020027ec(2);
    } else if (Func_020027cc(0x858) != 0) {
        Func_0200289e(0x13ab);
    } else {
        Func_020028a6(0x134e);
    }
    Func_020028be(19, 0); Func_02002822(); actor[91] = 0;
}
