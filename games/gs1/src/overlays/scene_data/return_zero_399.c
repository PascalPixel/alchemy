#include "types.h"

#define SceneData_ReturnZero Func_02000160

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}
