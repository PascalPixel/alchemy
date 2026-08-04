#include "types.h"

extern u8 **Func_0200b304(s32 group, s32 slot);
extern void Func_0200b33c(u8 *object);

/* Release the scene object's optional attachment and clear its publication. */
void Func_02005948(void)
{
    u8 **publication = Func_0200b304(35, 4);
    u8 *state;
    u8 *object;

    if (publication == 0)
        return;

    state = *publication;
    object = *(u8 **)(state + 20);
    if (object == 0)
        return;

    Func_0200b33c(object);
    *(u8 **)(state + 20) = 0;
}
