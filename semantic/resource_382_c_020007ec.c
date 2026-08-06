#include "types.h"
  
  
 

extern u8 * Func_020021d6(int);
extern void Func_020021c6(void);
extern void Func_02002264(int);
extern void Func_0200223c(int,int);
extern void Func_02001f62(int,int,int);
extern void Func_02001f52(int,int);
extern void Func_02002148(int);
extern void Func_020021fc(void);
void Func_020007ec(void)
{
    u8 *actor = Func_020021d6(14);
    s16 facing = *(s16 *)(actor + 6);
    u16 *state = (u16 *)(actor + 100);

    *state |= 2;
    Func_020021c6();
    Func_02002264(0x122c);
    Func_0200223c(14, 0);
    Func_02001f62(14, 0, 2);
    Func_02001f52(14, 10);
    *(s16 *)(actor + 6) = facing;
    Func_02002148(1);
    Func_020021fc();
    *state &= 1;
}
