#include "types.h"
  
  
 
extern u8 * Func_020024b6(int);
extern void Func_020024a6(void);
extern void Func_02002544(int);
extern void Func_0200251c(int,int);
extern void Func_02002242(int,int,int);
extern void Func_02002232(int,int);
extern void Func_02002428(int);
extern void Func_020024dc(void);
void Func_02000acc(void)
{
    u8 *actor = Func_020024b6(15); s16 facing = *(s16 *)(actor + 6); u16 *state = (u16 *)(actor + 100);
    *state |= 2; Func_020024a6(); Func_02002544(0x133b); Func_0200251c(15, 0);
    Func_02002242(15, 0, 2); Func_02002232(15, 10); *(s16 *)(actor + 6) = facing;
    Func_02002428(1); Func_020024dc(); *state &= 1;
}
