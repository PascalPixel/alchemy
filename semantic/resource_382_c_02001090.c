/*
 * BYTE-EXACT and adopted 2026-08-07 with -fthumb-stack-args-before-stores and
 * -fthumb-high-move-before-store: the residuals were `movs r2, #16' sinking
 * below the two stacked-argument stores at 0x020010fa, and `mov r2, r8'
 * sinking below the byte store at 0x02001114.
 */

#include "types.h"
  
  
extern u8 * Func_02002a7c(int);
extern void Func_02002b70(int);
extern void Func_02002a02(int,int,int,int,int,int);
extern void Func_02002a12(int,int,int,int,int,int);
extern void Func_02002a80(int);
extern void Func_02002a28(int,int,int,int,int,int);
extern void Func_02002a38(int,int,int,int,int,int);
extern void Func_02002aa6(int);
extern void Func_02002a5c(int,int,int,int,int,int);
extern void Func_02002020(int,int,int);
void Func_02001090(void)
{
    u8 *leader = Func_02002a7c(0); u8 *sprite = *(u8 **)(leader + 80);
    Func_02002b70(188);
    Func_02002a02(42,33,34,16,2,2); Func_02002a12(42,35,36,16,2,2); Func_02002a80(4);
    Func_02002a28(40,33,34,16,2,2); Func_02002a38(40,35,36,16,2,2); Func_02002aa6(4);
    Func_02002a5c(33,21,2,2,3,16); leader[35] &= (u8)~1; sprite[9] |= 12; Func_02002020(64,272,11);
}
