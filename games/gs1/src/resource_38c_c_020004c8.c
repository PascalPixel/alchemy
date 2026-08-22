#include "resource_38c.h"

#include "types.h"

extern u8 *Func_02000a14(s32);

/* The workspace pointer the overlay's own rows all reach through. */
#define WORKSPACE (*(u8 **) 0x03001EBC)

s32 Func_020004c8(void)
{
    u8 *actor;
    u8 *record;
    s32 bits;

    *(s32 *) (WORKSPACE + 448) = 521;
    actor = Func_02000a14(27);
    /* The stored zero is the mask's starting value: the reference builds
     * -13 by subtracting from the register the `strb` already set to 0
     * rather than materialising 0xf3 or negating 13. */
    actor[0x23] = bits = 0;
    record = *(u8 **) (actor + 0x50);
    bits -= 13;
    bits &= record[9];
    bits |= 8;
    record[9] = bits;
    return 0;
}
