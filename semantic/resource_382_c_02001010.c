#include "types.h"
  
 
extern u8 * Func_020029f8(int);
extern void Func_02002aea(int);
extern void Func_0200296c(void *,int,int);
extern void Func_02002992(int,int,int,int,int,int);
extern void Func_02001f52(int,int,int);
void Func_02001010(void)
{
    u8 *leader = Func_020029f8(0); u8 *sprite = *(u8 **)(leader + 80);
    Func_02002aea(158); Func_0200296c((void *)0x0200a880, 49, 10); Func_02002992(33, 20, 1, 3, 18, 10);
    leader[35] &= (u8)~1; sprite[9] |= 12; Func_02001f52(296, 176, 9);
}
