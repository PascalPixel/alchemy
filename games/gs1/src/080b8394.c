#include "types.h"

s32 Object_SetMode(s32, s32);
s32 Object_ResetMotion(s32);
s32 *Func_080b7dd0();

void Func_080b8394(void) {
    s32 object;

    object = *Func_080b7dd0();
    Object_ResetMotion(object);
    Object_SetMode(object, 2);
}
