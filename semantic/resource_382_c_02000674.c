#include "types.h"
  
  
 

extern void Func_02002030(void);
extern void Func_020020ce(int);
extern void Func_020020a6(int,int);
extern void Func_02001dcc(int,int,int);
extern void Func_020020f0(int,int);
extern int Func_02002078(int,int);
extern void Func_0200211c(int,int);
extern void Func_02002080(void);
void Func_02000674(void)
{
    u8 *workspace;

    Func_02002030();
    Func_020020ce(0x1229);
    Func_020020a6(13, 1);
    Func_02001dcc(13, 0, 2);
    Func_020020f0(13, 0);
    if (Func_02002078(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Func_0200211c(13, 0);
    Func_02002080();
}
