#include "types.h"

s32 Func_08009080(s32, s32);
s32 Func_08009140(s32);
s32 *Func_080b7dd0();

void Func_080b8394(void) {
    s32 object;

    object = *Func_080b7dd0();
    Func_08009140(object);
    Func_08009080(object, 2);
}
