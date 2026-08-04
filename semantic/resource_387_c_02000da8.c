#include "types.h"

/* Turn the object's attached presentation state by one eighth-turn. */
void Func_02000da8(u8 *object)
{
    u8 *state = *(u8 **)(object + 80);
    *(u16 *)(state + 30) += (s32)-0x800;
}
