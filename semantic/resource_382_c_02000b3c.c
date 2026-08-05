#include "types.h"
  
  
 
extern void Func_020024f8(void);
extern void Func_02002596(int);
extern void Func_0200256e(int,int);
extern void Func_02002294(int,int,int);
extern void Func_020025b8(int,int);
extern int Func_02002540(int,int);
extern void Func_020025e4(int,int);
extern void Func_02002548(void);
void Func_02000b3c(void)
{
    u8 *workspace;
    Func_020024f8(); Func_02002596(0x133c); Func_0200256e(16, 1); Func_02002294(16, 0, 2); Func_020025b8(16, 0);
    if (Func_02002540(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Func_020025e4(16, 0); Func_02002548();
}
