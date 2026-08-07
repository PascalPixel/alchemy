/*
 * BYTE-EXACT and adopted 2026-08-07 with -fthumb-stack-args-before-stores: the
 * last residual was the stacked argument pair, which the reference
 * materialises into two registers -- the second one down -- before issuing
 * either `str' to sp, where one scratch register interleaves them.
 */

#include "types.h"
  
 
extern u8 * Func_0200299c(int);
extern void Func_02002a8e(int);
extern void Func_02002910(void *,int,int);
extern void Func_02002936(int,int,int,int,int,int);
extern void Func_02001ef6(int,int,int);
void Func_02000fb4(void)
{
    u8 *leader = Func_0200299c(0); u8 *sprite = *(u8 **)(leader + 80);
    Func_02002a8e(158); Func_02002910((void *)0x0200a86a, 54, 13); Func_02002936(33, 20, 1, 3, 23, 12);
    leader[35] &= (u8)~1; sprite[9] |= 12; Func_02001ef6(376, 224, 8);
}
