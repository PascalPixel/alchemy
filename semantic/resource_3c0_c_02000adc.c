#include "types.h"

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_0808a098(s32 actorId, void *descriptor);
extern void Func_0808a0f0(s32 actorId, s32 x, s32 y);

void Func_02000adc(void)
{
    if (Func_080770c0(0x9b7) != 0)
        return;
    Func_080770c8(0x20e);
    Func_0808a0f0(12, 0x780000, 0x3380000);
    Func_0808a098(12, (void *)0x020097a8);
}
