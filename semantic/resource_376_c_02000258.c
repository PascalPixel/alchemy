#include "types.h"
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a170(s32 message);
extern void Func_0808a190(s32 actor, s32 mode);

/* Complete 32-byte actor-16 dialogue owner through its one-word pool. */
void Func_02000258(void)
{
    Func_0808a018();
    Func_0808a170(0x11be);
    Func_0808a190(16, 0);
    Func_0808a020();
}
