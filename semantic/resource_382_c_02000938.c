#include "types.h"
extern u8 *Func_0808a080(int); extern void Func_0808a018(void); extern void Func_0808a100(int,int);
extern void Func_0808a010(int); extern void Func_0808a180(int,int); extern void Func_0808a020(void);
void Func_02000938(int actor)
{
    u8 *record = Func_0808a080(actor);
    record[91] = 1; Func_0808a018(); Func_0808a100(actor, 1); Func_0808a010(2);
    Func_0808a180(actor, 0); Func_0808a020(); record[91] = 0;
}
