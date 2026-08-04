#include "types.h"
extern void Func_0808a018(void); extern void Func_0808a170(int); extern void Func_0808a100(int,int);
extern void Func_0200173c(int,int,int); extern void Func_0808a180(int,int); extern int Func_08077040(int);
extern int Func_080770c0(int); extern void Func_0808a020(void);
void Func_02000bec(void)
{
    Func_0808a018(); Func_0808a170(0x1342); Func_0808a100(19, 0); Func_0200173c(19, 0, 2);
    Func_0808a180(19, 0); Func_0808a100(19, 1);
    if (Func_08077040(231) != -1 && Func_080770c0(0x858) == 0) {
        u8 *workspace = *(u8 **)0x03001ebc; *(s16 *)(workspace + 370) = 1;
    }
    Func_0808a020();
}
