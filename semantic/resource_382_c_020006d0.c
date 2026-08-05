#include "types.h"
  
  


extern void Func_0200208c(void);
extern void Func_0200212a(int);
extern void Func_02001e20(int,int,int);
extern void Func_02002144(int,int);
extern int Func_020020cc(int,int);
extern void Func_02002170(int,int);
extern void Func_020020d4(void);
void Func_020006d0(void)
{
    u8 *workspace;

    Func_0200208c();
    Func_0200212a(0x122f);
    Func_02001e20(17, 0, 2);
    Func_02002144(17, 0);
    if (Func_020020cc(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Func_02002170(17, 0);
    Func_020020d4();
}
