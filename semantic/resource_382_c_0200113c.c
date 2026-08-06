#include "types.h"
  
 
extern u8 * Func_02002b24(int);
extern void Func_02002c16(int);
extern void Func_02002a98(void *,int,int);
extern void Func_02002abe(int,int,int,int,int,int);
extern void Func_0200207c(int,int,int);
void Func_0200113c(void)
{
    u8 *leader = Func_02002b24(0); u8 *sprite = *(u8 **)(leader + 80);
    Func_02002c16(158); Func_02002a98((void *)0x0200a8ac, 35, 9); Func_02002abe(33,20,1,3,4,10);
    leader[35] &= (u8)~1; sprite[9] |= 12; Func_0200207c(72,160,12);
}
