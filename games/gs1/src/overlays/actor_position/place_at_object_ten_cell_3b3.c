#include "types.h"

/*
 * Place a staged actor at object ten's grid cell -- resource_3b3.
 */

/*
 * The Func_ aliases name the call words encoded in the overlay image. The
 * declarations are old-style because the call sites vary in arity.
 */
u8 *Func_02004fbc();
void Func_02004fb2(void);
s32 Func_020029e6();
void Func_02004fd6(void);

/*
 * Fetch object ten, shift its +8 and +16 fixed-point fields down to grid
 * coordinates, and place there. The last two literal arguments go on the
 * stack.
 */
void StagedActor_PlaceAtObjectTenCell(void)
{
    u8 *obj = Func_02004fbc(10);
    s32 x;
    s32 z;

    Func_02004fb2();

    x = *(s32 *)(obj + 8) >> 20;
    z = *(s32 *)(obj + 16) >> 20;

    Func_020029e6(2, x, z, 1, 1, 0);
    Func_02004fd6();
}
