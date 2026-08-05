#include "types.h"

extern u8 **Func_020047ec(s32, s32);
extern void Func_02004824(u8 *);

/* Release the optional published attachment; complete owner, no pool. */
void Func_020023b8(void)
{
    u8 **publication = Func_020047ec(35, 4);
    u8 *state;
    u8 *object;

    if (publication == 0)
        return;
    state = *publication;
    object = *(u8 **)(state + 20);
    if (object == 0)
        return;
    Func_02004824(object);
    *(u8 **)(state + 20) = 0;
}
