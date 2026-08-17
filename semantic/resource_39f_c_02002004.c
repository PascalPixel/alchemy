#include "types.h"

extern void Func_02004dac(void);
extern void Func_02002eb8(s32, s32);
extern void Func_02004e86(s32, s32, s32, s32);
extern void Func_02002dc4(s32, s32, s32, s32);
extern void Func_02002e52(s32);
extern void Func_02004e66(s32, s32);
/* Used for its return value: r0 is not reloaded before Func_02004daa. */
extern int Func_02004e04(s32);
extern void Func_02004daa(int, s32);
extern void Func_02004df0(s32);
extern void Func_02004dee(s32);
extern void Func_02004e5c(s32, s32, s32);
extern void Func_02004e18(void);
void Func_02002004(void)
{
    /* No argument register is written before this branch: the caller's r0-r3
     * reach the import unchanged. */
    Func_02004dac();

    Func_02002eb8(18, 1);

    /* r1 is `movs r1,#1 / negs r1,r1`, i.e. -1, not 1. */
    Func_02004e86(744 << 16, -1, 504 << 16, 1);

    Func_02002dc4(18, 744, 504, 0x90000);
    Func_02002e52(18);
    Func_02004e66(18, 15);

    /* Only r1 is set for the second branch; r0 still carries what
     * Func_02004e04 returned. */
    Func_02004daa(Func_02004e04(18), 0);

    Func_02004df0(30);

    /* 0x30a from the pool word at 0x02002074. */
    Func_02004dee(0x30a);

    Func_02004e5c(22, 744 << 16, 504 << 16);

    /* No argument registers are set. */
    Func_02004e18();
}
