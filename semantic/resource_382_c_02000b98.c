#include "types.h"
  
   
extern void Func_02002554(void);
extern void Func_020025f2(int);
extern void Func_020022e8(int,int,int);
extern void Func_0200260c(int,int);
extern int Func_02002594(int,int);
extern void Func_02002638(int,int);
extern void Func_0200259c(void);
void Func_02000b98(void)
{
    u8 *workspace;
    Func_02002554(); Func_020025f2(0x133f); Func_020022e8(18, 0, 2); Func_0200260c(18, 0);
    if (Func_02002594(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Func_02002638(18, 0); Func_0200259c();
}
