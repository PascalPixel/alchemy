#include "types.h"
  
extern void Func_020028f4(int,int,int);
extern void Func_02002916(int,int,int);
extern void Func_020029ca(int);
void Func_02000ef4(int x, int z, int state)
{
    u8 *workspace;
    Func_020028f4(0, 0x8000, 0x4000); Func_02002916(0, x, z);
    workspace = *(u8 **)0x03001ebc; *(s32 *)(workspace + 456) = 16; Func_020029ca(state);
}
