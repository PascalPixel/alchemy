#include "types.h"

extern u8 *Func_0808a080(s32 id);

void Func_02003610(u8 *object)
{
    if (object != 0) {
        u8 *source;
        u8 *source_owner;
        u8 *owner;

        object[0x23] = 0;
        source = Func_0808a080(0);
        source_owner = *(u8 **)(source + 0x50);
        owner = *(u8 **)(object + 0x50);
        owner[9] = (owner[9] & ~0x0c) | (source_owner[9] & 0x0c);
    }
}
