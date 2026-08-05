#include "types.h"
  
  
 

extern void Func_02001fd0(void);
extern void Func_0200206e(int);
extern void Func_02001d64(int,int,int);
extern void Func_02002088(int,int);
extern int Func_02002010(int,int);
extern void Func_020020b4(int,int);
extern void Func_020020d0(int,int,int);
extern void Func_02002024(void);
void Func_02000614(void)
{
    u8 *workspace;

    Func_02001fd0();
    Func_0200206e(0x1223);
    Func_02001d64(9, 0, 2);
    Func_02002088(9, 0);
    if (Func_02002010(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Func_020020b4(9, 0);
    Func_020020d0(9, 0x5000, 0);
    Func_02002024();
}
