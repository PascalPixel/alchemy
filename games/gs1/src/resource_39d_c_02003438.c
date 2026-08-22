#include "types.h"








/* Configure and publish the scene's presentation layer. */
extern void Func_02006c50(s32 value, s32 mode);
extern void Func_02006c60(s32 left, s32 right);
extern void Func_02006c7c(void);
extern void Func_02006c5a(s32 enabled);
extern void Func_02006c76(void);
extern void Func_02006c82(void);
void Func_02003438(void)
{
    Func_02006c50(93, 1);
    Func_02006c60(24, 9);
    Func_02006c7c();
    Func_02006c5a(1);
    Func_02006c76();
    Func_02006c82();
}
