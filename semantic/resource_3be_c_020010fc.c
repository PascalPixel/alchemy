#include "types.h"
extern u8 *Func_0808a080(int);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_020011f8(int,int,int);
extern void Func_080091c0(int,int,int,int,int,int);
extern void Func_080770c8(int);
void Func_020010fc(void)
{
    u8 *a=Func_0808a080(8);
    Func_0808a018();
    a=Func_0808a080(8);
    if((*(int *)(a+8)>>20)<=30){
        Func_020011f8(8,0,0);
        Func_080091c0(29,19,1,1,27,19);
        Func_080770c8(0x9a2);
    }
    Func_0808a020();
}
