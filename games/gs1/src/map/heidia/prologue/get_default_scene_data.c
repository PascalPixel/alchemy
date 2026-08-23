#include "types.h"

/* Complete eight-byte literal-address getter, including its sole pool word. */

#define GetDefaultSceneData Func_02000d40

u8 *GetDefaultSceneData(void)
{
    return (u8 *)0x0200b508;
}
