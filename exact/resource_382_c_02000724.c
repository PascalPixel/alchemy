#include "types.h"
  
  


extern void Func_020020e0(void);
extern void Func_0200217e(int);
extern void Func_02001e74(int,int,int);
/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */
extern int Func_02002198(int,int);
extern int Func_02002120(int,int);
extern void Func_020021c4(int,int);
extern void Func_02002128(void);
void Func_02000724(void)
{
    u8 *workspace;

    Func_020020e0();
    Func_0200217e(0x1232);
    Func_02001e74(18, 0, 2);
    Func_02002198(18, 0);
    if (Func_02002120(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Func_020021c4(18, 0);
    Func_02002128();
}
