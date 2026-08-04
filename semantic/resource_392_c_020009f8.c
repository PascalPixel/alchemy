/* Run the six-word placement query and forward a successful result. */
#include "types.h"
void Func_0808a018(void); void Func_0808a020(void);
s32 Func_02000474(s32 *result);
void Func_02000608(s32,s32,s32,s32,s32,void (*)(void));
void Func_020009f8(void)
{
    s32 result[6];
    Func_0808a018();
    if (Func_02000474(result))
        Func_02000608(result[0], result[1], result[2], result[3], result[4],
                      (void (*)(void))result[5]);
    Func_0808a020();
}
