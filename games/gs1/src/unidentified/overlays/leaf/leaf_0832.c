#include "types.h"
 
 
extern void Func_020022b0(void);
extern void Func_02002346(s32);
/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */
extern s32 Func_02002366(s32, s32);
extern void Func_020022ca(void);
void Func_020008cc(void) { Func_020022b0(); Func_02002346(0x24d3); Func_02002366(12, 0); Func_020022ca(); }
