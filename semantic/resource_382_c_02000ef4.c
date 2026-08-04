#include "types.h"
extern void Func_0808a090(int,int,int); extern void Func_0808a0c8(int,int,int); extern void Func_0808a248(int);
void Func_02000ef4(int x, int z, int state)
{
    u8 *workspace;
    Func_0808a090(0, 0x8000, 0x4000); Func_0808a0c8(0, x, z);
    workspace = *(u8 **)0x03001ebc; *(s32 *)(workspace + 456) = 16; Func_0808a248(state);
}
