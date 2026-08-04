#include "types.h"
extern void Func_0808a018(void); extern void Func_0808a020(void); extern void Func_0808a010(int);
extern u8 *Func_0808a080(int); extern void Func_0808a148(int,int,int); extern void Func_0808a150(int,int,int);
extern void Func_0808a100(int,int); extern void Func_0808a170(int); extern void Func_0808a180(int,int);
extern void Func_0808a0b8(int,int,int); extern void Func_0808a0e8(int); extern void Func_0808a0f0(int,int,int);
extern void Func_080770c8(int);
/* Complete actor-11 recovery scene through its one-word pool. */
void Func_02000f44(void)
{
    u8 *a;
    Func_0808a018();
    Func_0808a148(11,0,0);
    Func_0808a148(0,11,0);
    Func_0808a100(0,1);
    Func_0808a010(10);
    Func_0808a150(0,11,0);
    Func_0808a170(0x23d9);
    Func_0808a180(11,0);
    Func_0808a100(11,2);
    a=Func_0808a080(0);
    if(a)
        Func_0808a0b8(11,*(s16*)(a+10),*(s16*)(a+18));
    Func_0808a0e8(11);
    Func_0808a0f0(11,0,0);
    Func_0808a010(20);
    Func_080770c8(0x9a0);
    Func_0808a020();
}
