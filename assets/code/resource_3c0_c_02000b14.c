#include "types.h"

void Func_02000b14(void)
{
    u8 *state = *(u8 *volatile *)0x03001f30;
    state[52] = 1;
}
