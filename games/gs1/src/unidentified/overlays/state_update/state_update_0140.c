#include "types.h"

extern s16 *Data_0200add0;
extern s32 Func_02002238(void);

/* Clear the active story variant once the scene controller is idle. */
void Func_02000674(void)
{
    if (Func_02002238() == 0)
        *Data_0200add0 = -1;
}
