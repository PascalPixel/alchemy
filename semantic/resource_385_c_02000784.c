#include "types.h"

/* Complete actor-12 timed two-flag dialogue scene through its pool. */
extern void Func_0808a018(void);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a010(s32 frames);
extern void Func_0808a148(s32 actor, s32 mode, s32 frames);
extern void Func_080770c8(s32 flag);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a188(s32 actor, s32 mode, s32 frames);
extern void Func_0808a020(void);

void Func_02000784(void)
{
    Func_0808a018();
    Func_0808a138(12, 1);
    Func_0808a010(20);
    Func_0808a148(12, 0, 20);
    Func_080770c8(0x306);
    Func_080770c8(0x868);
    Func_0808a170(0x1caf);
    Func_0808a188(12, 0, 20);
    Func_0808a020();
}
