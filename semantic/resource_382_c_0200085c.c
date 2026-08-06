#include "types.h"
  
  
 

extern u8 * Func_02002246(int);
extern void Func_02002236(void);
extern void Func_020022d4(int);
extern void Func_020022ac(int,int);
extern void Func_02001fd2(int,int,int);
extern void Func_02001fc2(int,int);
extern void Func_020021b8(int);
extern void Func_0200226c(void);
void Func_0200085c(void)
{
    u8 *actor = Func_02002246(15);
    s16 facing = *(s16 *)(actor + 6);
    u16 *state = (u16 *)(actor + 100);

    *state |= 2;
    Func_02002236();
    Func_020022d4(0x122d);
    Func_020022ac(15, 0);
    Func_02001fd2(15, 0, 2);
    Func_02001fc2(15, 10);
    *(s16 *)(actor + 6) = facing;
    Func_020021b8(1);
    Func_0200226c();
    *state &= 1;
}
