#include "types.h"
  
   
extern void Func_02002338(void);
extern void Func_020023d6(int);
extern void Func_020020cc(int,int,int);
/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */
extern int Func_020023f0(int,int);
extern int Func_02002378(int,int);
extern void Func_0200241c(int,int);
extern void Func_02002380(void);
void Func_0200097c(void)
{
    u8 *workspace;
    Func_02002338(); Func_020023d6(0x1330); Func_020020cc(8, 0, 2); Func_020023f0(8, 0);
    if (Func_02002378(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Func_0200241c(8, 0); Func_02002380();
}
