#include "types.h"

extern s16 *Data_0200add0;
extern s32 Func_0808a2a0(void);

/* Clear the active story variant once the scene controller is idle. */
void Func_02000674(void)
{
    if (Func_0808a2a0() == 0)
        *Data_0200add0 = -1;
}
