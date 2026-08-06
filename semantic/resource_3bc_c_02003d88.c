#include "types.h"






/*
 * resource_3bc owner at 0x02003d88, 84 bytes: the resource_3bb indexed-record
 * mover with this overlay's independently resolved callback pool word.
 */
extern u8 * Func_020087b4();
extern void Func_020086be();
extern void Func_020086a6();
extern void Func_02008964();
s32 Func_02003d88(u8 *object)
{
    s16 *index = (s16 *)(object + 100);
    u8 *record = Func_020087b4(*index);

    Func_020086be(record,
                  *(s32 *)(object + 8),
                  *(s32 *)(object + 12) + 0x02400000,
                  *(s32 *)(object + 16));
    Func_020086a6(record, (void *)0x0200ce74);
    record[0x55] = 0;
    Func_02008964(0x53);
    *index = 0;
    return 0;
}
