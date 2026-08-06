#include "types.h"
  
  
extern u8 * Func_02002322(int);
extern void Func_0200230a(void);
extern void Func_0200237a(int,int);
extern void Func_02002310(int);
extern void Func_020023c8(int,int);
extern void Func_0200232c(void);
void Func_02000938(int actor)
{
    u8 *record = Func_02002322(actor);
    record[91] = 1; Func_0200230a(); Func_0200237a(actor, 1); Func_02002310(2);
    Func_020023c8(actor, 0); Func_0200232c(); record[91] = 0;
}
