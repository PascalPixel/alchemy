#include "types.h"

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */

#define GetEmptySceneData Func_02000d3c
s32 GetEmptySceneData(void)
{
    return 0;
}
